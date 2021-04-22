#include "RayEngine.hpp"
#include <math.h>
#include <iostream>
#include <tuple>
#include <type_traits>

using std::min;
using std::max;



#ifdef ALLOW_PRINT
#define PRINT print
#else
#define PRINT false
#endif


RayEngine::RayEngine( void )
{
    numPoly = 0;
    this->polygons.resize(POLYGONS);

    // r = g = b = 0;
    // px = 0;

    // mat = new Matrix( 3, 3 );
    // mat->m1 = new Matrix( 2, 2 );
    // mat->m2 = new Matrix( 2, 2 );
    // mat->m3 = new Matrix( 2, 2 );
    
}


void RayEngine::resize( const unsigned _x, const unsigned _y )
{
    if( rBuffer && gBuffer && bBuffer && aBuffer && xPx == _x && yPx == _y ) {
        // resize requested for exactly same size
        // don't make any changes
        // cout << "Resize aborting\n";
        return;
    }


    // px = _x;
    if( rBuffer && gBuffer && bBuffer && aBuffer )
    {
        delete rBuffer;
        delete gBuffer;
        delete bBuffer;
        delete aBuffer;
    }

    // cout << "Resize engine at " << _x << ", " << _y << "\n";

    rBuffer = new float[_x*_y];
    gBuffer = new float[_x*_y];
    bBuffer = new float[_x*_y];
    aBuffer = new float[_x*_y];

    xPx = _x;
    yPx = _y;
}

// acceps two Vec3
// the z coord is dropped from both, demoding them to a Vec2
// then the angle between x0,y0 and x1,y1 is calculated and 
// returned in degrees
// double getVec2Angle(const Vec3& _v0, const Vec3& _v1) {
//     Vec a(2,_v0[0],_v0[1]);
//     Vec b(2,_v1[0],_v1[1]);

//     double angleDeg = Vec::angle(a,b);
//     return angleDeg;
// }

int g_i = 0;
int g_j = 0;


void RayEngine::render( void ) noexcept {
    // cout << "Engine Alpha: " << (enableAlpha?"true":"false") << "\n";
    if( enableShadows ) {
        if( memberRefractShadows ) {
            if( enableAlpha ) {
                _render<true, true, true>();
            } else {
                _render<true, true, false>();
            }
        } else {
            if( enableAlpha ) {
                _render<true, false, true>();
            } else {
                _render<true, false, false>();
            }
        }
    } else {
        if( enableAlpha ) {
            _render<false, false, true>();
        } else {
            _render<false, false, false>();
        }
    }
}

template <bool enableShadowsT, bool refractShadowsT, bool saveAlphaT>
void RayEngine::_render( void ) noexcept {
    Vec3 color;

    const Vec3 e = camera.o;
    const Vec3 a = e + camera.d;
    const Vec3 w = Vec3::normalize(e-a);

    const Vec3 u = Vec3::normalize(Vec3::cross( up, e-a ));
    const Vec3 v = Vec3::cross( w, u );

    // i is x
    // j is y

    const unsigned fixedYWidth = yPx;
    const unsigned fixedXWidth = xPx;

    Ray r;
    r.o = e;

    bool hitSomething = false;
    double visibilityLevel = -45;
    (void)hitSomething;
    (void)visibilityLevel;


    // cout << "Render with x " << xPx << " y " << yPx << "\n";

    for( unsigned j = 0; j < yPx; j++ ) {
        for( unsigned i = 0; i < xPx; i++ )
        {
            const double cu = ((2.0f*i + 1)/(2.0f*fixedXWidth) - 0.5f); // xPx
            const double cv = ((2.0f*j + 1)/(2.0f*fixedYWidth) - 0.5f); // yPx


            const Vec3 pixel = a + u*cu + v*cv;
            // const Ray r(e, Vec3::normalize(pixel-e));
            r.d = Vec3::normalize(pixel-e);

#ifdef ALLOW_PRINT
            g_i = i;
            g_j = j;
#endif


#ifdef ALLOW_CHOKE
            const bool noChoke = ((signed)i > il && (signed)i < ih) && ((signed)j > jl && (signed)j < jh );
#endif

#ifdef ALLOW_CHOKE
            if(noChoke) {
#endif
            if constexpr (saveAlphaT) { 
                std::tie(hitSomething,visibilityLevel) = trace<enableShadowsT, refractShadowsT, false>( r, 0, color);
            } else {
                trace<enableShadowsT, refractShadowsT, false>( r, 0, color);
            }
#ifdef ALLOW_CHOKE
            } else {
                color = Vec3(0,0,0);
            }
#endif



#ifdef ALLOW_HIGHLIGHT
            if( (highlightX >= 0 && highlightY >= 0) && ((signed)i == highlightX && (signed)j == highlightY) ) {
                color = Vec3(2,0,0);
            }
#endif

            const auto writeOut = (i+j*xPx);

            this->rBuffer[writeOut] = color[0];
            this->gBuffer[writeOut] = color[1];
            this->bBuffer[writeOut] = color[2];

            if constexpr (saveAlphaT) {
                this->aBuffer[writeOut] = 1.0 - visibilityLevel;
                // cout << "x: " << i << " y: " << j << " vis " << visibilityLevel << "\n";
            }

        }
    }
}


