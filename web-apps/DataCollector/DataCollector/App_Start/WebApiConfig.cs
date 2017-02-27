using System.Configuration;
using System.Web.Http;
using Rn.Logging;
using Rn.Logging.Enums;
using Rn.Logging.Outputs;

namespace DataCollector
{
  public static class WebApiConfig
  {
    public static void Register(HttpConfiguration config)
    {
      // Web API configuration and services
      config.Formatters.Remove(config.Formatters.XmlFormatter);

      // Web API routes
      config.MapHttpAttributeRoutes();

      config.Routes.MapHttpRoute(
          name: "DefaultApi",
          routeTemplate: "api/{controller}/{id}",
          defaults: new { id = RouteParameter.Optional }
      );
    }
  }
}
