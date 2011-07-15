#pragma once

#include "Common.h"
#include "ScalarField3D.h"

#include <cmath>

//-----------------------------------
// FieldHelper
//-----------------------------------

class FieldHelper
{

public:
	static void CentralExpField(float sphereRadius, const Vector3 sphereCenter, const Vector3 position, ScalarFieldValue& outValue)
	{
		Vector3 radialVector = (position - sphereCenter) / sphereRadius;
		float radiusSquared = radialVector.squaredLength();

		float res = exp(-radiusSquared);
		float res2 = res - 2;

		outValue.Scalar = exp(-radiusSquared) - 0.367879f;

		outValue.Gradient = -2 * outValue.Scalar * radialVector;
	}

	static ScalarFieldValue CentralField(float sphereRadius, const Vector3 sphereCenter, const Vector3 position)
	{
		// Spherical field with polynomial attenuation
		//
		// F(0) = 1						, r = 0
		// F(r) = 0						, r^2 >= 0.5f, 
		// F(r) = 4*(r^4 - r^2 + 0.25)	, 0 < r^2 < 0.5f 
		//
		// grad F(R) = 1/|R| * dF/dr . R
		// grad F(R) = (16r^2 - 8) . R

        ScalarFieldValue outValue;
 
        Vector3 radialVector = (position - sphereCenter) / sphereRadius;
		float radiusSquared = radialVector.squaredLength();

		outValue.Scalar = 0;
		outValue.Gradient = Vector3::ZERO;

		if(radiusSquared < 0.5f)
		{	
			outValue.Scalar = 4 * ( radiusSquared * radiusSquared - radiusSquared + 0.25f );
			outValue.Gradient = (16 * radiusSquared - 8) * radialVector;
		}

        return outValue;
	}
};
