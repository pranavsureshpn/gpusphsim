#ifndef _Metaball_Cuda_Mgr_H_
#define _Metaball_Cuda_Mgr_H_

//#include <vector>
#include <vector_types.h>
#include <cuda_runtime_api.h>
class SamplingGridVertice;

class MetaballCudaMgr
{
public:
	static MetaballCudaMgr* getSingletonPtr();
	~MetaballCudaMgr();

	void mallocSpherePosition(const std::size_t elementCount, const std::size_t elementSize);
	void setSpherePosition(const float *src, const std::size_t elementCount);
	void freeSpherePosition();
	float *d_spherePosition;//float3
	std::size_t d_spherePositionsCount;//element count


	void mallocSamplingGridVertices(const std::size_t elementCount, const std::size_t elementSize);
	void setSamplingGridVertices(const float *src, const std::size_t elementCount);
	void freeSamplingGridVertices();
	float *d_samplingGridVertices;//float4
	std::size_t d_samplingGridVerticesCount;

	void mallocSamplingGridVerticesScalar(const std::size_t elementCount, const std::size_t elementSize);
//	void setSamplingGridVerticesScalar(const float *src, const std::size_t elementCount);
	void freeSamplingGridVerticesScalar();
	float *d_samplingGridVerticesScalar;
	float *h_samplingGridVerticesScalar;//float
	std::size_t d_samplingGridVerticesScalarCount;

	void mallocSamplingGridCubes(const std::size_t elementCount, const std::size_t elementSize);
	void setSamplingGridCubes(const float *src, const std::size_t elementCount);
	void freeSamplingGridCubes();
	float *d_samplingGridCubes;
	std::size_t d_samplingGridCubesCount;

	void Scalar();
	void FetchScalarValues(SamplingGridVertice *vertex, const std::size_t vertexCount);

	void SetThreadsPerBlock(const std::size_t threadsPerBlock_);

	void SetGridDim(const std::size_t gridx, const std::size_t gridy, const std::size_t gridz);
	void SetBlockDim(const std::size_t blockx,const std::size_t blocky,const std::size_t blockz);

	void SetSpaceResolution(const float resx, const float resy, const float resz);
	void SetSamplesNumber(const unsigned int smplx, const unsigned int smply, const unsigned int smplz);
	void MetaballCudaMgr::SetExtendCubes(const unsigned int N, const float sphereRadius_,
		const float resx, const float resy, const float resz);


	void initilize(
		const unsigned int NBRSamplesX, const unsigned int NBRSamplesY, const unsigned int NBRSamplesZ,
		const float resx, const float resy, const float resz,
		const unsigned int ParticleEffectCubesScope,
		const float sphereRadius_
		);

protected:
	MetaballCudaMgr();
	void launch_Scalar1D();
	void launch_ScalarSphere1D_EffectRange();
	void launch_Scalar3D();
	void deviceQuery();


	static MetaballCudaMgr *m_singleton;

	std::size_t threadsPerBlock;
//  	dim3 gridDim;
//  	dim3 blockDim;
	cudaDeviceProp deviceProp;


};
#endif