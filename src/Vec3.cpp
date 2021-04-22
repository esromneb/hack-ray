#include "Vec3.hpp"
#include <math.h>

Vec3::Vec3( const Vec3 &in )
{
//    y = in.y;
//    data = new double[y];
    for( int i = 0; i < 3; i++ )
        this->data[i] = in.data[i];
}

Vec3& Vec3::operator= (const Vec3 &rhs)
{
    if (this == &rhs)      // Same object?
      return *this;        
/*
    if( y != rhs.y )
    {
        Vec3 *bad = new Vec3(0);
        return *bad;
    }
*/
    for( int i = 0; i < 3; i++ )
    {
        this->data[i] = rhs.data[i];
    }

    return *this;
}


const double Vec3::operator[] (const int index) const
{
    return data[index];
}

double& Vec3::operator[] (const int index)
{
    return data[index];
}
double Vec3::mag( void ) const
{
    double ret = 0;
    for( int i = 0; i < 3; i++ )
    {
        ret += data[i]*data[i];
    }

    ret = sqrt( ret );
    return ret;
}

double Vec3::dot( const Vec3 &rhs ) const
{

    Vec3 tmp = (*this)*rhs;
    double ret = 0;

    for( int i = 0; i < 3; i++ ) {
        ret += tmp[i];
    }

    return ret;
}

double Vec3::dot( const Vec3 &lhs, const Vec3 &rhs )
{

    Vec3 tmp = lhs*rhs;
    double ret = 0;

    for( int i = 0; i < 3; i++ ) {
        ret += tmp[i];
    }

    return ret;
}




Vec3 Vec3::operator *( const Vec3 &rhs) const
{
    Vec3 ret;

    for( int i = 0; i < 3; i++ ) {
        ret.data[i] = data[i] * rhs.data[i];
    }

    return ret;
}

Vec3 Vec3::operator* ( double rhs ) const
{
    Vec3 ret;
    for( int i = 0; i < 3; i++ ) {
        ret.data[i] = data[i] * rhs;
    }
    return ret;
}

Vec3 Vec3::operator/ ( double rhs ) const
{
    Vec3 ret;// = new Vec3(y);
    for( int i = 0; i < 3; i++ ) {
        ret.data[i] = data[i] / rhs;
    }
    return ret;
}

const Vec3 Vec3::operator+ ( const Vec3 &rhs ) const
{
    Vec3 ret;// = new Vec3(y);
    for( int i = 0; i < 3; i++ ) {
        ret.data[i] = data[i] + rhs.data[i];
    }
    return ret;
}
const Vec3 Vec3::operator- ( const Vec3 &rhs ) const
{
    Vec3 ret;// = new Vec3(3);
    for( int i = 0; i < 3; i++ ) {
        ret.data[i] = data[i] - rhs.data[i];
    }
    return ret;
}
const Vec3 Vec3::operator+ ( const double rhs ) const
{
    Vec3 ret;// = new Vec3(y);
    for( int i = 0; i < 3; i++ ) {
        ret.data[i] = data[i] + rhs;
    }
    return ret;
}
Vec3 Vec3::cross( const Vec3 &left, const Vec3 &right )
{
    Vec3 ret;
    ret.data[0] = left[1] * right[2] - left[2]*right[1];
    ret.data[1] = left[2] * right[0] - left[0] * right[2];
    ret.data[2] = left[0] * right[1] - left[1] * right[0];
    return ret;
}

void Vec3::normalize( void )
{
    const double mag = this->mag();
    data[0] = data[0] / mag;
    data[1] = data[1] / mag;
    data[2] = data[2] / mag;
}

Vec3 Vec3::normalize(const Vec3 &rhs) {
    const double mag = rhs.mag();

    Vec3 ret (
        rhs.data[0] / mag,
        rhs.data[1] / mag,
        rhs.data[2] / mag
        );

    return ret;
}

std::string Vec3::str(bool vertical) const {
    std::string out;

    if( vertical ) {
        out += "x: ";
        out += std::to_string(data[0]);
        out += "\ny: ";
        out += std::to_string(data[1]);
        out += "\nz: ";
        out += std::to_string(data[2]);
        out += "\n";
    } else {
        out = "[";
        out += std::to_string(data[0]);
        out += ",";
        out += std::to_string(data[1]);
        out += ",";
        out += std::to_string(data[2]);
        out += "]";
    }

    return out;
}

void Vec3::saturateMin( const double min )
{
    for( int i = 0; i < 3; i++ ) {
        if( data[i] < min ) {
            data[i] = min;
        }
    }
}

Vec3 Vec3::reflect(const Vec3 ray, const Vec3 normal) {
    Vec3 rhs = normal * ray.dot(normal) * 2;
    Vec3 ret = ray - rhs;
    return ret;
}
