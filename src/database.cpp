#include "../include/database.h"
#include "../include/errorcodes.h"
#include "../include/exceptions.h"

// ctor()
wxDatabase::wxDatabase()
 : wxDatabaseErrorReporter()
{
}

// dtor()
wxDatabase::~wxDatabase()
{
  CloseResultSets();
  CloseStatements();
}

int wxDatabase::RunQuery(const wxString& strQuery)
{
  return RunQuery(strQuery, true);
}

void wxDatabase::CloseResultSets()
{
  // Iterate through all of the result sets and close them all
  wxDatabaseResultSetHashSet::iterator start = m_ResultSets.begin();
  wxDatabaseResultSetHashSet::iterator stop = m_ResultSets.end();
  while (start != stop)
  {
    wxLogDebug(_("ResultSet NOT closed and cleaned up by the DatabaseLayer dtor"));
    delete (*start);
    start++;
  }
  m_ResultSets.clear();
}

void wxDatabase::CloseStatements()
{
  // Iterate through all of the statements and close them all
  wxDatabaseStatementHashSet::iterator start = m_Statements.begin();
  wxDatabaseStatementHashSet::iterator stop = m_Statements.end();
  while (start != stop)
  {
    wxLogDebug(_("PreparedStatement NOT closed and cleaned up by the DatabaseLayer dtor"));
    delete (*start);
    start++;
  }
  m_Statements.clear();
}

bool wxDatabase::CloseResultSet(wxDatabaseResultSet* pResultSet)
{
  if (pResultSet != NULL)
  {
    // Check if we have this result set in our list
    if (m_ResultSets.find(pResultSet) != m_ResultSets.end())
    {
      // Remove the result set pointer from the list and delete the pointer
      delete pResultSet;
      m_ResultSets.erase(pResultSet);
      return true;
    }

    // If not then iterate through all of the statements and see
    //  if any of them have the result set in their lists
    wxDatabaseStatementHashSet::iterator it;
    for( it = m_Statements.begin(); it != m_Statements.end(); ++it )
    {
      // If the statement knows about the result set then it will close the 
      //  result set and return true, otherwise it will return false
      wxPreparedStatement* pStatement = *it;
      if (pStatement != NULL)
      {
        if (pStatement->CloseResultSet(pResultSet))
        {
          return true;
        }
      }
    }

    // If we don't know about the result set and the statements don't
    //  know about it, the just delete it
    delete pResultSet;
    return true;
  }
  else
  {
    // Return false on NULL pointer
    return false;
  }

}

bool wxDatabase::CloseStatement(wxPreparedStatement* pStatement)
{
  if (pStatement != NULL)
  {
    // See if we know about this pointer, if so then remove it from the list
    if (m_Statements.find(pStatement) != m_Statements.end())
    {
      // Remove the statement pointer from the list and delete the pointer
      delete pStatement;
      m_Statements.erase(pStatement);
      return true;
    }

    // Otherwise just delete it
    delete pStatement;
    return true;
  }
  else
  {
    // Return false on NULL pointer
    return false;
  }
}


int wxDatabase::GetSingleResultInt(const wxString& strSQL, const wxString& strField, bool bRequireUniqueResult /*= true*/)
{
  wxVariant variant(strField);
  return GetSingleResultInt(strSQL, &variant, bRequireUniqueResult);
}

int wxDatabase::GetSingleResultInt(const wxString& strSQL, int nField, bool bRequireUniqueResult /*= true*/)
{
  wxVariant variant((long)nField);
  return GetSingleResultInt(strSQL, &variant, bRequireUniqueResult);
}

