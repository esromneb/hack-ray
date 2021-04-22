Raytracer
===
This project came from a homework assignment I had 12 years ago in my undergraduate computer graphics class (UC Davis ECS175).  The purpose of this project is to modernize and package the raytracer for use in the browser via WASM.  In general I had the idea that this should be able to render semi-realtime, however I'm not sure it's there yet.  The name (Double Ray) is just a play on the double precision float datatype.

Images
===
![](img/ideal/refraction_9.png)
![](img/ideal/soap_bubble_1.png)
![](img/ideal/color_balls_refraction_2.png)
![](img/ideal/test_shadow_2.png)

Project Priorities
===
These are the priorities I kept in mind through development:
* Correctness
* Ease of Use
* Performance
* Code cleanliness


State of things
===
Currently the project is a bit limited in functionality, but it does support:
* Spheres only
* Reflection, Refraction, Diffuse, Specular (phong), light shadows
* Light sources are a direction only, and have no origin
* Multiple shadowing modes

Things I want to add
===
* Poloygon support (triangles)
* Bounding boxes to improve performance
* Diffuse shading that includes reflection from lights
* Spherical light model
* Change FOV (currently fixed at 53.0155)

How to use this project
===
* I'm still trying to understand the best way to publish / include this project.



Compile
===
```bash
make
```

Run
===
```bash
cd out
sudo python -m SimpleHTTPServer 80
```

open ray.html in firefox (chrome is slower for me)


Notes
===
We use template/controls.html to generate ray.html



Deps
===
On ubuntu:

```bash
git clone https://github.com/emscripten-core/emsdk.git
cd emsdk/
git pull
./emsdk install latest
./emsdk activate latest
source ./emsdk_env.sh
realpath ./emsdk_env.sh 
```

Instructions from: https://emscripten.org/docs/getting_started/downloads.html


See Also Emscripten
===
* https://emscripten.org/docs/compiling/Deploying-Pages.html - emscripten instructions for custom html
* https://blog.scottlogic.com/2014/03/12/native-code-emscripten-webgl-simmer-gently.html - webgl
* https://compile.fi/canvas-filled-three-ways-js-webassembly-and-webgl/ - 3 ways to canvas
* https://brionv.com/log/2019/10/24/exception-handling-in-emscripten-how-it-works-and-why-its-disabled-by-default/ - exceptions
* https://emscripten.org/docs/compiling/Travis.html - travis
* https://emscripten.org/docs/api_reference/emscripten.h.html?highlight=build_as_worker#worker-api - workers
* https://livebook.manning.com/book/webassembly-in-action/c-emscripten-macros/v-7/64 - in-depth passing c->js
* https://emscripten.org/docs/porting/connecting_cpp_and_javascript/Interacting-with-code.html
* https://emscripten.org/docs/api_reference/preamble.js.html


Libraries Used
===
* https://github.com/nlohmann/json
* https://github.com/lvandeve/lodepng
* https://github.com/CLIUtils/CLI11
