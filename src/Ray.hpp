#pragma once

#include "Vec3.hpp"

class Ray
{
public:
    Ray();
    Ray(const Vec3& _o, const Vec3& _d);
    Vec3 o = {0,0,0};
    Vec3 d = {0,0,0};
    Vec3 pointAt(const double t) const;
};

