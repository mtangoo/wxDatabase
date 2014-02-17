#ifndef _WX_DATABASE_ODBC_RESULT_SET_METADATA_H_
#define _WX_DATABASE_ODBC_RESULT_SET_METADATA_H_

#include "wx/database/wxprec.h"

class wxOdbcResultSetMetaData : public wxResultSetMetaData
{
public:
  // ctor
  wxOdbcResultSetMetaData(wxOdbcInterface* pInterface, SQLHSTMT sqlOdbcStatement);

  // dtor
  virtual ~wxOdbcResultSetMetaData() { }

  virtual int GetColumnType(int i);
  virtual int GetColumnSize(int i);
  virtual wxString GetColumnName(int i);
  virtual int GetColumnCount();
  
private:
  wxOdbcInterface* m_pInterface;
  SQLHSTMT m_pOdbcStatement;
};

#endif//_WX_DATABASE_ODBC_RESULT_SET_METADATA_H_
