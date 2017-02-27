using System.Collections.Generic;
using System.Reflection;
using System.Threading.Tasks;
using Dapper;
using DataCollector.Models;
using DataCollector.Models.Database;
using Rn.Core.IO;

namespace DataCollector.DAL.Repos
{
  public class LightRepo : ILightRepo
  {
    private readonly IDataCollectorDbContext _dbContext;
    private readonly IRnEmbedded _embedded;

    public LightRepo(IDataCollectorDbContext dbContext, IRnEmbedded embedded)
    {
      _dbContext = dbContext;
      _embedded = embedded;
    }

    public async Task Add(TemperatueInfo lightInfo)
    {
      // Ensure that we have some data to log
      if (lightInfo.Ldr <= 0)
      {
        return;
      }

      var sql = _embedded.GetResourceAsString(
        Assembly.GetExecutingAssembly(),
        "Queries.LightRepo.Add.sql");

      await _dbContext.ExecuteAsync(sql, lightInfo);
    }

    public async Task<List<LightModel>> GetLastXEntries(int deviceId)
    {
      var sql = _embedded.GetResourceAsString(
       Assembly.GetExecutingAssembly(),
       "Queries.LightRepo.GetLastXEntries.sql");

      var data = await _dbContext.QueryAsync<LightModel>(sql, new { DeviceId = deviceId });

      return data.AsList();
    }
  }
}