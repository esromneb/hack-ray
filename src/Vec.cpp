#include "Vec.hpp"
#include "Matrix.hpp"

#include <cmath>

using namespace std;

Vec::Vec( const Vec &in )
{
    y = in.y;
    data = new double[y];
    for( unsigned i = 0; i < y; i++ ) {
        this->data[i] = in.data[i];
    }
}

Vec::Vec( const unsigned yin ): y(yin) {
    data = new double[yin];
}

Vec::Vec( const unsigned yin, const double d0 ): y(yin) {
    data = new double[yin];
    if( yin > 0 ) {
        data[0] = d0;
    }
}

Vec::Vec( const unsigned yin, const double d0, const double d1 ): y(yin) {
    data = new double[yin];
    if( yin > 0 ) {
        data[0] = d0;
    }
    if( yin > 1 ) {
        data[1] = d1;
    }
}

Vec::Vec( const unsigned yin, const double d0, const double d1, const double d2 ): y(yin) {
    data = new double[yin];
    if( yin > 0 ) {
        data[0] = d0;
    }
    if( yin > 1 ) {
        data[1] = d1;
    }
    if( yin > 2 ) {
        data[2] = d2;
    }
}

Vec::Vec()
{
    y = DEFAULT_VEC;
    data = new double[DEFAULT_VEC];
}
Vec& Vec::operator= (const Vec &rhs)
{
    if (this == &rhs)      // Same object?
      return *this;        

    if( y != rhs.y )
    {
        Vec *bad = new Vec(0);
        return *bad;
    }

    for( unsigned i = 0; i < y; i++ )
    {
        this->data[i] = rhs.data[i];
    }

    return *this;
}
Vec Vec::operator* ( const Vec &rhs ) const
{

    if( y != rhs.y )
    {
        Vec bad(0);// = new Vec(0);
        return bad;
    }

    Vec ret(y);// = new Vec(y);

    for( unsigned i = 0; i < y; i++ )
        ret.data[i] = data[i] * rhs.data[i];

    return ret;
}

double Vec::operator[] (const unsigned index)
{
    return data[index];
}

double Vec::mag( void ) const
{
    double ret = 0;
    for( unsigned i = 0; i < y; i++ )
    {
        ret += data[i]*data[i];
    }

    ret = sqrt( ret );
    return ret;
}

double Vec::dot( const Vec &rhs ) const
{
    if( y != rhs.y )
    {
        return 0;
    }

    Vec tmp = (*this)*rhs;
    double ret = 0;

    for( unsigned i = 0; i < y; i++ )
        ret += tmp[i];

    return ret;
        
}
Vec Vec::operator* ( double rhs )
{
    Vec ret(y);// = new Vec(y);
    for( unsigned i = 0; i < y; i++ )
    {
        ret.data[i] = data[i] * rhs;
    }
    return ret;
}

Vec Vec::operator/ ( double rhs )
{
    Vec ret(y);// = new Vec(y);
    for( unsigned i = 0; i < y; i++ )
    {
        ret.data[i] = data[i] / rhs;
    }
    return ret;
}

Vec Vec::operator+ ( Vec &rhs )
{
    if( y != rhs.y )
    {
        Vec bad(0);// = new Vec(0);
        return bad;
    }
    Vec ret(y);// = new Vec(y);
    for( unsigned i = 0; i < y; i++ )
    {
        ret.data[i] = data[i] + rhs.data[i];
    }
    return ret;
}
Vec Vec::operator- ( Vec &rhs )
{
    if( y != rhs.y )
    {
        Vec bad(0);// = new Vec(0);
        return bad;
    }
    Vec ret(y);// = new Vec(y);
    for( unsigned i = 0; i < y; i++ )
    {
        ret.data[i] = data[i] - rhs.data[i];
    }
    return ret;
}

Vec cross( Vec &left, Vec &right )
{

    if( left.y != right.y || left.y != 3 )
    {
        Vec bad(0);// = new Vec(0);
        return bad;
    }

    Vec ret(3);
    ret.data[0] = left[1] * right[2] - left[2]*right[1];
    ret.data[1] = left[2] * right[0] - left[0] * right[2];
    ret.data[2] = left[0] * right[1] - left[1] * right[0];
    return ret;


}

