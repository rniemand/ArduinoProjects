using System;

namespace DataCollector.Models.Database
{
  public class LightModel
  {
    public int Id { get; set; }
    public int DeviceId { get; set; }
    public double LightReading { get; set; }
    public DateTime TimeLoggedUtc { get; set; }
  }
}