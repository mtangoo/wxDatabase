#include "include/resultset.h"
#include "include/resultset_metadata.h"

// ctor()
wxDatabaseResultSet::wxDatabaseResultSet()
 : wxDatabaseErrorReporter()
{
}

// dtor()
wxDatabaseResultSet::~wxDatabaseResultSet()
{
  //wxPrintf(_("~DatabaseResultSet()\n"));
  CloseMetaData();
}

int wxDatabaseResultSet::GetResultInt(const wxString& strField)
{
  int nIndex = LookupField(strField);
  if (nIndex != -1)
  {
    return GetResultInt(nIndex);
  }
  return -1;
}

wxString wxDatabaseResultSet::GetResultString(const wxString& strField)
{
  int nIndex = LookupField(strField);
  if (nIndex != -1)
  {
    return GetResultString(nIndex);
  }
  return wxEmptyString;
}

long wxDatabaseResultSet::GetResultLong(const wxString& strField)
{
  int nIndex = LookupField(strField);
  if (nIndex != -1)
  {
    return GetResultLong(nIndex);
  }
  return -1;
}

bool wxDatabaseResultSet::GetResultBool(const wxString& strField)
{
  int nIndex = LookupField(strField);
  if (nIndex != -1)
  {
    return GetResultBool(nIndex);
  }
  return false;
}

wxDateTime wxDatabaseResultSet::GetResultDate(const wxString& strField)
{
  int nIndex = LookupField(strField);
  if (nIndex != -1)
  {
    return GetResultDate(nIndex);
  }
  return wxInvalidDateTime;
}

void* wxDatabaseResultSet::GetResultBlob(const wxString& strField, wxMemoryBuffer& Buffer)
{
  int nIndex = LookupField(strField);
  if (nIndex != -1)
  {
    return GetResultBlob(nIndex, Buffer);
  }
  return NULL;
}

double wxDatabaseResultSet::GetResultDouble(const wxString& strField)
{
  int nIndex = LookupField(strField);
  if (nIndex != -1)
  {
    return GetResultDouble(nIndex);
  }
  return -1;
}

bool wxDatabaseResultSet::IsFieldNull(const wxString& strField)
{
  int nIndex = LookupField(strField);
  if (nIndex != -1)
  {
    return IsFieldNull(nIndex);
  }
  return true;
}

void wxDatabaseResultSet::CloseMetaData()
{
  // Iterate through all of the meta data and close them all
  wxMetaDataHashSet::iterator start = m_MetaData.begin();
  wxMetaDataHashSet::iterator stop = m_MetaData.end();
  while (start != stop)
  {
    delete (*start);
    start++;
  }
  m_MetaData.clear();
}

bool wxDatabaseResultSet::CloseMetaData(wxResultSetMetaData* pMetaData)
{
  if (pMetaData != NULL)
  {
    // Check if we have this meta data in our list
    if (m_MetaData.find(pMetaData) != m_MetaData.end())
    {
      // Remove the meta data pointer from the list and delete the pointer
      delete pMetaData;
      m_MetaData.erase(pMetaData);
      return true;
    }

    // Delete the pointer
    delete pMetaData;
    return true;
  }
  else
  {
    // Return false on NULL pointer
    return false;
  }
}

