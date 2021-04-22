#include "Parser.hpp"
#include "RayApi.hpp"
#include "Macros.hpp"
#include "Material.hpp"

#include "json.hpp"


#include <string>
#include <iostream>
#include <fstream>
#include <streambuf>

using namespace std;

static bool valid_array(const nlohmann::json& obj, const std::string& key) {
    return obj.contains(key) && obj[key].is_array();
}

static bool valid_vec3(const nlohmann::json& obj, const std::string& key) {
    return valid_array(obj, key) && obj[key].size() == 3 && obj[key][0].is_number() && obj[key][1].is_number() && obj[key][2].is_number();
}

static bool valid_number(const nlohmann::json& obj, const std::string& key) {
    return obj.contains(key) && obj[key].is_number();
}

static bool valid_object(const nlohmann::json& obj, const std::string& key) {
    return obj.contains(key) && obj[key].is_object();
}

static bool valid_bool(const nlohmann::json& obj, const std::string& key) {
    return obj.contains(key) && obj[key].is_boolean();
}


/*
// Working code for using a input input adapter callback
// using nlohmann::detail::input_adapter ;
// using nlohmann::detail::input_adapter_protocol ;
typedef nlohmann::detail::parser<nlohmann::basic_json<>>::parse_event_t json_parse_event_t;
typedef std::function<bool(int depth, json_parse_event_t event, nlohmann::basic_json<>& parsed)> json_cb_t;
json_cb_t the_callback = [](int depth, json_parse_event_t event, nlohmann::basic_json<>& parsed) {
    const uint8_t _event = (uint8_t)event;
    cout << "Depth: " << depth << " in the callback " << (int) _event << "\n";
    return true;
};

*/



using nlohmann::detail::input_buffer_adapter ;

// this function is required to parse without exceptions
std::optional<nlohmann::json> jsonParseExceptionFree(const std::string& str) {
    const char* first = str.c_str();
    auto len = str.size();

    // The resulting json object
    nlohmann::json j;

    // some stuff I don't understand
    auto ia = std::make_shared<input_buffer_adapter>(reinterpret_cast<const char*>(&(*first)), len);
    nlohmann::detail::parser<nlohmann::basic_json<>>(ia, nullptr, false).parse(false, j);

    if( j.is_discarded() ) {
        // parse error
        return {};
    } else {
        // parsed correctly
        return j;
    }
}



std::tuple<unsigned,std::string> Parser::parseFile(const std::string& path, RayEngine* e, const unsigned restrictParse) {
    std::ifstream ifs(path);


    std::string contents((std::istreambuf_iterator<char>(ifs)), std::istreambuf_iterator<char>());

    ifs >> contents;

    return Parser::parse(contents.c_str(), e, restrictParse);
}


// 0 is ok
// 1 is bad json
// 2 is missing required keys


std::tuple<unsigned,std::string> Parser::parse(const char* const str, RayEngine* e, const unsigned restrictParse) {

    // cout << "Parsed called with:\n\n" << str << "\n\n";

    nlohmann::json obj;

    std::string _str = str;

    auto maybeParsed = jsonParseExceptionFree(_str);

    if( maybeParsed.has_value() ) {
        obj = maybeParsed.value();
    } else {
        const std::string parseError = "Json parse error";
        // cout << parseError << "\n";
        return std::make_tuple(1,parseError);
    }

    // try {
    //     obj = nlohmann::json::parse(str);
    // } catch(nlohmann::json::parse_error &er) {
    //     cout << er.what() << "\n";
    //     return std::make_tuple(1,er.what());
    // }

    setRayApiTarget(e);

    unsigned subCode = 0;
    std::string subMessage;

    switch(restrictParse) {
        default:
        case 0: 
            std::tie(subCode, subMessage) = parseCamera(obj);
            if( subCode ) {
                return std::make_tuple(subCode, subMessage);
            }

            std::tie(subCode, subMessage) = parseGlobal(obj);
            if( subCode ) {
                return std::make_tuple(subCode, subMessage);
            }

            std::tie(subCode, subMessage) = parseSpheres(obj);
            if( subCode ) {
                return std::make_tuple(subCode, subMessage);
            }

            std::tie(subCode, subMessage) = parseLights(obj);
            if( subCode ) {
                return std::make_tuple(subCode, subMessage);
            }

            break;
        case 1:
            return parseCamera(obj);
            break;
        case 2:
            return parseGlobal(obj);
            break;
        case 3:
            return parseSpheres(obj);
            break;
        case 4:
            return parseLights(obj);
            break;
    }


    return std::make_tuple(0,"");

}

std::tuple<unsigned,std::string> Parser::parseCamera(const nlohmann::json& obj) {
    if( valid_object(obj, "camera") ) {

        // cout << "Have Camera\n";
        const auto camera = obj["camera"];

        if( 
            valid_vec3(camera, "loc") && 
            valid_vec3(camera, "dir") && 
            valid_vec3(camera, "rot") && 
            valid_number(camera, "depth")
            ) {
            // cout << "Valid Camera Block\n";
            // cout << "loc size " << camera["loc"].size() << "\n";

            // cout << "Grab Camera Rot: " << camera["rot"][2] << "\n";

            setCamera(VEC3_SPREAD(camera["loc"]), VEC3_SPREAD(camera["dir"]), VEC3_SPREAD(camera["rot"]));
            setDepth(camera["depth"]);

        } else {
            return std::make_tuple(2,"Camera tree missing required keys");
        }

    } else {
        return std::make_tuple(2,"Camera tree missing");
    }


    return std::make_tuple(0,"");
}


