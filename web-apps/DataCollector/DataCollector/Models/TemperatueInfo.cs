using System;
using System.Diagnostics;

namespace DataCollector.Models
{
  [DebuggerDisplay("{Temperature} *C ({Humidity} %)")]
  public class TemperatueInfo
  {
    public int DeviceId { get; set; }
    public double Temperature { get; set; }
    public double Humidity { get; set; }
    public double HeatIndex { get; set; }
    public double Ldr { get; set; }
    public DateTime TimeLoggedUtc { get; set; }
    public string DeviceKey { get; set; }
    public string DeviceIp { get; set; }

    public TemperatueInfo()
    {
      TimeLoggedUtc = DateTime.UtcNow;
    }
  }
}