#pragma once

#include "Common.h"
#include "ScalarField3D.h"

#include <vector>

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

class MarchingCubesInterface
{
public:
	MarchingCubesInterface(DynamicMesh* meshBuilder);
	virtual ~MarchingCubesInterface();

	void Initialize(float samplingSpaceSize, float samplingResolution, float samplingThreshold);
	void unInitialize();
	void CreateMesh();

	DynamicMesh* GetMeshBuilder() const { return m_meshBuilder; }
	void SetMeshBuilder(DynamicMesh* meshBuilder) { m_meshBuilder = meshBuilder; }

	const ScalarField3D* GetScalarField() const { return m_scalarField; }
	void SetScalarField(const ScalarField3D* scalarField) { m_scalarField = scalarField; }

    void AddField(ScalarField3D* field);
    void RemoveField(ScalarField3D* field);

protected:
 	virtual void SampleSpace()=0;
 	virtual void March()=0;
 	virtual void SampleCube(SamplingGridCube& cube)=0;

	// on buffer
	virtual void ResetGridVertexBuffer(const size_t elementNum) = 0;
	virtual void ResetGridCubesBuffer(const size_t elementNum) = 0;
	virtual void freeGridVertexBuffer() = 0;
	virtual void freeGridCubesBuffer() = 0;
	virtual SamplingGridVertice& GetGridVertice(int i, int j, int k) = 0;
	virtual SamplingGridCube& GetGridCube(int i, int j, int k) = 0;
// 	virtual SamplingGridVertice& GetGridVertexElement(const size_t index) = 0;
// 	virtual SamplingGridCube& GetGridCubesElement(const size_t index) = 0;
// 


protected:
	float m_samplingSpaceSize;
	float m_samplingResolution;
	float m_samplingThreshold;

	int m_nbrSamples;


// 	std::vector<SamplingGridVertice> m_samplingGridVertices;
// 	std::vector<SamplingGridCube> m_samplingGridCubes;

	DynamicMesh* m_meshBuilder;
	const ScalarField3D* m_scalarField;	

    FieldList m_fields;
};
