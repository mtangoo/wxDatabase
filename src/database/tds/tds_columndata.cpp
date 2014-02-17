#include "wx/database/wxprec.h"

#if wxUSE_DATABASE_TDS

wxTdsColumnData::wxTdsColumnData(TDSCOLUMN* pColumn)
{
	m_nColumnType = GetColumnType(pColumn);
	m_nColumnSize = GetColumnSize(pColumn);
	m_strColumnName = GetColumnName(pColumn);
}

int wxTdsColumnData::GetColumnType(TDSCOLUMN* pColumn)
{
	int returnType = wxResultSetMetaData::COLUMN_UNKNOWN;
	switch (pColumn->column_type)
	{
	case SYBSINT1:
		//case SYBSINT2:
		//case SYBSINT4:
		//case SYBSINT8:
		//case SYBSUINT1:
		//case SYBSUINT2:
		//case SYBSUINT4:
		//case SYBSUINT8:
	case SYBINTN:
	case SYBNUMERIC:
		returnType = wxResultSetMetaData::COLUMN_INTEGER;
		break;
	case SYBDECIMAL:
	case SYBFLTN:
	case SYBMONEY:
	case SYBMONEYN:
		returnType = wxResultSetMetaData::COLUMN_DOUBLE;
		break;
	case SYBVARCHAR:
	case XSYBVARCHAR:
	case XSYBNVARCHAR:
		returnType = wxResultSetMetaData::COLUMN_STRING;
		break;
	case SYBVARBINARY:
	case SYBBINARY:
	case SYBIMAGE:
		returnType = wxResultSetMetaData::COLUMN_BLOB;
		break;
	case SYBDATETIME:
	case SYBDATETIMN:
		returnType = wxResultSetMetaData::COLUMN_DATE;
		break;
	default:
		returnType = wxResultSetMetaData::COLUMN_UNKNOWN;
		break;
	};

	return returnType;
}

int wxTdsColumnData::GetColumnSize(TDSCOLUMN* pColumn)
{
	return pColumn->column_size;
}

wxString wxTdsColumnData::GetColumnName(TDSCOLUMN* pColumn)
{
	// We might consider a version of ConvertFromUnicodeStream that excepts the string length parameter
	wxString colName(/*AML (wxChar*)*/pColumn->column_name, pColumn->column_namelen);
	return colName;
}

#endif//wxUSE_DATABASE_TDS
