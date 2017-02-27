using System;
using System.Collections.Generic;
using System.Configuration;
using System.IO;
using System.Linq;
using System.Text;
using DataCollector.Models;

namespace DataCollector
{
  public static class TemperatureLogger
  {
    public static string LogFile { get; private set; }
    public static List<TemperatueInfo> Samples { get; set; }

    //private static FileStream _fs;
    //private static StreamWriter _sw;
    private static readonly string _sep;

    // Constructor
    static TemperatureLogger()
    {
      LogFile = ConfigurationManager.AppSettings["Rn.DataFile"];
      _sep = ConfigurationManager.AppSettings["Rn.LineSep"];

      Samples = new List<TemperatueInfo>();

      //CreateLogFile();
    }

    // Public methods
    public static void LogValue(TemperatueInfo data)
    {
      LogDataToFile(data);

      // Trim data points when we hit our max
      if (Samples.Count >= 100)
      {
        Samples.Remove(Samples.First());
      }

      // Slot the latest data point into the collection
      Samples.Add(data);
    }


    // Internal methods
    //private static void CreateLogFile()
    //{
    //  RollOldLogFile();

    //  try
    //  {
    //    _fs = new FileStream(LogFile, FileMode.OpenOrCreate, FileAccess.ReadWrite, FileShare.ReadWrite);
    //    _sw = new StreamWriter(_fs);
    //  }
    //  catch (Exception ex)
    //  {
    //    throw new Exception("Here", ex);
    //  }

    //  // Write file header
    //  var sb = new StringBuilder();

    //  sb
    //    .Append("TimeLoggedUtc").Append(_sep)
    //    .Append("Temperature").Append(_sep)
    //    .Append("Humidity").Append(_sep)
    //    .Append("HeatIndex").Append(_sep)
    //    .Append("Ldr").Append(_sep)
    //    .Append("DeviceId").Append(_sep)
    //    .Append("DeviceIp");

    //  _sw.WriteLine(sb.ToString());
    //}

    private static void RollOldLogFile()
    {
      if (!File.Exists(LogFile))
      {
        return;
      }

      File.Delete(LogFile);

      //var oldLogFile = $"{LogFile}.old";
      //if (File.Exists(oldLogFile))
      //{
      //  File.Delete(oldLogFile);
      //}

      //File.Move(LogFile, oldLogFile);
    }

    private static void LogDataToFile(TemperatueInfo data)
    {
      var sb = new StringBuilder();

      sb
        .Append(data.TimeLoggedUtc).Append(_sep)
        .Append(data.Temperature).Append(_sep)
        .Append(data.Humidity).Append(_sep)
        .Append(data.Ldr).Append(_sep)
        .Append(data.HeatIndex).Append(_sep)
        .Append(data.DeviceId).Append(_sep)
        .Append(data.DeviceIp);

      using (var fs = new FileStream(LogFile, FileMode.OpenOrCreate, FileAccess.ReadWrite, FileShare.ReadWrite))
      {
        using (var sw = new StreamWriter(fs))
        {
          fs.Position = fs.Length;
          sw.WriteLine(sb.ToString());
          sw.Flush();
          fs.Flush();
        }
      }
    }
  }
}