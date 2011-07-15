#include "SphericalField.h"

#include "FieldHelper.h"

//-----------------------------------
// SphericalField
//-----------------------------------

SphericalField::SphericalField(const Vector3& center, float radius)
: m_center(center), m_radiusSquared(radius * radius), m_radius(radius)
{
}

SphericalField::~SphericalField()
{
}

float SphericalField::Scalar(const Vector3& position) const
{
	//The scalar value is proportional to 1/r^2
	float lengthSquared = position.squaredDistance(m_center);
	return m_radiusSquared / lengthSquared;
}

Vector3 SphericalField::Gradient(const Vector3& position) const
{
	//Compute the relative position from the center
	//The gradient is is proportional to 1/r^3 and radial to the sphere
	Vector3 relativePos = m_center - position;
	float lengthSquared = relativePos.squaredLength();
	return (2 * m_radiusSquared / (lengthSquared*lengthSquared)) * relativePos;
}

ColourValue SphericalField::Color(const Vector3& position) const
{
	//The color is uniform
	return m_color;
}