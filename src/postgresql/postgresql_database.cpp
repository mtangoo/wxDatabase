#include "include/postgresql/postgresql_preparedstatement.h"
#include "include/postgresql/postgresql_resultset.h"
#include "include/postgresql/postgresql_interface.h"
#include "include/postgresql/postgresql_database.h"
#include "include/errorcodes.h"
#include "include/exceptions.h"


// ctor
wxPostgresDatabase::wxPostgresDatabase()
 : wxDatabase()
{
#ifndef DONT_USE_DYNAMIC_DATABASE_LAYER_LINKING
  m_pInterface = new wxDynamicPostgresInterface();
  if (!m_pInterface->Init())
  {
    SetErrorCode(wxDATABASE_ERROR_LOADING_LIBRARY);
    SetErrorMessage(wxT("Error loading PostgreSQL library"));
    ThrowDatabaseException();
    return;
  }
#endif
  m_strServer = _("localhost");
  m_strUser = _("");
  m_strPassword = _("");
  m_strDatabase = _("");
  m_strPort = _("");
}

wxPostgresDatabase::wxPostgresDatabase(const wxString& strDatabase)
 : wxDatabase()
{
#ifndef DONT_USE_DYNAMIC_DATABASE_LAYER_LINKING
  m_pInterface = new wxDynamicPostgresInterface();
  if (!m_pInterface->Init())
  {
    SetErrorCode(wxDATABASE_ERROR_LOADING_LIBRARY);
    SetErrorMessage(wxT("Error loading PostgreSQL library"));
    ThrowDatabaseException();
    return;
  }
#endif
  m_strServer = _("localhost");
  m_strUser = _("");
  m_strPassword = _("");
  m_strPort = _("");

  Open(strDatabase);
}

wxPostgresDatabase::wxPostgresDatabase(const wxString& strServer, const wxString& strDatabase)
 : wxDatabase()
{
#ifndef DONT_USE_DYNAMIC_DATABASE_LAYER_LINKING
  m_pInterface = new wxDynamicPostgresInterface();
  if (!m_pInterface->Init())
  {
    SetErrorCode(wxDATABASE_ERROR_LOADING_LIBRARY);
    SetErrorMessage(wxT("Error loading PostgreSQL library"));
    ThrowDatabaseException();
    return;
  }
#endif
  m_strServer = strServer;
  m_strUser = _("");
  m_strPassword = _("");
  m_strPort = _("");

  Open(strDatabase);
}

wxPostgresDatabase::wxPostgresDatabase(const wxString& strDatabase, const wxString& strUser, const wxString& strPassword)
 : wxDatabase()
{
#ifndef DONT_USE_DYNAMIC_DATABASE_LAYER_LINKING
  m_pInterface = new wxDynamicPostgresInterface();
  if (!m_pInterface->Init())
  {
    SetErrorCode(wxDATABASE_ERROR_LOADING_LIBRARY);
    SetErrorMessage(wxT("Error loading PostgreSQL library"));
    ThrowDatabaseException();
    return;
  }
#endif
  m_strServer = _("localhost");
  m_strUser = strUser;
  m_strPassword = strPassword;
  m_strPort = _("");

  Open(strDatabase);
}

wxPostgresDatabase::wxPostgresDatabase(const wxString& strServer, const wxString& strDatabase, const wxString& strUser, const wxString& strPassword)
 : wxDatabase()
{
#ifndef DONT_USE_DYNAMIC_DATABASE_LAYER_LINKING
  m_pInterface = new wxDynamicPostgresInterface();
  if (!m_pInterface->Init())
  {
    SetErrorCode(wxDATABASE_ERROR_LOADING_LIBRARY);
    SetErrorMessage(wxT("Error loading PostgreSQL library"));
    ThrowDatabaseException();
    return;
  }
#endif
  m_strServer = strServer;
  m_strUser = strUser;
  m_strPassword = strPassword;
  m_strPort = _("");

  Open(strDatabase);
}

