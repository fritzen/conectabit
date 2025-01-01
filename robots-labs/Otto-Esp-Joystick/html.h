char html[] PROGMEM = R"=====(

<html lang="en">

<head><meta http-equiv="Content-Type" content="text/html; charset=UTF-8">

  
<script>
  let Socket;
  function start() {
    //Socket = new WebSocket('ws://' + window.location.hostname + ':81/');
    Socket = new WebSocket('ws://192.168.0.2:81/');
    
    Socket.onmessage = function(evt) {
    //document.getElementById("rxConsole").value += evt.data;
    }
    
  }
  
  function enterpressed() {
   Socket.send("fritzen");     
  }
  </script>


<title>Robot</title>
<link rel="stylesheet" href="./styles.css" data-inprogress="">

<style>
    body {
  margin: 0;
  padding: 0;

    width: 100%;
  height: 100%;
}


.zone {
  display: none;
  position: absolute;
  width: 100%;
  height: 100%;
  left: 0;
}

.zone.active {
  display: block;
}

.zone > h1 {
  position: absolute;
  padding: 10px 10px;
  margin: 0;
  color: white;
  right: 0;
  bottom: 0;
}

.zone.dynamic {
  background: rgba(0, 0, 255, 0.1);
}


  </style>


</head>
<body translate="no" onload="javascript:start();">

<div id="zone_joystick">
    <div id="debug">
    <ul>
    <li class="position">
    position :
    <ul>
    <li class="x">x : <span class="data">762</span></li>
    <li class="y">y : <span class="data">545</span></li>
    </ul>
    </li>
    <li class="force">force : <span class="data">3.825388869121674</span></li>
    <li class="pressure">pressure : <span class="data">0.5</span></li>
    <li class="distance">distance : <span class="data">50</span></li>
    <li class="angle">
    angle :
    <ul>
    <li class="radian">radian : <span class="data">6.167909181208635</span></li>
    <li class="degree">degree : <span class="data">353.39516450324606</span></li>
    </ul>
    </li>
    <li class="direction">
    direction :
    <ul>
    <li class="x">x : <span class="data">right</span></li>
    <li class="y">y : <span class="data">down</span></li>
    <li class="angle">angle : <span class="data">right</span></li>
    </ul>
    </li>
    </ul>

</div>

<div class="zone dynamic" style="touch-action: none;"><h1>fritzen.io</h1></div>

</div>



<script src="./nipplejs.js"></script>
<script id="rendered-js">
      var s = function (sel) {
  return document.querySelector(sel);
};
var sId = function (sel) {
  return document.getElementById(sel);
};

var joysticks = {
  dynamic: {
    zone: s('.zone.dynamic'),
    color: 'white',
    multitouch: true }
  
  };


var joystick;

// Get debug elements and map them
var elDebug = sId('debug');
var elDump = elDebug.querySelector('.dump');
var els = {
  position: {
    x: elDebug.querySelector('.position .x .data'),
    y: elDebug.querySelector('.position .y .data') },

  force: elDebug.querySelector('.force .data'),
  pressure: elDebug.querySelector('.pressure .data'),
  distance: elDebug.querySelector('.distance .data'),
  angle: {
    radian: elDebug.querySelector('.angle .radian .data'),
    degree: elDebug.querySelector('.angle .degree .data') },

  direction: {
    x: elDebug.querySelector('.direction .x .data'),
    y: elDebug.querySelector('.direction .y .data'),
    angle: elDebug.querySelector('.direction .angle .data') } };




var timeoutCreate;
function createThrottle(evt) {
  clearTimeout(timeoutCreate);
  timeoutCreate = setTimeout(() => {
    createNipple(evt);
  }, 100);
}

createNipple('dynamic');

function bindNipple() {
  joystick.on('start end', function (evt, data) {
    dump(evt.type);
    debug(data);
  }).on('move', function (evt, data) {
    debug(data);
  }).on('dir:up plain:up dir:left plain:left dir:down ' +
  'plain:down dir:right plain:right',
  function (evt, data) {
    dump(evt.type);
  }).
  on('pressure', function (evt, data) {
    debug({
      pressure: data });

  });
}

function createNipple(evt) {
  var type = typeof evt === 'string' ?
  evt : evt.target.getAttribute('data-type');
  if (joystick) {
    joystick.destroy();
  }
  s('.zone.' + type).className += ' active';
  joystick = nipplejs.create(joysticks[type]);
  bindNipple();
}

function testInterval(refValue, degreeValue) {
  const step = 22.5;
  let max = refValue + step;
  let min = refValue - step;
  return (degreeValue >= min && degreeValue <= max);
}

// Print data into elements
function debug(obj) {
  if (obj.angle) {
    //console.log(obj.angle.degree);
    //console.log(obj.distance);


          if (obj.distance > 0) {
            let degree = obj.angle.degree;
            const right_up = 45;
            const up = 90;
            const left_up = 135;
            const left = 180;
            const left_down = 225;
            const down = 270;
            const right_down = 315;
            const right = 0;

            let choice = "";

            if (testInterval(right_up, degree)){
              choice = "right_up";
            } else if (testInterval(up, degree)) {
              choice = "up";
            } else if (testInterval(left_up, degree)) {
              choice = "left_up";
            } else if (testInterval(left, degree)) {
              choice = "left";
            } else if (testInterval(left_down, degree)) {
              choice = "left_down";
            }  else if (testInterval(down, degree)) {
              choice = "down";
            } else if (testInterval(right_down, degree)) {
              choice = "right_down";
            } else {
              choice = "right";
            }
            console.log(choice);
            Socket.send(choice);
          }
  }
  
  function parseObj(sub, el) {
    for (var i in sub) {
      if (typeof sub[i] === 'object' && el) {
        parseObj(sub[i], el[i]);
      } else if (el && el[i]) {
        el[i].innerHTML = sub[i];
      }
    }
  }
  setTimeout(function () {
    parseObj(obj, els);
  }, 0);
}

var nbEvents = 0;

// Dump data
function dump(evt) {
  setTimeout(function () {

    if (evt == "end") {
      console.log(JSON.stringify(evt));
      Socket.send(evt);
    }

    var newEvent = document.createElement('div');
    newEvent.innerHTML = '#' + nbEvents + ' : <span class="data">' +
    evt + '</span>';

    nbEvents += 1;
  }, 0);
}

    </script>


</body></html>

)=====";
