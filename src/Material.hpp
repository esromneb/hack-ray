#pragma once

#include "Vec3.hpp"

class Material {
public:
    // %ambient
    float ka = 0;

    // %specular
    float ks = 0;

    // %reflected
    float kr = 0;

    // %transmitted
    float kt = 0;

    // %diffuse (also specifies color of this object)
    Vec3 kd = {1,0,0};

    // specilar exponent (how tight is specular reflection)
    unsigned n = 0;

    // index of refraction
    // Vacuum 1
    // Air 1.000293
    // Water 1.333
    // Ice 1.31
    // Window glass 1.52
    // Diamond 2.42
    float refraction = 1;

    constexpr static unsigned defaultN = 32;
    constexpr static float defaultRefraction = 1;
};
