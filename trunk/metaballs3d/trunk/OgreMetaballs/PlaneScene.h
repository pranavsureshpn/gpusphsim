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
// PlaneScene
//-----------------------------------

class PlaneScene : public MetaballsAbstractScene
{
public:
	PlaneScene();
	virtual ~PlaneScene();

	virtual void CreateFields() override;
	virtual void UpdateFields(float time) override;

	virtual const ScalarField3D* GetScalarField() const override;
	virtual float GetSceneSize() const override;
	virtual float GetSpaceResolution() const override;

private:
	AdditiveField* m_finalField;
	PlaneField* m_field1;
};
