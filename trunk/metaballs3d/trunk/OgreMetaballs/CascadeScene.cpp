#include "CascadeScene.h"

#include "SphericalField.h"
#include "ToroidalField.h"
#include "AdditiveField.h"
#include "PlaneField.h"

#include <cstdlib> 
#include <cmath>

//-----------------------------------
// Random
//-----------------------------------


float random()
{
	return 1 - 2*((float)rand()) / (float)RAND_MAX;
}

//-----------------------------------
// CascadeScene
//-----------------------------------

CascadeScene::CascadeScene()
{
	m_lastSpawnTime = 0;
	m_lastUpdateTime = 0;

	m_nbrMaxBalls = 4;
	m_lifeTime = 3.2f*10.0f;
	m_baseRadius = 0.5f*ScaleFactor*10.0f;
	m_minRadius  = 0.2f*ScaleFactor*10.0f;
	//m_baseRadius = 500.0f;
	//m_minRadius = 350.0f;
}

CascadeScene::~CascadeScene()
{
}

void CascadeScene::CreateFields()
{
	m_finalField = new AdditiveField();

	m_finalField->AddField(
		new PlaneField(Vector3(0,-1,0), 
						0.0f//GetSceneSize() * 0.45f - 1
		)
	);
}

void CascadeScene::UpdateFields(float time)
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
	if(time - m_lastSpawnTime > spawnDelay)
	{
		m_lastSpawnTime = time;
		CascadeMetaBall* ball = new CascadeMetaBall();

		ball->Position = Vector3(
			random() * 1.0f * ScaleFactor,
			-1.1f           * ScaleFactor,
			random() * 1.0f * ScaleFactor);	

		ball->Speed = Vector3(
			random() * 0.2f * ScaleFactor,
			1.0f + 0.3f * random() * ScaleFactor,
			random() * 0.2f * ScaleFactor);

		ball->Lifetime = m_lifeTime;

		ball->Field = new SphericalField(ball->Position, m_baseRadius);

		m_balls.push_back(ball);

		m_finalField->AddField(ball->Field);
	}

	//Update all the balls
	BallList::iterator iter;
	for(iter = m_balls.begin(); iter < m_balls.end();)
	{
		CascadeMetaBall* ball = *iter;
		ball->Lifetime -= deltaTime;

		//Delete dead metaballs
		if(ball->Lifetime < 0)
		{
			m_finalField->RemoveField(ball->Field);
			delete ball->Field;
			delete *iter;

			iter = m_balls.erase(iter);
			continue;
		}

		ball->Position += ball->Speed * deltaTime;
		ball->Field->SetCenter(ball->Position);

		float lifeRatio = ball->Lifetime / m_lifeTime;
		float radius = m_baseRadius /**sin( 3.141 * lifeRatio)*/;
		if(lifeRatio < 0.5f && radius < m_minRadius)
		{
			radius = m_minRadius;
		}

		ball->Field->SetRadius(radius);

		++iter;
	}

	m_lastUpdateTime = time;
}

const ScalarField3D* CascadeScene::GetScalarField() const
{
	return m_finalField;
}

// void CascadeScene::SetSceneSize()
// {
// 	m_SceneSize = 4.0f*ScaleFactor;
// }

// float CascadeScene::GetSpaceResolution() const
// {
// 	return 0.09f*ScaleFactor;
// 	//return 10.0f;
// }

