#pragma once

//-----------------------------------
// MarchingCubesData
//-----------------------------------

class MarchingCubesData
{
public:
	static const int* GetTriangleList(int idx);
	static int GetVertexList(int idx);
	static int GetVertexInEdge(int idx1, int idx2);
};

