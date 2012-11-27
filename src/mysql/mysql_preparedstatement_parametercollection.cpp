#include "include/mysql/mysql_preparedstatement_parametercollection.h"


wxMysqlPreparedStatementParameterCollection::wxMysqlPreparedStatementParameterCollection()
{
}

wxMysqlPreparedStatementParameterCollection::~wxMysqlPreparedStatementParameterCollection()
{
  MysqlParameterArray::iterator start = m_Parameters.begin();
  MysqlParameterArray::iterator stop = m_Parameters.end();

  while (start != stop)
  {
    if ((*start) != NULL)
    {
      wxMysqlParameter* pParameter = (wxMysqlParameter*)(*start);
      wxDELETE(pParameter);
      (*start) = NULL;
    }
    start++;
  }
}

int wxMysqlPreparedStatementParameterCollection::GetSize()
{
  return m_Parameters.size();
}

MYSQL_BIND* wxMysqlPreparedStatementParameterCollection::GetMysqlParameterBindings()
{
  MYSQL_BIND* pBindings = new MYSQL_BIND[m_Parameters.size()];
  
  memset(pBindings, 0, sizeof(MYSQL_BIND)*m_Parameters.size());

  for (unsigned int i=0; i<m_Parameters.size(); i++)
  {
    pBindings[i].buffer_type=m_Parameters[i]->GetBufferType();
    pBindings[i].buffer=(void*)m_Parameters[i]->GetDataPtr();
    pBindings[i].buffer_length=m_Parameters[i]->GetDataLength();
    pBindings[i].length=m_Parameters[i]->GetDataLengthPtr();
  }

  return pBindings;
}

void wxMysqlPreparedStatementParameterCollection::SetParam(int nPosition, int nValue)
{
  //MysqlParameter Parameter(nValue);
  //SetParam(nPosition, Parameter);
  wxMysqlParameter* pParameter = new wxMysqlParameter(nValue);
  pParameter->SetEncoding(GetEncoding());
  SetParam(nPosition, pParameter);
}

void wxMysqlPreparedStatementParameterCollection::SetParam(int nPosition, double dblValue)
{
  //MysqlParameter Parameter(dblValue);
  //SetParam(nPosition, Parameter);
  wxMysqlParameter* pParameter = new wxMysqlParameter(dblValue);
  pParameter->SetEncoding(GetEncoding());
  SetParam(nPosition, pParameter);
}

void wxMysqlPreparedStatementParameterCollection::SetParam(int nPosition, const wxString& strValue)
{
  //MysqlParameter Parameter(strValue);
  //SetParam(nPosition, Parameter);
  wxMysqlParameter* pParameter = new wxMysqlParameter(strValue);
  pParameter->SetEncoding(GetEncoding());
  SetParam(nPosition, pParameter);
}

void wxMysqlPreparedStatementParameterCollection::SetParam(int nPosition)
{
  //MysqlParameter Parameter;
  //SetParam(nPosition, Parameter);
  wxMysqlParameter* pParameter = new wxMysqlParameter();
  pParameter->SetEncoding(GetEncoding());
  SetParam(nPosition, pParameter);
}

void wxMysqlPreparedStatementParameterCollection::SetParam(int nPosition, const void* pData, long nDataLength)
{
  //MysqlParameter Parameter(pData, nDataLength);
  //SetParam(nPosition, Parameter);
  wxMysqlParameter* pParameter = new wxMysqlParameter(pData, nDataLength);
  pParameter->SetEncoding(GetEncoding());
  SetParam(nPosition, pParameter);
}

void wxMysqlPreparedStatementParameterCollection::SetParam(int nPosition, const wxDateTime& dateValue)
{
  //MysqlParameter Parameter(dateValue);
  //SetParam(nPosition, Parameter);
  wxMysqlParameter* pParameter = new wxMysqlParameter(dateValue);
  pParameter->SetEncoding(GetEncoding());
  SetParam(nPosition, pParameter);
}

void wxMysqlPreparedStatementParameterCollection::SetParam(int nPosition, bool bValue)
{
  //MysqlParameter Parameter(bValue);
  //SetParam(nPosition, Parameter);
  wxMysqlParameter* pParameter = new wxMysqlParameter(bValue);
  pParameter->SetEncoding(GetEncoding());
  SetParam(nPosition, pParameter);
}

void wxMysqlPreparedStatementParameterCollection::SetParam(int nPosition, wxMysqlParameter* pParameter)
{
  // First make sure that there are enough elements in the collection
  while (m_Parameters.size() < (unsigned int)(nPosition))
  {
    //MysqlParameter EmptyParameter;
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

