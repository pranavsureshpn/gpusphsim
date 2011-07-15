
#include "MeshBuilder.h"



namespace Metaballs{

	MeshBuilder::MeshBuilder(String name)
	{
		m_manualObject = new Ogre::ManualObject(name);
		m_manualObject->setDynamic(true);

		m_vertexCounter = 0;
	}

	void MeshBuilder::Reserve(int num)
	{
		m_manualObject->estimateIndexCount(num);
		m_manualObject->estimateVertexCount(num);
	}

	void MeshBuilder::StartMesh()
	{
		m_manualObject->begin("");
	}

	void MeshBuilder::UpdateMesh()
	{
		m_manualObject->beginUpdate(0);
	}

	/*int MeshBuilder::AddVertice(SVertexElement vertex)
	{
		myObject->position(vertex.position.x, vertex.position.y, vertex.position.z);
		myObject->normal(vertex.normal.x, vertex.normal.y, vertex.normal.z);
		myObject->colour(vertex.color.x, vertex.color.y, vertex.color.z);
		myObject->textureCoord(vertex.position.x,vertex.position.y);
		return m_vertexCounter++;
	}*/

	void MeshBuilder::AddTri(int v1, int v2, int v3)
	{
		m_manualObject->triangle(v1, v2, v3);
	}

	Ogre::ManualObject* MeshBuilder::FinalizeMesh()
	{
		m_manualObject->end();

		Ogre::ManualObject* object = m_manualObject;
		m_manualObject = 0;
		return object;
	}
}