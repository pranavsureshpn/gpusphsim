#pragma once

#include "Common.h"
#include "MetaballsAbstractScene.h"

//-----------------------------------
// Forward declarations
//-----------------------------------

class SphericalField;
class ToroidalField;
class AdditiveField;

//-----------------------------------
// TorusScene
//-----------------------------------

class TorusScene : public MetaballsAbstractScene
{
public:
	TorusScene();
	virtual ~TorusScene();

	virtual void CreateFields() override;
	virtual void UpdateFields(float time) override;

	virtual const ScalarField3D* GetScalarField() const override;
	virtual float GetSceneSize() const override;
	virtual float GetSpaceResolution() const override;

private:
	AdditiveField* m_finalField;
	SphericalField* m_field1;
	ScalarField3D* m_field2;
	SphericalField* m_field3;
	SphericalField* m_field4;
};
