using System;

namespace DataCollector.Models.Database
{
  public class TemperatureModel
  {
    public int Id { get; set; }
    public int DeviceId { get; set; }
    public double Temperature { get; set; }
    public double Humidity { get; set; }
    public double HeatIndex { get; set; }
    public DateTime TimeLoggedUtc { get; set; }
  }
}