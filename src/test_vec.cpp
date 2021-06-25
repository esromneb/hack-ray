#include "Vec.hpp"
#include "Material.hpp"

#include <iostream>
#include <tuple>

using namespace std;



int test_add(void) {
    Vec a(3);
    Vec b(3);

    a.data[0] = 1;
    a.data[1] = 0;
    a.data[2] = 0;

    b.data[0] = 2;
    b.data[1] = 0;
    b.data[2] = 0;

    auto c = a+b;

    cout << "C: " << c.str(false) << "\n";

    if( c.data[0] != 3 || c.data[1] != 0 || c.data[2] != 0 ) {
        return 1;
    }

    return 0;
}

int test_fail(void) {
    return 0;
}

typedef std::tuple<uint8_t, size_t> hit_t;
int test_print(void) {

    Vec a(3);
    Vec b(4);

    a.data[0] = 1;
    a.data[1] = 2;
    a.data[2] = 3;

    b.data[0] = 10;
    b.data[1] = 11;
    b.data[2] = 12;
    b.data[3] = 13;

    cout << "A: " << a.str(false) << "\n";
    cout << "B:\n" << b.str(true) << "\n";
    cout << "B: " << b.str() << "\n\n\n\n";

    // hit_t bestHit;

    // bestHit = std::make_tuple((uint8_t)4, 44);

    // auto [ hitWhat, index ] = bestHit;

    // cout << "hit what " << (int)hitWhat << " at index " << index << "\n";

    return 0;
}

int main(void) {
    cout << "in test vec\n";

    unsigned all_results = 0;
    unsigned test = 0;

    test = test_add();
    all_results |= (test==0)<<0;

    test = test_fail();
    all_results |= (test==0)<<1;

    test = test_print();
    all_results |= (test==0)<<2;


    if( all_results != 0x7 ) {
        cout << "FAIL " << all_results << "\n";
        return 1;
    }


    return 0;

}