// v is ray direction
// n is the surface normal
// ni_over_nt is n1/n2
// refracted (output) is the refracted ray direaction
bool refract(const Vec3& v, const Vec3& n, const double ni_over_nt, Vec3& refracted) {
    const Vec3 uv = Vec3::normalize(v);
    const double dt = uv.dot(n);
    
    const double discriminant = 1.0 - ni_over_nt * ni_over_nt * (1-dt*dt);

    if(discriminant > 0){
        refracted = ((uv-n*dt)*ni_over_nt) - n*sqrt(discriminant);
        return true;
    } else {
        return false; // no refracted ray
    }
}

float schlick(const float cosine, const float ref_idx) {
    float r0 = (1 - ref_idx) / (1 + ref_idx); // ref_idx = n2/n1
    r0 = r0 * r0;
    return r0 + (1 - r0) * pow((1 - cosine), 5);
}




// Vec3 intersect;
// Vec3 norm;
// Vec3 refl;
// Vec3 fp;

std::tuple<Vec3, Vec3, Vec3, Vec3> intersectSphere(const Ray& r, const Sphere& s, const double t0) {
    Vec3 intersect;
    Vec3 norm;
    Vec3 refl;
    Vec3 fp;

    intersect = r.pointAt(t0);

    const double srInverse = s.r_inv;


    norm = Vec3( ( intersect[0] - s.c[0] )*srInverse, ( intersect[1] - s.c[1] )*srInverse, ( intersect[2] - s.c[2] )*srInverse );
    //norm = intersect - s.c;
    norm.normalize();

    //refl = r.d - norm * 2 * (camera.d.dot(norm));
    // refl = norm*2* norm.dot( r.d ) - r.d;
    refl = Vec3::reflect(r.d, norm);

    //lighting
    fp = r.o - intersect;

    return {intersect, norm, refl, fp};
}

#define NO_HIT ((double)9999999)

#define HIT_SPHERE ((uint8_t)0)
#define HIT_POLY   ((uint8_t)1)


// intersecetion
// norm
typedef std::tuple<uint8_t, size_t> hit_t;

