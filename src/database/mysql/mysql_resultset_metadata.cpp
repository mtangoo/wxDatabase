#include "wx/database/wxprec.h"

#if wxUSE_DATABASE_MYSQL

#include "mysql_com.h"

// ctor
wxMysqlResultSetMetaData::wxMysqlResultSetMetaData(wxMysqlDynamicInterface* pInterface, MYSQL_RES* pMetaData)
{
  m_pInterface = pInterface;
  m_pMetaData = pMetaData;
}

// dtor
wxMysqlResultSetMetaData::~wxMysqlResultSetMetaData()
{
  m_pInterface->GetMysqlFreeResult()(m_pMetaData);
}

int wxMysqlResultSetMetaData::GetColumnType(int i)
{
  int returnType = COLUMN_UNKNOWN;
  MYSQL_FIELD* pField = GetColumn(i);
  if (pField)
  {
    switch (pField->type)
    {
      case MYSQL_TYPE_SHORT:
      case MYSQL_TYPE_TINY:
      case MYSQL_TYPE_LONG:
      case MYSQL_TYPE_LONGLONG:
      case MYSQL_TYPE_INT24:
        returnType = COLUMN_INTEGER;
        break;
      case MYSQL_TYPE_FLOAT:
      case MYSQL_TYPE_DOUBLE:
        returnType = COLUMN_DOUBLE;
        break;
      case MYSQL_TYPE_DATE:
      case MYSQL_TYPE_DATETIME:
      case MYSQL_TYPE_NEWDATE:
      case MYSQL_TYPE_TIMESTAMP:
      case MYSQL_TYPE_TIME:
        returnType = COLUMN_DATE;
        break;
      case MYSQL_TYPE_TINY_BLOB:
      case MYSQL_TYPE_MEDIUM_BLOB:
      case MYSQL_TYPE_LONG_BLOB:
      case MYSQL_TYPE_BLOB:
        returnType = COLUMN_BLOB;
        break;
      case MYSQL_TYPE_VAR_STRING:
      case MYSQL_TYPE_STRING:
        returnType = COLUMN_STRING;
        break;
      default:
        wxPrintf(_("Don't understand type : %d\n"), returnType);
        returnType = COLUMN_UNKNOWN;
        break;
    };
  }
  return returnType;
}

int wxMysqlResultSetMetaData::GetColumnSize(int i)
{
  MYSQL_FIELD* pField = GetColumn(i);
  if (pField)
  {
    return pField->length;
  }
  return -1;
}

wxString wxMysqlResultSetMetaData::GetColumnName(int i)
{
  MYSQL_FIELD* pField = GetColumn(i);
  if (pField)
  {
    wxString columnName = ConvertFromUnicodeStream(pField->name);
    return columnName;
  }
  return wxEmptyString;
}

int wxMysqlResultSetMetaData::GetColumnCount()
{
  return m_pInterface->GetMysqlNumFields()(m_pMetaData);
}

MYSQL_FIELD* wxMysqlResultSetMetaData::GetColumn(int nField)
{
  MYSQL_FIELD* pFields = m_pMetaData->fields;
  return &(pFields[nField-1]);
}

#endif//wxUSE_DATABASE_MYSQL
