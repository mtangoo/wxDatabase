#ifndef _WX_DATABASE_MYSQL_RESULT_SET_METADATA_H_
#define _WX_DATABASE_MYSQL_RESULT_SET_METADATA_H_

#include "wx/database/wxprec.h"

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

#endif//_WX_DATABASE_MYSQL_RESULT_SET_METADATA_H_
