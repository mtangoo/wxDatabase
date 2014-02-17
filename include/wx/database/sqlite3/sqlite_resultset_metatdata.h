#ifndef _WX_DATABASE_SQLITE_RESULT_SET_METADATA_H_
#define _WX_DATABASE_SQLITE_RESULT_SET_METADATA_H_

#include "wx/database/wxprec.h"

class wxSqliteResultSetMetaData : public wxResultSetMetaData
{
public:
  // ctor
  wxSqliteResultSetMetaData(sqlite3_stmt* pStmt);

  virtual int GetColumnType(int i);
  virtual int GetColumnSize(int i);
  virtual wxString GetColumnName(int i);
  virtual int GetColumnCount();
  
private:
  sqlite3_stmt* m_pSqliteStatement;
};

#endif//_WX_DATABASE_SQLITE_RESULT_SET_METADATA_H_
