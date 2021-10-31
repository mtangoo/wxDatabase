#include "wx/database/wxprec.h"

#if wxUSE_DATABASE_ODBC

// ctor
wxOdbcParameter::wxOdbcParameter()
{
  m_nParameterType = wxOdbcParameter::PARAM_NULL;
  m_nBufferLength = SQL_NULL_DATA;
}

wxOdbcParameter::wxOdbcParameter(const wxString& strValue)
{
  m_nParameterType = wxOdbcParameter::PARAM_STRING;
  m_strValue = strValue;
  m_nBufferLength = GetEncodedStreamLength(m_strValue);
}

wxOdbcParameter::wxOdbcParameter(int nValue)
{
  m_nParameterType = wxOdbcParameter::PARAM_INT;
  m_nValue = nValue;
  m_nBufferLength = 0;
}

wxOdbcParameter::wxOdbcParameter(double dblValue)
{
  m_nParameterType = wxOdbcParameter::PARAM_DOUBLE;
  m_dblValue = dblValue;
  m_nBufferLength = 0;
}

wxOdbcParameter::wxOdbcParameter(bool bValue)
{
  m_nParameterType = wxOdbcParameter::PARAM_BOOL;
  m_bValue = bValue;
  m_nBufferLength = 0;
}

wxOdbcParameter::wxOdbcParameter(const wxDateTime& dateValue)
{
  m_nParameterType = wxOdbcParameter::PARAM_DATETIME;
    	      
  m_DateValue.year = dateValue.GetYear();
  m_DateValue.month = dateValue.GetMonth()+1;
	m_DateValue.day = dateValue.GetDay();
    	    
  m_DateValue.hour = dateValue.GetHour();
  m_DateValue.minute = dateValue.GetMinute();
  m_DateValue.second = dateValue.GetSecond();
  m_DateValue.fraction = dateValue.GetMillisecond();

  m_nBufferLength = 0;
}

wxOdbcParameter::wxOdbcParameter(const void* pData, long nDataLength)
{
  m_nParameterType = wxOdbcParameter::PARAM_BLOB;
  void* pBuffer = m_BufferValue.GetWriteBuf(nDataLength);
  memcpy(pBuffer, pData, nDataLength);
  m_nBufferLength = nDataLength;
}


long wxOdbcParameter::GetDataLength()
{
  if (m_nParameterType == wxOdbcParameter::PARAM_NULL)
  {
    return 0;
  }
  else
  {
    return m_nBufferLength;
  }
}

long long* wxOdbcParameter::GetDataLengthPointer()
{
  if (m_nParameterType == wxOdbcParameter::PARAM_NULL)
  {
    return &m_nBufferLength;
  }
  else
  {
    return NULL;
  }
}

void* wxOdbcParameter::GetDataPtr()
{
  const void *pReturn = NULL;
  
  switch (m_nParameterType)
  {
    case wxOdbcParameter::PARAM_STRING:
      m_CharBufferValue = ConvertToUnicodeStream(m_strValue);
      pReturn = m_CharBufferValue;
      //wxPrintf(_("Parameter: '%s'\n"), m_strValue.c_str());
      //pReturn = m_strValue.c_str();
      break;
    case wxOdbcParameter::PARAM_INT:
      pReturn = &m_nValue;
      break;
    case wxOdbcParameter::PARAM_DOUBLE:
      pReturn = &m_dblValue;
      break;
    case wxOdbcParameter::PARAM_DATETIME:
      pReturn = &m_DateValue;
      break;
    case wxOdbcParameter::PARAM_BOOL:
      pReturn = &m_bValue;
      break;
    case wxOdbcParameter::PARAM_BLOB:
      pReturn = m_BufferValue.GetData();
      break;
    case wxOdbcParameter::PARAM_NULL:
      pReturn = NULL;
      break;
    default:
      pReturn = NULL;
      break;
  };
  return (void*)pReturn;
}

