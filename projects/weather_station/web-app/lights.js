var cb = document.getElementById('tvlights');

var $ = function (id) {
  return document.getElementById(id);
}

var ajax = function (url, callback) {
  var xhttp = new XMLHttpRequest();
  xhttp.onreadystatechange = function () {
    if (this.readyState == 4 && this.status == 200) {
      callback(this.responseText);
    }
  };
  xhttp.open("GET", url, true);
  xhttp.send();
};

var updateBtn = function (x) {
  cb.checked = x === 'ON';
};

function toggleLights() {
  ajax((cb.checked ? 'on.htm' : 'off.htm'), updateBtn);
}

function getSensorData() {
  ajax('sensors.json', function (data) {
    data = JSON.parse(data);
    $('humidity').innerText = data.humidity + ' %';
    $('temp').innerText = data.temperature + ' * C';
    $('heat').innerText = Math.round(data.heatIndex) + ' * C';
    $('light').innerText = data.light;
    $('freeHeap').innerText = data.freeHeap;
    $('tickInt').innerText = data.tickInt;
    $('pubInt').innerText = data.pubInt + ' ' + Math.floor(data.pubTickCount / data.pubInt * 100) + '%';
    $('pingInt').innerText = data.pingInt + ' ' + Math.floor(data.pingTickCount / data.pingInt * 100) + '%';
    $('updateSensorInt').innerText = data.updateSensorInt + ' ' + Math.floor(data.updateTickCount / data.updateSensorInt * 100) + '%';
    setTimeout(getSensorData, 2000);
  });
}

// Kick everything off
ajax('status.htm', updateBtn);
getSensorData();