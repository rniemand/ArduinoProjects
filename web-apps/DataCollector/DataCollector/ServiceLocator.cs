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

    static ServiceLocator()
    {
      RnEmbedded = new RnEmbedded();
      WebConfig = new RnWebConfig();
      DapperAdapter = new DapperAdapter();

      SetupLogging();
      SetupDbContext();
    }

    // "Installers" for our services
    private static void SetupLogging()
    {
      ServiceLocator.LogManager = new RnLogManager();

      var logFilePath = ConfigurationManager.AppSettings["Rn.Logging.FilePath"];

      var config = new RollingFileOutputConfig(logFilePath, "default")
      {
        Enabled = true,
        Severity = LogSeverity.Trace,
        RolledLogFileKeepCount = 5
      };

      config.SetMaxLogFileSizeMb(10);

      var fileOutput = new RollingFileOutput(config);

      ServiceLocator.LogManager.RegisterLoggerOutput(fileOutput);
    }

    private static void SetupDbContext()
    {
      DbContext = new DataCollectorDbContext(LogManager, WebConfig, DapperAdapter);
    }
  }
}