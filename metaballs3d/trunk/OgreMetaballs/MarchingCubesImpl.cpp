#include "MarchingCubesImpl.h"

#include "ScalarField3D.h"
#include "DynamicMesh.h"
#include "MarchingCubesData.h"

#include "FieldHelper.h"

//-----------------------------------
// MarchingCubesImpl
//-----------------------------------

MarchingCubesImpl::MarchingCubesImpl(DynamicMesh* meshBuilder)
: m_meshBuilder(meshBuilder), m_scalarField(NULL)
{
}


MarchingCubesImpl::~MarchingCubesImpl()
{ 
}


void MarchingCubesImpl::Initialize(float samplingSpaceSize, float samplingResolution, float samplingThreshold)
{
	m_samplingSpaceSize = samplingSpaceSize;
	m_samplingResolution = samplingResolution;
	m_samplingThreshold = samplingThreshold;

	m_nbrSamples = ceil(m_samplingSpaceSize / m_samplingResolution);

	//Provide a loose estimate of the number of vertex and triangles in the final mesh
	m_meshBuilder->EstimateSize(m_nbrSamples * m_nbrSamples * m_nbrSamples);

	float samplingMinPos = -m_nbrSamples * m_samplingResolution / 2;

	//Create and initialize the sampling grid
	m_samplingGridVertices.clear();
	m_samplingGridVertices.resize(m_nbrSamples * m_nbrSamples * m_nbrSamples);

	//Iterate through every vertice in the grid
	for(int i=0; i<m_nbrSamples; i++)
	{
		for(int j=0; j<m_nbrSamples; j++)
		{
			for(int k=0; k<m_nbrSamples; k++)
			{
				//Initialize the vertice to a default state
				SamplingGridVertice& vertice = GetGridVertice(i,j,k);
				vertice.Position = Vector3(
					samplingMinPos + i * m_samplingResolution,
					samplingMinPos + j * m_samplingResolution,
					samplingMinPos + k * m_samplingResolution);
			}
		}
	}

	m_samplingGridCubes.clear();
	m_samplingGridCubes.resize((m_nbrSamples-1) * (m_nbrSamples-1) * (m_nbrSamples-1));

	for(int i=0; i<m_nbrSamples-1; i++)
	{
		for(int j=0; j<m_nbrSamples-1; j++)
		{			
			for(int k=0; k<m_nbrSamples-1; k++)
			{
				SamplingGridCube& cube = GetGridCube(i,j,k);

				//Store the vertices of the cube defined by i,j and k
				cube.Vertices[0] = &GetGridVertice(i  ,j+1,k);
				cube.Vertices[1] = &GetGridVertice(i+1,j+1,k);
				cube.Vertices[2] = &GetGridVertice(i+1,j  ,k);
				cube.Vertices[3] = &GetGridVertice(i  ,j  ,k);
				cube.Vertices[4] = &GetGridVertice(i  ,j+1,k+1);
				cube.Vertices[5] = &GetGridVertice(i+1,j+1,k+1);
				cube.Vertices[6] = &GetGridVertice(i+1,j  ,k+1);
				cube.Vertices[7] = &GetGridVertice(i  ,j  ,k+1);
			}
		}
	}
}

SamplingGridVertice& MarchingCubesImpl::GetGridVertice(int i, int j, int k)
{
	return m_samplingGridVertices[i + j * m_nbrSamples + k * m_nbrSamples * m_nbrSamples];
}

SamplingGridCube& MarchingCubesImpl::GetGridCube(int i, int j, int k)
{
	return m_samplingGridCubes[i + j * (m_nbrSamples-1) + k * (m_nbrSamples-1) * (m_nbrSamples-1)];
}

void MarchingCubesImpl::AddField(ScalarField3D* field)
{
    m_fields.push_back(field);
}

void MarchingCubesImpl::RemoveField(ScalarField3D* field)
{	
    FieldList::iterator iter = std::find(m_fields.begin(), m_fields.end(), field);
    if(iter != m_fields.end())
    {
        m_fields.erase(iter);
    }
}

