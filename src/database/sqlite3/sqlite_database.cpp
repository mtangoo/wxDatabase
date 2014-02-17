#include "wx/database/wxprec.h"

#if wxUSE_DATABASE_SQLITE

#include <wx/tokenzr.h>
#include <wx/filename.h>

// ctor()
wxSqliteDatabase::wxSqliteDatabase()
 : wxDatabase()
{
  m_pDatabase = NULL; //&m_Database; //new sqlite3;
  wxCSConv conv(_("UTF-8"));
  SetEncoding(&conv);
}

wxSqliteDatabase::wxSqliteDatabase(const wxString& strDatabase, bool mustExist /*= false*/)
 : wxDatabase()
{
  m_pDatabase = NULL; //new sqlite3;
  wxCSConv conv(_("UTF-8"));
  SetEncoding(&conv);
  Open(strDatabase, mustExist);
}

// dtor()
wxSqliteDatabase::~wxSqliteDatabase()
{
  //wxPrintf(_("~wxSqliteDatabase()\n"));
  Close();
  //wxDELETE(m_pDatabase);
}

// open database
bool wxSqliteDatabase::Open(const wxString& strDatabase, bool mustExist)
{
  if (strDatabase!= _(":memory:") && // :memory: is a special SQLite in-memory database
    mustExist && !(wxFileName::FileExists(strDatabase)))
  {
    SetErrorCode(wxDATABASE_ERROR);
    SetErrorMessage(_("The specified database file '") + strDatabase + _("' does not exist."));
    ThrowDatabaseException();
    return false;
  }
  return Open(strDatabase);
}

bool wxSqliteDatabase::Open(const wxString& strDatabase)
{
  ResetErrorCodes();

  //if (m_pDatabase == NULL)
  //  m_pDatabase = new sqlite3;

  wxCharBuffer databaseNameBuffer = ConvertToUnicodeStream(strDatabase);
  sqlite3* pDbPtr = (sqlite3*)m_pDatabase;
  int nReturn = sqlite3_open(databaseNameBuffer, &pDbPtr);
  m_pDatabase = pDbPtr;
  if (nReturn != SQLITE_OK)
  {
    SetErrorCode(wxSqliteDatabase::TranslateErrorCode(sqlite3_errcode((sqlite3*)m_pDatabase)));
    SetErrorMessage(ConvertFromUnicodeStream(sqlite3_errmsg((sqlite3*)m_pDatabase)));
    ThrowDatabaseException();
    return false;
  }
  return true;
}

// close database  
bool wxSqliteDatabase::Close()
{
  ResetErrorCodes();

  CloseResultSets();
  CloseStatements();

  if (m_pDatabase != NULL)
  {
    int nReturn = sqlite3_close((sqlite3*)m_pDatabase);
    if (nReturn != SQLITE_OK)
    {
      SetErrorCode(wxSqliteDatabase::TranslateErrorCode(sqlite3_errcode((sqlite3*)m_pDatabase)));
      SetErrorMessage(ConvertFromUnicodeStream(sqlite3_errmsg((sqlite3*)m_pDatabase)));
      ThrowDatabaseException();
      return false;
    }
    m_pDatabase = NULL;
  }

  return true;
}

bool wxSqliteDatabase::IsOpen()
{
  return (m_pDatabase != NULL);
}

void wxSqliteDatabase::BeginTransaction()
{
  wxLogDebug(_("Beginning transaction"));
  RunQuery(_("begin transaction;"), false);
}

void wxSqliteDatabase::Commit()
{
  wxLogDebug(_("Commiting transaction"));
  RunQuery(_("commit transaction;"), false);
}

void wxSqliteDatabase::RollBack()
{
  wxLogDebug(_("Rolling back transaction"));
  RunQuery(_("rollback transaction;"), false);
}

