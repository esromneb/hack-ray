// This file (pre.ray.js)
// gets appended to the top of ray.js
////////////////////////////////////////

let debugCustomDispatch = false;

// This is on worker thread
Module['onCustomMessage'] = (x) => {
  if( debugCustomDispatch ) {
    console.log('worker got msg: ' + JSON.stringify(x.data));
  }
  if(x.data.userData && x.data.userData.type) {
    Module['dispatchCustomMessageType'](x.data.userData);
  } else {
    console.log('worker DROPPED: ' + JSON.stringify(x.data));
  }
}


///
/// The main thread posts a js object to us that looks like
///
/// {"type":"call","fn":"renderNextRainbow","rt":"void"}
/// {"type":"call","fn":"coutIntDual","rt":"void","at":["number","number"],"av":[5,6]}
///
/// Which allows us to specify the full function signature as well as input values for the signature
/// The keys are
/// fn -> function name
/// rt -> return type
/// at -> argument types
/// av -> argument values
///

// This is on worker thread
Module['dispatchCustomMessageType'] = (x) => {
  if( debugCustomDispatch ) {
    console.log('in dispatchCustomMessageType' + JSON.stringify(x));
  }

  switch(x.type) {
    case 'call': {

        ///
        /// Handle most specific cases first so that we can try objets with missing keys below
        ///

        if ( x.fn && x.rt && x.at && x.av ) {
          // handle the void, any case
          // console.log('in void, any case');

          const boundFn = Module.cwrap(x.fn, 'void', x.at);
          // this is the spread operator
          boundFn(...x.av);

        } else if( x.fn && x.rt ) {
          // handle void, void case
          
          const boundFn = Module.cwrap(x.fn, 'void');
          boundFn();
        } else {
          console.log('dispatchCustomMessageType switch CALL DROPPED: ' + JSON.stringify(x));
        }
      break;
    }
    default: {
      console.log('dispatchCustomMessageType DROPPED: ' + JSON.stringify(x));
    }
  }
}

function postToMain(data0, data1) {
  if( data0 == 0 ) {
    postCustomMessage({type:'call',fn:'setupOrbit',rt:'void',at:['number'],av:[30]});
  } else if ( data0 == 1) {
    postCustomMessage(data1);
  }
}

// https://groups.google.com/forum/#!topic/emscripten-discuss/_M75jXb2-HY
// https://emscripten.org/docs/api_reference/preamble.js.html#preamble-js
// data0 is a char* from c
// the actual data0 will be a number
// we need to pass this to AsciiToString() which gives us a javascript
// string
function postB64ToMain(data0,tag) {
  let str = AsciiToString(data0);
  // console.log(bar);
  postCustomMessage({type:'b64',s:str,tag:tag});
}


function myfn() {
  console.log('myfn() got called');
}

////////////////////////////////////////
// end of pre.ray.js