#ifndef _WX_DATABASE_MYSQL_PREPARED_STATEMENT_PARAMETER_COLLECTION_H_
#define _WX_DATABASE_MYSQL_PREPARED_STATEMENT_PARAMETER_COLLECTION_H_

#include "wx/database/wxprec.h"

WX_DEFINE_ARRAY_PTR(wxMysqlParameter*, MysqlParameterArray);

class wxMysqlPreparedStatementParameterCollection : public wxDatabaseStringConverter
{
public:
  // ctor
  wxMysqlPreparedStatementParameterCollection();

  // dtor
  virtual ~wxMysqlPreparedStatementParameterCollection();

  int GetSize();
  MYSQL_BIND* GetMysqlParameterBindings();

  void SetParam(int nPosition, int nValue);
  void SetParam(int nPosition, double dblValue);
  void SetParam(int nPosition, const wxString& strValue);
  void SetParam(int nPosition);
  void SetParam(int nPosition, const void* pData, long nDataLength);
  void SetParam(int nPosition, const wxDateTime& dateValue);
  void SetParam(int nPosition, bool bValue);
  void SetParam(int nPosition, wxMysqlParameter* pParameter);
  
private:
  MysqlParameterArray m_Parameters;
};

#endif//_WX_DATABASE_MYSQL_PREPARED_STATEMENT_PARAMETER_COLLECTION_H_

