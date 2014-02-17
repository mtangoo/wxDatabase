#include "wx/database/wxprec.h"

#if wxUSE_DATABASE_POSTGRESQL

#include "wx/tokenzr.h"

#include "wx/arrimpl.cpp"
WX_DEFINE_OBJARRAY(ArrayOfPostgresPreparedStatementWrappers);

wxPostgresPreparedStatement::wxPostgresPreparedStatement(wxDynamicPostgresInterface* pInterface)
 : wxPreparedStatement()
{
  m_pInterface = pInterface;
}

wxPostgresPreparedStatement::wxPostgresPreparedStatement(wxDynamicPostgresInterface* pInterface, PGconn* pDatabase, const wxString& strSQL, const wxString& strStatementName)
 : wxPreparedStatement()
{
  m_pInterface = pInterface;
  AddStatement(pDatabase, strSQL, strStatementName);
}


wxPostgresPreparedStatement::~wxPostgresPreparedStatement()
{
  Close();
}


void wxPostgresPreparedStatement::Close()
{
  CloseResultSets();
  m_Statements.Clear();
}

void wxPostgresPreparedStatement::AddStatement(PGconn* pDatabase, const wxString& strSQL, const wxString& strStatementName)
{
  wxPostgresPreparedStatementWrapper Statement(m_pInterface, pDatabase, strSQL, strStatementName);
  Statement.SetEncoding(GetEncoding());
  m_Statements.push_back(Statement);
}

wxPostgresPreparedStatement* wxPostgresPreparedStatement::CreateStatement(wxDynamicPostgresInterface* pInterface, PGconn* pDatabase, const wxString& strSQL)
{
  wxArrayString Queries = ParseQueries(strSQL);

  wxArrayString::iterator start = Queries.begin();
  wxArrayString::iterator stop = Queries.end();

  wxPostgresPreparedStatement* pStatement = new wxPostgresPreparedStatement(pInterface);
  const char* strEncoding = pInterface->GetPQencodingToChar()(pInterface->GetPQclientEncoding()(pDatabase));
  wxCSConv conv((const wxChar*)strEncoding);
  pStatement->SetEncoding(&conv);
  while (start != stop)
  {
    wxString strName = wxPostgresPreparedStatement::GenerateRandomStatementName();
    pStatement->AddStatement(pDatabase, (*start), strName);
    wxCharBuffer nameBuffer = wxDatabaseStringConverter::ConvertToUnicodeStream(strName, strEncoding);
    wxCharBuffer sqlBuffer = wxDatabaseStringConverter::ConvertToUnicodeStream(TranslateSQL((*start)), strEncoding);
    PGresult* pResult = pInterface->GetPQprepare()(pDatabase, nameBuffer, sqlBuffer, 0, NULL);
    if (pResult == NULL)
    {
      delete pStatement;
      return NULL;
    }

    if (pInterface->GetPQresultStatus()(pResult) != PGRES_COMMAND_OK)
    {
      pStatement->SetErrorCode(wxPostgresDatabase::TranslateErrorCode(pInterface->GetPQresultStatus()(pResult)));
      pStatement->SetErrorMessage(wxDatabaseStringConverter::ConvertFromUnicodeStream(
        pInterface->GetPQresultErrorMessage()(pResult), strEncoding));
      pInterface->GetPQclear()(pResult);
      pStatement->ThrowDatabaseException();
      return pStatement;
    }
    pInterface->GetPQclear()(pResult);

    start++;
  }
    
  // Success?  Return the statement
  return pStatement;
}

// set field
void wxPostgresPreparedStatement::SetParamInt(int nPosition, int nValue)
{
  int nIndex = FindStatementAndAdjustPositionIndex(&nPosition);
  if (nIndex > -1)
  {
    m_Statements[nIndex].SetParam(nPosition, nValue);
  }
}

void wxPostgresPreparedStatement::SetParamDouble(int nPosition, double dblValue)
{
  int nIndex = FindStatementAndAdjustPositionIndex(&nPosition);
  if (nIndex > -1)
  {
    m_Statements[nIndex].SetParam(nPosition, dblValue);
  }
}

void wxPostgresPreparedStatement::SetParamString(int nPosition, const wxString& strValue)
{
  int nIndex = FindStatementAndAdjustPositionIndex(&nPosition);
  if (nIndex > -1)
  {
    m_Statements[nIndex].SetParam(nPosition, strValue);
  }
}

void wxPostgresPreparedStatement::SetParamNull(int nPosition)
{
  int nIndex = FindStatementAndAdjustPositionIndex(&nPosition);
  if (nIndex > -1)
  {
    m_Statements[nIndex].SetParam(nPosition);
  }
}

void wxPostgresPreparedStatement::SetParamBlob(int nPosition, const void* pData, long nDataLength)
{
  int nIndex = FindStatementAndAdjustPositionIndex(&nPosition);
  if (nIndex > -1)
  {
    m_Statements[nIndex].SetParam(nPosition, pData, nDataLength);
  }
}

