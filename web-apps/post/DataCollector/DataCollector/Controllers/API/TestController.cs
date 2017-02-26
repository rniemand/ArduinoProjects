using System;
using System.Linq;
using System.Web;
using System.Web.Http;
using DataCollector.Models;

namespace DataCollector.Controllers.API
{
  [RoutePrefix("API/v1/Test")]
  public class TestController : ApiController
  {
    [Route("")]
    public IHttpActionResult Get()
    {
      return Ok(TemperatureLogger.Samples);
    }

    [Route("Temperature")]
    public IHttpActionResult GetTemperature()
    {
      return Ok(TemperatureLogger.Samples.Select(x => new
      {
        TimeLoggedUtc = x.TimeLoggedUtc,
        Temperature = x.Temperature,
        Humidity = x.Humidity,
        HeatIndex = x.HeatIndex

      }).ToList());
    }

    [Route("Post")]
    public IHttpActionResult Post(TemperatueInfo tempData)
    {
      // Request.Headers.Host

      tempData.DeviceIp = HttpContext.Current.Request.UserHostAddress;

      try
      {
        // Log temperature data to log file
        TemperatureLogger.LogValue(tempData);
      }
      catch (Exception ex)
      {
        // TODO: capture failure...
      }

      // Return an OK
      return Ok();
    }
  }
}