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
// HeartScene
//-----------------------------------

class HeartScene : public MetaballsAbstractScene
{
public:
	HeartScene(void);
	~HeartScene(void);

	virtual void CreateFields() override;
	virtual void UpdateFields(float time) override;

	virtual const ScalarField3D* GetScalarField() const override;
	virtual float GetSceneSize() const override;
	virtual float GetSpaceResolution() const override;

private:
	ScalarField3D* m_finalField;
};

