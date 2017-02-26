using System.Diagnostics;
using System.Web.Http;

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
    public IHttpActionResult Post(Bob post)
    {


      return Ok();
    }
  }

  [DebuggerDisplay("{Temperature} *C ({Humidity} %)")]
  public class Bob
  {
    public double Temperature { get; set; }
    public double Humidity { get; set; }
    public double HeatIndex { get; set; }
  }
}