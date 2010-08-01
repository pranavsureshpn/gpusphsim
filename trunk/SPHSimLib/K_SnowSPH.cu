#ifndef __K_SnowSPH_cu__
#define __K_SnowSPH_cu__

#include "K_Common.cuh"
#include "cutil_math.h"
#include "vector_types.h"

using namespace SimLib;
using namespace SimLib::Sim::SnowSPH;

#include "K_UniformGrid_Utils.cu"
#include "K_Coloring.cu"
#include "K_SPH_Common.cuh"

class SnowSPHSystem
{
public:



	static __device__ void UpdateSortedValues(SnowSPHData &dParticlesSorted, SnowSPHData &dParticles, uint &index, uint &sortedIndex)
	{
		dParticlesSorted.position[index]	= FETCH_NOTEX(dParticles,position,sortedIndex);
		dParticlesSorted.velocity[index]	= FETCH_NOTEX(dParticles,velocity,sortedIndex);
		dParticlesSorted.veleval[index]		= FETCH_NOTEX(dParticles,veleval,sortedIndex);
	} 
};

#include "K_SnowSPH_Step1.cu"
#include "K_SnowSPH_Step2.cu"
#include "K_SnowSPH_Step3.cu"
#include "K_SnowSPH_Integrate.cu"

#endif
