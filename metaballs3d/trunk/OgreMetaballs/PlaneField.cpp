#include "PlaneField.h"

//-----------------------------------
// PlaneField
//-----------------------------------

PlaneField::PlaneField(Vector3 axis, float origin)
{
	m_axis = axis;
	m_axis.normalise();

	m_origin = origin;
}

PlaneField::~PlaneField()
{
}

float PlaneField::Scalar(const Vector3& position) const
{
	Vector3 relativePos = position - m_origin * m_axis;
	return relativePos.dotProduct(m_axis)+1;
}

Vector3 PlaneField::Gradient(const Vector3& position) const
{
	return m_axis;
}

ColourValue PlaneField::Color(const Vector3& position) const
{
	//The color is uniform
	return m_color;
}