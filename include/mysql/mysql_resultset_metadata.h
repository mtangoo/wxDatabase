#ifndef __MYSQL_RESULT_SET_METADATA_H__
#define __MYSQL_RESULT_SET_METADATA_H__

// For compilers that support precompilation, includes "wx.h".
#include "wx/wxprec.h"

#ifdef __BORLANDC__
    #pragma hdrstop
#endif

#ifndef WX_PRECOMP
    #include "wx/wx.h"
#endif


#include "mysql_interface.h"
#include "mysql_resultset_metadata.h"
#include "../resultset_metadata.h"
#include "mysql.h"

class wxMysqlResultSetMetaData : public wxResultSetMetaData
{
public:
  // ctor
  wxMysqlResultSetMetaData(wxMysqlDynamicInterface* pInterface, MYSQL_RES* pMetaData);

  // dtor
  virtual ~wxMysqlResultSetMetaData();

  virtual int GetColumnType(int i);
  virtual int GetColumnSize(int i);
  virtual wxString GetColumnName(int i);
  virtual int GetColumnCount();
  
private:
  MYSQL_FIELD* GetColumn(int nField);

  wxMysqlDynamicInterface* m_pInterface;
  MYSQL_RES* m_pMetaData;
};

#endif // __MYSQL_RESULT_SET_METADATA_H__
