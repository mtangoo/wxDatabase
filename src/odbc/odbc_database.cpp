#include "include/odbc/odbc_database.h" 
#include "include/odbc/odbc_inteface.h"
#include "include/odbc/odbc_preparedstatement.h" 
#include "include/odbc/odbc_resultset.h" 
#include "include/errorcodes.h"

// ctor()
wxOdbcDatabase::wxOdbcDatabase(): wxDatabase()
{
   m_bIsConnected = false;
   ResetErrorCodes();

   m_pInterface = new wxOdbcInterface();
   if (!m_pInterface->Init())
   {
     SetErrorCode(wxDATABASE_ERROR_LOADING_LIBRARY);
     SetErrorMessage(wxT("Error loading ODBC library"));
     ThrowDatabaseException();
     return;
   }

   SQLHENV sqlEnvHandle = (SQLHENV)m_sqlEnvHandle;
   SQLRETURN nRet = m_pInterface->GetSQLAllocHandle()(SQL_HANDLE_ENV, SQL_NULL_HANDLE, &sqlEnvHandle);
   m_sqlEnvHandle = sqlEnvHandle;
   if ( nRet != SQL_SUCCESS )
   {
     InterpretErrorCodes( nRet );
     ThrowDatabaseException();
   }

   nRet = m_pInterface->GetSQLSetEnvAttr()((SQLHENV)m_sqlEnvHandle, SQL_ATTR_ODBC_VERSION, (SQLPOINTER)SQL_OV_ODBC3, 0);
   if ( nRet != SQL_SUCCESS )
   {
     InterpretErrorCodes( nRet );
     ThrowDatabaseException();
   }

   SQLHDBC sqlHDBC = (SQLHDBC)m_sqlHDBC;
   nRet = m_pInterface->GetSQLAllocHandle()(SQL_HANDLE_DBC, (SQLHENV)m_sqlEnvHandle, &sqlHDBC);
   m_sqlHDBC = sqlHDBC;
   if ( nRet != SQL_SUCCESS )
   {
     InterpretErrorCodes( nRet );
     ThrowDatabaseException();
   }

   m_strDSN = wxEmptyString;
   m_strUser = wxEmptyString;
   m_strPassword = wxEmptyString;
   m_strConnection = wxEmptyString;
#if wxUSE_GUI
   m_bPrompt = false;
   m_pParent = NULL;
#endif
}

wxOdbcDatabase::~wxOdbcDatabase()
{
   Close();

   SQLRETURN nRet = m_pInterface->GetSQLFreeHandle()(SQL_HANDLE_DBC, (SQLHDBC)m_sqlHDBC);
   if ( nRet != SQL_SUCCESS )
   {
     InterpretErrorCodes( nRet );
     ThrowDatabaseException();
   }

   nRet = m_pInterface->GetSQLFreeHandle()(SQL_HANDLE_ENV, (SQLHENV)m_sqlEnvHandle);
   if ( nRet != SQL_SUCCESS )
   {
     InterpretErrorCodes( nRet );
     ThrowDatabaseException();
   }

   wxDELETE(m_pInterface);
}

