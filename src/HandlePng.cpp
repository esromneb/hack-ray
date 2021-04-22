#include "HandlePng.hpp"
#include "RayEngine.hpp"

#include "lodepng.h"

#include <iostream>
// #include <cmath>
#include <string>
#include <cstdint>
#include <chrono>




//The image argument has width * height RGBA pixels or width * height * 4 bytes
static std::tuple<unsigned,std::string> encodeOneStepp(const char* const filename, std::vector<unsigned char>& image, const unsigned width, const unsigned height) {
    //Encode the image
    const unsigned error = lodepng::encode(filename, image, width, height);

    std::string s = "";

    //if there's an error, display it
    if(error) {
        s = "PNG encoder error #" + std::to_string(error) + ": " + std::string(lodepng_error_text(error));
        std::cout << s << "\n";
    }

    return std::make_tuple(error, s);
}


///
/// Saves the engine's current render buffer to a png file on disk
///
std::tuple<unsigned,std::string> HandlePng::save(const std::string& path, RayEngine* engine) {

    std::vector<unsigned char> imageBuf;

    // cout << "Before copy\n";
    engine->copyToPixels(imageBuf);
    // cout << "After copy\n";

    return encodeOneStepp(path.c_str(), imageBuf, engine->xPx, engine->yPx);
}



/// Decode from disk to raw pixels with a single function call
/// the Pixels in the vector are 4 bytes per pixel, ordered RGBARGBA...
///
static std::tuple<unsigned,std::string> decodeOneStep(const char* filename, std::vector<unsigned char>& image,  png_size_t& sz) {
    // std::vector<unsigned char> image; //the raw pixels
    unsigned width, height;

    //decode
    unsigned error = lodepng::decode(image, width, height, filename);

    std::string s = "";

    //if there's an error, display it
    if(error) {
        s = "PNG decoder error #" + std::to_string(error) + ": " + std::string(lodepng_error_text(error));
        std::cout << s << "\n";
    } else {
        sz = std::make_tuple(width, height);
    }

    return std::make_tuple(error, s);
}

std::tuple<unsigned,std::string> HandlePng::load(const std::string& path, std::vector<unsigned char>& image,  png_size_t& sz) {
    // int error = 0;
    // std::string s = "";
    return decodeOneStep(path.c_str(), image, sz);
}



// This was taken from https://github.com/lvandeve/lodepng/blob/master/lodepng_unittest.cpp
static const std::string BASE64 = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";
//T and U can be std::string or std::vector<unsigned char>
template<typename T, typename U>
void toBase64(T& out, const U& in) {
  for(size_t i = 0; i < in.size(); i += 3) {
    int v = 65536 * in[i];
    if(i + 1 < in.size()) v += 256 * in[i + 1];
    if(i + 2 < in.size()) v += in[i + 2];
    out.push_back(BASE64[(v >> 18) & 0x3f]);
    out.push_back(BASE64[(v >> 12) & 0x3f]);
    if(i + 1 < in.size()) out.push_back(BASE64[(v >> 6) & 0x3f]);
    else out.push_back('=');
    if(i + 2 < in.size()) out.push_back(BASE64[(v >> 0) & 0x3f]);
    else out.push_back('=');
  }
}



std::string HandlePng::encodeB64(RayEngine* engine, bool includeType) {

    std::vector<unsigned char> image;

    // copy float bufs to bytes
    engine->copyToPixels(image);

    unsigned char* png;
    size_t pngsize;
    // render into png
    // this function calls new
    // we must call free
    // unsure if we are supposed to call free if it fails
    unsigned error = lodepng_encode32(&png, &pngsize, image.data(), engine->xPx, engine->yPx);

    if(error) {
        std::string s;
        s = "PNG decoder error #" + std::to_string(error) + ": " + std::string(lodepng_error_text(error));
        std::cout << s << "\n";
        return "";
    }

    // cout << "pngsize: " << pngsize << "\n";

    // convert from raw pointer to vector 
    // this maybe can be avoided by rewriting toBase64
    std::vector<unsigned char>convertIn;
    convertIn.assign(png, png+pngsize);

    // free pointer
    free(png);

    std::string out;

    if( includeType ) {
        out = "data:image/png;base64,";
    }
    // base64 is about 1.3 times larger
    // reserve with 1.4 to make sure no re-allocations are made because
    // toBase64() uses push_back()
    out.reserve( (size_t) ((float)pngsize * 1.4f) );

    toBase64(out, convertIn);

    return out;
}
