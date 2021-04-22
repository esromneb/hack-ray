#ifndef _FILEIO_CPP_
#define _FILEIO_CPP_

#include <iostream>
#include <fstream>
#include <string>
#include "fileio.h"

// #define FILENAME "unit_cube.txt"
// #define FILENAME "root_fs/scene2.txt"
// #define FILENAME "root_fs/unit_cube.txt"
// #define FILENAME "root_fs/scenep5.txt"
// #define FILENAME "root_fs/1tri.txt"
#define FILENAME "root_fs/1triB.txt"

using namespace std;

//string globalFaces[1000];
//int globalFaceCount = 0;
/*
void save( void )
{
	extern TreDEngine *globalEngine;
	ofstream file;
	file.open( FILENAME, ios::out );
	for( int i = 0; i < globalEngine->numPoly; i ++ )
	{
		if( globalEngine->polygons[ i ].num > 0 )
			file << "o" << endl;

		for( int j = 0; j < globalEngine->polygons[ i ].num; j++ )
		{
			file << "v " << globalEngine->polygons[ i ].x[j] << " " << globalEngine->polygons[ i ].y[j] << " " << globalEngine->polygons[ i ].z[j] << endl;
		}
	}

	for( int i = 0; i < globalFaceCount; i++ )
		file << globalFaces[i] << endl;

}*/


void readWaveFront( RayEngine &engine, bool verbose )
{
	char c;
	float x, y, z;
	string line;
	ifstream file;
	int tmpVerts[ 1000 ];
	int tmpNum;
	Vec3* verts = new Vec3[ 1000 ];
	int numVerts = 0;
	int vertOffset = 0;

//	int iNxt; //used in accordance with i from a for loop and it loops around

    cout << "About to open " << FILENAME;
    if( verbose ) {
        cout << " VERBOSE";
    }
    cout << "\n";

	file.open( FILENAME , ios::in );

	if( !file.is_open() )
		return;

    cout << "Open was successful: " << FILENAME << "\n";

	engine.numPoly = 0;

	while( !file.eof() )
	{
		getline( file, line );
		if( line.length() < 2 )
			continue;

		if( line[0] == 'o' && numVerts != 0 )
		{
			engine.numPoly++;
			vertOffset = numVerts;
            if( verbose ) {
                cout << "#poly from " << (engine.numPoly-1) << " to " << engine.numPoly << " at numVerts " << numVerts << " offset " << vertOffset << "\n";
            }
		}

		if( line[0] == 'v' && line[1] != 'n' )
		{
			sscanf( line.c_str() , "%c %f %f %f", &c, &x, &y, &z );
			
			verts[ numVerts ][0] = x;
			verts[ numVerts ][1] = y;
			verts[ numVerts ][2] = z;
			numVerts++;

            if( verbose ) {
                cout << "Insert Vert #" << (numVerts - 1) << ": " << x << ", " << y << ", " << z << "\n";
            }


			engine.polygons[ engine.numPoly ].insert( x, y, z );
		}

		if( line[0] == 'v' && line[1] == 'n' )
		{
			sscanf( line.c_str() , "%c%c %f %f %f", &c, &c, &x, &y, &z );
			engine.polygons[ engine.numPoly ].insertNorm( x, y, z );

            if( verbose ) {
                cout << "Insert Norm #" << engine.numPoly << ": " << x << ", " << y << ", " << z << "\n";
            }

		}

		if( line[0] == 'f' )
		{
            if( verbose ) {
                cout << "Insert Face #" << engine.numPoly << ": ";
            }

//			globalFaces[globalFaceCount] = line;   // used for saving?
//			globalFaceCount++;                     // used for saving?
			tmpNum = 0;
			for( unsigned int cur = 1; cur < line.length(); cur++ )
			{
				if( line[cur] == ' ' )
					continue;

				if( line[cur - 1] != ' ' )
					continue;

				tmpVerts[ tmpNum ] = atoi( line.c_str() + cur );
				tmpVerts[ tmpNum ] -= 1+vertOffset;

                if( verbose ) {
                    cout << tmpVerts[ tmpNum ] << ", ";
                }

				tmpNum++;
				
			}

			engine.polygons[ engine.numPoly ].insertTriangle( tmpVerts, tmpNum );

            cout << "\n";


			/*for( int i = 0; i < tmpNum; i++ )
			{
				iNxt = (i == (tmpNum - 1))?0:(i+1);

				engine.polygons[ engine.numPoly ].edges[ engine.polygons[ engine.numPoly ].edgeCount ] = tmpVerts[i] -1 - vertOffset;

				engine.polygons[ engine.numPoly ].edgeCount++;
				engine.polygons[ engine.numPoly ].edges[ engine.polygons[ engine.numPoly ].edgeCount ] = tmpVerts[iNxt] -1 - vertOffset;

				engine.polygons[ engine.numPoly ].edgeCount++;
			}*/
		}
	}


	engine.numPoly++;

    if( verbose  ) {
        cout << "Final Poly Count: " << engine.numPoly << "\n";
    }


	file.close();

}

#endif