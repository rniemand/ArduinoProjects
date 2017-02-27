var myChart = null;

var baseUrl = location.href.toLowerCase().split('/home')[0] + '/';
var apiUrl = baseUrl + 'API/v1/Temperature/1';

function drawLineChart() {
  delete myChart;

  $.ajax({ url: apiUrl, dataType: 'json' })
    .done(function (results) {
      var tempData = [];
      var humidData = [];
      var heat = [];

      results.forEach(function (x) {
        tempData.push({
          x: x.TimeLoggedUtc,
          y: x.Temperature
        });

        humidData.push({
          x: x.TimeLoggedUtc,
          y: x.Humidity
        });

        heat.push({
          x: x.TimeLoggedUtc,
          y: x.HeatIndex
        });
      });

      var ctx = document.getElementById("myChart");

      myChart = new Chart(ctx, {
        type: 'line',
        data: {
          datasets: [
            {
              label: 'Temperature',
              data: tempData,
              backgroundColor: "rgba(237,37,37,0.4)",
              borderColor: "rgba(237,37,37,1)"
            },
            {
              label: 'Humidity',
              data: humidData,
              backgroundColor: "rgba(123,255,107,0.4)",
              borderColor: "rgba(11,104,0,1)"
            },
            {
              label: 'HeatIndex',
              data: heat
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

      //setTimeout(drawLineChart, 100);
    });
}

drawLineChart();