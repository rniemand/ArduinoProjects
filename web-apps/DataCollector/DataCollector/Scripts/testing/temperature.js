var myChart = null;


function drawLineChart() {
  delete myChart;

  $.ajax({
    url: location.href + 'API/v1/Test/Temperature',
    dataType: 'json'
  })
    .done(function (results) {
      var tempData = [];
      var humidData = [];
      var heat = [];
      var ldr = [];

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

        ldr.push({
          x: x.TimeLoggedUtc,
          y: x.LDR
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
              label: 'Light',
              data: ldr,
              backgroundColor: "rgba(255,249,54,0.4)",
              borderColor: "rgba(255,249,54,1)"
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