// query database
int wxSqliteDatabase::RunQuery(const wxString& strQuery, bool bParseQuery)
{
  ResetErrorCodes();

  if (m_pDatabase == NULL)
    return false;

  wxArrayString QueryArray;
  if (bParseQuery)
    QueryArray = ParseQueries(strQuery);
  else
    QueryArray.push_back(strQuery);

  wxArrayString::iterator start = QueryArray.begin();
  wxArrayString::iterator stop = QueryArray.end();

  while (start != stop)
  {
    char* szErrorMessage = NULL;
    wxString strErrorMessage = wxT("");
    wxCharBuffer sqlBuffer = ConvertToUnicodeStream(*start);
    int nReturn = sqlite3_exec((sqlite3*)m_pDatabase, sqlBuffer, 0, 0, &szErrorMessage);
  
    if (szErrorMessage != NULL)
    {
      strErrorMessage = ConvertFromUnicodeStream(szErrorMessage);
      sqlite3_free(szErrorMessage);
    }

    if (nReturn != SQLITE_OK)
    {
      SetErrorCode(wxSqliteDatabase::TranslateErrorCode(sqlite3_errcode((sqlite3*)m_pDatabase)));
      SetErrorMessage(strErrorMessage);
      ThrowDatabaseException();
      return wxDATABASE_QUERY_RESULT_ERROR;
    }

    start++;
  }
  return (sqlite3_changes((sqlite3*)m_pDatabase));
}

wxDatabaseResultSet* wxSqliteDatabase::RunQueryWithResults(const wxString& strQuery)
{
  ResetErrorCodes();

  if (m_pDatabase != NULL)
  {
    wxArrayString QueryArray = ParseQueries(strQuery);
     
    for (unsigned int i=0; i<(QueryArray.size()-1); i++)
    {
      char* szErrorMessage = NULL;
      wxString strErrorMessage = wxT("");
      wxCharBuffer sqlBuffer = ConvertToUnicodeStream(QueryArray[i]);
      int nReturn = sqlite3_exec((sqlite3*)m_pDatabase, sqlBuffer, 0, 0, &szErrorMessage);
  
      if (szErrorMessage != NULL)
      {
        SetErrorCode(wxSqliteDatabase::TranslateErrorCode(sqlite3_errcode((sqlite3*)m_pDatabase)));
        strErrorMessage = ConvertFromUnicodeStream(szErrorMessage);
        sqlite3_free(szErrorMessage);
        return NULL;
      }

      if (nReturn != SQLITE_OK)
      {
        SetErrorCode(wxSqliteDatabase::TranslateErrorCode(sqlite3_errcode((sqlite3*)m_pDatabase)));
        SetErrorMessage(strErrorMessage);
        ThrowDatabaseException();
        return NULL;
      }
    }

    // Create a Prepared statement for the last SQL statement and get a result set from it
    wxSqlitePreparedStatement* pStatement = (wxSqlitePreparedStatement*)PrepareStatement(QueryArray[QueryArray.size()-1], false);
    wxSqliteResultSet* pResultSet = new wxSqliteResultSet(pStatement, true);
    if (pResultSet)
      pResultSet->SetEncoding(GetEncoding());

    LogResultSetForCleanup(pResultSet);
    return pResultSet;
  }
  else
  {
    return NULL;
  }
}

wxPreparedStatement* wxSqliteDatabase::PrepareStatement(const wxString& strQuery)
{
  return PrepareStatement(strQuery, true);
}

