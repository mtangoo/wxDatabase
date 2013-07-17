#ifndef __ODBC_RESULT_SET_METADATA_H__
#define __ODBC_RESULT_SET_METADATA_H__

// For compilers that support precompilation, includes "wx.h".
#include "wx/wxprec.h"

#ifdef __BORLANDC__
    #pragma hdrstop
#endif

#ifndef WX_PRECOMP
    #include "wx/wx.h"
#endif

#include "odbc_inteface.h"
#include "../resultset_metadata.h" 

#include <sql.h>

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

#endif // __ODBC_RESULT_SET_METADATA_H__
