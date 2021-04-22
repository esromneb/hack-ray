#pragma once

#include <string>
#include <tuple>
#include <vector>

class RayEngine;

typedef std::tuple<unsigned, unsigned> png_size_t;

class HandlePng {
public:
    static std::tuple<unsigned,std::string> save(const std::string& path, RayEngine* e);
    static std::tuple<unsigned,std::string> load(const std::string& path, std::vector<unsigned char>& image, png_size_t& sz);
    static std::string encodeB64(RayEngine* e, bool includeType = true);
};