int wxDatabase::GetSingleResultInt(const wxString& strSQL, const wxVariant* field, bool bRequireUniqueResult /*= true*/)
{
  bool valueRetrievedFlag = false;
  int value = -1;

  wxDatabaseResultSet* pResult = NULL;
#ifndef DONT_USE_DATABASE_LAYER_EXCEPTIONS
  try
  {
#endif
    pResult = ExecuteQuery(strSQL);

    while (pResult->Next())
    {
      if (valueRetrievedFlag)
      {
        // Close the result set, reset the value and throw an exception
        CloseResultSet(pResult);
        pResult = NULL;
        value = -1;
        SetErrorCode(wxDATABASE_NON_UNIQUE_RESULTSET);
        SetErrorMessage(wxT("A non-unique result was returned."));
        ThrowDatabaseException();
        return value;
      }
      else
      {
        if (field->IsType(_("string")))
          value = pResult->GetResultInt(field->GetString());
        else
          value = pResult->GetResultInt(field->GetLong());
        valueRetrievedFlag = true;

        // If the user isn't concerned about returning a unique result,
        //  then just exit after the first record is found
        if (!bRequireUniqueResult)
          break;
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

    throw e;
  }
#endif

  if (pResult != NULL)
  {
    CloseResultSet(pResult);
    pResult = NULL;
  }

  // Make sure that a value was retrieved from the database
  if (!valueRetrievedFlag)
  {
    value = -1;
    SetErrorCode(wxDATABASE_NO_ROWS_FOUND);
    SetErrorMessage(wxT("No result was returned."));
    ThrowDatabaseException();
    return value;
  }
  
  return value;
}

wxString wxDatabase::GetSingleResultString(const wxString& strSQL, int nField, bool bRequireUniqueResult /*= true*/)
{
  wxVariant variant((long)nField);
  return GetSingleResultString(strSQL, &variant, bRequireUniqueResult);
}

wxString wxDatabase::GetSingleResultString(const wxString& strSQL, const wxString& strField, bool bRequireUniqueResult /*= true*/)
{
  wxVariant variant(strField);
  return GetSingleResultString(strSQL, &variant, bRequireUniqueResult);
}

wxString wxDatabase::GetSingleResultString(const wxString& strSQL, const wxVariant* field, bool bRequireUniqueResult /*= true*/)
{
  bool valueRetrievedFlag = false;
  wxString value = wxEmptyString;

  wxDatabaseResultSet* pResult = NULL;
#ifndef DONT_USE_DATABASE_LAYER_EXCEPTIONS
  try
  {
#endif
    pResult = ExecuteQuery(strSQL);

    while (pResult->Next())
    {
      if (valueRetrievedFlag)
      {
        // Close the result set, reset the value and throw an exception
        CloseResultSet(pResult);
        pResult = NULL;
        value = wxEmptyString;
        SetErrorCode(wxDATABASE_NON_UNIQUE_RESULTSET);
        SetErrorMessage(wxT("A non-unique result was returned."));
        ThrowDatabaseException();
        return value;
      }
      else
      {
        if (field->IsType(_("string")))
          value = pResult->GetResultString(field->GetString());
        else
          value = pResult->GetResultString(field->GetLong());
        valueRetrievedFlag = true;

        // If the user isn't concerned about returning a unique result,
        //  then just exit after the first record is found
        if (!bRequireUniqueResult)
          break;
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

    throw e;
  }
#endif

  if (pResult != NULL)
  {
    CloseResultSet(pResult);
    pResult = NULL;
  }

  // Make sure that a value was retrieved from the database
  if (!valueRetrievedFlag)
  {
    value = wxEmptyString;
    SetErrorCode(wxDATABASE_NO_ROWS_FOUND);
    SetErrorMessage(wxT("No result was returned."));
    ThrowDatabaseException();
    return value;
  }
  
  return value;
}

long wxDatabase::GetSingleResultLong(const wxString& strSQL, int nField, bool bRequireUniqueResult /*= true*/)
{
  wxVariant variant((long)nField);
  return GetSingleResultLong(strSQL, &variant, bRequireUniqueResult);
}

long wxDatabase::GetSingleResultLong(const wxString& strSQL, const wxString& strField, bool bRequireUniqueResult /*= true*/)
{
  wxVariant variant(strField);
  return GetSingleResultLong(strSQL, &variant, bRequireUniqueResult);
}

long wxDatabase::GetSingleResultLong(const wxString& strSQL, const wxVariant* field, bool bRequireUniqueResult /*= true*/)
{
  bool valueRetrievedFlag = false;
  long value = -1;

  wxDatabaseResultSet* pResult = NULL;
#ifndef DONT_USE_DATABASE_LAYER_EXCEPTIONS
  try
  {
#endif
    pResult = ExecuteQuery(strSQL);

    while (pResult->Next())
    {
      if (valueRetrievedFlag)
      {
        // Close the result set, reset the value and throw an exception
        CloseResultSet(pResult);
        pResult = NULL;
        value = -1;
        SetErrorCode(wxDATABASE_NON_UNIQUE_RESULTSET);
        SetErrorMessage(wxT("A non-unique result was returned."));
        ThrowDatabaseException();
        return value;
      }
      else
      {
        if (field->IsType(_("string")))
          value = pResult->GetResultLong(field->GetString());
        else
          value = pResult->GetResultLong(field->GetLong());
        valueRetrievedFlag = true;

        // If the user isn't concerned about returning a unique result,
        //  then just exit after the first record is found
        if (!bRequireUniqueResult)
          break;
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

    throw e;
  }
#endif

  if (pResult != NULL)
  {
    CloseResultSet(pResult);
    pResult = NULL;
  }

  // Make sure that a value was retrieved from the database
  if (!valueRetrievedFlag)
  {
    value = -1;
    SetErrorCode(wxDATABASE_NO_ROWS_FOUND);
    SetErrorMessage(wxT("No result was returned."));
    ThrowDatabaseException();
    return value;
  }
  
  return value;
}

bool wxDatabase::GetSingleResultBool(const wxString& strSQL, int nField, bool bRequireUniqueResult /*= true*/)
{
  wxVariant variant((long)nField);
  return GetSingleResultBool(strSQL, &variant, bRequireUniqueResult);
}

bool wxDatabase::GetSingleResultBool(const wxString& strSQL, const wxString& strField, bool bRequireUniqueResult /*= true*/)
{
  wxVariant variant(strField);
  return GetSingleResultBool(strSQL, &variant, bRequireUniqueResult);
}

bool wxDatabase::GetSingleResultBool(const wxString& strSQL, const wxVariant* field, bool bRequireUniqueResult /*= true*/)
{
  bool valueRetrievedFlag = false;
  bool value = false;

  wxDatabaseResultSet* pResult = NULL;
#ifndef DONT_USE_DATABASE_LAYER_EXCEPTIONS
  try
  {
#endif
    pResult = ExecuteQuery(strSQL);

    while (pResult->Next())
    {
      if (valueRetrievedFlag)
      {
        // Close the result set, reset the value and throw an exception
        CloseResultSet(pResult);
        pResult = NULL;
        value = false;
        SetErrorCode(wxDATABASE_NON_UNIQUE_RESULTSET);
        SetErrorMessage(wxT("A non-unique result was returned."));
        ThrowDatabaseException();
        return value;
      }
      else
      {
        if (field->IsType(_("string")))
          value = pResult->GetResultBool(field->GetString());
        else
          value = pResult->GetResultBool(field->GetLong());
        valueRetrievedFlag = true;

        // If the user isn't concerned about returning a unique result,
        //  then just exit after the first record is found
        if (!bRequireUniqueResult)
          break;
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

    throw e;
  }
#endif

  if (pResult != NULL)
  {
    CloseResultSet(pResult);
    pResult = NULL;
  }

  // Make sure that a value was retrieved from the database
  if (!valueRetrievedFlag)
  {
    value = false;
    SetErrorCode(wxDATABASE_NO_ROWS_FOUND);
    SetErrorMessage(wxT("No result was returned."));
    ThrowDatabaseException();
    return value;
  }
  
  return value;
}
 
wxDateTime wxDatabase::GetSingleResultDate(const wxString& strSQL, int nField, bool bRequireUniqueResult /*= true*/)
{
  wxVariant variant((long)nField);
  return GetSingleResultDate(strSQL, &variant, bRequireUniqueResult);
}

wxDateTime wxDatabase::GetSingleResultDate(const wxString& strSQL, const wxString& strField, bool bRequireUniqueResult /*= true*/)
{
  wxVariant variant(strField);
  return GetSingleResultDate(strSQL, &variant, bRequireUniqueResult);
}

wxDateTime wxDatabase::GetSingleResultDate(const wxString& strSQL, const wxVariant* field, bool bRequireUniqueResult /*= true*/)
{
  bool valueRetrievedFlag = false;
  wxDateTime value = wxInvalidDateTime;

  wxDatabaseResultSet* pResult = NULL;
#ifndef DONT_USE_DATABASE_LAYER_EXCEPTIONS
  try
  {
#endif
    pResult = ExecuteQuery(strSQL);

    while (pResult->Next())
    {
      if (valueRetrievedFlag)
      {
        // Close the result set, reset the value and throw an exception
        CloseResultSet(pResult);
        pResult = NULL;
        value = wxInvalidDateTime;
        SetErrorCode(wxDATABASE_NON_UNIQUE_RESULTSET);
        SetErrorMessage(wxT("A non-unique result was returned."));
        ThrowDatabaseException();
        return value;
      }
      else
      {
        if (field->IsType(_("string")))
          value = pResult->GetResultDate(field->GetString());
        else
          value = pResult->GetResultDate(field->GetLong());
        valueRetrievedFlag = true;

        // If the user isn't concerned about returning a unique result,
        //  then just exit after the first record is found
        if (!bRequireUniqueResult)
          break;
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

    throw e;
  }
#endif

  if (pResult != NULL)
  {
    CloseResultSet(pResult);
    pResult = NULL;
  }

  // Make sure that a value was retrieved from the database
  if (!valueRetrievedFlag)
  {
    value = wxInvalidDateTime;
    SetErrorCode(wxDATABASE_NO_ROWS_FOUND);
    SetErrorMessage(wxT("No result was returned."));
    ThrowDatabaseException();
    return value;
  }
  
  return value;
}

void* wxDatabase::GetSingleResultBlob(const wxString& strSQL, int nField, wxMemoryBuffer& Buffer, bool bRequireUniqueResult /*= true*/)
{
  wxVariant variant((long)nField);
  return GetSingleResultBlob(strSQL, &variant, Buffer, bRequireUniqueResult);
}

void* wxDatabase::GetSingleResultBlob(const wxString& strSQL, const wxString& strField, wxMemoryBuffer& Buffer, bool bRequireUniqueResult /*= true*/)
{
  wxVariant variant(strField);
  return GetSingleResultBlob(strSQL, &variant, Buffer, bRequireUniqueResult);
}

void* wxDatabase::GetSingleResultBlob(const wxString& strSQL, const wxVariant* field, wxMemoryBuffer& Buffer, bool bRequireUniqueResult /*= true*/)
{
  bool valueRetrievedFlag = false;
  void* value = NULL;

  wxDatabaseResultSet* pResult = NULL;
#ifndef DONT_USE_DATABASE_LAYER_EXCEPTIONS
  try
  {
#endif
    pResult = ExecuteQuery(strSQL);

    while (pResult->Next())
    {
      if (valueRetrievedFlag)
      {
        // Close the result set, reset the value and throw an exception
        CloseResultSet(pResult);
        pResult = NULL;
        value = NULL;
        SetErrorCode(wxDATABASE_NON_UNIQUE_RESULTSET);
        SetErrorMessage(wxT("A non-unique result was returned."));
        ThrowDatabaseException();
        return value;
      }
      else
      {
        if (field->IsType(_("string")))
          value = pResult->GetResultBlob(field->GetString(), Buffer);
        else
          value = pResult->GetResultBlob(field->GetLong(), Buffer);
        valueRetrievedFlag = true;

        // If the user isn't concerned about returning a unique result,
        //  then just exit after the first record is found
        if (!bRequireUniqueResult)
          break;
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

    throw e;
  }
#endif

  if (pResult != NULL)
  {
    CloseResultSet(pResult);
    pResult = NULL;
  }

  // Make sure that a value was retrieved from the database
  if (!valueRetrievedFlag)
  {
    value = NULL;
    SetErrorCode(wxDATABASE_NO_ROWS_FOUND);
    SetErrorMessage(wxT("No result was returned."));
    ThrowDatabaseException();
    return value;
  }
  
  return value;
}
 
double wxDatabase::GetSingleResultDouble(const wxString& strSQL, int nField, bool bRequireUniqueResult /*= true*/)
{
  wxVariant variant((long)nField);
  return GetSingleResultDouble(strSQL, &variant, bRequireUniqueResult);
}

double wxDatabase::GetSingleResultDouble(const wxString& strSQL, const wxString& strField, bool bRequireUniqueResult /*= true*/)
{
  wxVariant variant(strField);
  return GetSingleResultDouble(strSQL, &variant, bRequireUniqueResult);
}

double wxDatabase::GetSingleResultDouble(const wxString& strSQL, const wxVariant* field, bool bRequireUniqueResult /*= true*/)
{
  bool valueRetrievedFlag = false;
  double value = -1;

  wxDatabaseResultSet* pResult = NULL;
#ifndef DONT_USE_DATABASE_LAYER_EXCEPTIONS
  try
  {
#endif
    pResult = ExecuteQuery(strSQL);

    while (pResult->Next())
    {
      if (valueRetrievedFlag)
      {
        // Close the result set, reset the value and throw an exception
        CloseResultSet(pResult);
        pResult = NULL;
        value = -1;
        SetErrorCode(wxDATABASE_NON_UNIQUE_RESULTSET);
        SetErrorMessage(wxT("A non-unique result was returned."));
        ThrowDatabaseException();
        return value;
      }
      else
      {
        if (field->IsType(_("string")))
          value = pResult->GetResultDouble(field->GetString());
        else
          value = pResult->GetResultDouble(field->GetLong());
        valueRetrievedFlag = true;

        // If the user isn't concerned about returning a unique result,
        //  then just exit after the first record is found
        if (!bRequireUniqueResult)
          break;
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

    throw e;
  }
#endif

  if (pResult != NULL)
  {
    CloseResultSet(pResult);
    pResult = NULL;
  }

  // Make sure that a value was retrieved from the database
  if (!valueRetrievedFlag)
  {
    value = -1;
    SetErrorCode(wxDATABASE_NO_ROWS_FOUND);
    SetErrorMessage(wxT("No result was returned."));
    ThrowDatabaseException();
    return value;
  }
  
  return value;
}

wxArrayInt wxDatabase::GetResultsArrayInt(const wxString& strSQL, int nField)
{
  wxVariant variant((long)nField);
  return GetResultsArrayInt(strSQL, &variant);
}

wxArrayInt wxDatabase::GetResultsArrayInt(const wxString& strSQL, const wxString& strField)
{
  wxVariant variant(strField);
  return GetResultsArrayInt(strSQL, &variant);
}

wxArrayInt wxDatabase::GetResultsArrayInt(const wxString& strSQL, const wxVariant* field)
{
  wxArrayInt returnArray;
  
  wxDatabaseResultSet* pResult = NULL;
#ifndef DONT_USE_DATABASE_LAYER_EXCEPTIONS
  try
  {
#endif
    pResult = ExecuteQuery(strSQL);

    while (pResult->Next())
    {
      if (field->IsType(_("string")))
        returnArray.Add(pResult->GetResultInt(field->GetString()));
      else
        returnArray.Add(pResult->GetResultInt(field->GetLong()));
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

wxArrayString wxDatabase::GetResultsArrayString(const wxString& strSQL, int nField)
{
  wxVariant variant((long)nField);
  return GetResultsArrayString(strSQL, &variant);
}

wxArrayString wxDatabase::GetResultsArrayString(const wxString& strSQL, const wxString& strField)
{
  wxVariant variant(strField);
  return GetResultsArrayString(strSQL, &variant);
}

wxArrayString wxDatabase::GetResultsArrayString(const wxString& strSQL, const wxVariant* field)
{
  wxArrayString returnArray;
  
  wxDatabaseResultSet* pResult = NULL;
#ifndef DONT_USE_DATABASE_LAYER_EXCEPTIONS
  try
  {
#endif
    pResult = ExecuteQuery(strSQL);

    while (pResult->Next())
    {
      if (field->IsType(_("string")))
        returnArray.Add(pResult->GetResultString(field->GetString()));
      else
        returnArray.Add(pResult->GetResultString(field->GetLong()));
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

wxArrayLong wxDatabase::GetResultsArrayLong(const wxString& strSQL, int nField)
{
  wxVariant variant((long)nField);
  return GetResultsArrayLong(strSQL, &variant);
}

wxArrayLong wxDatabase::GetResultsArrayLong(const wxString& strSQL, const wxString& strField)
{
  wxVariant variant(strField);
  return GetResultsArrayLong(strSQL, &variant);
}

wxArrayLong wxDatabase::GetResultsArrayLong(const wxString& strSQL, const wxVariant* field)
{
  wxArrayLong returnArray;
  
  wxDatabaseResultSet* pResult = NULL;
#ifndef DONT_USE_DATABASE_LAYER_EXCEPTIONS
  try
  {
#endif
    pResult = ExecuteQuery(strSQL);

    while (pResult->Next())
    {
      if (field->IsType(_("string")))
        returnArray.Add(pResult->GetResultLong(field->GetString()));
      else
        returnArray.Add(pResult->GetResultLong(field->GetLong()));
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

#if wxCHECK_VERSION(2, 7, 0)
wxArrayDouble wxDatabase::GetResultsArrayDouble(const wxString& strSQL, int nField)
{
  wxVariant variant((long)nField);
  return GetResultsArrayDouble(strSQL, &variant);
}

wxArrayDouble wxDatabase::GetResultsArrayDouble(const wxString& strSQL, const wxString& strField)
{
  wxVariant variant(strField);
  return GetResultsArrayDouble(strSQL, &variant);
}

wxArrayDouble wxDatabase::GetResultsArrayDouble(const wxString& strSQL, const wxVariant* field)
{
  wxArrayDouble returnArray;
  
  wxDatabaseResultSet* pResult = NULL;
#ifndef DONT_USE_DATABASE_LAYER_EXCEPTIONS
  try
  {
#endif
    pResult = ExecuteQuery(strSQL);

    while (pResult->Next())
    {
      if (field->IsType(_("string")))
        returnArray.Add(pResult->GetResultDouble(field->GetString()));
      else
        returnArray.Add(pResult->GetResultDouble(field->GetLong()));
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
#endif

