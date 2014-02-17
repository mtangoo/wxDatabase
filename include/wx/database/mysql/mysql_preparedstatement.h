#ifndef _WX_DATABASE_MYSQL_PREPARED_STATEMENT_H_
#define _WX_DATABASE_MYSQL_PREPARED_STATEMENT_H_

#include "wx/database/wxprec.h"
#include "wx/database/mysql/mysql_preparedstatement_wrapper.h"

class wxDatabaseResultSet;

WX_DEFINE_ARRAY_PTR(wxMysqlPreparedStatementWrapper*, MysqlStatementWrapperArray);
    
class wxMysqlPreparedStatement : public wxPreparedStatement
{
public:
  // ctor
  wxMysqlPreparedStatement(wxMysqlDynamicInterface* pInterface);
  wxMysqlPreparedStatement(wxMysqlDynamicInterface* pInterface, MYSQL_STMT* pStatement);

  // dtor
  virtual ~wxMysqlPreparedStatement();

  virtual void Close();
  
  void AddPreparedStatement(MYSQL_STMT* pStatement);
  
  // get field
  virtual void SetParamInt(int nPosition, int nValue);
  virtual void SetParamDouble(int nPosition, double dblValue);
  virtual void SetParamString(int nPosition, const wxString& strValue);
  virtual void SetParamNull(int nPosition);
  virtual void SetParamBlob(int nPosition, const void* pData, long nDataLength);
  virtual void SetParamDate(int nPosition, const wxDateTime& dateValue);
  virtual void SetParamBool(int nPosition, bool bValue);
  virtual int GetParameterCount();
  
  virtual int RunQuery();
  virtual wxDatabaseResultSet* RunQueryWithResults();

private:
  int FindStatementAndAdjustPositionIndex(int* pPosition);

  wxMysqlDynamicInterface* m_pInterface;
  MysqlStatementWrapperArray m_Statements;
};

#endif//_WX_DATABASE_MYSQL_PREPARED_STATEMENT_H_