void wxPostgresPreparedStatement::SetParamDate(int nPosition, const wxDateTime& dateValue)
{
  int nIndex = FindStatementAndAdjustPositionIndex(&nPosition);
  if (nIndex > -1)
  {
    m_Statements[nIndex].SetParam(nPosition, dateValue);
  }
}

void wxPostgresPreparedStatement::SetParamBool(int nPosition, bool bValue)
{
  int nIndex = FindStatementAndAdjustPositionIndex(&nPosition);
  if (nIndex > -1)
  {
    m_Statements[nIndex].SetParam(nPosition, bValue);
  }
}

int wxPostgresPreparedStatement::GetParameterCount()
{
  int nParameters = 0;

  for (unsigned int i=0; i<(m_Statements.size()); i++)
  {
    nParameters += m_Statements[i].GetParameterCount();
  }
  return nParameters;
}

  
int wxPostgresPreparedStatement::RunQuery()
{
  // Iterate through the statements and have them run their queries
  int nRows = wxDATABASE_QUERY_RESULT_ERROR;
  for (unsigned int i=0; i<(m_Statements.size()); i++)
  {
    nRows = m_Statements[i].RunQuery();
    if (m_Statements[i].GetErrorCode() != wxDATABASE_OK)
    {
      SetErrorCode(m_Statements[i].GetErrorCode());
      SetErrorMessage(m_Statements[i].GetErrorMessage());
      ThrowDatabaseException();
      return wxDATABASE_QUERY_RESULT_ERROR;
    }
  }
  return nRows;
}

wxDatabaseResultSet* wxPostgresPreparedStatement::RunQueryWithResults()
{
  for (unsigned int i=0; i<(m_Statements.size()-1); i++)
  {
    m_Statements[i].RunQuery();
    if (m_Statements[i].GetErrorCode() != wxDATABASE_OK)
    {
      SetErrorCode(m_Statements[i].GetErrorCode());
      SetErrorMessage(m_Statements[i].GetErrorMessage());
      ThrowDatabaseException();
      return NULL;
    }
  }
  wxPostgresPreparedStatementWrapper* pLastStatement = &(m_Statements[m_Statements.size()-1]);
  wxDatabaseResultSet* pResultSet = pLastStatement->RunQueryWithResults();
  if (pLastStatement->GetErrorCode() != wxDATABASE_OK)
  {
    SetErrorCode(pLastStatement->GetErrorCode());
    SetErrorMessage(pLastStatement->GetErrorMessage());
    ThrowDatabaseException();
  }

  LogResultSetForCleanup(pResultSet);
  return pResultSet;
}

wxString wxPostgresPreparedStatement::GenerateRandomStatementName()
{
  // Just come up with a string prefixed with "databaselayer_" and 10 random characters
  wxString strReturn = _("databaselayer_");
  for (int i=0; i<10; i++)
  {
    strReturn << (int) (10.0*rand()/(RAND_MAX+1.0));
  }
  return strReturn;
}

int wxPostgresPreparedStatement::FindStatementAndAdjustPositionIndex(int* pPosition)
{
  if (m_Statements.size() == 0)
    return 0;
    
  // Go through all the elements in the vector
  // Get the number of parameters in each statement
  // Adjust the nPosition for the the broken up statements
  for (unsigned int i=0; i<m_Statements.size(); i++)
  {
    int nParametersInThisStatement = m_Statements[i].GetParameterCount();

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

wxString wxPostgresPreparedStatement::TranslateSQL(const wxString& strOriginalSQL)
{
  int nParameterIndex = 1;
  wxString strReturn = wxEmptyString;//strOriginalSQL;
  /*
  int nFound = strReturn.Replace(_("?"), wxString::Format(_("$%d"), nParameterIndex), false);
  while (nFound != 0)
  {
    nParameterIndex++;
    nFound = strReturn.Replace(_("?"), wxString::Format(_("$%d"), nParameterIndex), false);
  }
  */
  bool bInStringLiteral = false;
  size_t len = strOriginalSQL.length();
  for (size_t i = 0; i < len; i++)
  {
    wxChar character = strOriginalSQL[i];
    if ('\'' == character)
    {
      // Signify that we are inside a string literal inside the SQL
      bInStringLiteral = !bInStringLiteral;
      // Pass the character on to the return string
      strReturn += character;
    }
    else
    {
      if ('?' == character)
      {
        if (bInStringLiteral)
        {
          // Pass the character on to the return string
          strReturn += character;
        }
        else
        {
          // Replace the question mark with a prepared statement placeholder
          strReturn += wxString::Format(_("$%d"), nParameterIndex);
          nParameterIndex++;
        }
      }
      else
      {
        // Pass the character on to the return string
        strReturn += character;
      }
    }
  }

  return strReturn;
}

#endif//wxUSE_DATABASE_POSTGRESQL
