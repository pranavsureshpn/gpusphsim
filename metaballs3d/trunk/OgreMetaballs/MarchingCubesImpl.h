#pragma once

#include "Common.h"
#include "ScalarField3D.h"

#include <vector>



#include "MarchingCubesInterface.h"

class MarchingCubesImpl : public MarchingCubesInterface
{
public:
	MarchingCubesImpl(DynamicMesh* meshBuilder);
	virtual ~MarchingCubesImpl();

protected:
	virtual void SampleSpace();
	virtual void March();
	virtual void SampleCube(SamplingGridCube& cube);

	virtual void ResetGridVertexBuffer(const size_t elementNum);
	virtual void ResetGridCubesBuffer(const size_t elementNum);
	virtual void freeGridVertexBuffer();
	virtual void freeGridCubesBuffer();
	virtual SamplingGridVertice& GetGridVertice(int i, int j, int k);
	virtual SamplingGridCube& GetGridCube(int i, int j, int k);

private:
	std::vector<SamplingGridVertice> m_samplingGridVertices;
	std::vector<SamplingGridCube> m_samplingGridCubes;

};