bool wxOdbcDatabase::Open( )
{
   ResetErrorCodes();

   if ( !m_strDSN.IsEmpty() )
   {
#ifdef wxUSE_UNICODE
     wxCharBuffer dsnCharBuffer = ConvertToUnicodeStream(m_strDSN);
     wxCharBuffer userCharBuffer = ConvertToUnicodeStream(m_strUser);
     wxCharBuffer passwordCharBuffer = ConvertToUnicodeStream(m_strPassword);
#else
     void* dsnCharBuffer = (void*)m_strDSN.c_str();
     void* userCharBuffer = (void*)m_strUser.c_str();
     void* passwordCharBuffer = (void*)m_strPassword.c_str();
#endif

     SQLRETURN nRet;
     nRet = m_pInterface->GetSQLConnect()((SQLHDBC)m_sqlHDBC, (SQLTCHAR FAR*)(const char*)dsnCharBuffer,
                SQL_NTS, (SQLTCHAR FAR*)(const char*)userCharBuffer, SQL_NTS,
                (SQLTCHAR FAR*)(const char*)passwordCharBuffer, SQL_NTS);
     if ( nRet != SQL_SUCCESS && nRet != SQL_SUCCESS_WITH_INFO )
     {
       InterpretErrorCodes( nRet );
       ThrowDatabaseException();
     }
   }
   else if ( !m_strConnection.IsEmpty() )
   {
     SQLTCHAR buff[8192];
     SQLSMALLINT iLen;

     memset(buff, 0, 8192*sizeof(SQLTCHAR));
     
     //wxCharBuffer connectionCharBuffer = ConvertToUnicodeStream(m_strConnection);
     void* connectionCharBuffer = (void*)m_strConnection.ToStdString().c_str();
#if wxUSE_GUI
     SQLRETURN nRet = m_pInterface->GetSQLDriverConnect()((SQLHDBC)m_sqlHDBC, m_pParent ? (SQLHWND)m_pParent->GetHandle() : NULL, (SQLTCHAR*)(const char*)connectionCharBuffer,
         (SQLSMALLINT)m_strConnection.Length(), (SQLTCHAR*)buff, 8192, &iLen, m_bPrompt ? SQL_DRIVER_PROMPT : SQL_DRIVER_NOPROMPT);
#else
     SQLRETURN nRet = m_pInterface->GetSQLDriverConnect()((SQLHDBC)m_sqlHDBC, NULL, (SQLTCHAR*)(const char*)connectionCharBuffer,
         (SQLSMALLINT)m_strConnection.Length(), (SQLTCHAR*)buff, 8192, &iLen, SQL_DRIVER_NOPROMPT);
#endif

     if ( nRet != SQL_SUCCESS && nRet != SQL_SUCCESS_WITH_INFO )
     {
       InterpretErrorCodes( nRet );
       ThrowDatabaseException();
     }
   }
   else
   {
     return false;
   }

   m_bIsConnected = true;    
        
   return true;
}

bool wxOdbcDatabase::Open( const wxString& strConnection )
{
    m_strDSN = wxEmptyString;
    m_strUser = wxEmptyString;
    m_strPassword = wxEmptyString;
    m_strConnection = strConnection;
#if wxUSE_GUI
    m_bPrompt = false;
    m_pParent = NULL;
#endif

    return Open();
}

#if wxUSE_GUI
bool wxOdbcDatabase::Open(const wxString& strConnection, bool bPromptForInfo, wxWindow* parent)
{
    m_strConnection = strConnection;
    m_bPrompt = bPromptForInfo;
    m_pParent = parent;
    m_strDSN = wxEmptyString;
    m_strUser = wxEmptyString;
    m_strPassword = wxEmptyString;

    return Open();
}
#endif

bool wxOdbcDatabase::Open( const wxString& strDSN, const wxString& strUser, const wxString& strPassword )
{
    m_strDSN = strDSN;
    m_strUser = strUser;
    m_strPassword = strPassword;
    m_strConnection = wxEmptyString;
#if wxUSE_GUI
    m_bPrompt = false;
    m_pParent = NULL;
#endif
        
    return Open();
}

bool wxOdbcDatabase::Close()
{
   ResetErrorCodes();

   CloseResultSets();
   CloseStatements();

   if (m_bIsConnected) 
   {
      SQLRETURN nRet = m_pInterface->GetSQLDisconnect()((SQLHDBC)m_sqlHDBC);
      if ( nRet != SQL_SUCCESS )
      {
        InterpretErrorCodes( nRet );
        ThrowDatabaseException();
      }

      m_bIsConnected=false;
   }

   return true;
}

bool wxOdbcDatabase::IsOpen()
{
  return m_bIsConnected;
}

void wxOdbcDatabase::BeginTransaction()
{
   ResetErrorCodes();

   SQLRETURN nRet = m_pInterface->GetSQLSetConnectAttr()((SQLHDBC)m_sqlHDBC, SQL_ATTR_AUTOCOMMIT, (SQLPOINTER)SQL_AUTOCOMMIT_OFF, 0);
   if ( nRet != SQL_SUCCESS )
   {
     InterpretErrorCodes( nRet );
     ThrowDatabaseException();
   }
}

