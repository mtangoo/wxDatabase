#include "include/mysql/mysql_param.h"


// ctor
wxMysqlParameter::wxMysqlParameter()
{
  m_nParameterType = wxMysqlParameter::PARAM_NULL;
}

wxMysqlParameter::wxMysqlParameter(const wxString& strValue)
{
  m_nParameterType = wxMysqlParameter::PARAM_STRING;
  m_strValue = strValue;
  m_CharBufferValue = ConvertToUnicodeStream(m_strValue);
  if (_("") == strValue)
  {
    m_nBufferLength = 0;
  }
  else
  {
    m_nBufferLength = GetEncodedStreamLength(m_strValue);
  }
}

wxMysqlParameter::wxMysqlParameter(int nValue)
{
  m_nParameterType = wxMysqlParameter::PARAM_INT;
  m_nValue = nValue;
  //m_strValue = wxString::Format(_("%d"), nValue);
}

wxMysqlParameter::wxMysqlParameter(double dblValue)
{
  m_nParameterType = wxMysqlParameter::PARAM_DOUBLE;
  m_dblValue = dblValue;
  //m_strValue = wxString::Format(_("%f"), dblValue);
}

wxMysqlParameter::wxMysqlParameter(bool bValue)
{
  m_nParameterType = wxMysqlParameter::PARAM_BOOL;
  m_bValue = bValue;
}

wxMysqlParameter::wxMysqlParameter(const wxDateTime& dateValue)
{
  m_nParameterType = wxMysqlParameter::PARAM_DATETIME;

  m_pDate = new MYSQL_TIME();

  memset(m_pDate, 0, sizeof(MYSQL_TIME));

  m_pDate->year = dateValue.GetYear();
  m_pDate->month = dateValue.GetMonth()+1;
  m_pDate->day = dateValue.GetDay();
  m_pDate->hour = dateValue.GetHour();
  m_pDate->minute = dateValue.GetMinute();
  m_pDate->second = dateValue.GetSecond();
  m_pDate->neg = 0;

  m_nBufferLength = sizeof(MYSQL_TIME);
}

wxMysqlParameter::wxMysqlParameter(const void* pData, long nDataLength)
{
  m_nParameterType = wxMysqlParameter::PARAM_BLOB;
  void* pBuffer = m_BufferValue.GetWriteBuf(nDataLength);
  memcpy(pBuffer, pData, nDataLength);
  m_nBufferLength = nDataLength;
}

wxMysqlParameter::~wxMysqlParameter()
{
  if ((m_nParameterType == wxMysqlParameter::PARAM_DATETIME) && (m_pDate != NULL))
  {
    delete m_pDate;
    m_pDate = NULL;
  }
}

long unsigned int wxMysqlParameter::GetDataLength()
{
  return m_nBufferLength;
}

long unsigned int* wxMysqlParameter::GetDataLengthPtr()
{
  return &m_nBufferLength;
}

const void* wxMysqlParameter::GetDataPtr()
{
  const void *pReturn = NULL;
  
  switch (m_nParameterType)
  {
    case wxMysqlParameter::PARAM_STRING:
      pReturn = m_CharBufferValue;
      break;
    case wxMysqlParameter::PARAM_INT:
      pReturn = &m_nValue;
      break;
    case wxMysqlParameter::PARAM_DOUBLE:
      pReturn = &m_dblValue;
      break;
    case wxMysqlParameter::PARAM_DATETIME:
      pReturn = m_pDate;
      break;
    case wxMysqlParameter::PARAM_BOOL:
      pReturn = &m_bValue;
      break;
    case wxMysqlParameter::PARAM_BLOB:
      pReturn = m_BufferValue.GetData();
      break;
    case wxMysqlParameter::PARAM_NULL:
      pReturn = NULL;
      break;
    default:
      pReturn = NULL;
      break;
  };
  return pReturn;
}

int wxMysqlParameter::GetParameterType()
{
  return m_nParameterType;
}

enum_field_types wxMysqlParameter::GetBufferType()
{
  enum_field_types returnType = MYSQL_TYPE_NULL;

  switch (m_nParameterType)
  {
    case wxMysqlParameter::PARAM_STRING:
      returnType = MYSQL_TYPE_VAR_STRING;
      break;
    case wxMysqlParameter::PARAM_INT:
      returnType = MYSQL_TYPE_LONG;
      break;
    case wxMysqlParameter::PARAM_DOUBLE:
      returnType = MYSQL_TYPE_DOUBLE;
      break;
    case wxMysqlParameter::PARAM_DATETIME:
      returnType = MYSQL_TYPE_TIMESTAMP;
      break;
    case wxMysqlParameter::PARAM_BOOL:
      returnType = MYSQL_TYPE_TINY;
      break;
    case wxMysqlParameter::PARAM_BLOB:
      returnType = MYSQL_TYPE_LONG_BLOB;
      break;
    case wxMysqlParameter::PARAM_NULL:
      returnType = MYSQL_TYPE_NULL;
      break;
    default:
      returnType = MYSQL_TYPE_NULL;
      break;
  };

  return returnType;
}

