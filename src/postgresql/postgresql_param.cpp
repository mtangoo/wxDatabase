#include "include/postgresql/postgresql_param.h"

// ctor
wxPostgresParameter::wxPostgresParameter()
{
  m_nParameterType = wxPostgresParameter::PARAM_NULL;
}

wxPostgresParameter::wxPostgresParameter(const wxString& strValue)
{
  m_nParameterType = wxPostgresParameter::PARAM_STRING;
  m_strValue = strValue;
  m_nBufferLength = m_strValue.Length();
}

wxPostgresParameter::wxPostgresParameter(int nValue)
{
  m_nParameterType = wxPostgresParameter::PARAM_INT;
  //m_nValue = nValue;
  m_strValue = wxString::Format(_("%d"), nValue);
}

wxPostgresParameter::wxPostgresParameter(double dblValue)
{
  m_nParameterType = wxPostgresParameter::PARAM_DOUBLE;
  //m_dblValue = dblValue;
  m_strValue = wxString::Format(_("%f"), dblValue);
}

wxPostgresParameter::wxPostgresParameter(bool bValue)
{
  m_nParameterType = wxPostgresParameter::PARAM_BOOL;
  m_bValue = bValue;
}

wxPostgresParameter::wxPostgresParameter(const wxDateTime& dateValue)
{
  m_nParameterType = wxPostgresParameter::PARAM_DATETIME;
  m_strDateValue = dateValue.Format(_("%Y-%m-%d %H:%M:%S"));
  m_nBufferLength = m_strDateValue.Length();
}

wxPostgresParameter::wxPostgresParameter(const void* pData, long nDataLength)
{
  m_nParameterType = wxPostgresParameter::PARAM_BLOB;
  void* pBuffer = m_BufferValue.GetWriteBuf(nDataLength);
  memcpy(pBuffer, pData, nDataLength);
  m_nBufferLength = nDataLength;
}


long wxPostgresParameter::GetDataLength()
{
  return m_nBufferLength;
}

long* wxPostgresParameter::GetDataLengthPointer()
{
  return &m_nBufferLength;
}

const void* wxPostgresParameter::GetDataPtr()
{
  const void *pReturn = NULL;
  
  switch (m_nParameterType)
  {
    case wxPostgresParameter::PARAM_STRING:
      m_CharBufferValue = ConvertToUnicodeStream(m_strValue);
      pReturn = m_CharBufferValue;
      break;
    case wxPostgresParameter::PARAM_INT:
      //pReturn = &m_nValue;
      m_CharBufferValue = ConvertToUnicodeStream(m_strValue);
      pReturn = m_CharBufferValue;
      break;
    case wxPostgresParameter::PARAM_DOUBLE:
      //pReturn = &m_dblValue;
      m_CharBufferValue = ConvertToUnicodeStream(m_strValue);
      pReturn = m_CharBufferValue;
      break;
    case wxPostgresParameter::PARAM_DATETIME:
      m_CharBufferValue = ConvertToUnicodeStream(m_strDateValue);
      pReturn = m_CharBufferValue;
      break;
    case wxPostgresParameter::PARAM_BOOL:
      pReturn = &m_bValue;
      break;
    case wxPostgresParameter::PARAM_BLOB:
      pReturn = m_BufferValue.GetData();
      break;
    case wxPostgresParameter::PARAM_NULL:
      pReturn = NULL;
      break;
    default:
      pReturn = NULL;
      break;
  };
  return pReturn;
}

int wxPostgresParameter::GetParameterType()
{
  return m_nParameterType;
}

bool wxPostgresParameter::IsBinary()
{
  return (wxPostgresParameter::PARAM_BLOB == m_nParameterType);
}

