﻿using System;
using System.Text;
using System.Threading.Tasks;
using System.Web;
using System.Web.Http;
using DataCollector.DAL.Repos;
using DataCollector.Models;
using Rn.Logging.Interfaces;

namespace DataCollector.Controllers.API
{
  [RoutePrefix("API/v1/Publish")]
  public class PublishController : ApiController
  {
    private readonly IRnLogger _logger;
    private readonly IDeviceRepo _deviceRepo;
    private readonly ITemperatureRepo _temperatureRepo;
    private readonly ILightRepo _lightRepo;

    public PublishController()
    {
      _logger = ServiceLocator.LogManager.GetLogger("PublishController");
      _deviceRepo = ServiceLocator.DeviceRepo;
      _lightRepo = ServiceLocator.LightRepo;
      _temperatureRepo = ServiceLocator.TemperatureRepo;
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

      // Set the DeviceId - used for inserting data into the DB
      payload.DeviceId = device.Id;
      payload.TimeLoggedUtc = DateTime.UtcNow;

      try
      {
        _logger.Trace("Attempting to save POST data");
        AppendClientsIpAddress(payload);

        await _temperatureRepo.Add(payload);
        await _lightRepo.Add(payload);
      }
      catch (Exception ex)
      {
        // TODO: capture failure...
        _logger.Error("Error saving POST data", ex);

        var sb = new StringBuilder();
        var error = $"{WalkStack(ex, sb)}";
        return Ok(error);
      }

      return Ok("OK");
    }

    // helper methods
    private static void AppendClientsIpAddress(TemperatueInfo payload)
    {
      payload.DeviceIp = HttpContext.Current.Request.UserHostAddress;
    }

    private static string WalkStack(Exception ex, StringBuilder sb)
    {
      sb.Append(ex.Message).Append(" | ");

      if (ex.InnerException != null)
      {
        return WalkStack(ex, sb);
      }

      return sb.ToString();
    }
  }
}