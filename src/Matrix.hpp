#pragma once

class Vec;

class Matrix {
public:
    int x;
    int y;
    Matrix( int xin, int yin );
    ~Matrix( void );
    Vec operator* ( const Vec &rhs );
    double ** data;
    double det( void );
    Matrix *m1, *m2, *m3;
};
