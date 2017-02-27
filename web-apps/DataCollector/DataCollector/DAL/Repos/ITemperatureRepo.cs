using System.Threading.Tasks;
using DataCollector.Models;

namespace DataCollector.DAL.Repos
{
  public interface ITemperatureRepo
  {
    Task Add(TemperatueInfo category);
  }
}