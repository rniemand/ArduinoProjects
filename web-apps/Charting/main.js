var channelId = 235274;
var readKey = 'PY5WT8H69IGPBO80';
var recordLimit = 10;
var refreshInterval = 30 * 1000; // every 30 seconds

var updateTimeout = null;
var chartAll = null;
var chartLight = null;
var chartTemp = null;
var chartHumidity = null;
var table = null;
var firstRun = true;

var graphData = {
  light: [],
  temperature: [],
  humidity: [],
  everything: [],
  latestReadings: []
};

var fields = {};

var allData = {};

var buildUrl = function () {
  return 'https://api.thingspeak.com/channels/' + channelId +
    '/feeds.json?results=' + recordLimit + '&api_key=' + readKey;
};

var colors = {
  temp: '#e82939',
  heatIndex: '#da3bef',
  humidity: '#5151f1',
  light: '#f0e851'
};

var baseOptions = {
  'title': 'Weather Station',
  'width': '100%',
  'height': 450,
  crosshair: { trigger: 'focus' },
  pointSize: 2,
  /*
  series: {
    0: { lineWidth: 1 },
    1: { lineWidth: 2 },
    2: { lineWidth: 1 }
  },
  */
  colors: [colors.temp, colors.heatIndex, colors.humidity, colors.light],
  aggregationTarget: 'series',
  areaOpacity: 0.2,
  //chartArea: { left: 20, top: 0, width: '100%', height: '75%' }
  isStacked: false,
  interpolateNulls: true
};

var forceRefresh = function () {
  // Change update interval
  recordLimit = parseInt($('#numRecords').val());
  var updateInt = parseInt($('#refreshInt').val());
  if (updateInt < 5) updateInt = 5;
  refreshInterval = updateInt * 1000;
  $('#refreshInt').val(refreshInterval / 1000);

  // Clear timeout
  clearTimeout(updateTimeout);
  updateTimeout = null;

  // Run an update now!
  updateData();
};

var getDataPoints = function (field, numPoints, name) {
  var currentKey = parseInt(Object.getOwnPropertyNames(allData).pop());

  if (numPoints >= currentKey) numPoints = currentKey;
  var endKey = currentKey - numPoints;
  var data = [
    ['Time', name]
  ];

  for (var i = currentKey; i >= endKey; i--) {
    if (!allData[i]) continue;

    data.push([
      allData[i].created_at,
      allData[i][field]
    ]);
  }

  return data;
}

var updateGraphs = function () {

  chartAll.draw(google.visualization.arrayToDataTable(
    graphData.everything),
    opt_all);

  chartLight.draw(google.visualization.arrayToDataTable(
    getDataPoints('field4', 10, 'Light Reading')),
    opt_light);

  chartTemp.draw(google.visualization.arrayToDataTable(
    getDataPoints('field1', 10, 'Temperature')),
    opt_temp);

  chartHumidity.draw(google.visualization.arrayToDataTable(
    getDataPoints('field3', 10, 'Humidity')),
    opt_humid);

  drawTable();

  updateTimeout = setTimeout(updateData, refreshInterval);
};

var handleFirstRun = function () {
  if (!firstRun) return;

  firstRun = false;
  recordLimit = parseInt($('#numRecords').val());
};

var extractFieldMappings = function (rawData) {
  if (!firstRun) return;

  if (rawData.channel.hasOwnProperty('field1')) {
    fields['field1'] = rawData.channel.field1;
  }

  if (rawData.channel.hasOwnProperty('field2')) {
    fields['field2'] = rawData.channel.field2;
  }

  if (rawData.channel.hasOwnProperty('field3')) {
    fields['field3'] = rawData.channel.field3;
  }

  if (rawData.channel.hasOwnProperty('field4')) {
    fields['field4'] = rawData.channel.field4;
  }
};

