#include "MyTestScene.h"

#include <cstdlib> 
#include <cmath>

#include "SphericalField.h"
#include "ToroidalField.h"
#include "AdditiveField.h"
#include "PlaneField.h"
#include "../../OgreSnowSim/OgreSimBuffer.h"


//-----------------------------------
// Random
//-----------------------------------

extern float random();
extern float ScaleFactor;

//-----------------------------------
// CascadeScene
//-----------------------------------

MyTestScene::MyTestScene()
{
//	m_lastSpawnTime = 0;
//	m_lastUpdateTime = 0;

	m_nbrMaxBalls = 20;//4
//	m_lifeTime = 3.2f*10.0f;
	m_baseRadius = 0.5f*ScaleFactor*0.2f;
	m_minRadius  = 0.2f*ScaleFactor*0.2f;
}

MyTestScene::~MyTestScene()
{
	destroyBalls();
}
void MyTestScene::destroyBalls()
{
	BallList::iterator iter;
	for(iter = m_balls.begin(); iter < m_balls.end();)
	{
		CascadeMetaBall* ball = *iter;

		m_finalField->RemoveField(ball->Field);
		delete ball->Field;
		delete *iter;

		iter = m_balls.erase(iter);
	}
}
void MyTestScene::CreateFields()
{
	m_finalField = new AdditiveField();

// 	m_finalField->AddField(
// 		new PlaneField(Vector3(0,-1,0), 
// 		0.0f//GetSceneSize() * 0.45f - 1
// 		)
// 		);

	createBalls();
}

void MyTestScene::createBalls()
{
	destroyBalls();

	for(size_t i=0; i<m_nbrMaxBalls; ++i)
	{
		CascadeMetaBall* ball = new CascadeMetaBall();

		ball->Position = Vector3(
			random() * 1.0f * ScaleFactor,
			-1.1f           * ScaleFactor,
			random() * 1.0f * ScaleFactor);	

		ball->Speed = Vector3(
			random() * 0.2f * ScaleFactor,
			1.0f + 0.3f * random() * ScaleFactor,
			random() * 0.2f * ScaleFactor);

		ball->Lifetime = 0.0f;

		ball->Field = new SphericalField(ball->Position, m_baseRadius);

		m_balls.push_back(ball);

		m_finalField->AddField(ball->Field);
	}

}

void MyTestScene::UpdateFields(float time)
{
// 	if(m_lastUpdateTime == 0)
// 	{
// 		m_lastUpdateTime = time;
// 		m_lastSpawnTime = time;
// 		return;
// 	}

//	float deltaTime = time - m_lastUpdateTime;
//	float spawnDelay = m_lifeTime / m_nbrMaxBalls;

	Ogre::HardwareVertexBufferSharedPtr VBuf = mParticlesEntity->getVBufPos();
	std::size_t elementTypeSizeInByte = VBuf->getVertexSize();
	std::size_t NUM = VBuf->getNumVertices();
	assert(m_nbrMaxBalls==NUM);	
	//printf("elementTypeSizeInByte=%d, num=%d\n", elementTypeSizeInByte, NUM);

	Ogre::Vector4* pVertexPos = static_cast<Ogre::Vector4*>(VBuf->lock(Ogre::HardwareBuffer::HBL_READ_ONLY));
	if(pVertexPos!=NULL)
	{
		//Update all the balls
		//BallList::iterator iter;
		//for(iter = m_balls.begin(); iter < m_balls.end();)
		for(std::size_t i=0; i<NUM; ++i)
		{
			//CascadeMetaBall* ball = *iter;
			CascadeMetaBall* ball = m_balls[i];
//			ball->Lifetime -= deltaTime;

			//Delete dead metaballs
	// 		if(ball->Lifetime < 0)
	// 		{
	// 			m_finalField->RemoveField(ball->Field);
	// 			delete ball->Field;
	// 			delete *iter;
	// 
	// 			iter = m_balls.erase(iter);
	// 			continue;
	// 		}

			ball->Position.x = pVertexPos[i].x;//ball->Position += ball->Speed * deltaTime;
			ball->Position.y = pVertexPos[i].y;
			ball->Position.z = pVertexPos[i].z;
			printf("buf[%d]=<%f, %f, %f>\n", i,ball->Position.x, ball->Position.y, ball->Position.z);

			ball->Field->SetCenter(ball->Position);

			//float radius = m_baseRadius /**sin( 3.141 * lifeRatio)*/;
			//ball->Field->SetRadius(radius);

			//++iter;
		}
	}else{
		printf("[error]pVertexPos is NULL!\n");
	}
	VBuf->unlock();

//	m_lastUpdateTime = time;
}

const ScalarField3D* MyTestScene::GetScalarField() const
{
	return m_finalField;
}

void MyTestScene::SetSceneSize()
{
	m_SceneSize = 4.0f * ScaleFactor;
}

float MyTestScene::GetSpaceResolution() const
{
	//return 0.09f*ScaleFactor;
	return 0.14f*ScaleFactor;
}

