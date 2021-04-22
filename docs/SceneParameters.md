# Scene Parameters

## Global
* "n" - this is used when calculating specular / diffuse light intensity.  The distance from the camera is used to dim the light.  This factor is added.  Note this factor does not affect ambient lighting/

* "nohit_color" - this is used when a ray doesn't hit anything.  typically this is black

* "scale" - All light values in the scene are calulcated using floating point numbers.  When it comes time to render, this value is used to convert to a [0,256) byte value for RGB.

