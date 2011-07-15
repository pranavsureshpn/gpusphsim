#pragma once

#include "Common.h"
#include "AABBox.h"

//-----------------------------------
// ScalarFieldValue
//-----------------------------------

struct ScalarFieldValue
{
	float Scalar;
	Vector3 Gradient;
	ColourValue Color;
};

//-----------------------------------
// ScalarField3D
//-----------------------------------

class ScalarField3D
{
public:
	ScalarField3D();
	virtual ~ScalarField3D();

	virtual float Scalar(const Vector3& position) const = 0;
	virtual Vector3 Gradient(const Vector3& position) const = 0;
	virtual ColourValue Color(const Vector3& position) const = 0;

    virtual const AABBox* BoundingBox() const { return NULL; }

	void SetColor(ColourValue color) { m_color = color; }

protected:
	ColourValue m_color;
};
