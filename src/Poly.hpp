#pragma once

#include "Macros.hpp"
#include "Vec3.hpp"
#include <vector>

using namespace std; //fixme remove

class Poly
{
    public:
    int num, numNorm; //should end out the same
    vector<double> x;
    vector<double> y;
    vector<double> z;

    vector<Vec3> norm;
    vector<Vec3> triNorm;

    vector<double> red;
    vector<double> green;
    vector<double> blue;

    vector<double> intensity;
    bool dontDraw[TRIANGLES/3];
    //vector<int> edges; //this will be sized num*2 cause the edges are stored in index 0,1 and 2,3 etc...
    vector<int> triangles;
    int edgeCount;
    void insert( float Xin, float Yin, float Zin );
    void insertNorm( float Xin, float Yin, float Zin );
    void insertTriangle( int *edges, int num );
    int trianglePointCount;
    void translate( float tx, float ty, float tz );
    void scale( float px, float py, float pz, float sx, float sy, float sz );
    void rotate( double px, double py, double theta );
/*    void rot_free( vec p1, vec p2, double theta );
    void rot_x( double theta );
    void rot_y( double theta );
    void rot_z( double theta );
//    void MatrixMult( Matrix &in );
    void old_rast(void);
    void rast( int view );

    */
    void setNorm(int which);
//    void color( int tri0, int x, int y, vector<double>* viewX, vector<double>* viewY, float &r, float &g, float &b ); //the triangle index of the 0th vert
    void findIntensity(int tri0, int x, int y, vector<double>* viewX, vector<double>* viewY, float &intensity);
    float area; //MUST BE UPDATED before calling color  This float has no meanning otherwise

    //lighting stuff
    void calcLighting( vector<double>* viewX, vector<double>* viewY, int view );
    float ka, ks, kr, kt;
    Vec3 kd;
    int n;
    /*vector<double> a;
    vector<double> b;
    vector<double> c;*/
    vector<Vec3> abcnorm;
    vector<double> d;
    //double a,b,c,d;

    Poly();
    ~Poly();
};
