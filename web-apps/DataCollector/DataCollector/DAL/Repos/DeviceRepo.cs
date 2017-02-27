using System.Linq;
using System.Reflection;
using System.Threading.Tasks;
using DataCollector.Models.Database;
using Rn.Core.IO;

namespace DataCollector.DAL.Repos
{
  public class DeviceRepo : IDeviceRepo
  {
    private readonly IDataCollectorDbContext _dbContext;
    private readonly IRnEmbedded _embedded;

    public DeviceRepo(IDataCollectorDbContext dbContext, IRnEmbedded embedded)
    {
      _dbContext = dbContext;
      _embedded = embedded;
    }

    public async Task<DeviceModel> GetByDeviceKey(string deviceKey)
    {
      var sql = _embedded.GetResourceAsString(
        Assembly.GetExecutingAssembly(),
        "Queries.DeviceRepo.GetByDeviceKey.sql");

      var data = await _dbContext.QueryAsync<DeviceModel>(sql, new { DeviceKey = deviceKey });

      return data.FirstOrDefault();
    }
  }
}