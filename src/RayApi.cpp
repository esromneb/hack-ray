#include "RayApi.hpp"
#include "HandlePng.hpp"

#include <iostream>
#include <cmath>
#include <vector>
#include <chrono>



using namespace std;

static RayEngine* engine = 0;
static resize_canvas_cb_t resizeCallback=0;
static copy_gl_cb_t copyGlCallback=0;
static copy_b64_cb_t copyB64Callback=0;

static bool printRenderTime = false;
static bool printRotCamera = false;

// this is required due to the way I originally wrote the files in this function:
// as naked functions with a global engine
// now that I've moved these to their own file we have an ugly hack
// where we still have to call this before calling any functions
// this also makes it thread unsafe (not that anything else is threadsafe)
void setRayApiTarget(RayEngine* e) {
    engine = e;
}



void setResizeCallback(const resize_canvas_cb_t cb) {
    resizeCallback = cb;
}

void setCopyGlCallback(const copy_gl_cb_t cb) {
    copyGlCallback = cb;
}

void setCopyB64Callback(const copy_b64_cb_t cb) {
    copyB64Callback = cb;
}




// automatically resize the vector
// (grow only)
template<class T>
void growOnly(const size_t index, T& vec) {
    if( (index + 1) > vec.size() ) {
        vec.resize((index+1));
    }
}




///
/// These are all of the Javascript -> C++ functions
///