var updateData = function () {
  console.log('fetching new data');

  $.getJSON(buildUrl())
    .done(function (rawData) {
      // NOTE: ordering here is important
      extractFieldMappings(rawData);
      handleFirstRun();

      // Reset data
      graphData.everything = [['Time', 'temperature', 'heatIndex', 'humidity']];
      graphData.light = [['Time', 'lightReading']];
      graphData.temperature = [['Time', 'temperature']];
      graphData.humidity = [['Time', 'humidity']];
      graphData.latestReadings = [];

      var tempReadings = {
        light: null,
        humidity: null,
        temperature: null,
        heatIndex: null
      };

      // Loop through and map all new data
      rawData.feeds.forEach(function (dataPoint) {
        dataPoint.created_at = moment.utc(dataPoint.created_at)._d;

        // Cast values into actual numbers
        dataPoint.field1 = parseFloat(dataPoint.field1);
        dataPoint.field2 = parseFloat(dataPoint.field2);
        dataPoint.field3 = parseFloat(dataPoint.field3);
        dataPoint.field4 = parseFloat(dataPoint.field4);

        // Add the data point to the collection
        if (!allData.hasOwnProperty(dataPoint.entry_id)) {
          allData[dataPoint.entry_id] = dataPoint;
        }

        graphData.everything.push([dataPoint.created_at, dataPoint.field1, dataPoint.field2, dataPoint.field3]);

        graphData.light.push([dataPoint.created_at, dataPoint.field4]);
        graphData.humidity.push([dataPoint.created_at, dataPoint.field3]);
        graphData.temperature.push([dataPoint.created_at, dataPoint.field1]);

        if (!tempReadings.light && !isNaN(dataPoint.field4))
          tempReadings.light = [dataPoint.created_at, dataPoint.field4];

        if (!tempReadings.humidity && !isNaN(dataPoint.field3))
          tempReadings.humidity = [dataPoint.created_at, dataPoint.field3];

        if (!tempReadings.temperature && !isNaN(dataPoint.field1))
          tempReadings.temperature = [dataPoint.created_at, dataPoint.field1];

        if (!tempReadings.heatIndex && !isNaN(dataPoint.field2))
          tempReadings.heatIndex = [dataPoint.created_at, dataPoint.field2];
      });

      // Populate last readings table
      graphData.latestReadings.push(['Light Reading', tempReadings.light[1], tempReadings.light[0]]);
      graphData.latestReadings.push(['Temperature', tempReadings.temperature[1], tempReadings.temperature[0]]);
      graphData.latestReadings.push(['Humidity', tempReadings.humidity[1], tempReadings.humidity[0]]);
      graphData.latestReadings.push(['Heat Index', tempReadings.heatIndex[1], tempReadings.heatIndex[0]]);

      // Finally update the graphs to display the latest data
      updateGraphs();
    })
    .fail(function () {
      console.log('there was an error fetching graph data');
    });
};

function drawTable() {
  var data = new google.visualization.DataTable();
  data.addColumn('string', 'Name');
  data.addColumn('number', 'Last Reading');
  data.addColumn('datetime', 'Reading Time');
  data.addRows(graphData.latestReadings);

  table.draw(data, { showRowNumber: true, width: '100%' });
}

var opt_all = $.extend({}, baseOptions, {});
var opt_temp = $.extend({}, baseOptions, { 'title': 'Temperature', colors: [colors.temp] });
var opt_humid = $.extend({}, baseOptions, { 'title': 'Humidity', colors: [colors.humidity] });
var opt_light = $.extend({}, baseOptions, { 'title': 'Light', colors: [colors.light] });

// Load the Visualization API and the corechart package.
google.charts.load('current', { 'packages': ['corechart', 'gauge', 'table'] });

// Set a callback to run when the Google Visualization API is loaded.
google.charts.setOnLoadCallback(drawChart);

// Callback that creates and populates a data table, instantiates the pie chart, passes in the data and draws it.
function drawChart() {
  // Instantiate and draw our chart, passing in some baseOptions.
  chartAll = new google.visualization.AreaChart(document.getElementById('chart_all'));
  chartLight = new google.visualization.AreaChart(document.getElementById('chart_light'));
  chartTemp = new google.visualization.AreaChart(document.getElementById('chart_temp'));
  chartHumidity = new google.visualization.AreaChart(document.getElementById('chart_humidity'));
  table = new google.visualization.Table(document.getElementById('chart_guages'));

  updateData();
}