void wxOdbcDatabase::Commit()
{
   ResetErrorCodes();

   SQLRETURN nRet = m_pInterface->GetSQLEndTran()(SQL_HANDLE_DBC, (SQLHDBC)m_sqlHDBC, SQL_COMMIT);
   if ( nRet != SQL_SUCCESS )
   {
     InterpretErrorCodes( nRet );
     ThrowDatabaseException();
   }

   nRet = m_pInterface->GetSQLSetConnectAttr()((SQLHDBC)m_sqlHDBC, SQL_ATTR_AUTOCOMMIT, (SQLPOINTER)SQL_AUTOCOMMIT_ON, SQL_IS_INTEGER);
   if ( nRet != SQL_SUCCESS )
   {
     InterpretErrorCodes( nRet );
     ThrowDatabaseException();
   }
}

void wxOdbcDatabase::RollBack()
{
   ResetErrorCodes();

   SQLRETURN nRet = m_pInterface->GetSQLEndTran()(SQL_HANDLE_DBC, (SQLHDBC)m_sqlHDBC, SQL_ROLLBACK);
   if ( nRet != SQL_SUCCESS )
   {
     InterpretErrorCodes( nRet );
     ThrowDatabaseException();
   }
   
   nRet = m_pInterface->GetSQLSetConnectAttr()((SQLHDBC)m_sqlHDBC, SQL_ATTR_AUTOCOMMIT, (SQLPOINTER)SQL_AUTOCOMMIT_ON, SQL_IS_INTEGER);
   if ( nRet != SQL_SUCCESS )
   {
     InterpretErrorCodes( nRet );
     ThrowDatabaseException();
   }
}

int wxOdbcDatabase::RunQuery( const wxString& strQuery, bool bParseQuery )
{
   ResetErrorCodes();

   //wxPrintf("Running: '%s'\n", strQuery.c_str());
   wxOdbcPreparedStatement* pStatement = (wxOdbcPreparedStatement*)PrepareStatement( strQuery, bParseQuery );

   if ( pStatement )
   {
     try
     {
       int nRows = pStatement->RunQuery();
       wxDELETE( pStatement );
       return nRows;
     }
     catch (...)
     {
       SetErrorCode(pStatement->GetErrorCode());
       SetErrorMessage(pStatement->GetErrorMessage());
       wxDELETE( pStatement );
       ThrowDatabaseException();
       return wxDATABASE_QUERY_RESULT_ERROR;
     }
   }
   else
     return wxDATABASE_QUERY_RESULT_ERROR;
}

wxDatabaseResultSet* wxOdbcDatabase::RunQueryWithResults(const wxString& strQuery)
{
   ResetErrorCodes();

   wxOdbcPreparedStatement* pStatement = (wxOdbcPreparedStatement*)PrepareStatement( strQuery, true );

   if ( pStatement )
   {
     try
     {
       pStatement->SetOneTimer(true);
       wxDatabaseResultSet* pResults = pStatement->RunQueryWithResults(false /*false for "Don't log this result set for cleanup*/);
       LogResultSetForCleanup(pResults);
       return pResults;
     }
     catch (...)
     {
       SetErrorCode(pStatement->GetErrorCode());
       SetErrorMessage(pStatement->GetErrorMessage());
       wxDELETE( pStatement );
       ThrowDatabaseException();
       return NULL;
     }
   }
   else
     return NULL;
}

void* wxOdbcDatabase::allocStmth()
{
    ResetErrorCodes();

    SQLHANDLE handle = NULL;
        
    SQLRETURN nRet = m_pInterface->GetSQLAllocHandle()(SQL_HANDLE_STMT, (SQLHDBC)m_sqlHDBC, &handle);
    if ( nRet != SQL_SUCCESS )
    {
        InterpretErrorCodes( nRet );
        ThrowDatabaseException();
    }
    return handle;
}

wxPreparedStatement* wxOdbcDatabase::PrepareStatement( const wxString& strQuery )
{
  wxPreparedStatement* pStatement = PrepareStatement(strQuery, true);
  LogStatementForCleanup(pStatement);
  return pStatement;
}

