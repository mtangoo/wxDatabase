
#include "include/odbc/odbc_database.h"
#include "include/odbc/odbc_resultset.h"
#include "include/odbc/odbc_preparedstatement.h"
#include "include/errorcodes.h"

// ctor
wxOdbcPreparedStatement::wxOdbcPreparedStatement(wxOdbcInterface* pInterface, SQLHENV sqlEnvHandle, SQLHDBC sqlHDBC)
 : wxPreparedStatement()
{
    m_pInterface = pInterface;
    m_sqlEnvHandle = sqlEnvHandle;
    m_sqlHDBC = sqlHDBC;
    m_bOneTimeStatement = false;
}

wxOdbcPreparedStatement::wxOdbcPreparedStatement(wxOdbcInterface* pInterface, SQLHENV sqlEnvHandle, SQLHDBC sqlHDBC, SQLHSTMT sqlStatementHandle)
 : wxPreparedStatement()
{
    m_pInterface = pInterface;
    m_sqlEnvHandle = sqlEnvHandle;
    m_sqlHDBC = sqlHDBC;
    m_bOneTimeStatement = false;
    m_Statements.push_back(sqlStatementHandle);
}

wxOdbcPreparedStatement::wxOdbcPreparedStatement(wxOdbcInterface* pInterface, SQLHENV sqlEnvHandle, SQLHDBC sqlHDBC, StatementVector statements)
 : wxPreparedStatement()
{
    m_pInterface = pInterface;
    m_sqlEnvHandle = sqlEnvHandle;
    m_sqlHDBC = sqlHDBC;
    m_bOneTimeStatement = false;
    m_Statements = statements;
}

// dtor
wxOdbcPreparedStatement::~wxOdbcPreparedStatement()
{
  FreeParameters();
  Close();
}

void wxOdbcPreparedStatement::Close()
{
  CloseResultSets();

  StatementVector::iterator start = m_Statements.begin();
  StatementVector::iterator stop = m_Statements.end();
  while (start != stop)
  {
    if ((*start) != NULL)
    {
      SQLRETURN nRet = m_pInterface->GetSQLFreeHandle()(SQL_HANDLE_STMT, (SQLHSTMT)*start);
      if ( nRet != SQL_SUCCESS && nRet != SQL_SUCCESS_WITH_INFO )
      {
        InterpretErrorCodes(nRet, (SQLHSTMT)*start);
        ThrowDatabaseException();
        return;
      }
      (*start) = NULL;
    }
    start++;
  }
  m_Statements.Clear();
}

void wxOdbcPreparedStatement::FreeParameters()
{
  ArrayOfOdbcParameters::iterator start = m_Parameters.begin();
  ArrayOfOdbcParameters::iterator stop = m_Parameters.end();

  while (start != stop)
  {
    if ((*start) != NULL)
    {
      wxOdbcParameter* pParameter = *start;
      delete(pParameter);
      (*start) = NULL;
    }
    start++;
  }
  m_Parameters.Clear();
}

void wxOdbcPreparedStatement::AddPreparedStatement(SQLHSTMT sqlStatementHandle)
{
  m_Statements.push_back(sqlStatementHandle);
}

// get field
void wxOdbcPreparedStatement::SetParamInt(int nPosition, int nValue)
{
    ResetErrorCodes();

    wxOdbcParameter* pParameter = new wxOdbcParameter(nValue);
    SetParam(nPosition, pParameter);
}

void wxOdbcPreparedStatement::SetParamDouble(int nPosition, double dblValue)
{
    ResetErrorCodes();

    wxOdbcParameter* pParameter = new wxOdbcParameter(dblValue);
    SetParam(nPosition, pParameter);
}

void wxOdbcPreparedStatement::SetParamString(int nPosition, const wxString& strValue)
{
    ResetErrorCodes();

    wxOdbcParameter* pParameter = new wxOdbcParameter(strValue);
    SetParam(nPosition, pParameter);
}

void wxOdbcPreparedStatement::SetParamNull(int nPosition)
{
    ResetErrorCodes();

    wxOdbcParameter* pParameter = new wxOdbcParameter();
    SetParam(nPosition, pParameter);
}

void wxOdbcPreparedStatement::SetParamBlob(int nPosition, const void* pData, long nDataLength)
{
    ResetErrorCodes();

    wxOdbcParameter* pParameter = new wxOdbcParameter(pData, nDataLength);
    SetParam(nPosition, pParameter);
}

void wxOdbcPreparedStatement::SetParamDate(int nPosition, const wxDateTime& dateValue)
{
    ResetErrorCodes();

    wxOdbcParameter* pParameter = new wxOdbcParameter(dateValue);
    SetParam(nPosition, pParameter);
}

void wxOdbcPreparedStatement::SetParamBool(int nPosition, bool bValue)
{
    ResetErrorCodes();

    wxOdbcParameter* pParameter = new wxOdbcParameter(bValue);
    SetParam(nPosition, pParameter);
}

