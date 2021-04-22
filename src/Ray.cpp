#include "Ray.hpp"

Ray::Ray() : o(0,0,0), d(0,0,0) {

}

Ray::Ray(const Vec3& _o, const Vec3& _d): o(_o), d(_d) {
    // empty
}

Vec3 Ray::pointAt(const double t) const {
    return Vec3( o[0] + d[0]*t,
                 o[1] + d[1]*t,
                 o[2] + d[2]*t);
}
