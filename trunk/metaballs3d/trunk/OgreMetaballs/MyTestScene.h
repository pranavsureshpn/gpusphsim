#pragma once

#include "Common.h"
#include "MetaballsAbstractScene.h"
#include "CascadeScene.h"
//-----------------------------------
// Forward declarations
//-----------------------------------

class SphericalField;
class ToroidalField;
class AdditiveField;
class PlaneField;


//-----------------------------------
// CascadeScene
//-----------------------------------
class MyTestScene : public MetaballsAbstractScene
{
public:
	MyTestScene();
	virtual ~MyTestScene();

//	void setParticleNum(const int num_){ m_nbrMaxBalls = num_;}

protected:
	virtual void CreateFields() override;
	virtual void UpdateFields(float time) override;

	virtual const ScalarField3D* GetScalarField() const override;
	virtual void SetSceneSize();
	virtual float GetSpaceResolution() const override;

	void createBalls();
	void destroyBalls();

private:
	AdditiveField* m_finalField;

//	int m_nbrMaxBalls;
//	float m_lifeTime;
	float m_baseRadius;
//	float m_minRadius;
	BallList m_balls;

//	float m_lastSpawnTime;
//	float m_lastUpdateTime;
	void createBalls_Random();
	void createBalls_HardwareBufferPosition();
};