// returns true for hitting anything
// returns a double "visible amount" to do with shadow tracing, 0 is blocked 1 is visible
template <bool enableShadowsT, bool refractShadowsT, bool shadowFeelingT>
std::tuple<bool,double> RayEngine::trace(
    const Ray& r,
    const int depthIn,
    Vec3 &color) noexcept {

    if( depthIn > this->depth ) {
        if( PRINT ) {
            cout << "Abort\n";
        }
        return std::make_tuple(false,1.0);
    }

    if( PRINT ) {
        cout << "trace px " << g_i << ", " << g_j << " depth " << depthIn << "\n";
    }

    // Vec3 fp;
    // color[0] = color[1] = color[2] = 0;

    // double vd, vo, t; // tri stuff
    // Vec3 d1,d2,d3; // tri stuff
    


    // when we reflect a ray, we recurse
    // after the recursion is done, we add these to the results
    // these are set by lighting
    Vec3 savedColor(0,0,0);
    Vec3 savedRefl(0,0,0);
    Vec3 savedIntersect(0,0,0);
    Vec3 savedNormal(0,0,0);
    double savedKr = 0;      // reflection coefficient
    double savedRefractionIndex = 1;
    double savedKt = 0;      // transmission (refraction) coefficient


    // distance of closest hit
    // this value is a magic number signifying "no hit"
    double hitDistance = NO_HIT;
    hit_t bestHit;


    // const int nSphere = spheres.size();
    for( unsigned i = 0; i < spheres.size(); i++ )
    {
        const Sphere &s = spheres[i];

        const Vec3 oc = r.o - s.c;
        const double a = r.d.dot(r.d);

        // const double b = 2*(r.d[0] * (r.o[0] - s.c[0]) + r.d[1] * (r.o[1] - s.c[1]) + r.d[2] * (r.o[2] - s.c[2]));
        const double sb = 2.0 * oc.dot(r.d);

        // double c = pow((r.o[0] - s.c[0]),2) + pow((r.o[1] - s.c[1]),2) + pow((r.o[2] - s.c[2]),2) - s.r*s.r;
        const double sc = oc.dot(oc) - (s.r_sq);

        const double discriminant = (sb*sb) - (4*a*sc);

        if(PRINT) {
            cout << "Discr: " << discriminant << " " << i << "\n";
        }

        /// dis < 0   no intersection
        /// dis = 0   tangent intersection (1 point only)
        /// dis > 0   two points of intersection

        if( discriminant < 0 ) {
            if( PRINT ) {
                cout << "sphere break A " << i << "\n";
            }
            continue;
        }

        const double t0 = ((-sb) - sqrt(discriminant)) / (2*a);
        const double t1 = ((-sb) + sqrt(discriminant)) / (2*a);

        if( PRINT ) {
            cout << "t0 " << t0 << " t1 " << t1 << "\n";
        }

        ///
        /// 
        /// 

        double sphereBestT;

        // has hit detection issues
        if constexpr ( false ) {
            sphereBestT = min( t0, t1 );
        }

        
        // the tolerance here can be changed to 0 if we "advance"
        // the reflected ray just a bit.  whichever is more efficient
        if constexpr ( true ) {

            // this fixes shadow issues
            const double sphereLift = 1E-12;

            if( t0 > sphereLift && t1 > sphereLift ) {
                sphereBestT = min( t0, t1 );
            } else if( t0 > sphereLift && t1 <= sphereLift ) {
                sphereBestT = t0;
            } else if ( t0 <= sphereLift && t1 > sphereLift ) {
                sphereBestT = t1;
            } else {
                // sphere is behind ray
                continue;
            }
        }

        // if we are doing a shadow feeler

        if constexpr( refractShadowsT ) {

            // If we are refracting shadows
            // we need to check every sphere along the path of the ray
            // for the cloesest hit, as the closest one may be transparent
            // FIXME this may be wrong as a sold object far away is different
            // than a refracted object up close
            if constexpr ( shadowFeelingT ) {
                if( s.m.kt == 0 ) {
                    return std::make_tuple(true, 0.0);
                }
            }
        } else {

            // we are sending one single ray back to the light source
            // any hit, no matter the distance, means the light is blocked
            if constexpr( shadowFeelingT ) {
                return std::make_tuple(true, 0.0);
            }
        }


        // if this hit is closer then the last
        // save it
        if( sphereBestT < hitDistance ) {
            if( PRINT ) {
                cout << "Update best hit to " << sphereBestT << " (sphere " << i << ")\n";
            }
            hitDistance = sphereBestT;
            bestHit = std::make_tuple(HIT_SPHERE, i);
        }
    } // for spheres

    if( hitDistance != NO_HIT ) {
        Vec3 intersect;
        Vec3 norm;
        Vec3 refl;
        Vec3 fp;
        Material material;
        const auto [ hitWhat, index ] = bestHit;

        if( hitWhat == HIT_SPHERE ) {
            std::tie(intersect, norm, refl, fp) = intersectSphere(r, spheres[index], hitDistance);
            material = spheres[index].m;
        }

        // we start with ambient light
        // we add the diffuse color of the object to the ambient light
        // and then multiply by the objecs ambient coeffient
        // this gives the best look I feel however I guessed for this
        color = (this->ia+material.kd) * material.ka;

        // a bad guess, don't want multiply
        // color = (this->ia+material.kd) * material.ka * material.kd;
        if constexpr ( !shadowFeelingT ) {
            for( unsigned iLight = 0; (iLight < lights.size()); iLight++ ) {
                double visibilityLevel;

                // decide if we can see this light
                if constexpr ( enableShadowsT ) {
                    Ray shadowFeeler;
                    shadowFeeler.o = intersect;
                    shadowFeeler.d = lights[iLight].d * -1;
                    // shadowFeeler.o = shadowFeeler.o + shadowFeeler.d * 2;
                    Vec3 shadowColor; // unused

                    // trace( shadowFeeler, depth, effect, shadowColor, false, bSphere, objectNum, true );

                    int shadowDepth;

                    if constexpr (refractShadowsT) {
                        // set depth to next value as we will be refracting the shadow detection
                        // in this case, we fudge so that we will always allow 1 shadow feeler
                        // to go out.  (this could cause us to actually recurse +1 from what we requested)
                        // however it will prevent shadowed colors from showing up in final bounce reflections
                        shadowDepth = min(this->depth,depthIn+1);
                    } else {
                        // set depth to maximum to prevent further bounces
                        shadowDepth = this->depth;
                    }

                    if(PRINT) {
                        cout << "\n" << std::string(depthIn, ' ') << "Looking at light " << iLight << " " << shadowFeeler.o.str(false) << " " << shadowFeeler.d.str(false) << " with depth " << shadowDepth << "\n";
                    }

                    bool lightBlocked;

                    std::tie(lightBlocked, visibilityLevel) = trace<enableShadowsT,refractShadowsT,true>( shadowFeeler, shadowDepth, shadowColor );

                    if(PRINT) {
                        cout << "Light " << iLight;
                    }


                    // just because the light was blocked doesn't mean it was 100% blocked
                    // in order to ignore this light, we need a true and 0.0
                    // if it's a value larger than 0 we need to do the full color
                    // calculation in order to get the shadow right with transparent (kt) spheres

                    if( lightBlocked && visibilityLevel <= 0.0) {
                        if(PRINT) {
                            cout << " Blocked\n";
                        }
                        continue;
                    }

                    if(PRINT) {
                        cout << " Visible\n";
                    }
                } // if enable shadows



                // now that we know we can see the light
                const Vec3 negLightDirection = lights[iLight].d*-1;

                const Vec3 diffuse = (material.kd * norm.dot( negLightDirection ) );

                if(PRINT) {
                    cout << "norm dot " << norm.dot( negLightDirection ) << "\n";
                    cout << "diffuse " << diffuse.str(false) << "\n";
                }

                const Vec3 negRayDirection = r.d*-1;
                Vec3 idealR = Vec3::reflect(lights[iLight].d, norm );

                const float specular = material.ks * pow( std::max((double)0, idealR.dot( negRayDirection )), material.n);

                if(PRINT) {
                    cout << "specular: " << specular << "\n";
                    cout << "fp: " << fp.str(false) << " mag " << fp.mag() << "\n";
                    Vec3 t1 = lights[iLight].color / ( fp.mag() + this->c );
                    cout << "t1: " << t1.str(false) << "\n";

                    Vec3 t2 = diffuse + specular;
                    cout << "t2: " << t2.str(false) << "\n";

                    // cout << "Visiblity: " << visibilityLevel << "\n";
                }

                Vec3 lightEffects;

                // "no fog"
                // in this mode we do not take into account the distance
                // all objects will be lit with equal intensity regardless of distance
                // if( false ) {
                //     lightEffects =
                //       ( lights[iLight].color ) 
                //     * (diffuse + specular) * (visibilityLevel);
                // }

                // "fog mode"
                // in this mode the distance between the intersection point and then
                // camera affects the intensity of the light
                // this is where the global.c parameter comes into play
                if constexpr ( enableShadowsT ) {
                    lightEffects =
                      ( lights[iLight].color / ( fp.mag() + this->c ) ) 
                    * (diffuse + specular) * (visibilityLevel);
                } else {
                    lightEffects =
                      ( lights[iLight].color / ( fp.mag() + this->c ) ) 
                    * (diffuse + specular);
                }

                // Lights cannot have negative effect
                lightEffects.saturateMin(0.0);

                color = color + lightEffects;
            }
        }

        savedColor = color;
        savedRefl = refl;
        savedKr = material.kr;
        savedIntersect = intersect;
        savedNormal = norm;
        savedRefractionIndex = material.refraction;
        savedKt = material.kt;
    }




    //final actions after main loops--------



    if( hitDistance != NO_HIT ) {


        float reflect_prob = 1;

        // when shadow feeling we will calculate these
        // bool refractedHit = false;
        double childRefractedVisibility = 0;
        double thisRefractedVisibility = 0;

        if( savedKt != 0 ) {
            Vec3 outward_normal;
            Vec3 refracted;

            const float ref_idx = savedRefractionIndex;
            float ni_over_nt;
            float cosine;

            bool rayEntering = false;

            // when ray shoot through object back into vacuum,
            // ni_over_nt = ref_idx, surface normal has to be inverted.
            if ( Vec3::dot(r.d, savedNormal) > 0){
                outward_normal = savedNormal * -1.0;
                ni_over_nt = ref_idx;
                cosine = Vec3::dot(r.d, savedNormal);
                if( PRINT ) {
                    cout << "Ray exiting, cos:" << cosine << "\n";
                }
                rayEntering = false;
            }
            // when ray shoots into object,
            // ni_over_nt = 1 / ref_idx.
            else{
                outward_normal = savedNormal;
                ni_over_nt = 1.0 / ref_idx;
                cosine = -Vec3::dot(r.d, savedNormal);
                if( PRINT ) {
                    cout << "Ray entering, cos:" << cosine << "\n";
                }
                rayEntering = true;
            }




            // refracted ray exists
            if(refract(r.d, outward_normal, ni_over_nt, refracted)){
                reflect_prob = schlick(cosine, ref_idx);
            }
            // refracted ray does not exist
            else{
                // total reflection
                reflect_prob = 1.0;
            }

            if(PRINT) {
                cout << "reflect prob: " << reflect_prob << "\n";
            }


        // Refraction (transmission)
            Vec3 refractedColor(0,0,0);
            Ray refractedRay(savedIntersect,refracted);
            
            if( PRINT ) {
                cout << "\n" << std::string(depthIn, ' ') << "Tracing Refraction:\n";
            }
            auto [refractedHit, _childRefractedVisibility] =
                trace<enableShadowsT,refractShadowsT,shadowFeelingT>(refractedRay, depthIn+1, refractedColor);

            // the color we get back from refraction is damped by kt

            // color = savedColor + (refractedColor*savedKt); // orig

            const double refractionDamp = savedKt * (1-reflect_prob);

            if( PRINT ) {
                cout << "\n" << std::string(depthIn, ' ') << "Refraction Damp: " << refractionDamp << " \n";
            }

            // for whatever reason, our shadow feeler takes a double hit on visiblity
            // when refracting.  to fight this, we only calculate diminished visibility when
            // we enter a sphere.  when we exit, we just pass 1

            if( rayEntering ) {
                thisRefractedVisibility = refractionDamp;             // save up to outer scope
            } else {
                thisRefractedVisibility = 1;
            }
            childRefractedVisibility = _childRefractedVisibility; // save up to outer scope


            color = savedColor + (refractedColor * refractionDamp );

            savedColor = color; // HACK, REWORK
        } else {
            reflect_prob = 0;
            color = savedColor;
        }





        if constexpr( !shadowFeelingT ) {
            if( ((savedKr > 0) || (reflect_prob > 0)) ) {
                
                const Ray reflRay(savedIntersect,savedRefl);

                if( PRINT ) {
                    cout << "\n" << std::string(depthIn, ' ') << "Tracing Reflection using " << reflRay.d.str(false) << "\n";
                }


                Vec3 newColor(0,0,0);
                trace<enableShadowsT,refractShadowsT,shadowFeelingT>( reflRay, depthIn+1, newColor);

                // frenel adds the schlick function
                // which reduces the reflection on the sphere greatly as you move in from the edge

                double noTransmissonFactor = savedKr;
                double fullTransmissionFactor = savedKr * reflect_prob;

                double blendFactor = (fullTransmissionFactor*savedKt) + noTransmissonFactor*(1-savedKt);
                color = savedColor + (newColor * blendFactor);

                // This line is the first step towards my concept of "mirror ball"
                // it doesn't look like I thought it would so abandon it
                // color = savedColor + (newColor * savedKr);

            } else {
                color = savedColor;
            }
        } else {
            color = savedColor;
        }

        const double returnVisibility = (thisRefractedVisibility * childRefractedVisibility);

        if( PRINT ) {
            cout << "\n" << std::string(depthIn, ' ') << "Return Vis: " << returnVisibility << " \n";
        }
        
        return std::make_tuple(true, returnVisibility);
    } else {
        // we hit nothing
        color = noHitColor;
        return std::make_tuple(false, 1.0);
    }
}




