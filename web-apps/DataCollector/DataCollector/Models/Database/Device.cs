namespace DataCollector.Models.Database
{
  public class Device
  {
    public int Id { get; set; }
    public bool Deleted { get; set; }
    public bool Enabled { get; set; }
    public string DeviceName { get; set; }
    public string DeviceKey { get; set; }
  }
}