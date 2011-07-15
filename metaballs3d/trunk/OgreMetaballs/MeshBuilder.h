#pragma once

#include "Ogre.h"

namespace Metaballs
{
	//-----------------------------------
	// MeshBuilder
	//-----------------------------------

	class MeshBuilder
	{
	public:
		MeshBuilder(String name);

		void StartMesh();
		void UpdateMesh();

		void Reserve(int num);

		//int AddVertice(SVertexElement vertex);
		void  AddTri(int v1, int v2, int v3);

		Ogre::ManualObject* FinalizeMesh();

	protected:
		Ogre::ManualObject* m_manualObject;
		int m_vertexCounter;
	};
}
