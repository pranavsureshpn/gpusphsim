#pragma once

//-----------------------------------
// Forward declarations
//-----------------------------------

class ScalarField3D;
namespace SnowSim
{
	class OgreSimRenderable;
}
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
	void SetSceneSize(const float size_){ m_SceneSize = size_; }

	void SetSpaceResolution(const float sr){m_SpaceResolution = sr;}
	float GetSpaceResolution() const { return m_SpaceResolution; }

	void SetParticleRadius(const float pr){m_ParticleRadius = pr;}
	float GetParticleRadius() const { return m_ParticleRadius; }

	void SetParticlesEntity(SnowSim::OgreSimRenderable *particlesEntity){mParticlesEntity = particlesEntity;}
protected:
	float m_SceneSize;
	float m_SpaceResolution;
	float m_ParticleRadius;
	SnowSim::OgreSimRenderable *mParticlesEntity;

private:

};

extern float ScaleFactor;