wxPostgresDatabase::wxPostgresDatabase(const wxString& strServer, int nPort, const wxString& strDatabase, const wxString& strUser, const wxString& strPassword)
 : wxDatabase()
{
#ifndef DONT_USE_DYNAMIC_DATABASE_LAYER_LINKING
  m_pInterface = new wxDynamicPostgresInterface();
  if (!m_pInterface->Init())
  {
    SetErrorCode(wxDATABASE_ERROR_LOADING_LIBRARY);
    SetErrorMessage(wxT("Error loading PostgreSQL library"));
    ThrowDatabaseException();
    return;
  }
#endif
  m_strServer = strServer;
  m_strUser = strUser;
  m_strPassword = strPassword;
  m_strPort = wxString::Format(_("%d"), nPort);

  Open(strDatabase);
}

// dtor
wxPostgresDatabase::~wxPostgresDatabase()
{
  Close();
  wxDELETE(m_pInterface);
}

// open database
bool wxPostgresDatabase::Open()
{
  ResetErrorCodes();

  wxCharBuffer serverCharBuffer;
  const char* pHost = NULL;
  wxCharBuffer pDatabaseBuffer = ConvertToUnicodeStream(m_strDatabase);
  const char* pDatabase = pDatabaseBuffer;
  wxCharBuffer userCharBuffer;
  const char* pUser = NULL;
  wxCharBuffer passwordCharBuffer;
  const char* pPassword = NULL;
  const char* pTty = NULL;
  const char* pOptions = NULL;
  wxCharBuffer portCharBuffer;
  const char* pPort = NULL;
  
  if (m_strServer != _("localhost") && m_strServer != _(""))
  {
    serverCharBuffer = ConvertToUnicodeStream(m_strServer);
    pHost = serverCharBuffer;
  }
  
  if (m_strUser != _(""))
  {
    userCharBuffer = ConvertToUnicodeStream(m_strUser);
    pUser = userCharBuffer;
  }

  if (m_strPassword != _(""))
  {
    passwordCharBuffer = ConvertToUnicodeStream(m_strPassword);
    pPassword = passwordCharBuffer;
  }

  if (m_strPort != _(""))
  {
    portCharBuffer = ConvertToUnicodeStream(m_strPort);
    pPort = portCharBuffer;
  }

  m_pDatabase = m_pInterface->GetPQsetdbLogin()(pHost, pPort, pOptions, pTty, pDatabase, pUser, pPassword);
  if (m_pInterface->GetPQstatus()((PGconn*)m_pDatabase) == CONNECTION_BAD)
  {
    SetErrorCode(wxPostgresDatabase::TranslateErrorCode(m_pInterface->GetPQstatus()((PGconn*)m_pDatabase)));
    SetErrorMessage(ConvertFromUnicodeStream(m_pInterface->GetPQerrorMessage()((PGconn*)m_pDatabase)));
    ThrowDatabaseException();
    return false;
  }

  m_pInterface->GetPQsetClientEncoding()((PGconn*)m_pDatabase, "UTF-8");
  wxCSConv conv((const wxChar*)(m_pInterface->GetPQencodingToChar()(m_pInterface->GetPQclientEncoding()((PGconn*)m_pDatabase))));
  SetEncoding(&conv);
  
  return true;
}

bool wxPostgresDatabase::Open(const wxString& strDatabase)
{
  m_strDatabase = strDatabase;
  return Open();
}

bool wxPostgresDatabase::Open(const wxString& strServer, const wxString& strDatabase)
{
  m_strServer = strServer;
  m_strUser = _("");
  m_strPassword = _("");
  m_strDatabase = strDatabase;
  m_strPort = _("");
  return Open();
}

bool wxPostgresDatabase::Open(const wxString& strDatabase, const wxString& strUser, const wxString& strPassword)
{
  m_strServer = _("localhost");
  m_strUser = strUser;
  m_strPassword = strPassword;
  m_strDatabase = strDatabase;
  m_strPort = _("");
  return Open();
}

bool wxPostgresDatabase::Open(const wxString& strServer, const wxString& strDatabase, const wxString& strUser, const wxString& strPassword)
{
  m_strServer = strServer;
  m_strUser = strUser;
  m_strPassword = strPassword;
  m_strDatabase = strDatabase;
  m_strPort = _("");
  return Open();
}

