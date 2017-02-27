using System.Threading.Tasks;
using System.Web.Http;
using DataCollector.DAL.Repos;
using Rn.Logging.Interfaces;

namespace DataCollector.Controllers.API
{
  [RoutePrefix("API/v1/Light")]
  public class LightController : ApiController
  {
    private readonly IRnLogger _logger;
    private readonly ILightRepo _lightRepo;

    public LightController()
    {
      _logger = ServiceLocator.LogManager.GetLogger("TemperatureController");
      _lightRepo = ServiceLocator.LightRepo;
    }

    [HttpGet, Route("{deviceId:int}")]
    public async Task<IHttpActionResult> Get(int deviceId)
    {
      if (deviceId <= 0)
      {
        return BadRequest("Please provide a deviceId");
      }

      var data = await _lightRepo.GetLastXEntries(deviceId);

      return Ok(data);
    }
  }
}