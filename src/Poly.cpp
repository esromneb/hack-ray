#include "Poly.hpp"

Poly::Poly()
{
//  points = new float2[100];
    num = 0;
    this->numNorm = 0;
    x.resize(VERTS);
    y.resize(VERTS);
    z.resize(VERTS);
    norm.resize(VERTS);
    triNorm.resize(TRIANGLES);

    abcnorm.resize( TRIANGLES / 3 );
    d.resize( TRIANGLES / 3 );


    red.resize(VERTS);
    blue.resize(VERTS);
    green.resize(VERTS);
    intensity.resize(VERTS);

    triangles.resize( TRIANGLES );
    edgeCount = 0;
    trianglePointCount = 0;

    ka = 0.25f;
    kd = Vec3( 0.9, 0.9, 0.9f );
    ks = 0.3;
    kr = 1;
    kt = 0;
    n = 2;

    for( int i = 0; i < TRIANGLES/3; i++ )
        this->dontDraw[i] = false;
}
Poly::~Poly()
{
}
void Poly::insert( float Xin, float Yin, float Zin )
{
    x[num] = Xin;
    y[num] = Yin;
    z[num] = Zin;
    num++;
}

void Poly::insertNorm( float Xin, float Yin, float Zin )
{
    norm[numNorm].data[0] = Xin;
    norm[numNorm].data[1] = Yin;
    norm[numNorm].data[2] = Zin;
    norm[numNorm] = norm[numNorm] / norm[numNorm].mag();
    numNorm++;
}

void Poly::insertTriangle( int *edges, int num )
{
    if( num > 3 )
    {
        int tmp[3];
        tmp[0] = edges[0];
        tmp[1] = edges[1];
        tmp[2] = edges[2];
        insertTriangle( tmp, 3 );
        int left = num - 1; //each triangle we draw gives us one less vert
        edges[1] = edges[0]; //overwright the one we eliminated (cause it was in the middle)
        insertTriangle( edges+1, left ); //incriment the pointer, and call again 
    }

    if( num == 3 )
    {
        int offset = trianglePointCount/3;
         Vec3 ab, ac, tmp;
         ab[0] = x[ triangles[offset+1] ] - x[ triangles[offset+0] ];
         ab[1] = y[ triangles[offset+1] ] - y[ triangles[offset+0] ];
         ab[2] = z[ triangles[offset+1] ] - z[ triangles[offset+0] ];
         ac[0] = x[ triangles[offset+2] ] - x[ triangles[offset+0] ];
         ac[1] = y[ triangles[offset+2] ] - y[ triangles[offset+0] ];
         ac[2] = z[ triangles[offset+2] ] - z[ triangles[offset+0] ];
         ab.normalize();
         ac.normalize();
         abcnorm[ offset ] = Vec3::cross( ab, ac );
         /*a[offset] = n[0];
         b[offset] = n[1];
         c[offset] = n[2];*/
         tmp[0] = x[ triangles[offset+0] ];
         tmp[1] = y[ triangles[offset+0] ];
         tmp[2] = z[ triangles[offset+0] ];
         d[offset] = -1* abcnorm[offset].dot( tmp );

        triangles[ trianglePointCount++ ] = edges[0];
        triangles[ trianglePointCount++ ] = edges[1];
        triangles[ trianglePointCount++ ] = edges[2];
    }

}

void Poly::calcLighting( vector<double>* viewX, vector<double>* viewY, int view )
{
    (void)viewX;
    (void)viewY;
    (void)view;
/*    extern TreDEngine *globalEngine;
    vec from(3), fp(3), r(3), tmp(3);
    double max_inten = -999999;
    switch( view )
    {
    case DROP_X:
        from.data[0] = 1;
        from.data[1] = 0;
        from.data[2] = 0;
    break;

    case DROP_Y:
        from.data[0] = 0;
        from.data[1] = 1;
        from.data[2] = 0;
    break;

    case DROP_Z:
        from.data[0] = 0;
        from.data[1] = 0;
        from.data[2] = 1;
    break;
    }

    for( int i = 0; i < this->numNorm; i++ )
    {
        
        fp = from - norm[i];

//        tmp = norm[i]*2;
//        r = tmp.dot( globalEngine->source );
//        r = r * ( norm[i] - globalEngine->source );

        tmp = norm[i] - globalEngine->source;
        r = tmp * 2 * norm[i].dot(globalEngine->source);
        
        r = r / r.mag();

        //we aleways have ambient light
        intensity[i] = ka * globalEngine->ia;
        
        //we only add diffuse light if light and viewer and light source on same side
        //if( (globalEngine->source.dot( norm[i] ) > 0 && norm[i].dot( from ) > 0 ) || (globalEngine->source.dot( norm[i] ) < 0 && norm[i].dot( from ) < 0 ) )
        if( (globalEngine->source.dot( norm[i] ) > 0 ) )
            intensity[i] += ( globalEngine->il / ( fp.mag() + globalEngine->c )  )*( kd * globalEngine->source.dot( norm[i] ) + ks * pow( r.dot( globalEngine->source ), n) );
//        intensity[i] = ka * globalEngine->ia + ( globalEngine->il / ( fp.mag() + globalEngine->c )  )*( kd * globalEngine->source.dot( norm[i] ) );

        max_inten = max( max_inten, intensity[i] );
//        cout << "For vert " << i << " total intensity= " << intensity[i] << " ka: " << ka << "ia: " <<  globalEngine->ia << " kd: " << kd << " fp.mag: " << fp.mag() << " C: " << globalEngine->c << " max_intensity is " << max_inten << endl;
        
    }

    //normalize at the end
    for( int i = 0; i < this->numNorm; i++ )
    {
        intensity[i] /= max_inten;
        //cout << "final int("<< i << "): " << intensity[i] << endl;
    }
    */
}