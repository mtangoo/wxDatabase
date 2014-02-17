#ifndef _WX_DATABASE_H_
#define _WX_DATABASE_H_

#include "wx/database/wxprec.h"
#include "wx/config.h"

WX_DECLARE_HASH_SET( wxDatabaseResultSet*, wxPointerHash, wxPointerEqual, wxDatabaseResultSetHashSet );
WX_DECLARE_HASH_SET( wxPreparedStatement*, wxPointerHash, wxPointerEqual, wxDatabaseStatementHashSet );

class WXDLLIMPEXP_DATABASE wxDatabase : public wxDatabaseErrorReporter, public wxDatabaseStringConverter
{
public:
  /// Constructor
  wxDatabase();
  
  /// Destructor
  virtual ~wxDatabase();
  
  // Open database
  virtual bool Open(const wxString& strDatabase) = 0;
  
  /***
   * Causes the database specified by db to become the default (current) database on the connection
   * If unimplemented just return false (Equivalent to unsuccessful operation)
   */
  virtual bool ChangeDatabase(const wxString& database) { return false; }
  
  /// close database  
  virtual bool Close() = 0;
 
  /// Is the connection to the database open?
  virtual bool IsOpen() = 0;

  // transaction support
  /// Begin a transaction
  virtual void BeginTransaction() = 0;
  /// Commit the current transaction
  virtual void Commit() = 0;
  /// Rollback the current transaction
  virtual void RollBack() = 0;
 
  // query database
  /// Run an insert, update, or delete query on the database
  virtual int RunQuery(const wxString& strQuery);
  /// Run an insert, update, or delete query on the database
  virtual int RunQuery(const wxString& strQuery, bool bParseQueries) = 0;
  /// Run a select query on the database
  virtual wxDatabaseResultSet* RunQueryWithResults(const wxString& strQuery) = 0;
  
  /// Close a result set returned by the database or a prepared statement previously
  virtual bool CloseResultSet(wxDatabaseResultSet* pResultSet);

  // wxPreparedStatement support
  /// Prepare a SQL statement which can be reused with different parameters
  virtual wxPreparedStatement* PrepareStatement(const wxString& strQuery) = 0;
  /// Close a prepared statement previously prepared by the database
  virtual bool CloseStatement(wxPreparedStatement* pStatement);

  // function names more consistent with JDBC and wxSQLite3
  // these just provide wrappers for existing functions
  /// See RunQuery
  int ExecuteUpdate(const wxString& strQuery) { return RunQuery(strQuery); }
  /// See RunQueryWithResults
  wxDatabaseResultSet* ExecuteQuery(const wxString& strQuery) { return RunQueryWithResults(strQuery); }

  // Database schema API contributed by M. Szeftel (author of wxActiveRecordGenerator)
  /// Check for the existence of a table by name
  virtual bool TableExists(const wxString& table) = 0;
  /// Check for the existence of a view by name
  virtual bool ViewExists(const wxString& view) = 0;
  /// Retrieve all table names
  virtual wxArrayString GetTables() = 0;
  /// Retrieve all view names
  virtual wxArrayString GetViews() = 0;
  /// Retrieve all column names for a table
  virtual wxArrayString GetColumns(const wxString& table) = 0;
  ///get Primary keys column names
  virtual wxArrayString GetPKColumns(const wxString& table) = 0;

  // Database single result retrieval API contributed by Guru Kathiresan
  /// With the GetSingleResultX API, two additional exception types are thrown:
  ///   wxDATABASE_NO_ROWS_FOUND - No database rows were returned
  ///   wxDATABASE_NON_UNIQUE_RESULTSET - More than one database row was returned

  /// Retrieve a single integer value from a query
  /// If multiple records are returned from the query, a wxDATABASE_NON_UNIQUE_RESULTSET exception
  ///  is thrown unless bRequireUniqueResult is false
  virtual int GetSingleResultInt(const wxString& strSQL, int nField, bool bRequireUniqueResult = true);
  virtual int GetSingleResultInt(const wxString& strSQL, const wxString& strField, bool bRequireUniqueResult = true);
  
  /// Retrieve a single string value from a query
  /// If multiple records are returned from the query, a wxDATABASE_NON_UNIQUE_RESULTSET exception
  ///  is thrown unless bRequireUniqueResult is false
  virtual wxString GetSingleResultString(const wxString& strSQL, int nField, bool bRequireUniqueResult = true);
  virtual wxString GetSingleResultString(const wxString& strSQL, const wxString& strField, bool bRequireUniqueResult = true);
  
  /// Retrieve a single long value from a query
  /// If multiple records are returned from the query, a wxDATABASE_NON_UNIQUE_RESULTSET exception
  ///  is thrown unless bRequireUniqueResult is false
  virtual long GetSingleResultLong(const wxString& strSQL, int nField, bool bRequireUniqueResult = true);
  virtual long GetSingleResultLong(const wxString& strSQL, const wxString& strField, bool bRequireUniqueResult = true);
  
  /// Retrieve a single bool value from a query
  /// If multiple records are returned from the query, a wxDATABASE_NON_UNIQUE_RESULTSET exception
  ///  is thrown unless bRequireUniqueResult is false
  virtual bool GetSingleResultBool(const wxString& strSQL, int nField, bool bRequireUniqueResult = true);
  virtual bool GetSingleResultBool(const wxString& strSQL, const wxString& strField, bool bRequireUniqueResult = true);
  
