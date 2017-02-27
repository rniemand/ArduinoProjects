﻿using System.Collections.Generic;
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

    public async Task Add(TemperatueInfo temperature)
    {
      // Ensure that we have some data to log
      if (temperature.HeatIndex <= 0 || temperature.Humidity <= 0 || temperature.Temperature <= 0)
      {
        return;
      }

      var sql = _embedded.GetResourceAsString(
        Assembly.GetExecutingAssembly(),
        "Queries.TemperatureRepo.Add.sql");

      await _dbContext.ExecuteAsync(sql, temperature);
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