#ifndef _WX_DATABASE_MYSQL_PREPARED_STATEMENT_RESULT_SET_H_
#define _WX_DATABASE_MYSQL_PREPARED_STATEMENT_RESULT_SET_H_

#include "wx/database/wxprec.h"

WX_DECLARE_HASH_MAP(int, wxMysqlPreparedStatementParameter*, wxIntegerHash, wxIntegerEqual, IntToMysqlParameterMap);
  
class wxMysqlPreparedStatementResultSet : public wxDatabaseResultSet
{
public:
  // ctor
  wxMysqlPreparedStatementResultSet(wxMysqlDynamicInterface* pInterface);
  wxMysqlPreparedStatementResultSet(wxMysqlDynamicInterface* pInterface, MYSQL_STMT* pStatement, bool bManageStatement = false);

  //dtor
  virtual ~wxMysqlPreparedStatementResultSet();
  
  virtual bool Next();
  virtual void Close();

  virtual int LookupField(const wxString& strField);

  // get field
  virtual int GetResultInt(int nField);
  virtual wxString GetResultString(int nField);
  virtual long GetResultLong(int nField);
  virtual bool GetResultBool(int nField);
  virtual wxDateTime GetResultDate(int nField);
  virtual void* GetResultBlob(int nField, wxMemoryBuffer& Buffer);
  virtual double GetResultDouble(int nField);
  virtual bool IsFieldNull(int nField);

  // get MetaData
  virtual wxResultSetMetaData* GetMetaData();

private:
  void ClearPreviousData();
  MYSQL_BIND* GetResultBinding(int nField);

  wxMysqlDynamicInterface* m_pInterface; 
  MYSQL_STMT* m_pStatement;
  MYSQL_BIND* m_pResultBindings;
  
  StringToIntMap m_FieldLookupMap;

  IntToMysqlParameterMap m_BindingWrappers;

  bool m_bManageStatement;
};

#endif//_WX_DATABASE_MYSQL_PREPARED_STATEMENT_RESULT_SET_H_