wxPreparedStatement* wxSqliteDatabase::PrepareStatement(const wxString& strQuery, bool bLogForCleanup)
{
  ResetErrorCodes();

  if (m_pDatabase != NULL)
  {
    wxSqlitePreparedStatement* pReturnStatement = new wxSqlitePreparedStatement((sqlite3*)m_pDatabase);
    if (pReturnStatement)
      pReturnStatement->SetEncoding(GetEncoding());
    
    wxArrayString QueryArray = ParseQueries(strQuery);

    wxArrayString::iterator start = QueryArray.begin();
    wxArrayString::iterator stop = QueryArray.end();

    while (start != stop)
    {
      const char* szTail=0;
      wxCharBuffer sqlBuffer;
      do
      {
        sqlite3_stmt* pStatement;
        wxString strSQL;
        if (szTail != 0)
        {
          strSQL = (wxChar*)szTail;
        }
        else
        {
          strSQL = (*start);
        }
        sqlBuffer = ConvertToUnicodeStream(strSQL);
#if SQLITE_VERSION_NUMBER>=3003009
        int nReturn = sqlite3_prepare_v2((sqlite3*)m_pDatabase, sqlBuffer, -1, &pStatement, &szTail);
#else
        int nReturn = sqlite3_prepare((sqlite3*)m_pDatabase, sqlBuffer, -1, &pStatement, &szTail);
#endif
   
        if (nReturn != SQLITE_OK)
        {
          SetErrorCode(wxSqliteDatabase::TranslateErrorCode(nReturn));
          SetErrorMessage(ConvertFromUnicodeStream(sqlite3_errmsg((sqlite3*)m_pDatabase)));
          wxDELETE(pReturnStatement);
          ThrowDatabaseException();
          return NULL;
        }
        pReturnStatement->AddPreparedStatement(pStatement);

#if wxUSE_UNICODE
      } while (strlen(szTail) > 0);
#else
      } while (wxStrlen(szTail) > 0);
#endif    
      
      start++;
    }

    if (bLogForCleanup)
      LogStatementForCleanup(pReturnStatement);
    return pReturnStatement;
  }
  else
  {
    return NULL;
  }
}

bool wxSqliteDatabase::TableExists(const wxString& table)
{
  // Initialize variables
  bool bReturn = false;
  // Keep these variables outside of scope so that we can clean them up
  //  in case of an error
  wxPreparedStatement* pStatement = NULL;
  wxDatabaseResultSet* pResult = NULL;

#if wxUSE_DATABASE_EXCEPTIONS
  try
  {
#endif
    wxString query = _("SELECT COUNT(*) FROM sqlite_master WHERE type='table' AND name=?;");
    pStatement = PrepareStatement(query);
    if (pStatement)
    {
      pStatement->SetParamString(1, table);
      pResult = pStatement->ExecuteQuery();
      if (pResult)
      {
        if (pResult->Next())
        {
          if(pResult->GetResultInt(1) != 0)
          {
            bReturn = true;
          }
        }
      }
    }
#if wxUSE_DATABASE_EXCEPTIONS
  }
  catch (wxDatabaseException& e)
  {
    if (pResult != NULL)
    {
      CloseResultSet(pResult);
      pResult = NULL;
    }

    if (pStatement != NULL)
    {
      CloseStatement(pStatement);
      pStatement = NULL;
    }

    throw e;
  }
#endif

  if (pResult != NULL)
  {
    CloseResultSet(pResult);
    pResult = NULL;
  }

  if (pStatement != NULL)
  {
    CloseStatement(pStatement);
    pStatement = NULL;
  }

  return bReturn;
}

bool wxSqliteDatabase::ViewExists(const wxString& view)
{
  // Initialize variables
  bool bReturn = false;
  // Keep these variables outside of scope so that we can clean them up
  //  in case of an error
  wxPreparedStatement* pStatement = NULL;
  wxDatabaseResultSet* pResult = NULL;

#if wxUSE_DATABASE_EXCEPTIONS
  try
  {
#endif
    wxString query = _("SELECT COUNT(*) FROM sqlite_master WHERE type='view' AND name=?;");
    pStatement = PrepareStatement(query);
    if (pStatement)
    {
      pStatement->SetParamString(1, view);
      pResult = pStatement->ExecuteQuery();
      if (pResult)
      {
        if (pResult->Next())
        {
          if(pResult->GetResultInt(1) != 0)
          {
            bReturn = true;
          }
        }
      }
    }
#if wxUSE_DATABASE_EXCEPTIONS
  }
  catch (wxDatabaseException& e)
  {
    if (pResult != NULL)
    {
      CloseResultSet(pResult);
      pResult = NULL;
    }

    if (pStatement != NULL)
    {
      CloseStatement(pStatement);
      pStatement = NULL;
    }

    throw e;
  }
#endif

  if (pResult != NULL)
  {
    CloseResultSet(pResult);
    pResult = NULL;
  }

  if (pStatement != NULL)
  {
    CloseStatement(pStatement);
    pStatement = NULL;
  }

  return bReturn;
}

