using System.Collections.Generic;
using System.Data;
using System.Threading.Tasks;

namespace DataCollector.DAL
{
  public interface IDataCollectorDbContext
  {
    string GetVersion();

    Task<IEnumerable<T>> QueryAsync<T>(
      string sql,
      object param = null,
      IDbTransaction transaction = null,
      int? commandTimeout = null,
      CommandType? commandType = null);

    Task<int> ExecuteAsync(string sql, object param = null, IDbTransaction transaction = null,
      int? commandTimeout = null, CommandType? commandType = null);
  }
}