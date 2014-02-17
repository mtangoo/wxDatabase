#ifndef _WX_DATABASE_MYSQL_PREPARED_STATEMENT_WRAPPER_H_
#define _WX_DATABASE_MYSQL_PREPARED_STATEMENT_WRAPPER_H_

#include "wx/database/wxprec.h"
#include "wx/database/mysql/mysql_preparedstatement_parameter_collection.h"

class wxDatabaseResultSet;

class wxMysqlPreparedStatementWrapper : public wxDatabaseErrorReporter, public wxDatabaseStringConverter
{
public:
  // ctor
  wxMysqlPreparedStatementWrapper(wxMysqlDynamicInterface* pInterface, MYSQL_STMT* pStatement);

  // dtor
  virtual ~wxMysqlPreparedStatementWrapper();

  void Close();

  // set field
  void SetParam(int nPosition, int nValue);
  void SetParam(int nPosition, double dblValue);
  void SetParam(int nPosition, const wxString& strValue);
  void SetParam(int nPosition);
  void SetParam(int nPosition, const void* pData, long nDataLength);
  void SetParam(int nPosition, const wxDateTime& dateValue);
  void SetParam(int nPosition, bool bValue);
  int GetParameterCount();
  
  int RunQuery();
  wxDatabaseResultSet* RunQueryWithResults();

private:  
  wxMysqlDynamicInterface* m_pInterface;
  MYSQL_STMT* m_pStatement;

  wxMysqlPreparedStatementParameterCollection m_Parameters;
};

#endif//_WX_DATABASE_MYSQL_PREPARED_STATEMENT_WRAPPER_H_

