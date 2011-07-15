#include "ToroidalField.h"

//-----------------------------------
// ToroidalField
//-----------------------------------

ToroidalField::ToroidalField(const Vector3& center, float radius, float sectionRadius)
: m_center(center), m_sectionRadius(sectionRadius), m_radius(radius)
{
}

ToroidalField::~ToroidalField()
{
}

float ToroidalField::Scalar(const Vector3& position) const
{
	//Compute the direction from the torus center to the sampled point.
	Vector3 centerToPos = position - m_center;

	//Project the former vector on the torus plane to get the direction to the section center.
	centerToPos.y = 0;

	//Eliminate the singularity on the torus axis by choosing an arbitrary direction
	if(centerToPos == Vector3::ZERO)
	{
		centerToPos = Vector3::UNIT_X;
	}

	//Compute the center of the cylindrical section along the radial direction.
	Vector3 sectionCenter = centerToPos.normalisedCopy() * m_radius;

	float lengthSquared = position.squaredDistance(sectionCenter);

	//The scalar value is proportional to 1/r^2
	return m_sectionRadius * m_sectionRadius / lengthSquared;
}

Vector3 ToroidalField::Gradient(const Vector3& position) const
{
	//Compute the direction from the torus center to the sampled point.
	Vector3 centerToPos = position - m_center;

	//Project the former vector on the torus plane to get the direction to the section center.
	centerToPos.y = 0;

	//Eliminate the singularity on the torus axis by choosing an arbitrary direction
	if(centerToPos == Vector3::ZERO)
	{
		centerToPos = Vector3::UNIT_X;
	}

	//Compute the center of the cylindrical section along the radial direction.
	Vector3 sectionCenter = centerToPos.normalisedCopy() * m_radius;

	//Compute the relative position of the sampled point
	Vector3 relativePos = sectionCenter - position;

	float lengthSquared = relativePos.squaredLength();

	//The gradient is is proportional to 1/r^3 and radial to the center of the section
	return 2 * m_sectionRadius * m_sectionRadius / (lengthSquared*lengthSquared) * relativePos;
}

ColourValue ToroidalField::Color(const Vector3& position) const
{
	//The color is uniform
	return m_color;
}
