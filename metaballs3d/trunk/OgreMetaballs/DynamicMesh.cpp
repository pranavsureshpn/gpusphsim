#include "DynamicMesh.h"

//-----------------------------------
// DynamicMesh
//-----------------------------------

DynamicMesh::DynamicMesh(const String& name)
: ManualObject(name), m_initialized(false), m_vertexCount(0)
{
	ManualObject::setDynamic(true);
}

DynamicMesh::~DynamicMesh()
{
}

void DynamicMesh::BeginMesh()
{
	//Update or begin the object if needed
	if(m_initialized)
	{		
		ManualObject::beginUpdate(0);
	}
	else
	{
		m_initialized = true;
		ManualObject::begin("");
	}
	m_vertexCount = 0;
}

void DynamicMesh::EndMesh()
{
	//Finalize the mesh
	ManualObject::end();
}

int DynamicMesh::AddVertex(const Vertex& vertex)
{
	return AddVertex(vertex.Position, vertex.Normal, vertex.Color);
}

int DynamicMesh::AddVertex(const Vector3& position, const Vector3& normal, const ColourValue& color)
{
	//Declare a new vertex in the vertex buffer
	ManualObject::position(position);
	ManualObject::normal(normal);
	ManualObject::colour(color);

	return m_vertexCount++;
}

void DynamicMesh::AddTriangle(int idx1, int idx2, int idx3)
{
	//Add a triangle to the index buffer
	triangle(idx1, idx2, idx3);
}

void DynamicMesh::EstimateSize(int size)
{
	//Allocate memory for future needs
	estimateIndexCount(size);
	estimateVertexCount(size);
}
