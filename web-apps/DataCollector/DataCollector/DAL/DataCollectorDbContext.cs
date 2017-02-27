using System.Collections.Generic;
using System.Data;
using System.Data.SQLite;
using System.Linq;
using System.Reflection;
using System.Threading.Tasks;
using Dapper;
using Rn.Core.Config;
using Rn.Core.IO;
using Rn.Logging.Interfaces;

namespace DataCollector.DAL
{
  public class DataCollectorDbContext : IDataCollectorDbContext
  {
    private readonly IRnLogger _logger;
    private readonly IWebConfig _webConfig;
    private readonly IDapperAdapter _dapper;
    private readonly IFile _file;
    private readonly IRnEmbedded _embedded;
    private IDbConnection _connection;
    private string _dbFileLocation;

    public DataCollectorDbContext(
      IRnLogManager logManager,
      IWebConfig webConfig,
      IDapperAdapter dapper,
      IFile file = null,
      IRnEmbedded embedded = null,
      IDbConnection connection = null)
    {
      _logger = logManager.GetLogger("DataCollectorDbContext");
      _webConfig = webConfig;
      _dapper = dapper;
      _file = file ?? new RnFile();
      _embedded = embedded ?? new RnEmbedded();

      // If you inject a DBConnection you are most likely running tests
      if (connection != null)
      {
        _connection = connection;
      }

      _logger.Trace("New instance created");
      SetDbFilePath();
      EnsureDbExists();
      EnsureDbConnected();
    }

    // Public methods
    public string GetVersion()
    {
      EnsureDbConnected();
      var verion = _connection.Query<string>("select sqlite_version();");
      return verion.First();
    }

    public async Task<IEnumerable<T>> QueryAsync<T>(
      string sql,
      object param = null,
      IDbTransaction transaction = null,
      int? commandTimeout = null,
      CommandType? commandType = null)
    {
      EnsureDbConnected();
      return await _dapper.QueryAsync<T>(_connection, sql, param, transaction, commandTimeout, commandType);
    }

    public async Task<int> ExecuteAsync(
      string sql,
      object param = null,
      IDbTransaction transaction = null,
      int? commandTimeout = null,
      CommandType? commandType = null)
    {
      EnsureDbConnected();
      return await _dapper.ExecuteAsync(_connection, sql, param, transaction, commandTimeout, commandType);
    }

    // Internal methods
    private void SetDbFilePath()
    {
      var path = _webConfig.GetAppSetting("DBFileLocation");
      var basePath = _webConfig.GetAppSetting("BaseFilePath");

      if (!basePath.EndsWith("\\"))
      {
        basePath += "\\";
      }

      _dbFileLocation = path.Replace("{BASE_PATH}", basePath);
    }

    private void EnsureDbExists()
    {
      // Check to see if the DB file exists
      if (DbFileExists())
      {
        _logger.Info("Database file exists, no need to re-create it");
        return;
      }

      // Create and seed the DB
      EnsureDbConnected();
      var assembly = Assembly.GetExecutingAssembly();

      // Create database tables
      RunDbFileQuery(assembly, "DbFiles.Tables.Devices.sql");
      RunDbFileQuery(assembly, "DbFiles.Tables.Temperature.sql");
      RunDbFileQuery(assembly, "DbFiles.Tables.Light.sql");

      // Seed table contents
      RunDbFileQuery(assembly, "DbFiles.Seed.Devices.sql");
      RunDbFileQuery(assembly, "DbFiles.Seed.Temperature.sql");
      RunDbFileQuery(assembly, "DbFiles.Seed.Light.sql");
    }

    private bool DbFileExists()
    {
      // Check to see if the DB exists
      if (!_file.Exists(_dbFileLocation))
      {
        return false;
      }

      if (!_webConfig.GetBoolAppSetting("DeleteDbOnStartup"))
      {
        return true;
      }

      _file.Delete(_dbFileLocation);
      return false;
    }

    private void EnsureDbConnected()
    {
      // Ensure that there is an connection to work with
      if (_connection == null)
      {
        var conString = $"Data Source={_dbFileLocation}";
        _connection = new SQLiteConnection(conString);
      }

      // Ensure that the connection is open
      if (_connection.State != ConnectionState.Open)
      {
        _connection.Open();
      }
    }

    private void RunDbFileQuery(Assembly assembly, string file)
    {
      _logger.Debug($"Running embedded file query '{file}'");
      var query = _embedded.GetResourceAsString(assembly, file);
      _connection.Execute(query);
    }
  }
}