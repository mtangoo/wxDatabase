#include "include/postgresql/postgresql_resultset.h"
#include "include/postgresql/postgresql_database.h"
#include "include/postgresql/postgresql_preparedstatement_wrapper.h"
#include "include/errorcodes.h"

wxPostgresPreparedStatementWrapper::wxPostgresPreparedStatementWrapper(wxDynamicPostgresInterface* pInterface, PGconn* pDatabase, const wxString& strSQL, const wxString& strStatementName)
 : wxDatabaseErrorReporter()
{
  m_pInterface = pInterface;
  m_pDatabase = pDatabase;
  m_strSQL = strSQL;
  m_strStatementName = strStatementName;
}

wxPostgresPreparedStatementWrapper::~wxPostgresPreparedStatementWrapper()
{
}

// set field
void wxPostgresPreparedStatementWrapper::SetParam(int nPosition, int nValue)
{
  m_Parameters.SetParam(nPosition, nValue);
}

void wxPostgresPreparedStatementWrapper::SetParam(int nPosition, double dblValue)
{
  m_Parameters.SetParam(nPosition, dblValue);
}

void wxPostgresPreparedStatementWrapper::SetParam(int nPosition, const wxString& strValue)
{
  m_Parameters.SetParam(nPosition, strValue);
}

void wxPostgresPreparedStatementWrapper::SetParam(int nPosition)
{
  m_Parameters.SetParam(nPosition);
}

void wxPostgresPreparedStatementWrapper::SetParam(int nPosition, const void* pData, long nDataLength)
{
  m_Parameters.SetParam(nPosition, pData, nDataLength);
}

void wxPostgresPreparedStatementWrapper::SetParam(int nPosition, const wxDateTime& dateValue)
{
  m_Parameters.SetParam(nPosition, dateValue);
}

void wxPostgresPreparedStatementWrapper::SetParam(int nPosition, bool bValue)
{
  m_Parameters.SetParam(nPosition, bValue);
}

int wxPostgresPreparedStatementWrapper::GetParameterCount()
{
  int nParameterCount = 0;
  bool bInStringLiteral = false;
  size_t len = m_strSQL.length();
  for (size_t i = 0; i < len; i++)
  {
    wxChar character = m_strSQL[i];
    if ('\'' == character)
    {
      // Signify that we are inside a string literal inside the SQL
      bInStringLiteral = !bInStringLiteral;
    }
    else if (('?' == character) && !bInStringLiteral)
    {
      nParameterCount++;
    }
  }
  return nParameterCount;
}

int wxPostgresPreparedStatementWrapper::RunQuery()
{
  long nRows = -1;
  int nParameters = m_Parameters.GetSize();
  char** paramValues = m_Parameters.GetParamValues();
  int* paramLengths = m_Parameters.GetParamLengths();
  int* paramFormats = m_Parameters.GetParamFormats();
  int nResultFormat = 0; // 0 = text, 1 = binary (all or none on the result set, not column based)
  wxCharBuffer statementNameBuffer = ConvertToUnicodeStream(m_strStatementName);
  PGresult* pResult = m_pInterface->GetPQexecPrepared()(m_pDatabase, statementNameBuffer, nParameters, paramValues, paramLengths, paramFormats, nResultFormat);
  if (pResult != NULL)
  {
    ExecStatusType status = m_pInterface->GetPQresultStatus()(pResult);
    if ((status != PGRES_COMMAND_OK) && (status != PGRES_TUPLES_OK))
    {
      SetErrorCode(wxPostgresDatabase::TranslateErrorCode(status));
      SetErrorMessage(ConvertFromUnicodeStream(m_pInterface->GetPQresultErrorMessage()(pResult)));
    }

    if (GetErrorCode() == wxDATABASE_OK)
    {
      wxString rowsAffected = ConvertFromUnicodeStream(m_pInterface->GetPQcmdTuples()(pResult));
      rowsAffected.ToLong(&nRows);
    }
    m_pInterface->GetPQclear()(pResult);
  }
  delete []paramValues;
  delete []paramLengths;
  delete []paramFormats;

  if (GetErrorCode() != wxDATABASE_OK)
  {
    ThrowDatabaseException();
    return wxDATABASE_QUERY_RESULT_ERROR;
  }

  return (int)nRows;
}

wxDatabaseResultSet* wxPostgresPreparedStatementWrapper::RunQueryWithResults()
{
  int nParameters = m_Parameters.GetSize();
  char** paramValues = m_Parameters.GetParamValues();
  int* paramLengths = m_Parameters.GetParamLengths();
  int* paramFormats = m_Parameters.GetParamFormats();
  int nResultFormat = 0; // 0 = text, 1 = binary (all or none on the result set, not column based)
  wxCharBuffer statementNameBuffer = ConvertToUnicodeStream(m_strStatementName);
  PGresult* pResult = m_pInterface->GetPQexecPrepared()(m_pDatabase, statementNameBuffer, nParameters, paramValues, paramLengths, paramFormats, nResultFormat);
  if (pResult != NULL)
  {
    ExecStatusType status = m_pInterface->GetPQresultStatus()(pResult);
    if ((status != PGRES_COMMAND_OK) && (status != PGRES_TUPLES_OK))
    {
      SetErrorCode(wxPostgresDatabase::TranslateErrorCode(status));
      SetErrorMessage(ConvertFromUnicodeStream(m_pInterface->GetPQresultErrorMessage()(pResult)));
    }
    else
    {
      delete []paramValues;
      delete []paramLengths;
      delete []paramFormats;

      wxPostgresResultSet* pResultSet = new wxPostgresResultSet(m_pInterface, pResult);
      pResultSet->SetEncoding(GetEncoding());
      return pResultSet;
    }
    m_pInterface->GetPQclear()(pResult);
  }
  delete []paramValues;
  delete []paramLengths;
  delete []paramFormats;

  ThrowDatabaseException();

  return NULL;
}