wxPreparedStatement* wxOdbcDatabase::PrepareStatement( const wxString& strQuery, bool bParseQuery )
{
    ResetErrorCodes();

    wxArrayString QueryArray;
    if (bParseQuery)
      QueryArray = ParseQueries(strQuery);
    else
      QueryArray.push_back(strQuery);

    wxOdbcPreparedStatement* pReturnStatement = new wxOdbcPreparedStatement(m_pInterface, (SQLHENV)m_sqlEnvHandle, (SQLHDBC)m_sqlHDBC);

    if (pReturnStatement)
        pReturnStatement->SetEncoding(GetEncoding());

    for (unsigned int i=0; i<(QueryArray.size()); i++)
    {
//#if wxUSE_UNICODE
//        void* sqlBuffer = (void*)(QueryArray[i].c_str());
//#else
        wxCharBuffer sqlBuffer = ConvertToUnicodeStream(QueryArray[i]);
//#endif
        //wxPrintf(_("Preparing statement: '%s'\n"), sqlBuffer);

        SQLHSTMT pSqlStatement = allocStmth();
        SQLRETURN nRet = m_pInterface->GetSQLPrepare()(pSqlStatement, (SQLTCHAR*)(const char*)sqlBuffer, SQL_NTS);
        if ( nRet != SQL_SUCCESS && nRet != SQL_SUCCESS_WITH_INFO )
        {
            InterpretErrorCodes( nRet );
            m_pInterface->GetSQLFreeStmt()(pSqlStatement, SQL_CLOSE);
            ThrowDatabaseException();
            return NULL;
        }

        if ( pSqlStatement )
            pReturnStatement->AddPreparedStatement( pSqlStatement );
    }

    return pReturnStatement;
}

bool wxOdbcDatabase::TableExists(const wxString& table)
{
  bool bReturn = false;
  // Use SQLTables
  SQLHSTMT pStatement = allocStmth();
  wxCharBuffer tableBuffer = ConvertToUnicodeStream(table);
  wxString tableType = _("TABLE");
  wxCharBuffer tableTypeBuffer = ConvertToUnicodeStream(tableType);
  int tableTypeBufferLength = GetEncodedStreamLength(tableType);
  SQLRETURN nRet = m_pInterface->GetSQLTables()(pStatement,
      NULL, 0,
      NULL, 0,
      (SQLTCHAR*)(const char*)tableBuffer, SQL_NTS,
      (SQLTCHAR*)(const char*)tableTypeBuffer, tableTypeBufferLength);

  if (nRet != SQL_SUCCESS)
  {
    InterpretErrorCodes( nRet );
    m_pInterface->GetSQLFreeStmt()(pStatement, SQL_CLOSE);
    ThrowDatabaseException();
    return false;
  }

  nRet = m_pInterface->GetSQLFetch()(pStatement);
  if (nRet == SQL_SUCCESS || nRet == SQL_SUCCESS_WITH_INFO)
    bReturn = true;

  m_pInterface->GetSQLFreeStmt()(pStatement, SQL_CLOSE);
  
  return bReturn;
}

bool wxOdbcDatabase::ViewExists(const wxString& view)
{
  bool bReturn = false;
  // Use SQLTables
  SQLHSTMT pStatement = allocStmth();
  wxCharBuffer viewBuffer = ConvertToUnicodeStream(view);
  wxString tableType = _("VIEW");
  wxCharBuffer tableTypeBuffer = ConvertToUnicodeStream(tableType);
  int tableTypeBufferLength = GetEncodedStreamLength(tableType);
  SQLRETURN nRet = m_pInterface->GetSQLTables()(pStatement,
      NULL, 0,
      NULL, 0,
      (SQLTCHAR*)(const char*)viewBuffer, SQL_NTS,
      (SQLTCHAR*)(const char*)tableTypeBuffer, tableTypeBufferLength);

  if (nRet != SQL_SUCCESS)
  {
    InterpretErrorCodes( nRet );
    m_pInterface->GetSQLFreeStmt()(pStatement, SQL_CLOSE);
    ThrowDatabaseException();
    return false;
  }

  nRet = m_pInterface->GetSQLFetch()(pStatement);
  if (nRet == SQL_SUCCESS || nRet == SQL_SUCCESS_WITH_INFO)
    bReturn = true;

  m_pInterface->GetSQLFreeStmt()(pStatement, SQL_CLOSE);
  
  return bReturn;
}

