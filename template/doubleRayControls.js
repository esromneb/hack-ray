

function doRenderOfficial() {
  postCustomMessage({type:'call',fn:'doRenderOfficial',rt:'void'});
}

function setupEngineClick() {
  postCustomMessage({type:'call',fn:'setupEngine',rt:'void'});
}

function nextRainbowClick() {
  postCustomMessage({type:'call',fn:'renderNextRainbow',rt:'void'});
}


function coutIntClick() {
  postCustomMessage({type:'call',fn:'coutInt',rt:'void',at:['number'],av:[4]});
}

function coutIntDualClick() {
  postCustomMessage({type:'call',fn:'coutIntDual',rt:'void',at:['number','number'],av:[5,6]});
}

function dumpPoly(n) {
  postCustomMessage({type:'call',fn:'dumpPoly',rt:'void',at:['number'],av:[n]});
}


function addVec3Signature(vin, n) {
  var vin2 = typeof vin !== 'undefined' ? vin : [];
  var n2 = typeof n !== 'undefined' ? n : 1;

  for(let i = 0; i < n2; i++) {

    vin2.push('number');
    vin2.push('number');
    vin2.push('number');
  }

  return vin2;
}

function addNumberSignature(vin, n) {
  var vin2 = typeof vin !== 'undefined' ? vin : [];
  var n2 = typeof n !== 'undefined' ? n : 1;

  for(let i = 0; i < n2; i++) {
    vin2.push('number');
  }

  return vin2;
}

function setCamera(location, direction, rotation) {
  if( !Array.isArray(location) || location.length != 3 ) {
    console.log("Illegal argument passed to setCamera()");
    return;
  }

  if( !Array.isArray(direction) || direction.length != 3 ) {
    console.log("Illegal argument passed to setCamera()");
    return;
  }

  if( !Array.isArray(rotation) || rotation.length != 3 ) {
    console.log("Illegal argument passed to setCamera()");
    return;
  }

  const sig = addVec3Signature([],3);

  const args = [location,direction,rotation].flat();

  postCustomMessage({type:'call',fn:'setCamera',rt:'void',at:sig,av:args});
}

function setDepth(val) {
  postCustomMessage({type:'call',fn:'setDepth',rt:'void',at:['number'],av:[val]});
}

function setAmbientColor(color) {
  if( !Array.isArray(color) || color.length != 3 ) {
    console.log("Illegal argument passed to setAmbientColor()");
    return;
  }

  const sig = addVec3Signature([],1);

  const args = [color].flat();

  postCustomMessage({type:'call',fn:'setAmbientColor',rt:'void',at:sig,av:args});
}

function setNoHitColor(color) {
  if( !Array.isArray(color) || color.length != 3 ) {
    console.log("Illegal argument passed to setNoHitColor()");
    return;
  }

  const sig = addVec3Signature([],1);

  const args = [color].flat();

  postCustomMessage({type:'call',fn:'setNoHitColor',rt:'void',at:sig,av:args});
}




function setGlobalC(val) {
  postCustomMessage({type:'call',fn:'setGlobalC',rt:'void',at:['number'],av:[val]});
}

function setSphereCount(val) {
  postCustomMessage({type:'call',fn:'setSphereCount',rt:'void',at:['number'],av:[val]});
}

function setSphere(
  index,
  radius,
  location,
  ambient,
  specular,
  reflected,
  transmitted,
  diffuse,
  n,
  refraction
  ) {

  if( !Array.isArray(location) || location.length != 3 ) {
    console.log("Illegal argument passed to setSphere()");
    return;
  }

  if( !Array.isArray(diffuse) || diffuse.length != 3 ) {
    console.log("Illegal argument passed to setSphere()");
    return;
  }

  const argc = 
      1   // index
    + 1   // radius
    + 3   // location
    + 1   // ambient
    + 1   // specular
    + 1   // reflected
    + 1   // transmitted
    + 3   // diffuse
    + 1   // n
    + 1;  // refraction

  const sig = addNumberSignature([],argc);

  const args = [index,radius,location,ambient,specular,reflected,transmitted,diffuse,n,refraction].flat();

  postCustomMessage({type:'call',fn:'setSphere',rt:'void',at:sig,av:args});

}


function setLightCount(val) {
  postCustomMessage({type:'call',fn:'setLightCount',rt:'void',at:['number'],av:[val]});
}

function setLight(
  index,
  direction,
  color
  ) {

  if( !Array.isArray(direction) || direction.length != 3 ) {
    console.log("Illegal argument passed to setLight()");
    return;
  }

  if( !Array.isArray(color) || color.length != 3 ) {
    console.log("Illegal argument passed to setLight()");
    return;
  }

  const argc = 
      1   // index
    + 3   // direction
    + 3;  // color

  const sig = addNumberSignature([],argc);

  const args = [index,direction,color].flat();

  postCustomMessage({type:'call',fn:'setLight',rt:'void',at:sig,av:args});

}


function setupOrbit(val) {
  postCustomMessage({type:'call',fn:'setupOrbit',rt:'void',at:['number'],av:[val]});
}

function nextOrbitRender(doRender) {
  postCustomMessage({type:'call',fn:'nextOrbitRender',rt:'void',at:['boolean'],av:[doRender]});
}

function dumpCamera() {
  postCustomMessage({type:'call',fn:'dumpCamera',rt:'void'});
}

function debugCallback() {
  postCustomMessage({type:'call',fn:'debugCallback',rt:'void'});
}

