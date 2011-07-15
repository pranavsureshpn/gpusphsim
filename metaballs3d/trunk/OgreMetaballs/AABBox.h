#pragma once

#include "Common.h"

//-----------------------------------
// AABBox
//-----------------------------------

struct AABBox
{
public:
    AABBox(Vector3 min, Vector3 max);

    bool Collide(const AABBox& other);
    bool Contains(const Vector3& point) const;

    Vector3 Min;
    Vector3 Max;
};