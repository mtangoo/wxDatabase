#ifndef __POSTGRES_RESULT_SET_METADATA_H__
#define __POSTGRES_RESULT_SET_METADATA_H__

// For compilers that support precompilation, includes "wx.h".
#include "wx/wxprec.h"

#ifdef __BORLANDC__
    #pragma hdrstop
#endif

#ifndef WX_PRECOMP
    #include "wx/wx.h"
#endif

#include "include/resultset_metadata.h" 
#include "include/postgresql/postgresql_interface.h"
#include "include/postgresql/postgresql_resultset_metadata.h"


#include "libpq-fe.h"

class wxPostgresResultSetMetaData : public wxResultSetMetaData
{
public:
  // ctor
  wxPostgresResultSetMetaData(wxDynamicPostgresInterface* pInterface, PGresult* pResult);

  // dtor
  virtual ~wxPostgresResultSetMetaData() { }

  virtual int GetColumnType(int i);
  virtual int GetColumnSize(int i);
  virtual wxString GetColumnName(int i);
  virtual int GetColumnCount();
  
private:
  wxDynamicPostgresInterface* m_pInterface;
  PGresult* m_pResult;
};

#endif // __POSTGRES_RESULT_SET_METADATA_H__
