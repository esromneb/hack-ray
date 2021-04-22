#pragma once
#include <string>

class Vec3 {
public:
    double data[3];
    Vec3( const Vec3 &in );
    constexpr Vec3( const double x, const double y, const double z ) : data{x,y,z}{};
    constexpr Vec3() : data{0,0,0}{};
    Vec3& operator= (const Vec3 &rhs);
    Vec3 operator* ( const Vec3 &rhs ) const;
    Vec3 operator* ( double rhs ) const;
    Vec3 operator/ ( double rhs ) const;
    const Vec3 operator+ ( const Vec3 &rhs ) const;
    const Vec3 operator- ( const Vec3 &rhs ) const;
    const Vec3 operator+ ( const double rhs ) const;
    const double operator[] ( const int index ) const;
    double& operator[] ( const int index );
    double mag( void ) const;
    double dot( const Vec3 &rhs ) const;
    static double dot( const Vec3 &lhs, const Vec3 &rhs );
    void rot_y( double theta );
    void rot_x( double theta );
    void normalize( void );
    static Vec3 normalize(const Vec3 &rhs);
    std::string str(bool vertical = true) const;
    void saturateMin( const double min );
    static Vec3 reflect(const Vec3 ray, const Vec3 normal);
    static Vec3 cross( const Vec3 &left, const Vec3 &right );
};

