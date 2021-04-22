#include "Vec3.hpp"
#include "Vec.hpp"

#include <iostream>
#include <cmath>

using namespace std;

#ifdef asdf
Left: [-0.894874,-0.446318,0.001119]
Right: [-0.894874,0.446318,0.001119]
#endif

int main3(void) {
    cout << "Hello World\n";
    Vec v0(2);
    v0.data[1] = 43;
    return 0;
}


int main2(void) {
    cout << "foo\n";

    Vec v0(2);

    v0.data[0] = 2;
    v0.data[1] = sqrt(12);

    double _len = pow(v0.data[0],2) + pow(v0.data[1],2);

    double len = sqrt(_len);

    double fnLen = v0.mag();



    cout << "Len:   " << len << "\n";
    cout << "FnLen: " << fnLen << "\n";

    return 0;
}


int main4(void) {
    cout << "foo\n";

    Vec v0(2);

    v0.data[0] = -0.894874;
    v0.data[1] = -0.446318;


    Vec v1(2);

    v1.data[0] = -0.894874;
    v1.data[1] = 0.446318;

    double dotprod = v0.dot(v1);

    double inner = dotprod / (v0.mag()*v1.mag());

    double outer = acos(inner) * 180.0 / M_PI;


    double fnAngle0 = Vec::angle(v0,v1);
    double fnAngle1 = Vec::angle<false>(v0,v1);

    cout << "our: " << outer << "\n";
    cout << "fn : " << fnAngle0 << "\n";
    cout << "fn : " << fnAngle1 << "\n";


    // cout << v0.str(false) << "\n";
    // cout << v1.str(false) << "\n";

    return 0;
}


int main5(void) {
    // cout << "foo\n";

    Vec v0(2);

    v0.data[0] = 1;
    v0.data[1] = 1;


    Vec v1(2);

    v1.data[0] = 1;
    v1.data[1] = 1;

    double fnAngle0 = Vec::angle(v0,v1);

    cout << "fn : " << fnAngle0 << "\n";


    // cout << v0.str(false) << "\n";
    // cout << v1.str(false) << "\n";

    return 0;
}

int main(void) {
    // cout << "foo\n";

    Vec v0(3, 0,1,3);

    cout << "Loaded " << v0.str(false) << "\n";


    Vec v1(3);

    v1.data[0] = 1;
    v1.data[1] = 1;
    v1.data[2] = 3;

    double fnAngle0 = Vec::angle(v0,v1);

    cout << "fn : " << fnAngle0 << "\n";


    // cout << v0.str(false) << "\n";
    // cout << v1.str(false) << "\n";

    return 0;
}