function chokeOutput(a,b,c,d) {


  const sig = addNumberSignature([],4);

  const args = [a,b,c,d].flat();

  postCustomMessage({type:'call',fn:'chokeOutput',rt:'void',at:sig,av:args});
}

function setPrint(val) {
  postCustomMessage({type:'call',fn:'setPrint',rt:'void',at:['number'],av:[val]});
}

function setPrintRenderTime(val) {
  postCustomMessage({type:'call',fn:'setPrintRenderTime',rt:'void',at:['number'],av:[val]});
}

function setPrintRotCamera(val) {
  postCustomMessage({type:'call',fn:'setPrintRotCamera',rt:'void',at:['number'],av:[val]});
}


function setHighlightPixel(x,y) {
  postCustomMessage({type:'call',fn:'setHighlightPixel',rt:'void',at:['number','number'],av:[x,y]});
}

function parseJsonScene(str,andRender,useCanvas,n) {
  postCustomMessage({type:'call',fn:'parseJsonScene',rt:'void',at:['string','boolean','boolean','number'],av:[str,andRender,useCanvas,n]});
}

function resizeBuffer(x,y) {
  postCustomMessage({type:'call',fn:'resizeBuffer',rt:'void',at:['number','number'],av:[x,y]});
}

function resizeCanvas(x,y) {
  postCustomMessage({type:'call',fn:'resizeCanvas',rt:'void',at:['number','number'],av:[x,y]});
}

function setEnableAlpha(b) {
  postCustomMessage({type:'call',fn:'setEnableAlpha',rt:'void',at:['boolean'],av:[b]});
}


///
/// Looks at the parsed object and verifies all keys are there
/// makes all the c calls and then renders at the end
/// NOTE we may modify obj as we parse through due to default values
/// etc. (obj remains modified after the call to this function)
function doubleRayExampleDoJsonCalls(obj) {
  if(obj.camera) {
    const camera = obj.camera;
    if( 
      typeof(camera.loc) !== 'undefined' && 
      typeof(camera.dir) !== 'undefined' &&
      typeof(camera.rot) !== 'undefined' &&
      typeof(camera.depth) !== 'undefined' ) {
      setCamera(camera.loc, camera.dir, camera.rot);
      setDepth(camera.depth);
    } else {
      jsPrint("Camera tree missing required keys");
      return;
    }
  } else {
    jsPrint("Camera tree missing");
    return;
  }

  if(obj.global) {
    const gg = obj.global;
    if( (typeof(gg.color) !== 'undefined' || typeof(gg.ambient_color) !== 'undefined') && typeof(gg.c) !== 'undefined' ) {

      const ambient = (typeof(gg.color) !== 'undefined') ? gg.color : gg.ambient_color;

      setAmbientColor(ambient);
      setGlobalC(gg.c);
    } else {
      jsPrint("Global tree missing required keys");
      return;
    }

    if( typeof(gg.nohit_color) !== 'undefined' ) {
      setNoHitColor(gg.nohit_color);
    } else {
      /// default is black
      /// js is responsible for this because if a scene doesn't have a nohit key
      /// we will never set this and the nohit_color from the previous scene will be used
      /// is there a way to make C responsible for this?
      setNoHitColor([0,0,0]);
    }

  } else {
    jsPrint("Global tree missing");
    return;
  }

  if(obj.spheres) {
    setSphereCount(0); // will delete all spheres in vector
    
    const spheres = obj.spheres;
    setSphereCount(spheres.length); // prevents multiple resizes of vector in c++

    for(let i = 0; i < spheres.length; i++ ) {
      const s = spheres[i];
      if(
       typeof(s.rad) !== 'undefined' &&
       typeof(s.loc) !== 'undefined' &&
       typeof(s.ambient) !== 'undefined' &&
       typeof(s.specular) !== 'undefined' &&
       typeof(s.reflected) !== 'undefined' &&
       typeof(s.transmitted) !== 'undefined' &&
       typeof(s.diffuse) !== 'undefined' &&
       typeof(s.n) !== 'undefined' ) {

       if(typeof(s.refraction) === 'undefined') {
        s.refraction = 1.0;
       }

       setSphere(
         i,
         s.rad,
         s.loc,
         s.ambient,
         s.specular,
         s.reflected,
         s.transmitted,
         s.diffuse,
         s.n,
         s.refraction);

      } else {
        jsPrint("Sphere #" + i + " missing required keys");
      }
    }

      
  } else {
    // spheres tree missing
    // we assume the users just doesn't want any spheres
    setSphereCount(0); // will delete all spheres in vector
  }

  if(obj.lights) {
    setLightCount(0); // will delete all lights in vector
    
    const lights = obj.lights;
    setLightCount(lights.length); // prevents multiple resizes of vector in c++

    for(let i = 0; i < lights.length; i++ ) {
      const s = lights[i];
      if(
       typeof(s.dir) !== 'undefined' &&
       typeof(s.color) !== 'undefined' ) {
        setLight(
          i,
          s.dir,
          s.color
          );

      } else {
        jsPrint("Light #" + i + " missing required keys");

        // c++ will segfault unless we give "legal" default light
        setLight(
          i,
          [1,0,0],
          [0,0,0]
          );
      }
    }

  } else {
    // spheres tree missing
    // we assume the users just doesn't want any spheres
    setLightCount(0); // will delete all lights in vector
  }

  doRenderOfficial();
}