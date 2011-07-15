#pragma once

#include "Common.h"
#include "ScalarField3D.h"

//-----------------------------------
// ToroidalField
//-----------------------------------

class ToroidalField : public ScalarField3D
{
public:
	ToroidalField(const Vector3& center, float radius, float sectionRadius);
	virtual ~ToroidalField();

	void SetSectionRadius(float r) { m_sectionRadius = r; UpdateBoundingBox(); }
	void SetRadius(float r) { m_radius = r; UpdateBoundingBox(); }

	virtual float Scalar(const Vector3& position) const override;
	virtual Vector3 Gradient(const Vector3& position) const override;
	virtual ColourValue Color(const Vector3& position) const override;

    virtual const AABBox* BoundingBox() const override { return &m_boundingBox; }

protected:
    void UpdateBoundingBox() { m_boundingBox = AABBox(m_center, Vector3(m_radius, m_sectionRadius, m_radius)); }

private:
	float m_radius;
	float m_sectionRadius;
	Vector3 m_center;

    AABBox m_boundingBox;
};