
#include "include/postgresql/postgresql_preparedstatement_parameter_collector.h"
#include "wx/arrimpl.cpp"
WX_DEFINE_OBJARRAY(ArrayOfPostgresParameters);

wxPostgresPreparedStatementParameterCollection::~wxPostgresPreparedStatementParameterCollection()
{
  m_Parameters.Clear();
}

int wxPostgresPreparedStatementParameterCollection::GetSize()
{
  return m_Parameters.size();
}

char** wxPostgresPreparedStatementParameterCollection::GetParamValues()
{
  char** paramValues = new char*[m_Parameters.size()];

  for (unsigned int i=0; i<m_Parameters.size(); i++)
  {
    // Get a pointer to the appropriate data member variable for this parameter
    paramValues[i] = (char*)(m_Parameters[i].GetDataPtr());
  }

  return paramValues;
}

int* wxPostgresPreparedStatementParameterCollection::GetParamLengths()
{
  int* paramLengths = new int[m_Parameters.size()];
  
  for (unsigned int i=0; i<m_Parameters.size(); i++)
  {
    // Get a pointer to the m_nBufferLength member variable for this parameter
    paramLengths[i] = m_Parameters[i].GetDataLength();
  }

  return paramLengths;
}

int* wxPostgresPreparedStatementParameterCollection::GetParamFormats()
{
  int* paramFormats = new int[m_Parameters.size()];
  
  for (unsigned int i=0; i<m_Parameters.size(); i++)
  {
    paramFormats[i] = (m_Parameters[i].IsBinary()) ? 1 : 0;
  }

  return paramFormats;
}

void wxPostgresPreparedStatementParameterCollection::SetParam(int nPosition, int nValue)
{
  wxPostgresParameter Parameter(nValue);
  SetParam(nPosition, Parameter);
}

void wxPostgresPreparedStatementParameterCollection::SetParam(int nPosition, double dblValue)
{
  wxPostgresParameter Parameter(dblValue);
  SetParam(nPosition, Parameter);
}

void wxPostgresPreparedStatementParameterCollection::SetParam(int nPosition, const wxString& strValue)
{
  wxPostgresParameter Parameter(strValue);
  SetParam(nPosition, Parameter);
}

void wxPostgresPreparedStatementParameterCollection::SetParam(int nPosition)
{
  wxPostgresParameter Parameter;
  SetParam(nPosition, Parameter);
}

void wxPostgresPreparedStatementParameterCollection::SetParam(int nPosition, const void* pData, long nDataLength)
{
  wxPostgresParameter Parameter(pData, nDataLength);
  SetParam(nPosition, Parameter);
}

void wxPostgresPreparedStatementParameterCollection::SetParam(int nPosition, const wxDateTime& dateValue)
{
  wxPostgresParameter Parameter(dateValue);
  SetParam(nPosition, Parameter);
}

void wxPostgresPreparedStatementParameterCollection::SetParam(int nPosition, bool bValue)
{
  wxPostgresParameter Parameter(bValue);
  SetParam(nPosition, Parameter);
}

void wxPostgresPreparedStatementParameterCollection::SetParam(int nPosition, wxPostgresParameter& Parameter)
{
  // First make sure that there are enough elements in the collection
  while (m_Parameters.size() < (unsigned int)(nPosition))
  {
    wxPostgresParameter EmptyParameter;
    m_Parameters.push_back(EmptyParameter);
  }
  m_Parameters[nPosition-1] = Parameter;
}

