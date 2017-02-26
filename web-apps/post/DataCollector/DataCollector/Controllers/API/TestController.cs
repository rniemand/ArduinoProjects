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

  public class Bob
  {
    public string Hello { get; set; }
  }
}