wxArrayString wxOdbcDatabase::GetTables()
{
  wxArrayString returnArray;
  SQLHSTMT pStatement = allocStmth();
  wxString tableType = _("TABLE");
  wxCharBuffer tableTypeBuffer = ConvertToUnicodeStream(tableType);
  int tableTypeBufferLength = GetEncodedStreamLength(tableType);
  SQLRETURN nRet = m_pInterface->GetSQLTables()(pStatement,
      NULL, 0,
      NULL, 0,
      NULL, 0,
      (SQLTCHAR*)(const char*)tableTypeBuffer, tableTypeBufferLength);

  if (nRet != SQL_SUCCESS)
  {
    InterpretErrorCodes( nRet );
    m_pInterface->GetSQLFreeStmt()(pStatement, SQL_CLOSE);
    ThrowDatabaseException();
    return returnArray;
  }

  nRet = m_pInterface->GetSQLFetch()(pStatement);
  while (nRet == SQL_SUCCESS || nRet == SQL_SUCCESS_WITH_INFO)
  {
    SQLTCHAR buff[8192];

    memset(buff, 0, 8192*sizeof(SQLTCHAR));

    SQLINTEGER  col_size         = 8192;
    SQLINTEGER  real_size        = 0;
    int nField = 3;

    SQLRETURN nGetDataReturn = m_pInterface->GetSQLGetData()(pStatement, nField, SQL_C_CHAR, buff,
      col_size, &real_size);
    if ( nGetDataReturn != SQL_SUCCESS && nGetDataReturn != SQL_SUCCESS_WITH_INFO )
    {
      InterpretErrorCodes(nRet);
      m_pInterface->GetSQLFreeStmt()(pStatement, SQL_CLOSE);
      ThrowDatabaseException();
      return returnArray;
    }
    wxString strTable = ConvertFromUnicodeStream((const char*)buff);
    returnArray.Add(strTable);
    nRet = m_pInterface->GetSQLFetch()(pStatement);
  }

  m_pInterface->GetSQLFreeStmt()(pStatement, SQL_CLOSE);
  
  return returnArray;
}

wxArrayString wxOdbcDatabase::GetViews()
{
  wxArrayString returnArray;
  SQLHSTMT pStatement = allocStmth();
  wxString tableType = _("VIEW");
  wxCharBuffer tableTypeBuffer = ConvertToUnicodeStream(tableType);
  int tableTypeBufferLength = GetEncodedStreamLength(tableType);
  SQLRETURN nRet = m_pInterface->GetSQLTables()(pStatement,
      NULL, 0,
      NULL, 0,
      NULL, 0,
      (SQLTCHAR*)(const char*)tableTypeBuffer, tableTypeBufferLength);

  if (nRet != SQL_SUCCESS)
  {
    InterpretErrorCodes( nRet );
    m_pInterface->GetSQLFreeStmt()(pStatement, SQL_CLOSE);
    ThrowDatabaseException();
    return returnArray;
  }

  nRet = m_pInterface->GetSQLFetch()(pStatement);
  while (nRet == SQL_SUCCESS || nRet == SQL_SUCCESS_WITH_INFO)
  {
    SQLTCHAR buff[8192];

    memset(buff, 0, 8192*sizeof(SQLTCHAR));

    SQLINTEGER  col_size         = 8192;
    SQLINTEGER  real_size        = 0;
    int nField = 3;

    SQLRETURN nGetDataReturn = m_pInterface->GetSQLGetData()( pStatement, nField, SQL_C_CHAR, buff,
      col_size, &real_size );
    if ( nGetDataReturn != SQL_SUCCESS && nGetDataReturn != SQL_SUCCESS_WITH_INFO )
    {
      InterpretErrorCodes(nRet);
      m_pInterface->GetSQLFreeStmt()(pStatement, SQL_CLOSE);
      ThrowDatabaseException();
      return returnArray;
    }
    wxString strView = ConvertFromUnicodeStream((const char*)buff);
    returnArray.Add(strView);
    nRet = m_pInterface->GetSQLFetch()(pStatement);
  }

  m_pInterface->GetSQLFreeStmt()(pStatement, SQL_CLOSE);
  
  return returnArray;
}

