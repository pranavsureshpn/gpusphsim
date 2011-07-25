#pragma once
#include "ScalarField3D.h"

class SphericalField;

//-----------------------------------
// MetaballField
//-----------------------------------

class MetaballField :
	public ScalarField3D
{
public:
	MetaballField();
	virtual ~MetaballField();

	void AddField(ScalarField3D* field);
	void RemoveField(ScalarField3D* field);

	void SetFieldBuffer(Ogre::Vector4* &ogreHardwareBuffer, const std::size_t size);
	void UpdateFieldBuffer(Ogre::Vector4* &ogreHardwareBuffer, const std::size_t size);

	virtual float Scalar(const Vector3& position) const override;
	virtual Vector3 Gradient(const Vector3& position) const override;
	virtual ColourValue Color(const Vector3& position) const override;

	//std::vector<ScalarField3D*> m_fields;
	std::vector<Ogre::Vector3> m_spherePosition;

	void SetSphereRaidusSquared(const Ogre::Real raidus){ m_radiusSquared = raidus*raidus; }

	Ogre::Real m_radiusSquared;
};
