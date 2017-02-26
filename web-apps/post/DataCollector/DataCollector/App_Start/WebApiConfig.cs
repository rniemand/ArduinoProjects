using System.Configuration;
using System.Web.Http;
using Rn.Logging;
using Rn.Logging.Enums;
using Rn.Logging.Interfaces;
using Rn.Logging.Outputs;

namespace DataCollector
{
  public static class WebApiConfig
  {
    public static void Register(HttpConfiguration config)
    {
      SetupLogging();

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

    private static void SetupLogging()
    {
      Services.LogManager = new RnLogManager();

      var logFilePath = ConfigurationManager.AppSettings["Rn.Logging.FilePath"];

      var config = new RollingFileOutputConfig(logFilePath, "default")
      {
        Enabled = true,
        Severity = LogSeverity.Trace,
        RolledLogFileKeepCount = 5
      };

      config.SetMaxLogFileSizeMb(10);

      var fileOutput = new RollingFileOutput(config);

      Services.LogManager.RegisterLoggerOutput(fileOutput);
    }
  }

  public static class Services
  {
    public static IRnLogManager LogManager { get; set; }
  }
}
