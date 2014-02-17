#include "wx/database/wxprec.h"

#if wxUSE_DATABASE_SQLITE

// ctor
wxSqlitePreparedStatement::wxSqlitePreparedStatement(sqlite3* pDatabase)
 : wxPreparedStatement()
{
  m_pDatabase = pDatabase;
}

wxSqlitePreparedStatement::wxSqlitePreparedStatement(sqlite3* pDatabase, sqlite3_stmt* pStatement)
 : wxPreparedStatement()
{
  m_pDatabase = pDatabase;
  m_Statements.push_back(pStatement);
}

wxSqlitePreparedStatement::wxSqlitePreparedStatement(sqlite3* pDatabase, wxSqliteStatementVector statements)
 : wxPreparedStatement()
{
  m_pDatabase = pDatabase;
  m_Statements = statements;
}

// dtor
wxSqlitePreparedStatement::~wxSqlitePreparedStatement()
{
  Close();
}

void wxSqlitePreparedStatement::Close()
{
  CloseResultSets();

  wxSqliteStatementVector::iterator start = m_Statements.begin();
  wxSqliteStatementVector::iterator stop = m_Statements.end();
  while (start != stop)
  {
    if ((*start) != NULL)
    {
      sqlite3_finalize((sqlite3_stmt*)(*start));
      (*start) = NULL;
      //wxDELETE(*start);
    }
    start++;
  }
  m_Statements.Clear();
}
/*
void SqlitePreparedStatement::AddPreparedStatement(CppSQLite3Statement* pStatement)
{
  m_Statements.push_back(pStatement);
}
*/
void wxSqlitePreparedStatement::AddPreparedStatement(sqlite3_stmt* pStatement)
{
  m_Statements.push_back(pStatement);
}

// get field
void wxSqlitePreparedStatement::SetParamInt(int nPosition, int nValue)
{
  ResetErrorCodes();

  int nIndex = FindStatementAndAdjustPositionIndex(&nPosition);
  if (nIndex > -1)
  {
    sqlite3_reset(m_Statements[nIndex]);
    int nReturn = sqlite3_bind_int(m_Statements[nIndex], nPosition, nValue);
    if (nReturn != SQLITE_OK)
    {
      SetErrorCode(wxSqliteDatabase::TranslateErrorCode(nReturn));
      SetErrorMessage(ConvertFromUnicodeStream(sqlite3_errmsg(m_pDatabase)));
      ThrowDatabaseException();
    }
  }
}

void wxSqlitePreparedStatement::SetParamDouble(int nPosition, double dblValue)
{
  ResetErrorCodes();

  int nIndex = FindStatementAndAdjustPositionIndex(&nPosition);
  if (nIndex > -1)
  {
    sqlite3_reset(m_Statements[nIndex]);
    int nReturn = sqlite3_bind_double(m_Statements[nIndex], nPosition, dblValue);
    if (nReturn != SQLITE_OK)
    {
      SetErrorCode(wxSqliteDatabase::TranslateErrorCode(nReturn));
      SetErrorMessage(ConvertFromUnicodeStream(sqlite3_errmsg(m_pDatabase)));
      ThrowDatabaseException();
    }
  }
}

void wxSqlitePreparedStatement::SetParamString(int nPosition, const wxString& strValue)
{
  ResetErrorCodes();

  int nIndex = FindStatementAndAdjustPositionIndex(&nPosition);
  if (nIndex > -1)
  {
    sqlite3_reset(m_Statements[nIndex]);
    wxCharBuffer valueBuffer = ConvertToUnicodeStream(strValue);
    int nReturn = sqlite3_bind_text(m_Statements[nIndex], nPosition, valueBuffer, -1, SQLITE_TRANSIENT);
    if (nReturn != SQLITE_OK)
    {
      SetErrorCode(wxSqliteDatabase::TranslateErrorCode(nReturn));
      SetErrorMessage(ConvertFromUnicodeStream(sqlite3_errmsg(m_pDatabase)));
      ThrowDatabaseException();
    }
  }
}

void wxSqlitePreparedStatement::SetParamNull(int nPosition)
{
  ResetErrorCodes();

  int nIndex = FindStatementAndAdjustPositionIndex(&nPosition);
  if (nIndex > -1)
  {
    sqlite3_reset(m_Statements[nIndex]);
    int nReturn = sqlite3_bind_null(m_Statements[nIndex], nPosition);
    if (nReturn != SQLITE_OK)
    {
      SetErrorCode(wxSqliteDatabase::TranslateErrorCode(nReturn));
      SetErrorMessage(ConvertFromUnicodeStream(sqlite3_errmsg(m_pDatabase)));
      ThrowDatabaseException();
    }
  }
}

void wxSqlitePreparedStatement::SetParamBlob(int nPosition, const void* pData, long nDataLength)
{
  ResetErrorCodes();

  int nIndex = FindStatementAndAdjustPositionIndex(&nPosition);
  if (nIndex > -1)
  {
    sqlite3_reset(m_Statements[nIndex]);
    int nReturn = sqlite3_bind_blob(m_Statements[nIndex], nPosition, (const void*)pData, nDataLength, SQLITE_STATIC);
    if (nReturn != SQLITE_OK)
    {
      SetErrorCode(wxSqliteDatabase::TranslateErrorCode(nReturn));
      SetErrorMessage(ConvertFromUnicodeStream(sqlite3_errmsg(m_pDatabase)));
      ThrowDatabaseException();
    }
  }
}

