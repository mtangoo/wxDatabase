#include "wx/database/wxprec.h"

#if wxUSE_DATABASE_POSTGRESQL

//#include "server/catalog/pg_type.h"

/* 
   SPECIAL NOTE:
    The PQf* functions are 0-based, but the JDBC wxResultSetMetaData
    functions are 1-based.  To be consistent with the 1-based JDBC (and also
    wxPreparedStatement paramters, we are using a 1-based system here.)
 */

wxPostgresResultSetMetaData::wxPostgresResultSetMetaData(wxDynamicPostgresInterface* pInterface, PGresult* pResult)
{
  m_pInterface = pInterface;
  m_pResult = pResult;
}

int wxPostgresResultSetMetaData::GetColumnType(int i)
{
  int columnType = m_pInterface->GetPQftype()(m_pResult, i-1);
  int returnType = COLUMN_UNKNOWN;
  switch (columnType)
  {
    case 20://INT8OID:
    case 21://INT2OID:
    case 23://INT4OID:
    case 26://OIDOID:
      returnType = COLUMN_INTEGER;
      break;
    case 16://BOOLOID:
      returnType = COLUMN_BOOL;
      break;
    case 1043://VARCHAROID:
    case 25://TEXTOID:
    case 2275://CSTRINGOID:
      returnType = COLUMN_STRING;
      break;
    case 700://FLOAT4OID:
    case 701://FLOAT8OID:
      returnType = COLUMN_DOUBLE;
      break;
    case 17://BYTEAOID:
      returnType = COLUMN_BLOB;
      break;
    case 1082://DATEOID:
    case 1083://TIMEOID:
    case 1114://TIMESTAMPOID:
    case 1184://TIMESTAMPTZOID:
      returnType = COLUMN_DATE;
      break;
    default:
      returnType = COLUMN_UNKNOWN;
      break;
  };
  return returnType;
}

int wxPostgresResultSetMetaData::GetColumnSize(int i)
{
  return m_pInterface->GetPQfsize()(m_pResult, i-1);
}

wxString wxPostgresResultSetMetaData::GetColumnName(int i)
{
  wxString columnName = ConvertFromUnicodeStream(m_pInterface->GetPQfname()(m_pResult, i-1));
  return columnName;
}

int wxPostgresResultSetMetaData::GetColumnCount()
{
  return m_pInterface->GetPQnfields()(m_pResult);
}

#endif//wxUSE_DATABASE_POSTGRESQL

