#include "Vec3.hpp"
#include "Parser.hpp"
#include "RayEngine.hpp"

#include <iostream>
#include <cmath>
#include <string>

using namespace std;




const std::string t0 = "{\"global\":{\"ambient_color\":[1,0,0]}}";
const std::string t1 = "{\"global\":{\"ambient_color\":[1,0,0]}";
const std::string t2 = "{\"global\":{\"ambient_color\":[1,0,0]},\"camera\":{\"loc\":[1,0,0]}}";
const std::string t3 = "{\"global\":{\"ambient_color\":[1,0,0]},\"camera\":{\"loc\":[1,0,0],\"dir\": [-1, 0, 0],\"rot\": [0, 0, 1],\"depth\": 6}}";         // good
const std::string t4 = "{\"global\":{\"ambient_color\":[1,0,0]},\"camera\":{\"loc\":[1,0,0],\"dir\": [-1, 0, 0],\"rot\": [0, 0, 1.00001],\"depth\": 6}}";   // good
const std::string t5 = "{\"global\":{\"ambient_color\":[1,0,0]},\"camera\":{\"loc\":[1,0,0],\"dir\": [-1, 0, 0],\"rot\": [0, 0],\"depth\": 6}}";            // bad
const std::string t6 = "{\"global\":{\"ambient_color\":[1,0,0]},\"camera\":{\"loc\":[1,0,0],\"dir\": [-1, 0, 0],\"rot\": [0, 0, 1],\"depeth\": 6}}";        // bad
const std::string t7 = "{\"global\":{\"ambient_color\":[1,0,0]},\"camera\":{\"loc\":[1,0,0],\"dir\": [-1, 0, 0],\"rot\": [0, 0, 1],\"depth\": [6]}}";       // bad
const std::string t8 = "{\"global\":{\"ambient_color\":[1,0,0]},\"camera\":{\"loc\":[1,0,0],\"dir\": \"a\",\"rot\": [0, 0, 1],\"depth\": 6}}";              // bad

const std::string t9 = "{\"global\":{\"ambient_color\":[1,0,0],\"c\":10}}";
const std::string t10 = "{\"global\":{\"color\":[1,0,0],\"c\":10}}";

const std::string t11 = "JUNKASDFASD{\"global\":{\"ambient_color\":[1,0,0]},\"camera\":{\"loc\":[1,0,0],\"dir\": [-1, 0, 0],\"rot\": [0, 0, 1],\"depth\": 6}}";         // good

const std::string t12 = "{\"spheres\":[{\"rad\":1.1,\"loc\":[0,0,0],\"ambient\":2.5,\"specular\":2.01,\"reflected\":0.0,\"transmitted\":0.0,\"diffuse\":[1.0,1.0,1.0],\"n\":40}]}";
const std::string t13 = "{\"spheres\":[{\"rad\":1.1,\"loc\":[0,0,0],\"ambient\":2.5,\"specular\":2.01,\"reflected\":0.0,\"transmitted\":0.0,\"diffuse\":[1.0,1.0,1.0],\"n\":41},{\"rad\":1.1,\"loc\":[0,0,0],\"ambient\":2.5,\"specular\":2.01,\"reflected\":0.0,\"transmitted\":0.0,\"diffuse\":[1.0,1.0,1.0],\"n\":42}]}";
const std::string t14 = "{\"spheres\":[{\"loc\":[0,0,0],\"ambient\":2.5,\"specular\":2.01,\"reflected\":0.0,\"transmitted\":0.0,\"diffuse\":[1.0,1.0,1.0],\"n\":40}]}";

const std::string t15 = "{\"lights\": [{\"dir\": [-0.4, 0, -1],\"color\": [20, 20, 20]}]}";
const std::string t16 = "{\"lights\": [{\"dr\": [-0.4, 0, -1],\"color\": [20, 20, 20]}]}";

// int test0(RayEngine *engine) {

//     {
//         auto [ret,error] = Parser::parse(t0.c_str(), engine);

//         cout << "Got code " << ret << " with message [" << error << "]\n";

//         if( ret != 0 ) {
//             return 1;
//         }
//     }

//     {
//         auto [ret,error] = Parser::parse(t1.c_str(), engine);

//         cout << "Got code " << ret << " with message [" << error << "]\n";
//         if( ret == 0 ) {
//             return 2;
//         }
//     }


//     return 0;
// }