void wxSqlitePreparedStatement::SetParamDate(int nPosition, const wxDateTime& dateValue)
{
  ResetErrorCodes();

  if (dateValue.IsValid())
  {
    int nIndex = FindStatementAndAdjustPositionIndex(&nPosition);
    if (nIndex > -1)
    {
      sqlite3_reset(m_Statements[nIndex]);
      wxCharBuffer valueBuffer = ConvertToUnicodeStream(dateValue.Format(_("%Y-%m-%d %H:%M:%S")));
      int nReturn = sqlite3_bind_text(m_Statements[nIndex], nPosition, valueBuffer, -1, SQLITE_TRANSIENT);
      if (nReturn != SQLITE_OK)
      {
        SetErrorCode(wxSqliteDatabase::TranslateErrorCode(nReturn));
        SetErrorMessage(ConvertFromUnicodeStream(sqlite3_errmsg(m_pDatabase)));
        ThrowDatabaseException();
      }
    }
  }
  else
  {
    int nIndex = FindStatementAndAdjustPositionIndex(&nPosition);
    if (nIndex > -1)
    {
      sqlite3_reset(m_Statements[nIndex]);
      int nReturn = sqlite3_bind_null(m_Statements[nIndex], nPosition);
      if (nReturn != SQLITE_OK)
      {
        SetErrorCode(wxSqliteDatabase::TranslateErrorCode(nReturn));
        SetErrorMessage(ConvertFromUnicodeStream(sqlite3_errmsg(m_pDatabase)));
        ThrowDatabaseException();
      }
    }
  }
}

void wxSqlitePreparedStatement::SetParamBool(int nPosition, bool bValue)
{
  ResetErrorCodes();

  int nIndex = FindStatementAndAdjustPositionIndex(&nPosition);
  if (nIndex > -1)
  {
    sqlite3_reset(m_Statements[nIndex]);
    int nReturn = sqlite3_bind_int(m_Statements[nIndex], nPosition, (bValue ? 1 : 0));
    if (nReturn != SQLITE_OK)
    {
      SetErrorCode(wxSqliteDatabase::TranslateErrorCode(nReturn));
      SetErrorMessage(ConvertFromUnicodeStream(sqlite3_errmsg(m_pDatabase)));
      ThrowDatabaseException();
    }
  }
}

int wxSqlitePreparedStatement::GetParameterCount()
{
  ResetErrorCodes();

  int nReturn = 0;
  wxSqliteStatementVector::iterator start = m_Statements.begin();
  wxSqliteStatementVector::iterator stop = m_Statements.end();
  while (start != stop)
  {
    nReturn += sqlite3_bind_parameter_count((sqlite3_stmt*)(*start));
    start++;
  }
  return nReturn;
}

int wxSqlitePreparedStatement::RunQuery()
{
  ResetErrorCodes();

  wxSqliteStatementVector::iterator start = m_Statements.begin();
  wxSqliteStatementVector::iterator stop = m_Statements.end();
  while (start != stop)
  {
    int nReturn = sqlite3_step((sqlite3_stmt*)(*start));
 
    if (nReturn != SQLITE_ROW)
      sqlite3_reset((sqlite3_stmt*)(*start));

    if ((nReturn != SQLITE_ROW) && (nReturn != SQLITE_DONE))
    {
      SetErrorCode(wxSqliteDatabase::TranslateErrorCode(nReturn));
      SetErrorMessage(ConvertFromUnicodeStream(sqlite3_errmsg(m_pDatabase)));
      ThrowDatabaseException();
      return wxDATABASE_QUERY_RESULT_ERROR;
    }
    start++;
  }

  return sqlite3_changes(m_pDatabase);
}

wxDatabaseResultSet* wxSqlitePreparedStatement::RunQueryWithResults()
{
  ResetErrorCodes();

  if (m_Statements.size() > 1)
  {
    for (unsigned int i=0; i<m_Statements.size()-1; i++)
    {
      int nReturn = sqlite3_step(m_Statements[i]);
 
      if (nReturn != SQLITE_ROW)
        sqlite3_reset(m_Statements[i]);

      if ((nReturn != SQLITE_ROW) && (nReturn != SQLITE_DONE))
      {
        wxLogError(_("Error with RunQueryWithResults\n"));
        SetErrorCode(wxSqliteDatabase::TranslateErrorCode(nReturn));
        SetErrorMessage(ConvertFromUnicodeStream(sqlite3_errmsg(m_pDatabase)));
        ThrowDatabaseException();
        return NULL;
      }
    }
  }
  // Work off the assumption that only the last statement will return result
  
  wxSqliteResultSet* pResultSet = new wxSqliteResultSet(this);
  if (pResultSet)
    pResultSet->SetEncoding(GetEncoding());

  LogResultSetForCleanup(pResultSet);
  return pResultSet;
}

int wxSqlitePreparedStatement::FindStatementAndAdjustPositionIndex(int* pPosition)
{
  // Don't mess around if there's just one entry in the vector
  if (m_Statements.size() == 0)
    return 0;
    
  // Go through all the elements in the vector
  // Get the number of parameters in each statement
  // Adjust the nPosition for the the broken up statements
  for (unsigned int i=0; i<m_Statements.size(); i++)
  {
    int nParametersInThisStatement = sqlite3_bind_parameter_count(m_Statements[i]);
    if (*pPosition > nParametersInThisStatement)
    {
      *pPosition -= nParametersInThisStatement;    // Decrement the position indicator by the number of parameters in this statement
    }
    else
    {
      // We're in the correct statement, return the index
      return i;
    }
  }
  return -1;
}

#endif//wxUSE_DATABASE_SQLITE