bool wxPostgresDatabase::Open(const wxString& strServer, int nPort, const wxString& strDatabase, const wxString& strUser, const wxString& strPassword)
{
  m_strServer = strServer;
  m_strUser = strUser;
  m_strPassword = strPassword;
  m_strDatabase = strDatabase;
  SetPort(nPort);
  return Open();
}

// close database
bool wxPostgresDatabase::Close()
{
  CloseResultSets();
  CloseStatements();
  
  if (m_pDatabase)
  {
    m_pInterface->GetPQfinish()((PGconn*)m_pDatabase);
    m_pDatabase = NULL;
  }

  return true;
}

void wxPostgresDatabase::SetPort(int nPort)
{
  m_strPort = wxString::Format(_("%d"), nPort);
}

bool wxPostgresDatabase::IsOpen()
{
  if (m_pDatabase)
    return (m_pInterface->GetPQstatus()((PGconn*)m_pDatabase) != CONNECTION_BAD);
  else
    return false;
}
  
// transaction support
void wxPostgresDatabase::BeginTransaction()
{
  RunQuery(_("BEGIN"), false);
}

void wxPostgresDatabase::Commit()
{
  RunQuery(_("COMMIT"), false);
}

void wxPostgresDatabase::RollBack()
{
  RunQuery(_("ROLLBACK"), false);
}

  
// query database
int wxPostgresDatabase::RunQuery(const wxString& strQuery, bool WXUNUSED(bParseQuery))
{
  // PostgreSQL takes care of parsing the queries itself so bParseQuery is ignored

  ResetErrorCodes();

  wxCharBuffer sqlBuffer = ConvertToUnicodeStream(strQuery);
  PGresult* pResultCode = m_pInterface->GetPQexec()((PGconn*)m_pDatabase, sqlBuffer);
  if ((pResultCode == NULL) || (m_pInterface->GetPQresultStatus()(pResultCode) != PGRES_COMMAND_OK))
  {
    SetErrorCode(wxPostgresDatabase::TranslateErrorCode(m_pInterface->GetPQresultStatus()(pResultCode)));
    SetErrorMessage(ConvertFromUnicodeStream(m_pInterface->GetPQerrorMessage()((PGconn*)m_pDatabase)));
    m_pInterface->GetPQclear()(pResultCode);
    ThrowDatabaseException();
    return wxDATABASE_QUERY_RESULT_ERROR;
  }
  else
  {
    wxString rowsAffected = ConvertFromUnicodeStream(m_pInterface->GetPQcmdTuples()(pResultCode));
    long rows = -1;
    rowsAffected.ToLong(&rows);
    m_pInterface->GetPQclear()(pResultCode);
    return (int)rows;
  }
}

wxDatabaseResultSet* wxPostgresDatabase::RunQueryWithResults(const wxString& strQuery)
{
  ResetErrorCodes();

  wxCharBuffer sqlBuffer = ConvertToUnicodeStream(strQuery);
  PGresult* pResultCode = m_pInterface->GetPQexec()((PGconn*)m_pDatabase, sqlBuffer);
  if ((pResultCode == NULL) || (m_pInterface->GetPQresultStatus()(pResultCode) != PGRES_TUPLES_OK))
  {
    SetErrorCode(wxPostgresDatabase::TranslateErrorCode(m_pInterface->GetPQstatus()((PGconn*)m_pDatabase)));
    SetErrorMessage(ConvertFromUnicodeStream(m_pInterface->GetPQerrorMessage()((PGconn*)m_pDatabase)));
    m_pInterface->GetPQclear()(pResultCode);
    ThrowDatabaseException();
    return NULL;
  }
  else
  {
    wxPostgresResultSet* pResultSet = new wxPostgresResultSet(m_pInterface, pResultCode);
    pResultSet->SetEncoding(GetEncoding());
    LogResultSetForCleanup(pResultSet);
    return pResultSet;
  }
}

// PreparedStatement support
wxPreparedStatement* wxPostgresDatabase::PrepareStatement(const wxString& strQuery)
{
  ResetErrorCodes();

  wxPostgresPreparedStatement* pStatement = wxPostgresPreparedStatement::CreateStatement(m_pInterface, (PGconn*)m_pDatabase, strQuery);
  LogStatementForCleanup(pStatement);
  return pStatement;
}

