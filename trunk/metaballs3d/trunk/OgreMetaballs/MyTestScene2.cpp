#include "MyTestScene2.h"

#include <cstdlib> 
#include <cmath>
#include <boost/math/special_functions/math_fwd.hpp>
#include <boost/math/special_functions/fpclassify.hpp>

#include "SphericalField.h"
#include "../../OgreSnowSim/OgreSimBuffer.h"


//-----------------------------------
// Random
//-----------------------------------

extern float random();


//-----------------------------------
// CascadeScene
//-----------------------------------

MyTestScene2::MyTestScene2()
{
//	m_lastSpawnTime = 0;
//	m_lastUpdateTime = 0;

//	m_nbrMaxBalls = 20;//4
//	m_lifeTime = 3.2f*10.0f;
//	m_baseRadius = 0.2f*ScaleFactor;
//	m_minRadius  = 0.02f*ScaleFactor;
}

MyTestScene2::~MyTestScene2()
{
	destroyBalls();
}
void MyTestScene2::destroyBalls()
{
	//delete m_finalField;
	//m_finalField = NULL;
}
void MyTestScene2::CreateFields()
{
	//m_finalField = new MetaballField();

	createBalls();
}
void MyTestScene2::createBalls()
{
	createBalls_HardwareBufferPosition();
}
void MyTestScene2::createBalls_HardwareBufferPosition()
{
	assert(mParticlesEntity&&"mParticlesEntity is NULL");
	destroyBalls();

	Ogre::HardwareVertexBufferSharedPtr VBuf = mParticlesEntity->getVBufPos();
	std::size_t elementTypeSizeInByte = VBuf->getVertexSize();
	std::size_t NUM = VBuf->getNumVertices();
//	assert(m_nbrMaxBalls==NUM);	
	//printf("elementTypeSizeInByte=%d, num=%d\n", elementTypeSizeInByte, NUM);

	Ogre::Vector4* pVertexPos = static_cast<Ogre::Vector4*>(VBuf->lock(Ogre::HardwareBuffer::HBL_READ_ONLY));
	if(pVertexPos!=NULL)
	{
		for(size_t i=0; i<NUM; ++i)
		{
			m_finalField.AddField(new SphericalField(Vector3(pVertexPos[i].x,pVertexPos[i].y,pVertexPos[i].z) , GetParticleRadius()));
		}
	}else{
		printf("[error]create balls, pVertexPos is NULL!\n");
	}
	VBuf->unlock();
}

void MyTestScene2::UpdateFields(float time)
{
	Ogre::HardwareVertexBufferSharedPtr VBuf = mParticlesEntity->getVBufPos();
	std::size_t elementTypeSizeInByte = VBuf->getVertexSize();
	std::size_t NUM = VBuf->getNumVertices();
	assert(m_balls.size()==NUM);	
	//printf("elementTypeSizeInByte=%d, num=%d\n", elementTypeSizeInByte, NUM);

	Ogre::Vector4* pVertexPos = static_cast<Ogre::Vector4*>(VBuf->lock(Ogre::HardwareBuffer::HBL_READ_ONLY));
	if(pVertexPos!=NULL)
	{
		//Update all the balls
		for(std::size_t i=0; i<NUM; ++i)
		{
			if( boost::math::isnan(pVertexPos[i].x) 
			 || boost::math::isnan(pVertexPos[i].y) 
			 || boost::math::isnan(pVertexPos[i].z) )
			{
				printf("buf[%d]=<%f, %f, %f>\n", i,pVertexPos[i].x, pVertexPos[i].y, pVertexPos[i].z);
				continue;
			}
			((SphericalField*)(m_finalField.m_fields[i]))->SetCenter(Vector3(pVertexPos[i].x,pVertexPos[i].y,pVertexPos[i].z));
		}
	}else{
		printf("[error]pVertexPos is NULL!\n");
	}
	VBuf->unlock();
}

const ScalarField3D* MyTestScene2::GetScalarField() const
{
	return &m_finalField;
}

