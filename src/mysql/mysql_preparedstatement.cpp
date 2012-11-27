#include "include/mysql/mysql_preparedstatement.h" 


wxMysqlPreparedStatement::wxMysqlPreparedStatement(wxMysqlDynamicInterface* pInterface)
 : wxPreparedStatement()
{
  m_pInterface = pInterface;
  m_Statements.clear();
}

wxMysqlPreparedStatement::wxMysqlPreparedStatement(wxMysqlDynamicInterface* pInterface, MYSQL_STMT* pStatement)
 : wxPreparedStatement()
{
  m_pInterface = pInterface;
  AddPreparedStatement(pStatement);
}

wxMysqlPreparedStatement::~wxMysqlPreparedStatement()
{
  Close();
}


void wxMysqlPreparedStatement::Close()
{
  CloseResultSets();

  // Free the statements
  MysqlStatementWrapperArray::iterator start = m_Statements.begin();
  MysqlStatementWrapperArray::iterator stop = m_Statements.end();

  while (start != stop)
  {
    if ((*start) != NULL)
    {
      wxMysqlPreparedStatementWrapper* pWrapper = (wxMysqlPreparedStatementWrapper*)(*start);
      wxDELETE(pWrapper);
      (*start) = NULL;
    }
    start++;
  }
}

void wxMysqlPreparedStatement::AddPreparedStatement(MYSQL_STMT* pStatement)
{
  wxMysqlPreparedStatementWrapper* pStatementWrapper = new wxMysqlPreparedStatementWrapper(m_pInterface, pStatement);
  if (pStatementWrapper)
    pStatementWrapper->SetEncoding(GetEncoding());
  m_Statements.push_back(pStatementWrapper);
}

// get field
void wxMysqlPreparedStatement::SetParamInt(int nPosition, int nValue)
{
  int nIndex = FindStatementAndAdjustPositionIndex(&nPosition);
  if (nIndex > -1)
  {
    m_Statements[nIndex]->SetParam(nPosition, nValue);
  }
}

void wxMysqlPreparedStatement::SetParamDouble(int nPosition, double dblValue)
{
  int nIndex = FindStatementAndAdjustPositionIndex(&nPosition);
  if (nIndex > -1)
  {
    m_Statements[nIndex]->SetParam(nPosition, dblValue);
  }
}

void wxMysqlPreparedStatement::SetParamString(int nPosition, const wxString& strValue)
{
  int nIndex = FindStatementAndAdjustPositionIndex(&nPosition);
  if (nIndex > -1)
  {
    m_Statements[nIndex]->SetParam(nPosition, strValue);
  }
}

void wxMysqlPreparedStatement::SetParamNull(int nPosition)
{
  int nIndex = FindStatementAndAdjustPositionIndex(&nPosition);
  if (nIndex > -1)
  {
    m_Statements[nIndex]->SetParam(nPosition);
  }
}

void wxMysqlPreparedStatement::SetParamBlob(int nPosition, const void* pData, long nDataLength)
{
  int nIndex = FindStatementAndAdjustPositionIndex(&nPosition);
  if (nIndex > -1)
  {
    m_Statements[nIndex]->SetParam(nPosition, pData, nDataLength);
  }
}

void wxMysqlPreparedStatement::SetParamDate(int nPosition, const wxDateTime& dateValue)
{
  int nIndex = FindStatementAndAdjustPositionIndex(&nPosition);
  if (nIndex > -1)
  {
    m_Statements[nIndex]->SetParam(nPosition, dateValue);
  }
}

void wxMysqlPreparedStatement::SetParamBool(int nPosition, bool bValue)
{
  int nIndex = FindStatementAndAdjustPositionIndex(&nPosition);
  if (nIndex > -1)
  {
    m_Statements[nIndex]->SetParam(nPosition, bValue);
  }
}

int wxMysqlPreparedStatement::GetParameterCount()
{
  MysqlStatementWrapperArray::iterator start = m_Statements.begin();
  MysqlStatementWrapperArray::iterator stop = m_Statements.end();

  int nParameters = 0;
  while (start != stop)
  {
    nParameters += ((wxMysqlPreparedStatementWrapper*)(*start))->GetParameterCount();
    start++;
  }
  return nParameters;
}

int wxMysqlPreparedStatement::RunQuery()
{
  MysqlStatementWrapperArray::iterator start = m_Statements.begin();
  MysqlStatementWrapperArray::iterator stop = m_Statements.end();

  int nRows = -1;
  while (start != stop)
  {
    nRows = ((wxMysqlPreparedStatementWrapper*)(*start))->RunQuery();
    if (((wxMysqlPreparedStatementWrapper*)(*start))->GetErrorCode() != wxDATABASE_OK)
    {
      SetErrorCode(((wxMysqlPreparedStatementWrapper*)(*start))->GetErrorCode());
      SetErrorMessage(((wxMysqlPreparedStatementWrapper*)(*start))->GetErrorMessage());
      ThrowDatabaseException();
      return wxDATABASE_QUERY_RESULT_ERROR;
    }
    start++;
  }
  return nRows;
}

wxDatabaseResultSet* wxMysqlPreparedStatement::RunQueryWithResults()
{
  if (m_Statements.size() > 0)
  {
    for (unsigned int i=0; i<(m_Statements.size()-1); i++)
    {
      wxMysqlPreparedStatementWrapper* pStatement = m_Statements[i];
      pStatement->RunQuery();
      if (pStatement->GetErrorCode() != wxDATABASE_OK)
      {
        SetErrorCode(pStatement->GetErrorCode());
        SetErrorMessage(pStatement->GetErrorMessage());
        ThrowDatabaseException();
        return NULL;
      }
    }

    wxMysqlPreparedStatementWrapper* pLastStatement = m_Statements[m_Statements.size()-1];
    wxDatabaseResultSet* pResults = pLastStatement->RunQueryWithResults();
    if (pLastStatement->GetErrorCode() != wxDATABASE_OK)
    {
      SetErrorCode(pLastStatement->GetErrorCode());
      SetErrorMessage(pLastStatement->GetErrorMessage());
      ThrowDatabaseException();
    }
    LogResultSetForCleanup(pResults);
    return pResults;
  }
  else
    return NULL;
}

int wxMysqlPreparedStatement::FindStatementAndAdjustPositionIndex(int* pPosition)
{
  if (m_Statements.size() == 0)
    return 0;
    
  // Go through all the elements in the vector
  // Get the number of parameters in each statement
  // Adjust the nPosition for the the broken up statements
  for (unsigned int i=0; i<m_Statements.size(); i++)
  {
    int nParametersInThisStatement = m_Statements[i]->GetParameterCount();

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

