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

	void NewFieldBuffer(Ogre::Vector4* &ogreHardwareBuffer, const std::size_t size);
	void UpdateFieldBuffer(Ogre::Vector4* &ogreHardwareBuffer, const std::size_t size);
	void DeleteFieldBuffer();

	virtual float Scalar(const Vector3& position) const override;
	virtual Vector3 Gradient(const Vector3& position) const override;
	virtual ColourValue Color(const Vector3& position) const override;

	//std::vector<ScalarField3D*> m_fields;
	//std::vector<Ogre::Vector3> m_spherePosition;
	Ogre::Vector3 *m_spherePosition;
	std::size_t m_spherePositionBufElementSize;//how many element in the buffer.


	void SetSphereRaidusSquared(const Ogre::Real raidus){ m_radiusSquared = raidus*raidus; }

	Ogre::Real m_radiusSquared;
};
