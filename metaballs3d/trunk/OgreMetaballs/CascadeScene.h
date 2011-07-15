#pragma once

#include "Common.h"
#include "MetaballsAbstractScene.h"

//-----------------------------------
// Forward declarations
//-----------------------------------

class SphericalField;
class ToroidalField;
class AdditiveField;
class PlaneField;

//-----------------------------------
// CascadeMetaBall
//-----------------------------------

struct CascadeMetaBall
{
	float Lifetime;
	Vector3 Position;
	Vector3 Speed;
	SphericalField* Field;
};

//-----------------------------------
// CascadeScene
//-----------------------------------

typedef std::vector<CascadeMetaBall*> BallList;

class CascadeScene : public MetaballsAbstractScene
{
public:
	CascadeScene();
	virtual ~CascadeScene();

protected:
	virtual void CreateFields() override;
	virtual void UpdateFields(float time) override;

	virtual const ScalarField3D* GetScalarField() const override;
	virtual float GetSceneSize() const override;
	virtual float GetSpaceResolution() const override;

private:
	AdditiveField* m_finalField;

	int m_nbrMaxBalls;
	float m_lifeTime;
	float m_baseRadius;
	float m_minRadius;
	BallList m_balls; 

	float m_lastSpawnTime;
	float m_lastUpdateTime;
};
