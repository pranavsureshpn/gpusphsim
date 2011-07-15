#pragma once

#include "Common.h"
#include "ScalarField3D.h"

//-----------------------------------
// HeartField
//-----------------------------------

typedef float RealType;
class HeartField	: public ScalarField3D
{
public:
	HeartField();
	virtual ~HeartField();

	virtual ScalarFieldValue Sample(const Vector3& position) const override;
};
