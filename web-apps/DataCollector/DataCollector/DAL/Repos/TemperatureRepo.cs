using System.Collections.Generic;
using System.Reflection;
using System.Threading.Tasks;
using Dapper;
using DataCollector.Models;
using DataCollector.Models.Database;
using Rn.Core.IO;

namespace DataCollector.DAL.Repos
{
  public class TemperatureRepo : ITemperatureRepo
  {
    private readonly IDataCollectorDbContext _dbContext;
    private readonly IRnEmbedded _embedded;

    public TemperatureRepo(IDataCollectorDbContext dbContext, IRnEmbedded embedded)
    {
      _dbContext = dbContext;
      _embedded = embedded;
    }

    public async Task Add(TemperatueInfo tempInfo)
    {
      // Ensure that we have some data to log
      if (tempInfo.HeatIndex <= 0 || tempInfo.Humidity <= 0 || tempInfo.Temperature <= 0)
      {
        return;
      }

      var sql = _embedded.GetResourceAsString(
        Assembly.GetExecutingAssembly(),
        "Queries.TemperatureRepo.Add.sql");

      await _dbContext.ExecuteAsync(sql, tempInfo);
    }

    public async Task<List<TemperatureModel>> GetLastXEntries(int deviceId)
    {
      var sql = _embedded.GetResourceAsString(
        Assembly.GetExecutingAssembly(),
        "Queries.TemperatureRepo.GetLastXEntries.sql");

      var data = await _dbContext.QueryAsync<TemperatureModel>(sql, new { DeviceId = deviceId });

      return data.AsList();
    }
  }
}