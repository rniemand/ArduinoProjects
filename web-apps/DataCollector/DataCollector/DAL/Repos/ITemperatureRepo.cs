﻿using System.Collections.Generic;
using System.Threading.Tasks;
using DataCollector.Models;
using DataCollector.Models.Database;

namespace DataCollector.DAL.Repos
{
  public interface ITemperatureRepo
  {
    Task Add(TemperatueInfo temperature);
    Task<List<TemperatureModel>> GetLastXEntries(int deviceId);
  }
}