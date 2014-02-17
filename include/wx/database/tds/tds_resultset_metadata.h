#ifndef _WX_DATABASE_TDS_RESULT_SET_METADATA_H_
#define _WX_DATABASE_TDS_RESULT_SET_METADATA_H_

#include "wx/database/wxprec.h"

WX_DECLARE_OBJARRAY(wxTdsColumnData, ArrayOfTdsColumns);

class wxTdsResultSetMetaData : public wxResultSetMetaData
{
public:
  // ctor
  wxTdsResultSetMetaData(TDSRESULTINFO* pInfo);

  virtual int GetColumnType(int i);
  virtual int GetColumnSize(int i);
  virtual wxString GetColumnName(int i);
  virtual int GetColumnCount();
  
private:
  void CreateResultSetMetaData();

  ArrayOfTdsColumns m_Columns;
};

#endif//_WX_DATABASE_TDS_RESULT_SET_METADATA_H_
