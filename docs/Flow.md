# Flow




# main2.cpp
has all bound functions for js->c

# raytrace.cpp
news the engine, sets up the scene and renders


# Notes
* makeObjects chooses the scene (for now)







# proxy flow
* main is called and can run forever, however we don't need it to.
* all calls to Module.cwrap must be done on the worker thread
* in order to get to this from the main thread, we need to postCustomMessage().
* I'm writing my own async version of Module.cwrap to expand full functionality
* edit pre.ray.js and proxy_controls.html
* if the worker thread takes forever, all worker javascript is blocked, however any main javascript keeps running
* We can use this for our goal of background raytracing to load sprites