void Vec::rot_y( const double theta )
{
    Vec orig(4), transformed(4);
    Matrix rotM( 4, 4 );

    orig.data[3] = 1;

    rotM.data[0][0] = cos( theta );
    rotM.data[1][0] = 0;
    rotM.data[2][0] = sin(theta);;
    rotM.data[3][0] = 0;


    rotM.data[0][1] = 0;
    rotM.data[1][1] = 1;
    rotM.data[2][1] = 0;
    rotM.data[3][1] = 0;

    rotM.data[0][2] = -1*sin( theta );
    rotM.data[1][2] = 0;
    rotM.data[2][2] = cos(theta);
    rotM.data[3][2] = 0;

    rotM.data[0][3] = 0;
    rotM.data[1][3] = 0;
    rotM.data[2][3] = 0;
    rotM.data[3][3] = 1;

    orig.data[0] = data[0];
    orig.data[1] = data[1];
    orig.data[2] = data[2];

    transformed = rotM*orig;

    data[0] = transformed[0];
    data[1] = transformed[1];
    data[2] = transformed[2];
}


void Vec::rot_x( const double theta )
{
    Vec orig(4), transformed(4);
    Matrix rotM( 4, 4 );

    orig.data[3] = 1;

    rotM.data[0][0] = 1;
    rotM.data[1][0] = 0;
    rotM.data[2][0] = 0;
    rotM.data[3][0] = 0;


    rotM.data[0][1] = 0;
    rotM.data[1][1] = cos(theta);
    rotM.data[2][1] = -1*sin( theta );
    rotM.data[3][1] = 0;

    rotM.data[0][2] = 0;
    rotM.data[1][2] = sin(theta);
    rotM.data[2][2] = cos( theta );
    rotM.data[3][2] = 0;

    rotM.data[0][3] = 0;
    rotM.data[1][3] = 0;
    rotM.data[2][3] = 0;
    rotM.data[3][3] = 1;

    orig.data[0] = data[0];
    orig.data[1] = data[1];
    orig.data[2] = data[2];

    transformed = rotM*orig;

    data[0] = transformed[0];
    data[1] = transformed[1];
    data[2] = transformed[2];
}

// https://math.stackexchange.com/questions/651413/given-the-degrees-to-rotate-around-axis-how-do-you-come-up-with-rotation-matrix
// each new line here goes left-right across the matrix
void Vec::rot_z( const double theta )
{
    Vec orig(4), transformed(4);
    Matrix rotM( 4, 4 );

    orig.data[3] = 1;

    rotM.data[0][0] = cos(theta);
    rotM.data[1][0] = -1*sin(theta);
    rotM.data[2][0] = 0;
    rotM.data[3][0] = 0;


    rotM.data[0][1] = sin(theta);
    rotM.data[1][1] = cos(theta);
    rotM.data[2][1] = 0;
    rotM.data[3][1] = 0;

    rotM.data[0][2] = 0;
    rotM.data[1][2] = 0;
    rotM.data[2][2] = 1;
    rotM.data[3][2] = 0;

    rotM.data[0][3] = 0;
    rotM.data[1][3] = 0;
    rotM.data[2][3] = 0;
    rotM.data[3][3] = 1;

    orig.data[0] = data[0];
    orig.data[1] = data[1];
    orig.data[2] = data[2];

    transformed = rotM*orig;

    data[0] = transformed[0];
    data[1] = transformed[1];
    data[2] = transformed[2];
}

std::string Vec::str(bool vertical) const {
    std::string out = "";

    if( vertical ) {
        for(unsigned i = 0; i < y; i++) {
            out += std::to_string(data[i]);
            out += "\n";
            // cout << data[0] << "\n";
        }
    } else {
        out = "[";
        for(unsigned i = 0; i < y; i++) {
            out += std::to_string(data[i]);
            out += ",";
            // cout << data[0] << "\n";
        }
        out.resize(out.size()-1);
        out += "]";
    }
    return out;
}

// returns angle between 2 vectors, in degrees or radians
// seems like this is only correct for y = 2
template <bool degrees>
double Vec::angle(const Vec &v0, const Vec &v1) {
    if( v0.y != v1.y ) {
        return 0;
    }

    const double dotprod = v0.dot(v1);
    const double inner = dotprod / (v0.mag()*v1.mag());

    const double rads = acos(inner);
    if constexpr (degrees) {
        return rads * 180 / M_PI;
    } else {
        return rads;
    }
}
template double Vec::angle<false>(const Vec &, const Vec &);
template double Vec::angle<true>(const Vec &, const Vec &);


Vec::~Vec( void )
{
    if( data && y != 0 )
        delete[] data;
}