void MarchingCubesImpl::CreateMesh()
{
	//Begin the construction of the mesh
	GetMeshBuilder()->BeginMesh();

	//Sample the scalar field value on each vertice of grid
	SampleSpace();

	//For each cube defined by the grid, emit geometry if the cube intersect the surface.
	March();

	//Add a dummy triangle to avoid empty mesh     
	Vertex v;
	m_meshBuilder->AddVertex(v);
	m_meshBuilder->AddVertex(v);
	m_meshBuilder->AddVertex(v);

	m_meshBuilder->AddTriangle(0,1,2);

	//Finalize the mesh
	GetMeshBuilder()->EndMesh();
}

void MarchingCubesImpl::SampleSpace()
{
	//the openmp parallel for expect signed loop counter
	int size = m_samplingGridVertices.size();

	#if USE_OPENMP
	#pragma omp parallel for  
	#endif
	for(int i=0; i<size; i++)
	{		
		//The indirection created with 'vertice' actually improve performances
		SamplingGridVertice& vertice = m_samplingGridVertices[i];
		vertice.ScalarValue =  GetScalarField()->Scalar(vertice.Position);
	}
}

void MarchingCubesImpl::March()
{
	int size = m_samplingGridCubes.size();
	for(int i=0; i<size; i++)
	{
		SampleCube(m_samplingGridCubes[i]);
	}
}

void MarchingCubesImpl::SampleCube(SamplingGridCube& cube)
{
	//Build a index to determine the state of the cube. 
	//The #n bit is set to 1 if the vertice #n is inside of the surface.
	int cubeIndex = 0;
	for (int n=0; n<8; n++)
	{
		if (cube.Vertices[n]->ScalarValue <= m_samplingThreshold) 
			cubeIndex |= 1<<n;
	}

	//An edge of the cube intersect the surface if one vertice 
	// is inside of the surface and the other is outside.
	//Based on the previous index, we can use a static lookup table to get the list 
	// of vertex and triangles we need to generate.
	static Vertex meshVertex[12];
	static int vertexIdxs[12];

	//The table return a bit field in which the #n bit is set to 1 if the edge #n intersect the surface
	int intersectionList = MarchingCubesData::GetVertexList(cubeIndex);

    //For each edge of the cube
	for(int n=0; n<12; n++)
	{
		//If the edge is intersected
		if(intersectionList & 1<<n)
		{
			//Store the vertices corresponding to the edge.
			const SamplingGridVertice* cubeVertice0 = cube.Vertices[MarchingCubesData::GetVertexInEdge(n,0)];
			const SamplingGridVertice* cubeVertice1 = cube.Vertices[MarchingCubesData::GetVertexInEdge(n,1)];

			//Determine the position of the intersection along the edge by linear interpolation.
			float d0 = abs(cubeVertice0->ScalarValue - m_samplingThreshold);
			float d1 = abs(cubeVertice1->ScalarValue - m_samplingThreshold);
			float lerp = d1 / (d0+d1);

			//Generate the vertex at the intersection
			Vertex triangleVertex;
			triangleVertex.Position = lerp * cubeVertice0->Position + (1-lerp) * cubeVertice1->Position;
			triangleVertex.Normal = m_scalarField->Gradient(triangleVertex.Position);
			triangleVertex.Color = lerp * m_scalarField->Color(cubeVertice0->Position) + (1-lerp) * m_scalarField->Color(cubeVertice1->Position);
			
			triangleVertex.Normal.normalise();

			vertexIdxs[n] = GetMeshBuilder()->AddVertex(triangleVertex);
		}
	}

	//The table return an array of egde indexes, from 0 to 11, ended by a -1 value.
	//Every three edges define a triangle.
	const int* cubeEdgeArray = MarchingCubesData::GetTriangleList(cubeIndex);
	for(int n = 0; cubeEdgeArray[n] != -1; n+=3)
	{	
		GetMeshBuilder()->AddTriangle(
			vertexIdxs[cubeEdgeArray[n+1]], 
			vertexIdxs[cubeEdgeArray[n]], 
			vertexIdxs[cubeEdgeArray[n+2]]);
	}
}