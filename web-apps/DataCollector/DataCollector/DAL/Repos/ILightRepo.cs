using System.Collections.Generic;
using System.Threading.Tasks;
using DataCollector.Models;
using DataCollector.Models.Database;

namespace DataCollector.DAL.Repos
{
  public interface ILightRepo
  {
    Task Add(TemperatueInfo lightInfo);
    Task<List<LightModel>> GetLastXEntries(int deviceId);
  }
}