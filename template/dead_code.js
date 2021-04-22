
///
/// Old render method
/// This may still be needed for extracting sprites from the engine
///
function bufferToCanv() {
  const get4 = Module.cwrap('get4', 'number');
  const getNext = Module.cwrap('getNext', 'number');
  const getPixel = Module.cwrap('getPixel', 'number', ['number','number']);

  let px = 400;

  for(let i = 0; i < px; i++) {
    for(let j = 0; j < px; j++) {
      let p = getPixel(i,j);

      const r = ((p>>>16)&0xff) >> 0;
      const g = ((p>>>8)&0xff) >> 0;
      const b = ((p>>>0)&0xff) >> 0;

      // if( r != 0 ) {
      //   console.log(r);
      // }
      cdat[0] = r;
      cdat[1] = g;
      cdat[2] = b;
      ctx.putImageData( id, i, j );

    }
  }
}




///
/// Code to bind to c++ functions when not using --proxyu
/// 

function renderDebugClick() {
  const doDebugRender = Module.cwrap('doDebugRender', 'void');
  doDebugRender();
  setTimeout(bufferToCanv,1000);
}

function setScaleClick() {
  var el = document.getElementById("scaleText");

  const val = parseFloat(el.value);

  console.log(val);

  // alert(val);

  const setScale = Module.cwrap('setScale', 'void', ['number']);

  setScale(val);
}

