using System;
using System.Net.Http;
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
      return Ok("Hello ESP");
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