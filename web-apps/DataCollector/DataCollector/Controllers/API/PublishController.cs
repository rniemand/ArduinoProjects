using System;
using System.Threading.Tasks;
using System.Web;
using System.Web.Http;
using DataCollector.DAL.Repos;
using DataCollector.Models;
using Newtonsoft.Json;
using Rn.Logging.Interfaces;

namespace DataCollector.Controllers.API
{
  [RoutePrefix("API/v1/Publish")]
  public class PublishController : ApiController
  {
    private readonly IRnLogger _logger;
    private readonly DeviceRepo _deviceRepo;

    public PublishController()
    {
      _logger = ServiceLocator.LogManager.GetLogger("PublishController");
      _deviceRepo = new DeviceRepo(ServiceLocator.DbContext, ServiceLocator.RnEmbedded);
    }

    [HttpPost]
    [Route("")]
    public async Task<IHttpActionResult> Post(TemperatueInfo payload)
    {
      if (payload == null)
      {
        return BadRequest("No payload provided");
      }

      // Ensure that we have a device
      var device = await _deviceRepo.GetByDeviceKey(payload.DeviceKey);
      if (device == null)
      {
        return BadRequest("Invalid deviceId");
      }

      // Check to see if the device is enabled
      if (device.Enabled == false)
      {
        return BadRequest("Device has been disabled");
      }

      try
      {
        _logger.Trace("Attempting to save POST data");

        AppendClientsIpAddress(payload);




        //TemperatureLogger.LogValue(tempData);
      }
      catch (Exception ex)
      {
        // TODO: capture failure...
        _logger.Error("Error saving POST data", ex);
      }

      return Ok();
    }



    // helper methods
    private static void AppendClientsIpAddress(TemperatueInfo payload)
    {
      payload.DeviceIp = HttpContext.Current.Request.UserHostAddress;
    }
  }
}