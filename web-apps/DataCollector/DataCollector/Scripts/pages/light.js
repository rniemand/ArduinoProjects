var myChart = null;

var baseUrl = location.href.toLowerCase().split('/home')[0] + '/';
var apiUrl = baseUrl + 'API/v1/Light/1';

function drawLineChart() {
  delete myChart;

  $.ajax({ url: apiUrl, dataType: 'json' })
    .done(function (results) {
      var ldr = [];

      results.forEach(function (x) {
        ldr.push({
          x: x.TimeLoggedUtc,
          y: x.LightReading
        });
      });

      var ctx = document.getElementById("myChart");

      myChart = new Chart(ctx, {
        type: 'line',
        data: {
          datasets: [
            {
              label: 'Light',
              data: ldr,
              backgroundColor: "rgba(255,249,54,0.4)",
              borderColor: "rgba(255,249,54,1)"
            }
          ]
        },
        options: {
          scales: {
            xAxes: [
              {
                type: 'time',
                position: 'bottom'
              }
            ],
            yAxes: [
              {
                stacked: false
              }
            ]
          }
        }
      });
    });
}

drawLineChart();