#ifdef TYPE_TRAITS_EXAMPLE

template< class T, class P >
void typeTraitsExample(T a, P b) {
    bool match = false;
    if constexpr (std::is_pointer<T>::value) {
        cout << "is_pointer\n";
        match = true;
    }

    if ( std::is_base_of<std::vector<unsigned char>, T>::value ) {
        cout << "is_base_of vector\n";
        match = true;
    }

    if ( std::is_same<std::vector<unsigned char>, T>::value ) {
        cout << "is_same vector\n";
        match = true;
    }

    // use this
    if constexpr ( std::is_same<std::vector<unsigned char>&, T>::value ) {
        cout << "is_same vector&\n";
        match = true;
    }

    if( !match ) {
        cout << "no match\n";
    }
}
#endif


///
/// This template allows us to have 1 for loop that can copy
/// to the GL buffer (WASM), or to the vector of chars (PNG)
/// because we are in c++20 we can use constexpr if to get
/// performant inner loop
///
/// We check this by comparing the first argument
/// against a refrence to a vector of chars
/// if it matches, we are in the PNG mode
/// if it doesn't match we are in the WASM mode
/// the wasm mode is way more complicated requiring 1 funciton pointer and 2 void*
/// we also have a different pixel order in the 2 modes
///
template<bool writeAlphaT, class T, class P, class Q>
void _copyToPixels(T arg0, P arg1, Q arg2, const RayEngine* const engine) {
    double scale = engine->scale;
    // double scale = 1.0/0.006;
    // scale = 256;

    const uint32_t xPx = engine->xPx;
    const uint32_t yPx = engine->yPx;

    uint8_t ab = 255;

    for( unsigned y = 0; y < yPx; y++ ) {
        for( unsigned x = 0; x < xPx; x++ ) {
            
            const unsigned lookup = x + (xPx*(yPx - 1 - y));


            const float fr = engine->rBuffer[lookup];
            const float fg = engine->gBuffer[lookup];
            const float fb = engine->bBuffer[lookup];
            const float rs = (fr * scale);
            const float gs = (fg * scale);
            const float bs = (fb * scale);

            const uint8_t rb = (rs > 0) ? ( (rs>=255) ? 255 : rs ) : (0);
            const uint8_t gb = (gs > 0) ? ( (gs>=255) ? 255 : gs ) : (0);
            const uint8_t bb = (bs > 0) ? ( (bs>=255) ? 255 : bs ) : (0);

            if constexpr (writeAlphaT) {
                const float fa = engine->aBuffer[lookup];
                const float as = (fa * RayEngine::defaultScale); // alpha channel does not have adjustible scale
                ab = (as > 0) ? ( (as>=255) ? 255 : as ) : (0);
            }

            if constexpr ( std::is_same<std::vector<unsigned char>&, T>::value ) {
                // png
                arg0.emplace_back(rb);
                arg0.emplace_back(gb);
                arg0.emplace_back(bb);
                arg0.emplace_back(ab);
            } else {
                if constexpr (writeAlphaT) {
                    // wasm with alpha
                    *((uint32_t*)arg1 + ((y) * xPx) + x) = arg0((const SDL_PixelFormat*)arg2, rb, gb, bb, ab);
                } else {
                    // wasm no alpha
                    *((uint32_t*)arg1 + ((y) * xPx) + x) = arg0((const SDL_PixelFormat*)arg2, rb, gb, bb);
                }
            }
        } // for x
    } // for y
} // _copyToPixels


// wasm no alpha
void RayEngine::copyToPixels(wasm_gl_pixel_t fn, void* const pixels, void* const format) const {
    _copyToPixels<false>(fn, pixels, format, this);
}

// wasm with alpha
// this works however it seems like the canvas drops the alpha channel
// so output in canvas is identical to no alpha :(
void RayEngine::copyToPixels(wasm_gl_pixel_alpha_t fn, void* const pixels, void* const format) const {
    _copyToPixels<true>(fn, pixels, format, this);
}

// png
void RayEngine::copyToPixels(std::vector<unsigned char>& buffer) const {
    buffer.resize(0);
    buffer.reserve(yPx*xPx*4);

    if( enableAlpha ) {
        _copyToPixels<true, std::vector<unsigned char>&, void*>(buffer, 0, 0, this);
    } else {
        _copyToPixels<false, std::vector<unsigned char>&, void*>(buffer, 0, 0, this);
    }

}



// this is the modern c++ way to include a template
// due to the way I've written render() vs _render() these should
// never be needed
// template void RayEngine::_render<false, false, false>();

