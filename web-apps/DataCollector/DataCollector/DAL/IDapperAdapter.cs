using System.Collections.Generic;
using System.Data;
using System.Threading.Tasks;

namespace DataCollector.DAL
{
  public interface IDapperAdapter
  {
    Task<IEnumerable<T>> QueryAsync<T>(
      IDbConnection connection,
      string sql,
      object param = null,
      IDbTransaction transaction = null,
      int? commandTimeout = null,
      CommandType? commandType = null);

    Task<int> ExecuteAsync(
      IDbConnection connection,
      string sql,
      object param = null,
      IDbTransaction transaction = null,
      int? commandTimeout = null,
      CommandType? commandType = null);
  }
}