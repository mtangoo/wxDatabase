#include "wx/database/wxprec.h"

#if wxUSE_DATABASE_MYSQL

wxMysqlPreparedStatementWrapper::wxMysqlPreparedStatementWrapper(wxMysqlDynamicInterface* pInterface, MYSQL_STMT* pStatement)
 : wxDatabaseErrorReporter()
{
  m_pInterface = pInterface;
  m_pStatement = pStatement;
}

wxMysqlPreparedStatementWrapper::~wxMysqlPreparedStatementWrapper()
{
  Close();
}

void wxMysqlPreparedStatementWrapper::Close()
{
  if (m_pStatement != NULL)
  {
    m_pInterface->GetMysqlStmtClose()(m_pStatement);
    m_pStatement = NULL;
  }
}

// set field
void wxMysqlPreparedStatementWrapper::SetParam(int nPosition, int nValue)
{
  m_Parameters.SetParam(nPosition, nValue);
}

void wxMysqlPreparedStatementWrapper::SetParam(int nPosition, double dblValue)
{
  m_Parameters.SetParam(nPosition, dblValue);
}

void wxMysqlPreparedStatementWrapper::SetParam(int nPosition, const wxString& strValue)
{
  m_Parameters.SetParam(nPosition, strValue);
}

void wxMysqlPreparedStatementWrapper::SetParam(int nPosition)
{
  m_Parameters.SetParam(nPosition);
}

void wxMysqlPreparedStatementWrapper::SetParam(int nPosition, const void* pData, long nDataLength)
{
  m_Parameters.SetParam(nPosition, pData, nDataLength);
}

void wxMysqlPreparedStatementWrapper::SetParam(int nPosition, const wxDateTime& dateValue)
{
  m_Parameters.SetParam(nPosition, dateValue);
}

void wxMysqlPreparedStatementWrapper::SetParam(int nPosition, bool bValue)
{
  m_Parameters.SetParam(nPosition, bValue);
}

int wxMysqlPreparedStatementWrapper::GetParameterCount()
{
  return m_pInterface->GetMysqlStmtParamCount()(m_pStatement);
}

int wxMysqlPreparedStatementWrapper::RunQuery()
{
  MYSQL_BIND* pBoundParameters = m_Parameters.GetMysqlParameterBindings();

  int nBindReturn = m_pInterface->GetMysqlStmtBindParam()(m_pStatement, pBoundParameters);
  if (nBindReturn != 0)
  {
    SetErrorCode(wxMysqlDatabase::TranslateErrorCode(m_pInterface->GetMysqlStmtErrno()(m_pStatement)));
    SetErrorMessage(ConvertFromUnicodeStream(m_pInterface->GetMysqlStmtError()(m_pStatement)));
    wxDELETEA(pBoundParameters);
    ThrowDatabaseException();
    return wxDATABASE_QUERY_RESULT_ERROR;
  }
  else
  {
    int nReturn = m_pInterface->GetMysqlStmtExecute()(m_pStatement);
    if (nReturn != 0)
    {
      SetErrorCode(wxMysqlDatabase::TranslateErrorCode(m_pInterface->GetMysqlStmtErrno()(m_pStatement)));
      SetErrorMessage(ConvertFromUnicodeStream(m_pInterface->GetMysqlStmtError()(m_pStatement)));
      wxDELETEA(pBoundParameters);
      ThrowDatabaseException();
      return wxDATABASE_QUERY_RESULT_ERROR;
    }
  }
  wxDELETEA(pBoundParameters);

  return m_pInterface->GetMysqlStmtAffectedRows()(m_pStatement);
}

wxDatabaseResultSet* wxMysqlPreparedStatementWrapper::RunQueryWithResults()
{
  wxMysqlPreparedStatementResultSet* pResultSet = NULL;
  MYSQL_BIND* pBoundParameters = m_Parameters.GetMysqlParameterBindings();

  if (m_pInterface->GetMysqlStmtBindParam()(m_pStatement, pBoundParameters))
  {
    SetErrorCode(wxMysqlDatabase::TranslateErrorCode(m_pInterface->GetMysqlStmtErrno()(m_pStatement)));
    SetErrorMessage(ConvertFromUnicodeStream(m_pInterface->GetMysqlStmtError()(m_pStatement)));
    wxDELETEA(pBoundParameters);
    ThrowDatabaseException();
    return NULL;
  }
  else
  {
    if (m_pInterface->GetMysqlStmtExecute()(m_pStatement) != 0)
    {
      SetErrorCode(wxMysqlDatabase::TranslateErrorCode(m_pInterface->GetMysqlStmtErrno()(m_pStatement)));
      SetErrorMessage(ConvertFromUnicodeStream(m_pInterface->GetMysqlStmtError()(m_pStatement)));
      wxDELETEA(pBoundParameters);
      ThrowDatabaseException();
      return NULL;
    }
    else
    {
      pResultSet = new wxMysqlPreparedStatementResultSet(m_pInterface, m_pStatement);
      if (pResultSet)
        pResultSet->SetEncoding(GetEncoding());
    }
  }
  wxDELETEA(pBoundParameters);;
  
  return pResultSet;
}

#endif//wxUSE_DATABASE_MYSQL