std::tuple<unsigned,std::string> Parser::parseGlobal(const nlohmann::json& obj) {
    if( valid_object(obj, "global") ) {

        const auto gg = obj["global"];

        if(
            (valid_vec3(gg, "color") || valid_vec3(gg, "ambient_color")) &&
            valid_number(gg, "c")
            ) {
            const std::string ambientKey = valid_vec3(gg, "color") ? "color" : "ambient_color";

            // cout << "Ambient using key: " << ambientKey << "\n";

            setAmbientColor(VEC3_SPREAD(gg[ambientKey]));
            setGlobalC(gg["c"]);

        } else {
            return std::make_tuple(2,"Global tree missing required keys");
        }

        if(valid_vec3(gg, "nohit_color")) {
            setNoHitColor(VEC3_SPREAD(gg["nohit_color"]));
        } else {
            // if a scene doesn't have a nohit key
            // we will never set this and the nohit_color from the previous scene will be used (without this)
            setNoHitColor(0,0,0);
        }

        if(valid_number(gg, "scale")) {
            setScale(gg["scale"]);
        } else {
            setScale(RayEngine::defaultScale);
        }

        if(valid_bool(gg, "refract_shadows")) {
            setRefractShadows(gg["refract_shadows"]);
        } else {
            setRefractShadows(RayEngine::defaultMemberRefractShadows);
        }

        if(valid_bool(gg, "shadows")) {
            setEnableShadows(gg["shadows"]);
        } else {
            setEnableShadows(RayEngine::defaultEnableShadows);
        }

        if(valid_bool(gg, "alpha")) {
            setEnableAlpha(gg["alpha"]);
        } else {
            setEnableAlpha(RayEngine::defaultEnableAlpha);
        }

        if( valid_number(gg, "x") && valid_number(gg, "y") ) {
            // cout << "Parse doing custom resize\n";
            resizeBuffer( gg["x"], gg["y"] );
        } else {
            // cout << "Parse doing default resize\n";
            resizeBuffer( 400, 400 );
        }




    } else {
        return std::make_tuple(2,"Global tree missing");
    }
    return std::make_tuple(0,"");
}

std::tuple<unsigned,std::string> Parser::parseSpheres(const nlohmann::json& obj) {
    if( valid_array(obj, "spheres") ) {

        const auto spheres = obj["spheres"];

        const unsigned sphereCount = spheres.size();
        setSphereCount(sphereCount);
        // cout << "found " << sphereCount << " spheres\n";

        for(unsigned i = 0; i < sphereCount; i++) {
            auto s = spheres[i];
            // cout << s << "\n";

            if( 
                valid_number(s, "rad") && 
                valid_vec3(s, "loc") && 
                valid_number(s, "ambient") && 
                valid_number(s, "specular") && 
                valid_number(s, "reflected") && 
                valid_number(s, "transmitted") && 
                valid_vec3(s, "diffuse")
                ) {
                    float _refraction = Material::defaultRefraction;
                    if( valid_number(s, "refraction") ) {
                        _refraction = s["refraction"];
                    }

                    float _n = Material::defaultN;
                    if( valid_number(s, "n") ) {
                        _n = s["n"];
                    }

                    setSphere(
                        i,
                        s["rad"],
                        VEC3_SPREAD(s["loc"]),
                        s["ambient"],
                        s["specular"],
                        s["reflected"],
                        s["transmitted"],
                        VEC3_SPREAD(s["diffuse"]),
                        _n,
                        _refraction);
            } else {
                return std::make_tuple(2, "Sphere #" + std::to_string(i) + " missing required keys");
            }
        }
    } else {
        setSphereCount(0);
    }

    return std::make_tuple(0,"");
}

std::tuple<unsigned,std::string> Parser::parseLights(const nlohmann::json& obj) {
    if( valid_array(obj, "lights") ) {

        const auto lights = obj["lights"];

        const unsigned lightCount = lights.size();
        setLightCount(lightCount);
        // cout << "found " << lightCount << " lights\n";

        for(unsigned i = 0; i < lightCount; i++) {
            auto s = lights[i];

            if( 
                valid_vec3(s, "dir") && 
                valid_vec3(s, "color")
                ) {
                    setLight(
                        i,
                        VEC3_SPREAD(s["dir"]),
                        VEC3_SPREAD(s["color"])
                        );
            } else {
                // warning
                // FIXME a better way than printing here?
                cout << "Warning: Light #" + std::to_string(i) + " missing required keys\n";

                setLight(
                    i,
                    VEC3_SPREAD(Light::defaultDirection),
                    VEC3_SPREAD(Light::defaultColor)
                    );

            }
        }
    } else {
        setLightCount(0);
    }

    return std::make_tuple(0,"");
}