int wxOdbcPreparedStatement::GetParameterCount()
{
    ResetErrorCodes();

    int nReturn = 0;
    StatementVector::iterator start = m_Statements.begin();
    StatementVector::iterator stop = m_Statements.end();
    while (start != stop)
    {
        SQLSMALLINT num = 0;
        SQLRETURN nRet = m_pInterface->GetSQLNumParams()(((SQLHSTMT)(*start)), &num);
        if ( nRet != SQL_SUCCESS && nRet != SQL_SUCCESS_WITH_INFO )
        {
            InterpretErrorCodes(nRet, (SQLHSTMT)(*start));
            ThrowDatabaseException();
            return -1;
        }
        nReturn += num;
        start++;
    }
    return nReturn;
}

int wxOdbcPreparedStatement::RunQuery()
{
    ResetErrorCodes();

    BindParameters();

    SQLINTEGER nRows = wxDATABASE_QUERY_RESULT_ERROR;
    StatementVector::iterator start = m_Statements.begin();
    StatementVector::iterator stop = m_Statements.end();
    while (start != stop)
    {
        SQLRETURN nRet = 0;
        // Free any previous statement handles
        nRet = m_pInterface->GetSQLFreeStmt()((SQLHSTMT)(*start), SQL_CLOSE);
        if (nRet != SQL_SUCCESS && nRet != SQL_SUCCESS_WITH_INFO)
        {
            InterpretErrorCodes(nRet, (SQLHSTMT)(*start));
            ThrowDatabaseException();
            return wxDATABASE_QUERY_RESULT_ERROR;
        }

        // Execute the current statement
        nRet = m_pInterface->GetSQLExecute()((SQLHSTMT)(*start));
        if ( nRet != SQL_SUCCESS && nRet != SQL_SUCCESS_WITH_INFO && nRet != SQL_NO_DATA && nRet != SQL_NEED_DATA)
        {
            InterpretErrorCodes(nRet, (SQLHSTMT)(*start));
            ThrowDatabaseException();
            return wxDATABASE_QUERY_RESULT_ERROR;
        }

		    if ( nRet == SQL_NEED_DATA)
        {
            PTR pParmID;
            nRet = m_pInterface->GetSQLParamData()((SQLHSTMT)(*start), &pParmID);
            while (nRet == SQL_NEED_DATA)
            {
                // Find the parameter
                for (unsigned int i = 0; i < m_Parameters.size(); i++)
                {
                    wxOdbcParameter* pParameter = m_Parameters[i];
                    if (pParmID == pParameter->GetDataPtr())
                    {
                        // We found it.  Store the parameter.
                        nRet = m_pInterface->GetSQLPutData()((SQLHSTMT)(*start), pParmID, pParameter->GetDataLength());
                        if (nRet != SQL_SUCCESS)
                        {
                            InterpretErrorCodes(nRet, (SQLHSTMT)(*start));
                            ThrowDatabaseException();
                            return wxDATABASE_QUERY_RESULT_ERROR;
                        }
                        break;
                     }
                }
                 nRet = m_pInterface->GetSQLParamData()((SQLHSTMT)(*start), &pParmID);
             }

             // Check the last return code
             if (nRet != SQL_SUCCESS && nRet != SQL_NO_DATA_FOUND && nRet != SQL_SUCCESS_WITH_INFO)
             {
                 InterpretErrorCodes(nRet, (SQLHSTMT)(*start));
                 ThrowDatabaseException();
                 return wxDATABASE_QUERY_RESULT_ERROR;
             }
         }

        // Get the affected record count
        if (SQL_SUCCESS != m_pInterface->GetSQLRowCount()((SQLHSTMT)(*start), &nRows))
        {
          nRows = wxDATABASE_QUERY_RESULT_ERROR;
        }

        // Move to the next query
        start++;
    }
    return nRows;
}

wxDatabaseResultSet* wxOdbcPreparedStatement::RunQueryWithResults()
{
  return RunQueryWithResults(true);
}

wxDatabaseResultSet* wxOdbcPreparedStatement::RunQueryWithResults(bool bLogForCleanup)
{
    ResetErrorCodes();
    SQLSMALLINT ncol = 0;

    if (m_Statements.size() > 0)
    {
        BindParameters();

        for (unsigned int i=0; i<m_Statements.size(); i++)
        {
            SQLRETURN nRet = 0;
            nRet = m_pInterface->GetSQLFreeStmt()(m_Statements[i], SQL_CLOSE);
            if ( nRet != SQL_SUCCESS && nRet != SQL_SUCCESS_WITH_INFO )
            {
                InterpretErrorCodes(nRet, m_Statements[i]);
                ThrowDatabaseException();
                return NULL;
            }

            nRet = m_pInterface->GetSQLExecute()(m_Statements[i]);
            if ( nRet != SQL_SUCCESS && nRet != SQL_SUCCESS_WITH_INFO )
            {
                InterpretErrorCodes(nRet, m_Statements[i]);
                ThrowDatabaseException();
                return NULL;
            }

            nRet = m_pInterface->GetSQLNumResultCols()(m_Statements[i], &ncol);
            if ( nRet != SQL_SUCCESS && nRet != SQL_SUCCESS_WITH_INFO )
            {
                InterpretErrorCodes(nRet, m_Statements[i]);
                ThrowDatabaseException();
                return NULL;
            }
        }
    }

    // Work off the assumption that only the last statement will return result
    wxDatabaseResultSet* pResultSet = new wxOdbcResultSet(m_pInterface, this, m_bOneTimeStatement, (int)ncol);
    if (bLogForCleanup)
      LogResultSetForCleanup(pResultSet);
    return pResultSet;
}

