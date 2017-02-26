
function drawLineChart() {
  // Add a helper to format timestamp data
  Date.prototype.formatMMDDYYYY = function () {
    return (this.getMonth() + 1) +
      "/" +
      this.getDate() +
      "/" +
      this.getFullYear();
  }

  $.ajax({
    url: location.href + 'API/v1/Test/Temperature',
    dataType: 'json'
  }).done(function (results) {
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

    var myChart = new Chart(ctx, {
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
              //type: 'logarithmic',
              //display: false,
              stacked: true
            }
          ]
        }
      }
    });

    setTimeout(drawLineChart, 10000);
  });
}

drawLineChart();