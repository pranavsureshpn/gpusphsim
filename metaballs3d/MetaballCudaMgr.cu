#include "MetaballCudaMgr.cuh"

#include <cassert>
#include <cuda.h>
#include <cutil.h>
#include "./trunk/OgreMetaballs/MarchingCubesInterface.h"
//#include "../SPHSimLib/cuPrintf.cu"

__global__ void ScalarSphere1D(const float* position, 
					   const float radiusSquared,
					   const float* gridVertex, float* scalar,
					   const int ParticleCount, const int GridVertexCount);
// __global__ void ScalarSphere3D(const float* position, 
// 					   const float radiusSquared,
// 					   const float* gridVertex, float* scalar,
// 					   const int ParticleCount, const int GridVertexCount);

MetaballCudaMgr* MetaballCudaMgr::m_singleton=NULL;

MetaballCudaMgr::MetaballCudaMgr()
{
//	cudaPrintfInit();
	d_spherePosition = NULL;
	d_spherePositionsCount = 0;
	sphereRadius = 1.0;

	d_samplingGridVertices = NULL;
	d_samplingGridVerticesCount = 0;
	
	d_samplingGridVerticesScalar = NULL;
	d_samplingGridVerticesScalarCount = 0;
	h_samplingGridVerticesScalar = NULL;

	threadsPerBlock = 32;
	//gridDim.x = gridDim.y = gridDim.z = 1;
	//blockDim.x = blockDim.y = blockDim.z = 1;
	
	deviceQuery();
}
MetaballCudaMgr::~MetaballCudaMgr()
{
//	cudaPrintfEnd();
}
MetaballCudaMgr* MetaballCudaMgr::getSingletonPtr()
{
	if(!m_singleton)
	{
		m_singleton = new MetaballCudaMgr();
	}
	return m_singleton;
}
//
void MetaballCudaMgr::mallocSpherePosition(const std::size_t elementCount, const std::size_t elementSize)
{
	assert( elementSize==sizeof(float)*3 );
	d_spherePositionsCount = elementCount;
	CUDA_SAFE_CALL( cudaMalloc((void**)&d_spherePosition, elementCount*elementSize) );
}
void MetaballCudaMgr::setSpherePosition(const float *src, const std::size_t elementCount)
{
	assert(elementCount<=d_spherePositionsCount);
	CUDA_SAFE_CALL( cudaMemcpy(d_spherePosition, src, d_spherePositionsCount*sizeof(float)*3, cudaMemcpyHostToDevice) );
	d_spherePositionsCount = elementCount;
}
void MetaballCudaMgr::freeSpherePosition()
{
	if(d_spherePosition)
	{
		CUDA_SAFE_CALL( cudaFree(d_spherePosition) );
		d_spherePositionsCount = 0;
	}
}
//
void MetaballCudaMgr::mallocSamplingGridVertices(const std::size_t elementCount, const std::size_t elementSize)
{
	assert( elementSize==sizeof(float)*4 );
	d_samplingGridVerticesCount = elementCount;
	CUDA_SAFE_CALL( cudaMalloc((void**)&d_samplingGridVertices, elementCount*elementSize) );
	//CUDA_SAFE_CALL( cudaMalloc((void**)&d_samplingGridVerticesScalar,   elementCount*sizeof(float)) );
}
void MetaballCudaMgr::setSamplingGridVertices(const float *src, const std::size_t elementCount)
{
	assert(elementCount<=d_samplingGridVerticesCount);
	CUDA_SAFE_CALL( cudaMemcpy(d_samplingGridVertices, src, d_samplingGridVerticesCount*sizeof(float)*3, cudaMemcpyHostToDevice) );
	d_samplingGridVerticesCount = elementCount;
}
void MetaballCudaMgr::freeSamplingGridVertices()
{
	if(d_samplingGridVertices)
	{
		CUDA_SAFE_CALL( cudaFree(d_samplingGridVertices) );
		d_samplingGridVerticesCount = 0;
	}
}
//
void MetaballCudaMgr::mallocSamplingGridVerticesScalar(const std::size_t elementCount, const std::size_t elementSize)
{
	assert( elementSize==sizeof(float) );
	d_samplingGridVerticesScalarCount = elementCount;
	assert( d_samplingGridVerticesScalarCount == d_samplingGridVerticesCount);
	CUDA_SAFE_CALL( cudaMalloc((void**)&d_samplingGridVerticesScalar, elementCount*elementSize) );
	
	h_samplingGridVerticesScalar = (float*)malloc(elementCount*elementSize);
	assert(h_samplingGridVerticesScalar);
}
// void MetaballCudaMgr::setSamplingGridVerticesScalar(const float *src, const std::size_t elementCount)
// {
// 	assert(elementCount<=d_samplingGridVerticesScalarCount);
// 	CUDA_SAFE_CALL( cudaMemcpy(d_samplingGridVerticesScalar, src, d_samplingGridVerticesScalarCount*sizeof(float)*3, cudaMemcpyHostToDevice) );
// 	d_samplingGridVerticesScalarCount = elementCount;
// 	assert( d_samplingGridVerticesScalarCount == d_samplingGridVerticesCount);
// }
void MetaballCudaMgr::freeSamplingGridVerticesScalar()
{
	if(d_samplingGridVerticesScalar)
	{
		CUDA_SAFE_CALL( cudaFree(d_samplingGridVerticesScalar) );
		d_samplingGridVerticesScalarCount = 0;
		
		free(h_samplingGridVerticesScalar);
	}
}
void MetaballCudaMgr::Scalar()
{
	launch_Scalar1D();
	//launch_Scalar3D();
    CUDA_SAFE_CALL( cudaMemcpy(
			h_samplingGridVerticesScalar, 
			d_samplingGridVerticesScalar, 
			d_samplingGridVerticesScalarCount*sizeof(float), 
			cudaMemcpyDeviceToHost
		) 
	);
}
void MetaballCudaMgr::launch_Scalar1D()
{
	//printf("MetaballCudaMgr::Scalar(%d, %d, %d)\n", gridSizeX, gridSizeY, gridSizeZ);
	//dim3 grid(gridSizeX, gridSizeY, gridSizeZ);
	//int threadsPerBlock = 32;
	//assign a thread for each grid vertex
    int blocksPerGrid = (d_samplingGridVerticesCount + threadsPerBlock - 1) / threadsPerBlock;
	//printf("launch_Scalar1D<<<%d, %d>>>\n", blocksPerGrid, threadsPerBlock);

	ScalarSphere1D<<<blocksPerGrid, threadsPerBlock>>>(
		d_spherePosition, 
		sphereRadius*sphereRadius,
		d_samplingGridVertices, 
		d_samplingGridVerticesScalar,
		d_spherePositionsCount,
		d_samplingGridVerticesCount
	);
	//cudaPrintfDisplay(stdout, true);
    CUDA_SAFE_CALL( cudaThreadSynchronize() );
}
// void MetaballCudaMgr::launch_Scalar3D()
// {
// 	//printf("MetaballCudaMgr::Scalar(%d, %d, %d)\n", gridSizeX, gridSizeY, gridSizeZ);
// 	//dim3 grid(gridSizeX, gridSizeY, gridSizeZ);
// 	//int threadsPerBlock = 32;
// 	//assign a thread for each grid vertex
//     //int blocksPerGrid = (d_samplingGridVerticesCount + threadsPerBlock - 1) / threadsPerBlock;
// 	//printf("ScalarSphere<<<%d, %d>>>\n", blocksPerGrid, threadsPerBlock);
// 	dim3 gridDim(1,1,1);	dim3 blockDim(d_samplingGridVerticesCount,1,1);
// 
//     while(blockDim.x > deviceProp.maxThreadsDim[0]) {
//         blockDim.x/=2;
//         blockDim.y*=2;
//     }
// 	while(blockDim.y > deviceProp.maxThreadsDim[1]) {
//         blockDim.y/=2;
//         blockDim.z*=2;
//     }
// 	while(blockDim.z> deviceProp.maxThreadsDim[2]){
// 		blockDim.z/=2;
// 		gridDim.x*=2;
// 	}
// 	while(gridDim.x> deviceProp.maxGridSize[0]){
// 		gridDim.x/=2;
// 		gridDim.y*=2;
// 	}
// 	while(gridDim.y> deviceProp.maxGridSize[1]){
// 		gridDim.y/=2;
// 		gridDim.z*=2;
// 	}
//  	printf("gridDim<%u, %u, %u>\n", gridDim.x, gridDim.y, gridDim.z);
//  	printf("blockDim<%u, %u, %u>\n", blockDim.x, blockDim.y, blockDim.z);
// 	assert(gridDim.z< deviceProp.maxGridSize[2]);
// 
// 	ScalarSphere3D<<<gridDim, blockDim, threadsPerBlock>>>(
// 		d_spherePosition, 
// 		sphereRadius*sphereRadius,
// 		d_samplingGridVertices, 
// 		d_samplingGridVerticesScalar,
// 		d_spherePositionsCount,
// 		d_samplingGridVerticesCount
// 	);
// 	//cudaPrintfDisplay(stdout, true);
//     //CUDA_SAFE_CALL( cudaThreadSynchronize() );
// }
void MetaballCudaMgr::FetchScalarValues(SamplingGridVertice *vertex, const std::size_t vertexCount)
{
	assert(vertexCount==d_samplingGridVerticesScalarCount);
	for(size_t i=0; i<vertexCount; ++i)
	{
        vertex[i].ScalarValue = h_samplingGridVerticesScalar[i];
	}
}
//
__global__ void ScalarSphere1D(const float* position, 
					   const float radiusSquared,
					   const float* gridVertexPos, float* gridVertexScalar,
					   const int ParticleCount, const int GridVertexCount
					   )
{
	uint blockId = blockIdx.y*gridDim.x + blockIdx.x;
    uint vi = blockId*blockDim.x + threadIdx.x;
#if __CUDA_ARCH__ >= 200
	//printf("grid<%d, %d, %d>, ",                       gridDim.x,                  gridDim.y,                 gridDim.z);
	//printf("block<%d/%d, %d/%d, %d/%d>, ",blockIdx.x, blockDim.x,     blockIdx.y, blockDim.y,    blockIdx.z, blockDim.z);
	//printf("thread<%d, %d, %d>\n",       threadIdx.x,                threadIdx.y,               threadIdx.z            );
#endif	
	uint pi = 0;
	
	float scalarSum = 0.0f;
	
	if(vi < GridVertexCount)//gridVertexPos[vi]={gvx, gvy, gvz}
	{
		float gvx = gridVertexPos[4*vi  ];
		float gvy = gridVertexPos[4*vi+1];
		float gvz = gridVertexPos[4*vi+2];

		for(pi=0; pi<ParticleCount; ++pi)//position[pi]={px, py, pz}
		{
			float px = position[3*pi  ];
			float py = position[3*pi+1];
			float pz = position[3*pi+2];

			scalarSum += radiusSquared /((px-gvx)*(px-gvx)+ 
										(py-gvy)*(py-gvy)+ 
										(pz-gvz)*(pz-gvz));
		}
	}
	gridVertexScalar[vi] = scalarSum;
}
// __global__ void ScalarSphere3D(const float* position, 
// 					   const float radiusSquared,
// 					   const float* gridVertexPos, float* gridVertexScalar,
// 					   const int ParticleCount, const int GridVertexCount
// 					   )
// {
// 	uint gBlockID  =blockIdx.x +  					blockIdx.y * gridDim.x + 					blockIdx.z * gridDim.x * gridDim.y;	uint gThreadID =threadIdx.x + 					threadIdx.y * blockDim.x +					threadIdx.z * blockDim.x * blockDim.y +					gBlockID * blockDim.x * blockDim.y * blockDim.z;
// 
//     uint vi = gThreadID;
// #if __CUDA_ARCH__ >= 200
// 	//printf("grid<%d, %d, %d>, ",                       gridDim.x,                  gridDim.y,                 gridDim.z);
// 	//printf("block<%d/%d, %d/%d, %d/%d>, ",blockIdx.x, blockDim.x,     blockIdx.y, blockDim.y,    blockIdx.z, blockDim.z);
// 	//printf("thread<%d, %d, %d>\n",       threadIdx.x,                threadIdx.y,               threadIdx.z            );
// #endif	
// 	uint pi = 0;
// 	
// 	float scalarSum = 0.0f;
// 	
// 	if(vi < GridVertexCount)//gridVertexPos[vi]={gvx, gvy, gvz}
// 	{
// 		float gvx = gridVertexPos[4*vi  ];
// 		float gvy = gridVertexPos[4*vi+1];
// 		float gvz = gridVertexPos[4*vi+2];
// 
// 		for(pi=0; pi<ParticleCount; ++pi)//position[pi]={px, py, pz}
// 		{
// 			float px = position[3*pi  ];
// 			float py = position[3*pi+1];
// 			float pz = position[3*pi+2];
// 
// 			scalarSum += radiusSquared /((px-gvx)*(px-gvx)+ 
// 										(py-gvy)*(py-gvy)+ 
// 										(pz-gvz)*(pz-gvz));
// 		}
// 	}
// 	gridVertexScalar[vi] = scalarSum;
// }
//
void MetaballCudaMgr::SetThreadsPerBlock(const std::size_t threadsPerBlock_)
{
	assert(threadsPerBlock_< deviceProp.maxThreadsPerBlock);
	threadsPerBlock = threadsPerBlock_;
}
// void MetaballCudaMgr::SetGridDim( const std::size_t gridx,
// 								  const std::size_t gridy,
// 								  const std::size_t gridz)
// {
// 	gridDim.x = gridx * gridy * gridz;
//     while(gridDim.x > deviceProp.maxGridSize[0]) {//
//         gridDim.x/=2;
//         gridDim.y*=2;
//     }
// 	while(gridDim.y > deviceProp.maxGridSize[1]) {//
//         gridDim.y/=2;
//         gridDim.z*=2;
//     }
// 	assert(gridDim.z < deviceProp.maxGridSize[2]);
// 	printf("gridDim<%d, %d, %d>\n", gridDim.x, gridDim.y, gridDim.z);
// }
// void MetaballCudaMgr::SetBlockDim(const std::size_t blockx,
// 								  const std::size_t blocky,
// 								  const std::size_t blockz)
// {
// 	blockDim.x = blockx * blocky * blockz;
//     while(blockDim.x > deviceProp.maxThreadsDim[0]) {
//         blockDim.x/=2;
//         blockDim.y*=2;
//     }
// 	while(blockDim.y > deviceProp.maxThreadsDim[1]) {
//         blockDim.y/=2;
//         blockDim.z*=2;
//     }
// 	assert(blockDim.z< deviceProp.maxThreadsDim[2]);
// 	printf("blockDim<%d, %d, %d>\n", blockDim.x, blockDim.y, blockDim.z);
// }
void MetaballCudaMgr::deviceQuery()
{
	int deviceCount = 0;
	if (cudaGetDeviceCount(&deviceCount) != cudaSuccess) {
		printf("cudaGetDeviceCount FAILED CUDA Driver and Runtime version may be mismatched.\n");
		printf("\nFAILED\n");
	}
	if (deviceCount == 0)
        printf("There is no device supporting CUDA\n");

    cudaGetDeviceProperties(&deviceProp, 0);//device0

}