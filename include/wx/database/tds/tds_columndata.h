#ifndef _WX_DATABASE_TDS_RESULT_SET_COLUMN_METADATA_H_
#define _WX_DATABASE_TDS_RESULT_SET_COLUMN_METADATA_H_

#include "wx/database/wxprec.h"

class wxTdsColumnData
{
public:
  // ctor
  wxTdsColumnData(TDSCOLUMN* pColumn);

  int GetColumnType() { return m_nColumnType; }
  int GetColumnSize() { return m_nColumnSize; }
  wxString GetColumnName() { return m_strColumnName; }

private:
  int GetColumnType(TDSCOLUMN* pColumn);
  int GetColumnSize(TDSCOLUMN* pColumn);
  wxString GetColumnName(TDSCOLUMN* pColumn);

  int m_nColumnType;
  int m_nColumnSize;
  wxString m_strColumnName;
};

#endif//_WX_DATABASE_TDS_RESULT_SET_COLUMN_METADATA_H_

