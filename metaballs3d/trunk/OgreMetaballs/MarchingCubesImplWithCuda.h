#pragma once

#include "Common.h"
#include "ScalarField3D.h"

#include <vector>
#include "MarchingCubesInterface.h"

struct SamplingGridVertice_t
{
	Vector3 Position_t;
};
struct SamplingGridScalar_t
{
	float ScalarValue_t;
};
//-----------------------------------
// SamplingGridCube
//-----------------------------------

struct SamplingGridCube_t
{
	SamplingGridVertice_t* Vertices_t[8];
};
//-----------------------------------
// MarchingCubesImplWithCuda
//-----------------------------------

class MarchingCubesImplWithCuda : public MarchingCubesInterface
{
public:
	MarchingCubesImplWithCuda(DynamicMesh* meshBuilder);
	virtual ~MarchingCubesImplWithCuda();
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
	SamplingGridVertice_t *m_samplingGridVertices_t;
	SamplingGridCube_t    *m_samplingGridCubes_t;

	std::size_t m_samplingGridVerticesSize;
	std::size_t m_samplingGridCubesSize;
};
