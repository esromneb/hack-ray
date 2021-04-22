#pragma once

#include "RayEngine.hpp"

#include "json_fwd.hpp"

#include <tuple>


class Parser {
public:
    static std::tuple<unsigned,std::string> parse(const char* const str, RayEngine* e, const unsigned restrictParse = 0);
    static std::tuple<unsigned,std::string> parseFile(const std::string& path, RayEngine* e, const unsigned restrictParse = 0);
private:
    static std::tuple<unsigned,std::string> parseCamera(const nlohmann::json& obj);
    static std::tuple<unsigned,std::string> parseGlobal(const nlohmann::json& obj);
    static std::tuple<unsigned,std::string> parseSpheres(const nlohmann::json& obj);
    static std::tuple<unsigned,std::string> parseLights(const nlohmann::json& obj);
};