extern "C" {

void setPrintRenderTime(const bool b) {
    printRenderTime = b;
}

void setPrintRotCamera(const bool b) {
    printRotCamera = b;
}


void render(void) {

    const auto start = std::chrono::steady_clock::now();
    engine->render();
    const auto end = std::chrono::steady_clock::now();


    if( printRenderTime ) {
        const size_t elapsed_us = std::chrono::duration_cast<std::chrono::microseconds>( 
            end - start
        ).count();
        const double elapsed_ms = elapsed_us / 1000.0;
        cout << "Render took " << elapsed_ms << "ms" << "\n";
    }
}

void setGlobalC(const int c) {
    engine->c = c;
}

void setScale(const double s) {
    engine->scale = s;
}

void onRuntimeInitialized(void) {
    cout << "onRuntimeInitialized()" << "\n";
}

void onCustomMessage(void) {
    cout << "onCustomMessage()" << "\n";
}

void coutInt(const int a) {
    cout << "C++ got this int: " << a << "\n";

    // // float fly_0;
    // ARG_VEC3_SIG(fly);

    // fly_0 = 42;

    // cout << "fly " << fly_0 << "\n";

}

void coutIntDual(const int a, const int b) {
    cout << "C++ got these ints: " << a << ", " << b << "\n";
}

void setCamera(VEC3_ARG(location), VEC3_ARG(direction), VEC3_ARG(rotation)) {
    // cout << "Rot: " << rotation_0 << "," << rotation_1 << "," << rotation_2 << "\n";

    auto &camera = engine->camera;

    camera.o = VEC3_ARG_CTONS(location);

    camera.d = VEC3_ARG_CTONS(direction);
    camera.d.normalize();
    
    engine->up = VEC3_ARG_CTONS(rotation);
    engine->up.normalize();
}

void setDepth(const int d) {
    engine->depth = d;
}

void setAmbientColor(VEC3_ARG(color)) {
    engine->ia = VEC3_ARG_CTONS(color);
}

void setNoHitColor(VEC3_ARG(color)) {
    engine->noHitColor = VEC3_ARG_CTONS(color);
}




void setSphereCount(const int count) {
    auto &spheres = engine->spheres;
    spheres.resize(count);
}


void setSphere(
    const int index,
    const float radius,
    VEC3_ARG(location),
    const float ambient,
    const float specular,
    const float reflected,
    const float transmitted,
    VEC3_ARG(diffuse),
    float n,
    const float refraction
    ) {

    if(index < 0) {
        cout << "Sphere index cannot be less than 0\n";
        return;
    }

    if( n < 1 ) {
        cout << "Specular index cannot be less than 1\n";
        n = 1;
    }

    auto &spheres = engine->spheres;

    // cout << "spheres Before size " << spheres.size() << "\n";

// index , size
    // 0 , 1
    // 1 , 2

    // automatically resize the vector
    // (grow only)
    growOnly(index, spheres);
    // if( (index + 1) > spheres.size() ) {
    //     spheres.resize((index+1));
    // }
    // cout << "spheres After size " << spheres.size() << "\n";

    spheres[index].r = radius;
    spheres[index].c = VEC3_ARG_CTONS(location);
    spheres[index].m.ka = ambient;
    spheres[index].m.ks = specular;
    spheres[index].m.kr = reflected;
    spheres[index].m.kd = VEC3_ARG_CTONS(diffuse);
    spheres[index].m.n = n;
    spheres[index].m.kt = transmitted;
    spheres[index].m.refraction = refraction;
    spheres[index].calc();
}


void setLightCount(const int count) {
    auto &lights = engine->lights;
    lights.resize(count);
}

void setLight(
    const int index,
    VEC3_ARG(direction),
    VEC3_ARG(color)) {

    if(index < 0) {
        cout << "Light index cannot be less than 0\n";
        return;
    }

    auto &lights = engine->lights;

    growOnly(index, lights);

    lights[index].d = VEC3_ARG_CTONS(direction);
    lights[index].d.normalize();
    
    lights[index].color = VEC3_ARG_CTONS(color);
}

void dumpPoly(const int index) {
    const auto &poly = engine->polygons[index];

    cout << "Polygon " << index << ":\n";
    cout << "edgeCount: " << poly.edgeCount << "\n";
    cout << "tpc: " << poly.trianglePointCount << "\n";
    cout << "ka: " << poly.ka << "\n";
    cout << "kd: " << poly.kd.str() << "\n";
    cout << "ks: " << poly.ks << "\n";
    cout << "kr: " << poly.kr << "\n";
    cout << "kt: " << poly.kt << "\n";
    cout << "n : " << poly.n  << "\n";
}


void setHighlightPixel(const int x, const int y) {
#ifdef ALLOW_HIGHLIGHT
    engine->highlightX = x;
    engine->highlightY = y;
#else
    cout << "RayEngine was compiled without the ALLOW_HIGHLIGHT macro\n";
    (void)x;
    (void)y;
#endif
}


void setRefractShadows(const bool b) {
    engine->memberRefractShadows = b;
}

void setEnableShadows(const bool b) {
    engine->enableShadows = b;
}

void setEnableAlpha(const bool b) {
    engine->enableAlpha = b;
}

static bool mostRecentParseCallUsingCanvas = false;

void resizeBuffer(const unsigned x, const unsigned y) {
    engine->resize(x,y);

    if( resizeCallback && mostRecentParseCallUsingCanvas ) {
        resizeCallback(x,y);
    }
}





typedef std::function<void(void)> scene_animation_t;

std::vector<scene_animation_t> cameraOrbit;

unsigned nextOrbitFrame = 0;


///
/// First frame is camera's current position
/// next frame is moved
/// final frame should be adjacent and animate "seamless" into the first frame
void setupOrbit(const int _frames) {
    if( _frames < 0 ) {
        cout << "Frames cannot be negative\n";
        return;
    }
    if( _frames == 0 ) {
        cout << "Frames cannot be zero\n";
        return;
    }

    const unsigned frames = (unsigned)_frames;
    if( printRotCamera ) {
        cout << "Orbit frames: " << frames << "\n";
    }

    cameraOrbit.resize(0);
    nextOrbitFrame = 0;

    const float bump = 2*M_PI / frames;

    Vec cameraOrigin(4); // make a Vec sized 4
    Vec cameraDir(4); // make a Vec sized 4

    // load the Vec3 camera into it
    cameraOrigin.data[0] = engine->camera.o[0];
    cameraOrigin.data[1] = engine->camera.o[1];
    cameraOrigin.data[2] = engine->camera.o[2];
    cameraOrigin.data[3] = 0;

    // load the Vec3 camera into it
    cameraDir.data[0] = engine->camera.d[0];
    cameraDir.data[1] = engine->camera.d[1];
    cameraDir.data[2] = engine->camera.d[2];
    cameraDir.data[3] = 0;

    // the new values for the camera's origin
    // these are lambda captured by value below
    float originX = engine->camera.o[0];
    float originY = engine->camera.o[1];
    float originZ = engine->camera.o[2];

    float dirX = engine->camera.d[0];
    float dirY = engine->camera.d[1];
    float dirZ = engine->camera.d[2];

    for(unsigned i = 0; i < frames; i++) {

        cameraOrbit.emplace_back([=](void) {
            if( printRotCamera ) {
                cout << "executing frame i: " << i << "\n";
            }
            engine->camera.o = Vec3(originX, originY, originZ);

            engine->camera.d = Vec3(dirX, dirY, dirZ);
            engine->camera.d.normalize(); // should not be needed if only rotating
        });

        if( printRotCamera ) {
            cout << cameraOrigin.str() << "  " << cameraDir.str() << "\n";
        }

        // calculate next frame
        cameraOrigin.rot_z(bump);
        cameraDir.rot_z(bump);

        // load these from the Vec
        originX = cameraOrigin.data[0];
        originY = cameraOrigin.data[1];
        originZ = cameraOrigin.data[2];

        // load these from the Vec
        dirX = cameraDir.data[0];
        dirY = cameraDir.data[1];
        dirZ = cameraDir.data[2];

    }


}

void nextOrbitRender(const bool andRender) {
    if( cameraOrbit.size() == 0 ) {
        cout << "nextOrbitRender() cannot run without setupOrbit() first\n";
        return;
    }

    // not sure if needed, prevent any starting glitch
    nextOrbitFrame = nextOrbitFrame % cameraOrbit.size();

    // update the scene
    cameraOrbit[nextOrbitFrame]();

    if( andRender ) {
        // render
        render();
        if( copyGlCallback ) {
            copyGlCallback();
        }
    }

    nextOrbitFrame++;

    nextOrbitFrame = nextOrbitFrame % cameraOrbit.size();
}


///
///
///    "loc": [0, 1, -2],
///    "dir": [0, -0.2, 1],
///
void dumpCamera() {

    const auto& c = engine->camera;

    cout << "\"loc\": [" << c.o[0] << ", " << c.o[1] << ", " << c.o[2] << "],\n";
    cout << "\"dir\": [" << c.d[0] << ", " << c.d[1] << ", " << c.d[2] << "],\n";

    // cout << engine->camera.o.str() << "\n";
    // cout << engine->camera.d.str() << "\n";
}

void chokeOutput(int il, int ih, int jl, int jh) {
#ifdef ALLOW_CHOKE
    engine->il = il;
    engine->ih = ih;
    engine->jl = jl;
    engine->jh = jh;
#else
    cout << "RayEngine was compiled without the ALLOW_CHOKE macro\n";
    (void)il;
    (void)ih;
    (void)jl;
    (void)jh;
#endif
}

void setPrint(int p) {
#ifdef ALLOW_PRINT
    engine->print = p?true:false;
#else
    cout << "RayEngine was compiled without the ALLOW_PRINT macro\n";
    (void)p;
#endif
}

// only called from wasm (For now)
void parseJsonScene(const char* scene, const bool andRender, const bool useCanvas, const int tag ) {
    // cout << "parseJsonScene() Got: " << scene << "\n";
    // cout << "parseJsonScene() bool: " << andRender << "\n";

    // cout << "parseJsonScene() Got called tag:" << tag << "\n";

    // another hacked global
    mostRecentParseCallUsingCanvas = useCanvas;

    unsigned ret;
    std::string error;


    std::tie(ret,error) = Parser::parse(scene, engine);


    if( ret ) {
        cout << "Parser returned error #" << ret << ": " << error << "\n";
        return;
    } else {
        if( andRender ) {
            render();

            if(useCanvas) {
                if( copyGlCallback ) {
                    copyGlCallback();
                }
            } else {
                if( copyB64Callback ) {
                    std::string b64 = HandlePng::encodeB64(engine, true);
                    copyB64Callback(b64, tag);
                }
            }
        }
    }
}

// void parseJsonSceneFragment(const std::string scene, const bool andRender ) {
//     // cout << "Got: " << scene << "\n";
//     // cout << "bool: " << andRender << "\n";
// }



} // extern C