bool wxPostgresDatabase::TableExists(const wxString& table)
{
  // Initialize variables
  bool bReturn = false;
  // Keep these variables outside of scope so that we can clean them up
  //  in case of an error
  wxPreparedStatement* pStatement = NULL;
  wxDatabaseResultSet* pResult = NULL;

#ifndef DONT_USE_DATABASE_LAYER_EXCEPTIONS
  try
  {
#endif
    wxString query = _("SELECT COUNT(*) FROM information_schema.tables WHERE table_type='BASE TABLE' AND table_name=?;");
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
#ifndef DONT_USE_DATABASE_LAYER_EXCEPTIONS
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

bool wxPostgresDatabase::ViewExists(const wxString& view)
{
  // Initialize variables
  bool bReturn = false;
  // Keep these variables outside of scope so that we can clean them up
  //  in case of an error
  wxPreparedStatement* pStatement = NULL;
  wxDatabaseResultSet* pResult = NULL;

#ifndef DONT_USE_DATABASE_LAYER_EXCEPTIONS
  try
  {
#endif
    wxString query = _("SELECT COUNT(*) FROM information_schema.tables WHERE table_type='VIEW' AND table_name=?;");
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
#ifndef DONT_USE_DATABASE_LAYER_EXCEPTIONS
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

wxArrayString wxPostgresDatabase::GetTables()
{
  wxArrayString returnArray;

  wxDatabaseResultSet* pResult = NULL;
#ifndef DONT_USE_DATABASE_LAYER_EXCEPTIONS
  try
  {
#endif
    wxString query = _("SELECT table_name FROM information_schema.tables WHERE table_type='BASE TABLE' AND table_schema='public';");
    pResult = ExecuteQuery(query);

    while (pResult->Next())
    {
      returnArray.Add(pResult->GetResultString(1));
    }
#ifndef DONT_USE_DATABASE_LAYER_EXCEPTIONS
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

wxArrayString wxPostgresDatabase::GetViews()
{
  wxArrayString returnArray;

  wxDatabaseResultSet* pResult = NULL;
#ifndef DONT_USE_DATABASE_LAYER_EXCEPTIONS
  try
  {
#endif
    wxString query = _("SELECT table_name FROM information_schema.tables WHERE table_type='VIEW' AND table_schema='public';");
    pResult = ExecuteQuery(query);

    while (pResult->Next())
    {
      returnArray.Add(pResult->GetResultString(1));
    }
#ifndef DONT_USE_DATABASE_LAYER_EXCEPTIONS
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

wxArrayString wxPostgresDatabase::GetColumns(const wxString& table)
{
  // Initialize variables
  wxArrayString returnArray;

  // Keep these variables outside of scope so that we can clean them up
  //  in case of an error
  wxPreparedStatement* pStatement = NULL;
  wxDatabaseResultSet* pResult = NULL;

#ifndef DONT_USE_DATABASE_LAYER_EXCEPTIONS
  try
  {
#endif
    wxString query = _("SELECT column_name FROM information_schema.columns WHERE table_name=? ORDER BY ordinal_position;");
    pStatement = PrepareStatement(query);
    if (pStatement)
    {
      pStatement->SetParamString(1, table);
      pResult = pStatement->ExecuteQuery();
      if (pResult)
      {
        while (pResult->Next())
        {
          returnArray.Add(pResult->GetResultString(1));
        }
      }
    }
#ifndef DONT_USE_DATABASE_LAYER_EXCEPTIONS
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

  return returnArray;
}

int wxPostgresDatabase::TranslateErrorCode(int nCode)
{
  // Ultimately, this will probably be a map of Postgresql database error code values to DatabaseLayer values
  // For now though, we'll just return error
  return nCode;
  //return DATABASE_LAYER_ERROR;
}

bool wxPostgresDatabase::IsAvailable()
{
  bool bAvailable = false;
  wxDynamicPostgresInterface* pInterface = new wxDynamicPostgresInterface();
  bAvailable = pInterface && pInterface->Init();
  wxDELETE(pInterface);
  return bAvailable;
}

