#include "MyTestScene.h"

#include <cstdlib> 
#include <cmath>

#include "SphericalField.h"
#include "ToroidalField.h"
#include "AdditiveField.h"
#include "PlaneField.h"



//-----------------------------------
// Random
//-----------------------------------

extern float random();


//-----------------------------------
// CascadeScene
//-----------------------------------

MyTestScene::MyTestScene()
{
	m_lastSpawnTime = 0;
	m_lastUpdateTime = 0;

	m_nbrMaxBalls = 40;//4
	m_lifeTime = 6.2f;//3.2
	m_baseRadius = 0.05f;//0.5
	m_minRadius = 0.02f;//0.2
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

	m_finalField->AddField(new PlaneField(Vector3(0,-1,0), GetSceneSize() * 0.45f - 1));


	createBalls();
}

void MyTestScene::createBalls()
{
	destroyBalls();

	for(size_t i=0; i<10; ++i)
	{
		CascadeMetaBall* ball = new CascadeMetaBall();

		ball->Position = Vector3(
			random() * 1.0f,
			-1.1f,
			random() * 1.0f);	

		ball->Speed = Vector3(
			random() * 0.2f,
			1.0f + 0.3f * random(),
			random() * 0.2f);

		ball->Lifetime = m_lifeTime;

		ball->Field = new SphericalField(ball->Position, m_baseRadius);

		m_balls.push_back(ball);

		m_finalField->AddField(ball->Field);
	}

}

void MyTestScene::UpdateFields(float time)
{
	if(m_lastUpdateTime == 0)
	{
		m_lastUpdateTime = time;
		m_lastSpawnTime = time;
		return;
	}

	float deltaTime = time - m_lastUpdateTime;
	float spawnDelay = m_lifeTime / m_nbrMaxBalls;

	//Spawn a new metaball
// 	if(time - m_lastSpawnTime > spawnDelay)
// 	{
// 		m_lastSpawnTime = time;
// 		CascadeMetaBall* ball = new CascadeMetaBall();
// 
// 		ball->Position = Vector3(
// 			random() * 1.0f,
// 			-1.1f,
// 			random() * 1.0f);	
// 
// 		ball->Speed = Vector3(
// 			random() * 0.2f,
// 			1.0f + 0.3f * random(),
// 			random() * 0.2f);
// 
// 		ball->Lifetime = m_lifeTime;
// 
// 		ball->Field = new SphericalField(ball->Position, m_baseRadius);
// 
// 		m_balls.push_back(ball);
// 
// 		m_finalField->AddField(ball->Field);
//	}

	//Update all the balls
	BallList::iterator iter;
	for(iter = m_balls.begin(); iter < m_balls.end();)
	{
		CascadeMetaBall* ball = *iter;
		ball->Lifetime -= deltaTime;

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

		ball->Position += ball->Speed * deltaTime;
		ball->Field->SetCenter(ball->Position);

		float lifeRatio = ball->Lifetime / m_lifeTime;
		float radius = m_baseRadius *sin( 3.141 * lifeRatio);
		if(lifeRatio < 0.5f && radius < m_minRadius)
		{
			radius = m_minRadius;
		}

		ball->Field->SetRadius(radius);

		++iter;
	}

	m_lastUpdateTime = time;
}

const ScalarField3D* MyTestScene::GetScalarField() const
{
	return m_finalField;
}

float MyTestScene::GetSceneSize() const
{
	return 4.0f;
}

float MyTestScene::GetSpaceResolution() const
{
	return 0.03f;//0.09
}