wxArrayString wxSqliteDatabase::GetTables()
{
  wxArrayString returnArray;

  wxDatabaseResultSet* pResult = NULL;
#if wxUSE_DATABASE_EXCEPTIONS
  try
  {
#endif
    wxString query = _("SELECT name FROM sqlite_master WHERE type='table';");
    pResult = ExecuteQuery(query);

    while (pResult->Next())
    {
      returnArray.Add(pResult->GetResultString(1));
    }
#if wxUSE_DATABASE_EXCEPTIONS
  }
  catch (wxDatabaseException& e)
  {
    if (pResult != NULL)
    {
      CloseResultSet(pResult);
      pResult = NULL;
    }

    throw e;
  }
#endif

  if (pResult != NULL)
  {
    CloseResultSet(pResult);
    pResult = NULL;
  }

  return returnArray;
}

wxArrayString wxSqliteDatabase::GetViews()
{
  wxArrayString returnArray;

  wxDatabaseResultSet* pResult = NULL;
#if wxUSE_DATABASE_EXCEPTIONS
  try
  {
#endif
    wxString query = _("SELECT name FROM sqlite_master WHERE type='view';");
    pResult = ExecuteQuery(query);

    while (pResult->Next())
    {
      returnArray.Add(pResult->GetResultString(1));
    }
#if wxUSE_DATABASE_EXCEPTIONS
  }
  catch (wxDatabaseException& e)
  {
    if (pResult != NULL)
    {
      CloseResultSet(pResult);
      pResult = NULL;
    }

    throw e;
  }
#endif

  if (pResult != NULL)
  {
    CloseResultSet(pResult);
    pResult = NULL;
  }

  return returnArray;
}

wxArrayString wxSqliteDatabase::GetColumns(const wxString& table)
{
    wxArrayString returnArray;

  // Keep these variables outside of scope so that we can clean them up
  //  in case of an error
  wxDatabaseResultSet* pResult = NULL;
  wxResultSetMetaData* pMetaData = NULL;

#if wxUSE_DATABASE_EXCEPTIONS
  try
  {
#endif
    wxCharBuffer tableNameBuffer = ConvertToUnicodeStream(table);
    wxString query = wxString::Format(_("SELECT * FROM '%s' LIMIT 0;"), table.c_str());
    pResult = ExecuteQuery(query);
    pResult->Next();
    pMetaData = pResult->GetMetaData();

    // 1-based
    for(int i=1; i<=pMetaData->GetColumnCount(); i++)
    {
      returnArray.Add(pMetaData->GetColumnName(i));
    }

#if wxUSE_DATABASE_EXCEPTIONS
  }
  catch (wxDatabaseException& e)
  {
    if (pMetaData != NULL)
    {
      pResult->CloseMetaData(pMetaData);
      pMetaData = NULL;
    }

    if (pResult != NULL)
    {
      CloseResultSet(pResult);
      pResult = NULL;
    }

    throw e;
  }
#endif

  if (pMetaData != NULL)
  {
    pResult->CloseMetaData(pMetaData);
    pMetaData = NULL;
  }

  if (pResult != NULL)
  {
    CloseResultSet(pResult);
    pResult = NULL;
  }

  return returnArray;
}


