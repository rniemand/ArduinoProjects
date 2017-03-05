var cb = document.getElementById('tvlights');

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
  ajax((cb.checked ? 'on.html' : 'off.html'), updateBtn);
}

ajax('status.html', updateBtn);