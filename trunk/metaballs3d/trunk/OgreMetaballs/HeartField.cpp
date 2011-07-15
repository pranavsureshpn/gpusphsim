#include "HeartField.h"

#include <cmath>

HeartField::HeartField(void)
{
}

HeartField::~HeartField(void)
{
}

ScalarFieldValue HeartField::Sample(const Vector3& position) const
{
	ScalarFieldValue result;

	Vector3 relativePos = position;
	RealType x = 2 * relativePos.x;
	RealType y = relativePos.z;
	RealType z = 1.5f * relativePos.y;

	RealType x2 = x*x;
	RealType y2 = y*y;
	RealType z2 = z*z;

	RealType z3 = z2*z;

	RealType A = 2*x2 + 2*y2 + z2 - 1;
	RealType B = 0.1f*x2 + y2;

	RealType A2 = A*A;

	result.Scalar = 1-(A*A*A - B*z3);
	result.Gradient = -1.0f * Vector3(
		6*A2*x - 0.2f*x*z3,
		6*A2*y - 2*y*z3,
		3*A2*z - 3*B*z2);

	result.Color = m_color;

	return result;
}
