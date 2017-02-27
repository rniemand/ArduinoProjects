using System.Threading.Tasks;
using DataCollector.Models.Database;

namespace DataCollector.DAL.Repos
{
  public interface IDeviceRepo
  {
    Task<DeviceModel> GetByDeviceKey(string deviceKey);
  }
}