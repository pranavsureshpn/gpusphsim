#pragma once

//-----------------------------------
// Forward declarations
//-----------------------------------

class ScalarField3D;

//-----------------------------------
// MetaballsAbstractScene
//-----------------------------------

class MetaballsAbstractScene
{
public:
	MetaballsAbstractScene();
	virtual ~MetaballsAbstractScene();

	virtual void CreateFields() = 0;
	virtual void UpdateFields(float time) = 0;

	virtual const ScalarField3D* GetScalarField() const = 0;
	virtual float GetSceneSize() const = 0;
	virtual float GetSpaceResolution() const = 0;
};