void wxOdbcPreparedStatement::BindParameters()
{
  // Iterate through all of the parameters and bind them to the prepared statement
  for (unsigned int i = 1; i <= m_Parameters.size(); i++)
  {
    int nPosition = i;
    wxOdbcParameter* pParameter = m_Parameters[i-1];
    int nIndex = FindStatementAndAdjustPositionIndex(&nPosition);

    if ((nIndex > -1) && (pParameter != NULL))
    {
      // Determine the data type (if supported by the ODBC driver)
      SQLSMALLINT dataType;
      SQLULEN dataSize;
      SQLSMALLINT dataDigits;
      SQLSMALLINT isNullable;
      SQLRETURN ret = m_pInterface->GetSQLDescribeParam()(m_Statements[nIndex], nPosition, &dataType, &dataSize, 
        &dataDigits, &isNullable);
      if ( ret != SQL_SUCCESS )
      {
        dataType = pParameter->GetParameterType();
        dataSize = pParameter->GetColumnSize();
        dataDigits = pParameter->GetDecimalDigits();
      }

      SQLRETURN nRet = m_pInterface->GetSQLBindParameter()(m_Statements[nIndex], nPosition, SQL_PARAM_INPUT,
        pParameter->GetValueType(), dataType, dataSize, dataDigits, pParameter->GetDataPtr(),
        pParameter->GetDataLength(), pParameter->GetParameterLengthPtr());

      if ( nRet != SQL_SUCCESS && nRet != SQL_SUCCESS_WITH_INFO )
      {
        InterpretErrorCodes(nRet, m_Statements[nIndex]);
        ThrowDatabaseException();
        return;
      }
    }
  }
}

int wxOdbcPreparedStatement::FindStatementAndAdjustPositionIndex(int* pPosition)
{
    // Don't mess around if there's just one entry in the vector
    if (m_Statements.size() == 0)
        return 0;
        
    // Go through all the elements in the vector
    // Get the number of parameters in each statement
    // Adjust the nPosition for the the broken up statements
    for (unsigned int i=0; i<m_Statements.size(); i++)
    {
        SQLSMALLINT num = 0;
        SQLRETURN nRet = m_pInterface->GetSQLNumParams()(m_Statements[i], &num);
        if ( nRet != SQL_SUCCESS && nRet != SQL_SUCCESS_WITH_INFO )
        {
            InterpretErrorCodes(nRet, m_Statements[i]);
            ThrowDatabaseException();
            return -1;
        }

        int nParametersInThisStatement = (int)num;
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

void wxOdbcPreparedStatement::SetParam(int nPosition, wxOdbcParameter* pParameter)
{
  // First make sure that there are enough elements in the collection
  while (m_Parameters.size() < (unsigned int)(nPosition))
  {
    m_Parameters.push_back(NULL);//EmptyParameter);
  }
  // Free up any data that is being replaced so the allocated memory isn't lost
  if (m_Parameters[nPosition-1] != NULL)
  {
    delete (m_Parameters[nPosition-1]);
  }
  // Now set the new data
  m_Parameters[nPosition-1] = pParameter;
}

void wxOdbcPreparedStatement::InterpretErrorCodes( long nCode, SQLHSTMT stmth_ptr )
{
  wxLogDebug(_("OdbcPreparedStatement::InterpretErrorCodes()\n"));

  //if ((nCode != SQL_SUCCESS) ) // && (nCode != SQL_SUCCESS_WITH_INFO))
  {
    SQLINTEGER iNativeCode;
    SQLTCHAR strState[ERR_STATE_LEN];
    SQLTCHAR strBuffer[ERR_BUFFER_LEN];
    SQLSMALLINT iMsgLen;

    memset(strState, 0, ERR_STATE_LEN*sizeof(SQLTCHAR));
    memset(strBuffer, 0, ERR_BUFFER_LEN*sizeof(SQLTCHAR));

    if (stmth_ptr)
      m_pInterface->GetSQLGetDiagRec()(SQL_HANDLE_STMT, stmth_ptr, 1, strState, &iNativeCode, 
        strBuffer, ERR_BUFFER_LEN, &iMsgLen);  
    else
      m_pInterface->GetSQLGetDiagRec()(SQL_HANDLE_DBC, m_sqlHDBC, 1, strState, &iNativeCode,
        strBuffer, ERR_BUFFER_LEN, &iMsgLen);  
    
    SetErrorCode((int)iNativeCode);
    //SetErrorMessage(ConvertFromUnicodeStream((char*)strBuffer));
    SetErrorMessage(wxString((wxChar*)strBuffer));
  }
}


