#include "wx/database/wxprec.h"

#if wxUSE_DATABASE_TDS

#include "wx/arrimpl.cpp"
WX_DEFINE_OBJARRAY(ArrayOfTdsColumns);

// ctor
wxTdsResultSetMetaData::wxTdsResultSetMetaData(TDSRESULTINFO* pInfo)
{
  for (int i=0; i<pInfo->num_cols; i++)
  {
    wxTdsColumnData column(pInfo->columns[i]);
    m_Columns.push_back(column);
  }
}

int wxTdsResultSetMetaData::GetColumnType(int i)
{
  return m_Columns[i-1].GetColumnType();
}

int wxTdsResultSetMetaData::GetColumnSize(int i)
{
  return m_Columns[i-1].GetColumnSize();
}

wxString wxTdsResultSetMetaData::GetColumnName(int i)
{
  return m_Columns[i-1].GetColumnName();
}

int wxTdsResultSetMetaData::GetColumnCount()
{
  return m_Columns.size();
}

#endif//wxUSE_DATABASE_TDS

