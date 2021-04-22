#include "Matrix.hpp"

#include "Vec.hpp"


Matrix::Matrix(int xin, int yin): x(xin), y(yin)
{
    this->data = new double*[x];
    double * pool_y = new double[x*y];
    for(int i_x = 0; i_x < x; ++i_x)
    {
        this->data[i_x] = pool_y;
        pool_y += y;
        /*for(int i_y = 0;i_y < BOX_PER_DIM;++i_y){
            this->Quadrants[i_x][i_y] = pool_z;
            pool_z +=BOX_PER_DIM;
        }*/
    }
}


//WARNING NOT TESTED
double Matrix::det( void )
{
    if( x != y )
    {
        return 0;
    }

    if( x == 2 )
        return data[0][0]*data[1][1] - data[1][0]*data[0][1];

    if( x == 3 )
    {
        //Matrix m1(2,2);
        //Matrix m2(2,2);
        //Matrix m3(2,2);
        return data[0][0]*data[1][1]*data[2][2] - data[0][0]*data[2][1]*data[1][2] - data[1][0]*data[0][1]*data[2][2] + data[2][0]*data[0][1]*data[1][2] + data[1][0]*data[2][1]*data[0][2] - data[2][0]*data[1][1]*data[0][2];
        m1->data[0][0] = data[1][1];
        m1->data[1][0] = data[2][1];
        m1->data[0][1] = data[1][2];
        m1->data[1][1] = data[2][2];

        m2->data[0][0] = data[0][1];
        m2->data[1][0] = data[2][1];
        m2->data[0][1] = data[0][2];
        m2->data[1][1] = data[2][2];

        m3->data[0][0] = data[0][1];
        m3->data[1][0] = data[1][1];
        m3->data[0][1] = data[0][2];
        m3->data[1][1] = data[1][2];

        return data[0][0]* m1->det() - data[1][0]* m2->det() + data[2][0]*m3->det();
    }

    return 0;
}

Vec Matrix::operator *(const Vec &rhs)
{
    if( x != (signed)rhs.y )
    {
        Vec bad(0);// = new Vec(0);
        return bad;
    }

    Vec ret(y);// = new vec(y);
    for( int i = 0; i < x; i++ )
    {
        ret.data[i] = 0;
        for( int j = 0; j < y; j++ )
        {
            ret.data[i] += (this->data)[j][i] * (rhs.data)[j];
        }
    }

    return ret;
}



Matrix::~Matrix( void )
{
    delete[] *data;
    delete[] data;
}