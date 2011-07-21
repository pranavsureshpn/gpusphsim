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
	virtual void CreateFields();
	virtual void UpdateFields(float time);

	virtual const ScalarField3D* GetScalarField() const;
//	virtual void SetSceneSize();
//	virtual float GetSpaceResolution() const;
	
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
