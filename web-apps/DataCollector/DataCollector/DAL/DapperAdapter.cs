using System.Collections.Generic;
using System.Data;
using System.Threading.Tasks;
using Dapper;

namespace DataCollector.DAL
{
  public class DapperAdapter : IDapperAdapter
  {
    public async Task<IEnumerable<T>> QueryAsync<T>(
      IDbConnection connection,
      string sql,
      object param = null,
      IDbTransaction transaction = null,
      int? commandTimeout = null,
      CommandType? commandType = null)
    {
      return await connection.QueryAsync<T>(sql, param, transaction, commandTimeout, commandType);
    }

    public async Task<int> ExecuteAsync(
      IDbConnection connection,
      string sql, object param = null,
      IDbTransaction transaction = null,
      int? commandTimeout = null,
      CommandType? commandType = null)
    {
      return await connection.ExecuteAsync(sql, param, transaction, commandTimeout, commandType);
    }
  }
}