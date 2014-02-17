#ifndef __POSTGRES_RESULT_SET_METADATA_H__
#define __POSTGRES_RESULT_SET_METADATA_H__

#include "wx/database/wxprec.h"

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