wxArrayString wxSqliteDatabase::GetPKColumns(const wxString& table)
{
    wxArrayString returnArray;

  // Keep these variables outside of scope so that we can clean them up
  //  in case of an error
  wxDatabaseResultSet* pResult = NULL;
  wxResultSetMetaData* pMetaData = NULL;

#if wxUSE_DATABASE_EXCEPTIONS
  try
  {
#endif
    wxCharBuffer tableNameBuffer = ConvertToUnicodeStream(table);
    wxString query = wxString::Format(_("PRAGMA table_info('%s') ;"), table.c_str());
    pResult = ExecuteQuery(query);
    while(pResult->Next()) 
	{
		if(pResult->GetResultInt(wxT("pk"))==1)//its primary key
		{
			returnArray.Add(pResult->GetResultString(wxT("pk")));
		}
	} 

#if wxUSE_DATABASE_EXCEPTIONS
  }
  catch (wxDatabaseException& e)
  {
    if (pMetaData != NULL)
    {
      pResult->CloseMetaData(pMetaData);
      pMetaData = NULL;
    }

    if (pResult != NULL)
    {
      CloseResultSet(pResult);
      pResult = NULL;
    }

    throw e;
  }
#endif

  if (pMetaData != NULL)
  {
    pResult->CloseMetaData(pMetaData);
    pMetaData = NULL;
  }

  if (pResult != NULL)
  {
    CloseResultSet(pResult);
    pResult = NULL;
  }

  return returnArray;
}


int wxSqliteDatabase::TranslateErrorCode(int nCode)
{
  // Ultimately, this will probably be a map of SQLite database error code values to wxDatabase values
  // For now though, we'll just return error
  int nReturn = nCode;
  /*
  switch (nCode)
  {
    case SQLITE_ERROR:
      nReturn = wxDATABASE_SQL_SYNTAX_ERROR;
      break;
    case SQLITE_INTERNAL:
      nReturn = wxDATABASE_ERROR;
      break;
    case SQLITE_PERM:
      nReturn = wxDATABASE_ERROR;
      break;
    case SQLITE_ABORT:
      nReturn = wxDATABASE_ERROR;
      break;
    case SQLITE_BUSY:
      nReturn = wxDATABASE_ERROR;
      break;
    case SQLITE_LOCKED:
      nReturn = wxDATABASE_ERROR;
      break;
    case SQLITE_NOMEM:
      nReturn = wxDATABASE_ALLOCATION_ERROR;
      break;
    case SQLITE_READONLY:
      nReturn = wxDATABASE_ERROR;
      break;
    case SQLITE_INTERRUPT:
      nReturn = wxDATABASE_ERROR;
      break;
    case SQLITE_IOERR:
      nReturn = wxDATABASE_ERROR;
      break;
    case SQLITE_CORRUPT:
      nReturn = wxDATABASE_ERROR;
      break;
    case SQLITE_NOTFOUND:
      nReturn = wxDATABASE_ERROR;
      break;
    case SQLITE_FULL:
      nReturn = wxDATABASE_ERROR;
      break;
    case SQLITE_CANTOPEN:
      nReturn = wxDATABASE_ERROR;
      break;
    case SQLITE_PROTOCOL:
      nReturn = wxDATABASE_ERROR;
      break;
    case SQLITE_SCHEMA:
      nReturn = wxDATABASE_ERROR;
      break;
    case SQLITE_TOOBIG:
      nReturn = wxDATABASE_ERROR;
      break;
    case SQLITE_CONSTRAINT:
      nReturn = wxDATABASE_CONSTRAINT_VIOLATION;
      break;
    case SQLITE_MISMATCH:
      nReturn = wxDATABASE_INCOMPATIBLE_FIELD_TYPE;
      break;
    case SQLITE_MISUSE:
      nReturn = wxDATABASE_ERROR;
      break;
    case SQLITE_NOLFS:
      nReturn = wxDATABASE_ERROR;
      break;
    case SQLITE_AUTH:
      nReturn = wxDATABASE_ERROR;
      break;
    default:
      nReturn = wxDATABASE_ERROR;
      break;
  }
  */
  return nReturn;
}

#endif//wxUSE_DATABASE_SQLITE