SQLSMALLINT wxOdbcParameter::GetValueType()
{
  SQLSMALLINT nReturn = SQL_C_LONG;
  
  switch (m_nParameterType)
  {
    case wxOdbcParameter::PARAM_STRING:
      nReturn = SQL_C_CHAR;
      break;
    case wxOdbcParameter::PARAM_INT:
      nReturn = SQL_C_LONG;
      break;
    case wxOdbcParameter::PARAM_DOUBLE:
      nReturn = SQL_C_DOUBLE;
      break;
    case wxOdbcParameter::PARAM_DATETIME:
      nReturn = SQL_C_TYPE_TIMESTAMP;
      break;
    case wxOdbcParameter::PARAM_BOOL:
      nReturn = SQL_C_LONG;
      break;
    case wxOdbcParameter::PARAM_BLOB:
      nReturn = SQL_C_BINARY;
      break;
    case wxOdbcParameter::PARAM_NULL:
      nReturn = SQL_C_CHAR;
      break;
    default:
      nReturn = SQL_C_NUMERIC;
      break;
  };
  return nReturn;
}

SQLSMALLINT wxOdbcParameter::GetParameterType()
{
  SQLSMALLINT nReturn = SQL_INTEGER;
  
  switch (m_nParameterType)
  {
    case wxOdbcParameter::PARAM_STRING:
      nReturn = SQL_VARCHAR;
      break;
    case wxOdbcParameter::PARAM_INT:
      nReturn = SQL_INTEGER;
      break;
    case wxOdbcParameter::PARAM_DOUBLE:
      nReturn = SQL_DOUBLE;
      break;
    case wxOdbcParameter::PARAM_DATETIME:
      //nReturn = SQL_TYPE_TIMESTAMP;
      nReturn = SQL_TIMESTAMP;
      break;
    case wxOdbcParameter::PARAM_BOOL:
      nReturn = SQL_INTEGER;
      break;
    case wxOdbcParameter::PARAM_BLOB:
      nReturn = SQL_BINARY;
      //nReturn = SQL_LONGVARBINARY;
      break;
    case wxOdbcParameter::PARAM_NULL:
      nReturn = SQL_NULL_DATA;
      break;
    default:
      nReturn = SQL_INTEGER;
      break;
  };
  return nReturn;

}

bool wxOdbcParameter::IsBinary()
{
  return (wxOdbcParameter::PARAM_BLOB == m_nParameterType);
}

SQLSMALLINT wxOdbcParameter::GetDecimalDigits()
{
  // either decimal_digits or 0 (date, bool, int)
  SQLSMALLINT nReturn = 0;
  
  switch (m_nParameterType)
  {
    case wxOdbcParameter::PARAM_STRING:
    case wxOdbcParameter::PARAM_BLOB:
    case wxOdbcParameter::PARAM_INT:
    case wxOdbcParameter::PARAM_DATETIME:
    case wxOdbcParameter::PARAM_BOOL:
    case wxOdbcParameter::PARAM_NULL:
      nReturn = 0;
      break;
    case wxOdbcParameter::PARAM_DOUBLE:
      nReturn = 11;
      break;
    default:
      nReturn = 0;
      break;
  };
  return nReturn;
}

SQLUINTEGER wxOdbcParameter::GetColumnSize()
{
  SQLUINTEGER nReturn = 0;

  switch (m_nParameterType)
  {
    case wxOdbcParameter::PARAM_STRING:
    case wxOdbcParameter::PARAM_BLOB:
      nReturn = m_nBufferLength;
      break;
    case wxOdbcParameter::PARAM_INT:
    case wxOdbcParameter::PARAM_DOUBLE:
    case wxOdbcParameter::PARAM_BOOL:
    case wxOdbcParameter::PARAM_NULL:
      nReturn = 0;
      break;
    case wxOdbcParameter::PARAM_DATETIME:
      nReturn = 19;
      break;
    default:
      nReturn = 0;
      break;
  };
  return nReturn;
}

SQLLEN* wxOdbcParameter::GetParameterLengthPtr()
{
  return &m_nBufferLength;
}

#endif//wxUSE_DATABASE_ODBC