int test1(RayEngine* engine) {

    const unsigned restrictParse = 1;


    {
        auto [ret,error] = Parser::parse(t4.c_str(), engine, restrictParse);

        cout << "Got code " << ret << " with message [" << error << "]\n";
        if( ret != 0 ) {
            return 3;
        }
    }

    {
        auto [ret,error] = Parser::parse(t5.c_str(), engine, restrictParse);

        cout << "Got code " << ret << " with message [" << error << "]\n";
        if( ret != 2 ) {
            return 3;
        }
    }

    {
        auto [ret,error] = Parser::parse(t6.c_str(), engine, restrictParse);

        cout << "Got code " << ret << " with message [" << error << "]\n";
        if( ret != 2 ) {
            return 3;
        }
    }

    {
        auto [ret,error] = Parser::parse(t7.c_str(), engine, restrictParse);

        cout << "Got code " << ret << " with message [" << error << "]\n";
        if( ret != 2 ) {
            return 3;
        }
    }

    {
        auto [ret,error] = Parser::parse(t8.c_str(), engine, restrictParse);

        cout << "Got code " << ret << " with message [" << error << "]\n";
        if( ret != 2 ) {
            return 3;
        }
    }

    {
        auto [ret,error] = Parser::parse(t0.c_str(), engine, restrictParse);

        cout << "Got code " << ret << " with message [" << error << "]\n";
        if( ret != 2 ) {
            return 3;
        }
    }



    return 0;
}

int test2(RayEngine* engine) {
    const unsigned restrictParse = 2;


    {
        auto [ret,error] = Parser::parse(t9.c_str(), engine, restrictParse);

        cout << "Got code " << ret << " with message [" << error << "]\n";
        if( ret != 0 ) {
            return 3;
        }
    }

    {
        auto [ret,error] = Parser::parse(t10.c_str(), engine, restrictParse);

        cout << "Got code " << ret << " with message [" << error << "]\n";
        if( ret != 0 ) {
            return 3;
        }
    }


    return 0;
}


// Test the exception-less exceptions from json parse
int test3(RayEngine* engine) {
    
    {
        auto [ret,error] = Parser::parse(t11.c_str(), engine);

        cout << "Got code " << ret << " with message [" << error << "]\n";
        if( ret != 1 ) {
            return 2;
        }
    }

    return 0;
}



int test4(RayEngine* engine) {
    const unsigned restrictParse = 3;


    {
        auto [ret,error] = Parser::parse(t12.c_str(), engine, restrictParse);

        cout << "Got code " << ret << " with message [" << error << "]\n";
        if( ret != 0 ) {
            return 3;
        }
    }


    {
        auto [ret,error] = Parser::parse(t13.c_str(), engine, restrictParse);

        cout << "Got code " << ret << " with message [" << error << "]\n";
        if( ret != 0 ) {
            return 3;
        }
    }


    {
        auto [ret,error] = Parser::parse(t14.c_str(), engine, restrictParse);

        cout << "Got code " << ret << " with message [" << error << "]\n";
        if( ret != 2 ) {
            return 3;
        }
    }



    return 0;
}

int test5(RayEngine* engine) {
    const unsigned restrictParse = 4;


    {
        auto [ret,error] = Parser::parse(t15.c_str(), engine, restrictParse);

        cout << "Got code " << ret << " with message [" << error << "]\n";
        if( ret != 0 ) {
            return 3;
        }
    }

    {
        auto [ret,error] = Parser::parse(t16.c_str(), engine, restrictParse);

        cout << "Got code " << ret << " with message [" << error << "]\n";
        if( ret != 0 ) {
            return 3;
        }
    }


    // {
    //     auto [ret,error] = Parser::parse(t13.c_str(), engine, restrictParse);

    //     cout << "Got code " << ret << " with message [" << error << "]\n";
    //     if( ret != 0 ) {
    //         return 3;
    //     }
    // }



    return 0;
}

int test6(RayEngine* engine) {
    const unsigned restrictParse = 0;

    std::string p0 = "scenes/test_shadow_1.json";

    {
        auto [ret,error] = Parser::parseFile(p0, engine, restrictParse);

        cout << "Got code " << ret << " with message [" << error << "]\n";
        if( ret != 0 ) {
            return 3;
        }
    }


    return 0;
}




int main(void) {
    RayEngine* engine;
    engine = new RayEngine();
    engine->resize(400,400);

    std::vector<int> results;

    // results.emplace_back(test0(engine));
    results.emplace_back(test1(engine));
    results.emplace_back(test2(engine));
    results.emplace_back(test3(engine));
    results.emplace_back(test4(engine));
    results.emplace_back(test5(engine));
    results.emplace_back(test6(engine));

    unsigned failCount = 0;
    unsigned i = 0;
    for(const auto r : results) {
        if( r ) {
            failCount++;
            cout << "Test #" << i << " Failed";
        }
        i++;
    }

    if( failCount ) {
        cout << "\n" << failCount << " Tests failed\n";
    } else {
        cout << "\nALL Tests passed\n";
    }

}
