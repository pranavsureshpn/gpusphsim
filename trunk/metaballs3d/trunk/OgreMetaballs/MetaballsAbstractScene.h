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
	float GetSceneSize() const{ return m_SceneSize; }
	virtual float GetSpaceResolution() const = 0;

	virtual void SetSceneSize() = 0;
	void SetSceneSize(const float size_){ m_SceneSize = size_; }

protected:
	float m_SceneSize;

private:

};
