#pragma once

#include "Common.h"
#include "OgreManualObject.h"

//-----------------------------------
// DynamicMesh
//-----------------------------------

class DynamicMesh : public ManualObject
{
public:
	DynamicMesh(const String& name);
	virtual ~DynamicMesh();

	void BeginMesh();
	void EndMesh();

	int AddVertex(const Vertex& vertex);
	int AddVertex(const Vector3& position, const Vector3& normal, const ColourValue& color);
	void AddTriangle(int idx1, int idx2, int idx3);

	void EstimateSize(int size);

private:
	bool m_initialized;
	int m_vertexCount;
};