  /// Retrieve a single date/time value from a query
  /// If multiple records are returned from the query, a wxDATABASE_NON_UNIQUE_RESULTSET exception
  ///  is thrown unless bRequireUniqueResult is false
  virtual wxDateTime GetSingleResultDate(const wxString& strSQL, int nField, bool bRequireUniqueResult = true);
  virtual wxDateTime GetSingleResultDate(const wxString& strSQL, const wxString& strField, bool bRequireUniqueResult = true);
  
  /// Retrieve a single Blob value from a query
  /// If multiple records are returned from the query, a wxDATABASE_NON_UNIQUE_RESULTSET exception
  ///  is thrown unless bRequireUniqueResult is false
  virtual void* GetSingleResultBlob(const wxString& strSQL, int nField, wxMemoryBuffer& Buffer, bool bRequireUniqueResult = true);
  virtual void* GetSingleResultBlob(const wxString& strSQL, const wxString& strField, wxMemoryBuffer& Buffer, bool bRequireUniqueResult = true);
  
  /// Retrieve a single double value from a query
  /// If multiple records are returned from the query, a wxDATABASE_NON_UNIQUE_RESULTSET exception
  ///  is thrown unless bRequireUniqueResult is false
  virtual double GetSingleResultDouble(const wxString& strSQL, int nField, bool bRequireUniqueResult = true);
  virtual double GetSingleResultDouble(const wxString& strSQL, const wxString& strField, bool bRequireUniqueResult = true);

  /// Retrieve all the values of one field in a result set
  virtual wxArrayInt GetResultsArrayInt(const wxString& strSQL, int nField);
  virtual wxArrayInt GetResultsArrayInt(const wxString& strSQL, const wxString& Field);
  
  virtual wxArrayString GetResultsArrayString(const wxString& strSQL, int nField);
  virtual wxArrayString GetResultsArrayString(const wxString& strSQL, const wxString& Field);
  
  virtual wxArrayLong GetResultsArrayLong(const wxString& strSQL, int nField);
  virtual wxArrayLong GetResultsArrayLong(const wxString& strSQL, const wxString& Field);
  
  virtual wxArrayDouble GetResultsArrayDouble(const wxString& strSQL, int nField);
  virtual wxArrayDouble GetResultsArrayDouble(const wxString& strSQL, const wxString& Field); 
  
  /// Close all result set objects that have been generated but not yet closed
  void CloseResultSets();
  /// Close all prepared statement objects that have been generated but not yet closed
  void CloseStatements();

  //AML start
  /**
	Get a descriptive name for the type of database
  */
  const wxString& GetTypeName() { return m_typeName; }
  /**
	Is the connection via ODBC
  */
  bool IsViaODBC() { return m_isViaODBC; }
  /**
	Get the library path required by the database
  */
  const wxString& GetLibraryPath() { return m_libraryPath; }
  /**
	Get an instance of the first valid database specified in config.
  */
  static wxDatabase* GetDatabase(wxConfigBase& config, wxString* err = NULL, const wxString& path = "/");
private:
#if wxUSE_DATABASE_SQLITE
  static wxDatabase* GetSqliteDatabase(wxConfigBase& config, wxString* err = NULL);
#endif
#if wxUSE_DATABASE_POSTGRESQL
  static wxDatabase* GetPostgresDatabase(wxConfigBase& config, wxString* err = NULL);
#endif
#if wxUSE_DATABASE_MYSQL
  static wxDatabase* GetMysqlDatabase(wxConfigBase& config, wxString* err = NULL);
#endif
#if wxUSE_DATABASE_ODBC
  static wxDatabase* GetOdbcDatabase(wxConfigBase& config, wxString* err = NULL);
#endif
#if wxUSE_DATABASE_TDS
  static wxDatabase* GetTdsDatabase(wxConfigBase& config, wxString* err = NULL);
#endif
  //AML end
protected:
  /// Add result set object pointer to the list for "garbage collection"
  void LogResultSetForCleanup(wxDatabaseResultSet* pResultSet) { m_ResultSets.insert(pResultSet); }
  /// Add prepared statement object pointer to the list for "garbage collection"
  void LogStatementForCleanup(wxPreparedStatement* pStatement) { m_Statements.insert(pStatement); }
  
private:
  int GetSingleResultInt(const wxString& strSQL, const wxVariant* field, bool bRequireUniqueResult = true);
  wxString GetSingleResultString(const wxString& strSQL, const wxVariant* field, bool bRequireUniqueResult = true);
  long GetSingleResultLong(const wxString& strSQL, const wxVariant* field, bool bRequireUniqueResult = true);
  bool GetSingleResultBool(const wxString& strSQL, const wxVariant* field, bool bRequireUniqueResult = true);
  wxDateTime GetSingleResultDate(const wxString& strSQL, const wxVariant* field, bool bRequireUniqueResult = true);
  void* GetSingleResultBlob(const wxString& strSQL, const wxVariant* field, wxMemoryBuffer& Buffer, bool bRequireUniqueResult = true);
  double GetSingleResultDouble(const wxString& strSQL, const wxVariant* field, bool bRequireUniqueResult = true);
  wxArrayInt GetResultsArrayInt(const wxString& strSQL, const wxVariant* field);
  wxArrayString GetResultsArrayString(const wxString& strSQL, const wxVariant* field);
  wxArrayLong GetResultsArrayLong(const wxString& strSQL, const wxVariant* field);
#if wxCHECK_VERSION(2, 7, 0)
  wxArrayDouble GetResultsArrayDouble(const wxString& strSQL, const wxVariant* field);
#endif

  wxDatabaseResultSetHashSet m_ResultSets;
  wxDatabaseStatementHashSet m_Statements;
  //AML start
  wxString m_typeName;
  bool m_isViaODBC;
  wxString m_libraryPath;
  //AML end
};

#endif//_WX_DATABASE_H_

