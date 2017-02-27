using System.Configuration;
using DataCollector.DAL;
using DataCollector.DAL.Repos;
using Rn.Core.Config;
using Rn.Core.IO;
using Rn.Logging;
using Rn.Logging.Enums;
using Rn.Logging.Interfaces;
using Rn.Logging.Outputs;

namespace DataCollector
{
  public static class ServiceLocator
  {
    public static IRnLogManager LogManager { get; set; }
    public static IDataCollectorDbContext DbContext { get; set; }
    public static IWebConfig WebConfig { get; set; }
    public static IRnEmbedded RnEmbedded { get; set; }
    public static IDapperAdapter DapperAdapter { get; set; }
    public static IDeviceRepo DeviceRepo { get; set; }
    public static ITemperatureRepo TemperatureRepo { get; set; }
    public static ILightRepo LightRepo { get; set; }

    static ServiceLocator()
    {
      RnEmbedded = new RnEmbedded();
      WebConfig = new RnWebConfig();
      DapperAdapter = new DapperAdapter();

      SetupLogging();
      SetupDbContext();
      SetupRepos();
    }

    // "Installers" for our services
    private static void SetupLogging()
    {
      LogManager = new RnLogManager();

      // Don't create an instance of the logger if it is not needed
      if (!WebConfig.GetBoolAppSetting("Rn.Logging.Enabled"))
        return;

      var logFilePath = WebConfig.GetAppSetting("Rn.Logging.FilePath");

      var config = new RollingFileOutputConfig(logFilePath, "default")
      {
        Enabled = true,
        Severity = LogSeverity.Trace,
        RolledLogFileKeepCount = 5
      };

      config.SetMaxLogFileSizeMb(10);

      var fileOutput = new RollingFileOutput(config);

      LogManager.RegisterLoggerOutput(fileOutput);
    }

    private static void SetupDbContext()
    {
      DbContext = new DataCollectorDbContext(LogManager, WebConfig, DapperAdapter);
    }

    private static void SetupRepos()
    {
      DeviceRepo= new DeviceRepo(DbContext, RnEmbedded);
      TemperatureRepo = new TemperatureRepo(DbContext, RnEmbedded);
      LightRepo = new LightRepo(DbContext, RnEmbedded);
    }
  }
}