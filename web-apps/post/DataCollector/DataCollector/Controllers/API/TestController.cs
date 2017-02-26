using System;
using System.Linq;
using System.Web;
using System.Web.Http;
using DataCollector.Models;
using Rn.Logging.Interfaces;

namespace DataCollector.Controllers.API
{
  [RoutePrefix("API/v1/Test")]
  public class TestController : ApiController
  {
    private readonly IRnLogger _logger;

    public TestController()
    {
      _logger = Services.LogManager.GetLogger("TestController");
    }

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
        HeatIndex = x.HeatIndex,
        LDR = x.Ldr

      }).ToList());
    }

    [Route("Post")]
    public IHttpActionResult Post(TemperatueInfo tempData)
    {
      try
      {
        _logger.Trace("Attempting to save POST data");

        // Log temperature data to log file
        tempData.DeviceIp = HttpContext.Current.Request.UserHostAddress;
        TemperatureLogger.LogValue(tempData);
      }
      catch (Exception ex)
      {
        // TODO: capture failure...
        _logger.Error("Error saving POST data", ex);
      }

      // Return an OK
      return Ok();
    }
  }
}