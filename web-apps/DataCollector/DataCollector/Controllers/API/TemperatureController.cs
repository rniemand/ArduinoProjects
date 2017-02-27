using System.Threading.Tasks;
using System.Web.Http;
using DataCollector.DAL.Repos;
using Rn.Logging.Interfaces;

namespace DataCollector.Controllers.API
{
  [RoutePrefix("API/v1/Temperature")]
  public class TemperatureController : ApiController
  {
    private readonly IRnLogger _logger;
    private readonly ITemperatureRepo _temperatureRepo;

    public TemperatureController()
    {
      _logger = ServiceLocator.LogManager.GetLogger("TemperatureController");

      _temperatureRepo = new TemperatureRepo(
        ServiceLocator.DbContext,
        ServiceLocator.RnEmbedded);
    }

    [HttpGet, Route("{deviceId:int}")]
    public async Task<IHttpActionResult> Get(int deviceId)
    {
      if (deviceId <= 0)
      {
        return BadRequest("Please provide a deviceId");
      }

      var data = await _temperatureRepo.GetLastXEntries(deviceId);

      return Ok(data);
    }
  }
}