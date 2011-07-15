#pragma once

#include "Common.h"
#include "ScalarField3D.h"

#include <vector>

#define USE_OPENMP 0
#if USE_OPENMP
#include "omp.h"
#endif

//-----------------------------------
// Forward declarations
//-----------------------------------

class DynamicMesh;

//-----------------------------------
// SamplingGridVertice
//-----------------------------------

struct SamplingGridVertice
{
	Vector3 Position;
	float ScalarValue;
};

//-----------------------------------
// SamplingGridCube
//-----------------------------------

struct SamplingGridCube
{
	SamplingGridVertice* Vertices[8];
};

//-----------------------------------
// MarchingCubesImpl
//-----------------------------------

typedef std::vector<ScalarField3D*> FieldList;

class MarchingCubesImpl
{
public:
	MarchingCubesImpl(DynamicMesh* meshBuilder);
	~MarchingCubesImpl();

	void Initialize(float samplingSpaceSize, float samplingResolution, float samplingThreshold);
	void CreateMesh();

	DynamicMesh* GetMeshBuilder() const { return m_meshBuilder; }
	void SetMeshBuilder(DynamicMesh* meshBuilder) { m_meshBuilder = meshBuilder; }

	const ScalarField3D* GetScalarField() const { return m_scalarField; }
	void SetScalarField(const ScalarField3D* scalarField) { m_scalarField = scalarField; }

    void AddField(ScalarField3D* field);
    void RemoveField(ScalarField3D* field);

protected:
	void SampleSpace();
	void March();
	void SampleCube(SamplingGridCube& cube);

	SamplingGridVertice& GetGridVertice(int i, int j, int k);
	SamplingGridCube& GetGridCube(int i, int j, int k);

private:
	float m_samplingSpaceSize;
	float m_samplingResolution;
	float m_samplingThreshold;

	int m_nbrSamples;

	std::vector<SamplingGridVertice> m_samplingGridVertices;
	std::vector<SamplingGridCube> m_samplingGridCubes;

	DynamicMesh* m_meshBuilder;
	const ScalarField3D* m_scalarField;	

    FieldList m_fields;
};
