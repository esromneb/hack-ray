#include "JsApi.hpp"

#include <iostream>
#include <vector>
#include <stdint.h>
// #include <chrono>
// #include <unistd.h> //usleep
// #include <functional>
// #include <cmath>


#include <stdio.h>

#ifdef __EMSCRIPTEN__
#include <emscripten.h>
#endif



using namespace std;


///
/// These are all of the C++ -> Javascript
///


EM_JS(void, postB64, (const char* v, const int tag), {
    postB64ToMain(v, tag);
});


EM_JS(void, postFixed, (), {
    postToMain(0, "hello message0");
    postToMain(1, "hello message1");
    // alert('hello world!');
    // throw 'all done';
});

