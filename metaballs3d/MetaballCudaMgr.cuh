#ifndef _Metaball_Cuda_Mgr_H_
#define _Metaball_Cuda_Mgr_H_

//#include <vector>
class SamplingGridVertice;

class MetaballCudaMgr
{
public:
	static MetaballCudaMgr* getSingletonPtr();
	~MetaballCudaMgr();

	void mallocSpherePosition(const std::size_t elementCount, const std::size_t elementSize);
	void setSpherePosition(const float *src, const std::size_t elementCount);
	void freeSpherePosition();
	float *d_spherePosition;
	std::size_t d_spherePositionsCount;//element count
	float sphereRadius;

	void mallocSamplingGridVertices(const std::size_t elementCount, const std::size_t elementSize);
	void setSamplingGridVertices(const float *src, const std::size_t elementCount);
	void freeSamplingGridVertices();
	float *d_samplingGridVertices;
	std::size_t d_samplingGridVerticesCount;

	void mallocSamplingGridVerticesScalar(const std::size_t elementCount, const std::size_t elementSize);
//	void setSamplingGridVerticesScalar(const float *src, const std::size_t elementCount);
	void freeSamplingGridVerticesScalar();
	float *d_samplingGridVerticesScalar;
	float *h_samplingGridVerticesScalar;
	std::size_t d_samplingGridVerticesScalarCount;

	void Scalar(const int gridSizeX, const int gridSizeY, const int gridSizeZ);
	void ScalarCallback(SamplingGridVertice *vertex, const std::size_t vertexCount);


protected:
	MetaballCudaMgr();
	static MetaballCudaMgr *m_singleton;
};
#endif