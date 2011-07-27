#include "MarchingCubesInterface.h"

#include "ScalarField3D.h"
#include "DynamicMesh.h"
#include "MarchingCubesData.h"

#include "FieldHelper.h"

//-----------------------------------
// MarchingCubesImpl
//-----------------------------------

MarchingCubesInterface::MarchingCubesInterface(DynamicMesh* meshBuilder)
: m_meshBuilder(meshBuilder), m_scalarField(NULL)
{
}


MarchingCubesInterface::~MarchingCubesInterface()
{ 
}


void MarchingCubesInterface::Initialize(float samplingSpaceSize, float samplingResolution, float samplingThreshold)
{
	beforeInitialize();

	m_samplingSpaceSize = samplingSpaceSize;
	m_samplingResolution = samplingResolution;
	m_samplingThreshold = samplingThreshold;

	m_nbrSamples = ceil(m_samplingSpaceSize / m_samplingResolution);

	//Provide a loose estimate of the number of vertex and triangles in the final mesh
	m_meshBuilder->EstimateSize(m_nbrSamples * m_nbrSamples * m_nbrSamples);

	//float samplingMinPos = -m_nbrSamples * m_samplingResolution / 2;
	float samplingMinPos = 0.0f;

	//Create and initialize the sampling grid
	ResetGridVertexBuffer(m_nbrSamples * m_nbrSamples * m_nbrSamples);


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
	afterGridVerticeInit();

	ResetGridCubesBuffer((m_nbrSamples-1) * (m_nbrSamples-1) * (m_nbrSamples-1));


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
	afterGridCubeInit();

	afterInitialize();
}
void MarchingCubesInterface::unInitialize()
{
	freeGridVertexBuffer();
	freeGridCubesBuffer();
}

void MarchingCubesInterface::AddField(ScalarField3D* field)
{
    m_fields.push_back(field);
}

void MarchingCubesInterface::RemoveField(ScalarField3D* field)
{	
    FieldList::iterator iter = std::find(m_fields.begin(), m_fields.end(), field);
    if(iter != m_fields.end())
    {
        m_fields.erase(iter);
    }
}

void MarchingCubesInterface::CreateMesh()
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


