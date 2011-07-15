#include "AABBox.h"

//-----------------------------------
// AABBox
//-----------------------------------

AABBox::AABBox(Vector3 min, Vector3 max)
: Min(min), Max(max)
{
}

bool AABBox::Collide(const AABBox& other)
{
    return !(Min.x > other.Max.x || other.Min.x > Max.x ||
        Min.y > other.Max.y || other.Min.y > Max.y ||
        Min.z > other.Max.z || other.Min.z > Max.z);
}

bool AABBox::Contains(const Vector3& point) const
{
    return (Min.x < point.x && point.x < Max.x &&
       Min.y < point.y && point.y < Max.y &&
       Min.z < point.z && point.z < Max.z);
}