wxArrayString wxOdbcDatabase::GetColumns(const wxString& table)
{
  wxArrayString returnArray;
  // Use SQLColumns
  SQLHSTMT pStatement = allocStmth();
  wxCharBuffer tableBuffer = ConvertToUnicodeStream(table);
  int tableBufferLength = GetEncodedStreamLength(table);
  SQLRETURN nRet = m_pInterface->GetSQLColumns()(pStatement,
      NULL, 0,
      NULL, 0,
      (SQLTCHAR*)(const char*)tableBuffer, tableBufferLength,
      NULL, 0);

  if (nRet != SQL_SUCCESS)
  {
    InterpretErrorCodes( nRet );
    m_pInterface->GetSQLFreeStmt()(pStatement, SQL_CLOSE);
    ThrowDatabaseException();
    return returnArray;
  }

  nRet = m_pInterface->GetSQLFetch()(pStatement);
  while (nRet == SQL_SUCCESS || nRet == SQL_SUCCESS_WITH_INFO)
  {
    SQLPOINTER buff[8192];

    memset(buff, 0, 8192*sizeof(SQLTCHAR));

    SQLINTEGER  col_size         = 8192;
    SQLINTEGER  real_size        = 0;
    int nField = 4;

    SQLRETURN nGetDataReturn = m_pInterface->GetSQLGetData()( pStatement, nField, SQL_C_CHAR, buff,
      col_size, &real_size );
    if ( nGetDataReturn != SQL_SUCCESS && nGetDataReturn != SQL_SUCCESS_WITH_INFO )
    {
      InterpretErrorCodes(nRet);
      m_pInterface->GetSQLFreeStmt()(pStatement, SQL_CLOSE);
      ThrowDatabaseException();
      return returnArray;
    }
    wxString strColumn = ConvertFromUnicodeStream((const char*)buff);
    returnArray.Add(strColumn);
    nRet = m_pInterface->GetSQLFetch()(pStatement);
  }

  m_pInterface->GetSQLFreeStmt()(pStatement, SQL_CLOSE);

  return returnArray;
}

//void OdbcDatabaseLayer::InterpretErrorCodes( long nCode, SQLHSTMT stmth_ptr )
void wxOdbcDatabase::InterpretErrorCodes( long nCode, void* stmth_ptr )
{
  wxLogDebug(_("OdbcDatabaseLayer::InterpretErrorCodes()\n"));

  //if ((nCode != SQL_SUCCESS) ) // && (nCode != SQL_SUCCESS_WITH_INFO))
  {
    SQLINTEGER iNativeCode;
    SQLTCHAR strState[ERR_STATE_LEN];
    SQLTCHAR strBuffer[ERR_BUFFER_LEN];
    SQLSMALLINT iMsgLen;

    memset(strState, 0, ERR_STATE_LEN*sizeof(SQLTCHAR));
    memset(strBuffer, 0, ERR_BUFFER_LEN*sizeof(SQLTCHAR));

    if (stmth_ptr)
      m_pInterface->GetSQLGetDiagRec()(SQL_HANDLE_STMT, (SQLHSTMT)stmth_ptr, 1, strState, &iNativeCode, 
        strBuffer, ERR_BUFFER_LEN, &iMsgLen);  
    else
      m_pInterface->GetSQLGetDiagRec()(SQL_HANDLE_DBC, (SQLHDBC)m_sqlHDBC, 1, strState, &iNativeCode,
        strBuffer, ERR_BUFFER_LEN, &iMsgLen);  
 
    SetErrorCode((int)iNativeCode);
    //SetErrorMessage(ConvertFromUnicodeStream((char*)strBuffer));
    SetErrorMessage(wxString((wxChar*)strBuffer));
  }
}

bool wxOdbcDatabase::IsAvailable()
{
  bool bAvailable = false;
  wxOdbcInterface* pInterface = new wxOdbcInterface();
  bAvailable = pInterface && pInterface->Init();
  wxDELETE(pInterface);
  return bAvailable;
}

