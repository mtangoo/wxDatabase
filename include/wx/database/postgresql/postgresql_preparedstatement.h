#ifndef _WX_DATABASE_POSTGRESQL_PREPARED_STATEMENT_H_
#define _WX_DATABASE_POSTGRESQL_PREPARED_STATEMENT_H_

#include "wx/database/wxprec.h"
 
WX_DECLARE_OBJARRAY(wxPostgresPreparedStatementWrapper, ArrayOfPostgresPreparedStatementWrappers);

class wxPostgresPreparedStatement : public wxPreparedStatement
{
public:
  // ctor
  wxPostgresPreparedStatement(wxDynamicPostgresInterface* pInterface);
  wxPostgresPreparedStatement(wxDynamicPostgresInterface* pInterface, PGconn* pDatabase, const wxString& strSQL, const wxString& strStatementName);

  // dtor
  virtual ~wxPostgresPreparedStatement();

  virtual void Close();

  void AddStatement(PGconn* pDatabase, const wxString& strSQL, const wxString& strStatementName);
  static wxPostgresPreparedStatement* CreateStatement(wxDynamicPostgresInterface* pInterface, PGconn* pDatabase, const wxString& strSQL);
  
  // set field
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

  static wxString TranslateSQL(const wxString& strOriginalSQL);

private:
  wxDynamicPostgresInterface* m_pInterface;
  int FindStatementAndAdjustPositionIndex(int* pPosition);
  static wxString GenerateRandomStatementName();

  ArrayOfPostgresPreparedStatementWrappers m_Statements;
};

#endif//_WX_DATABASE_POSTGRESQL_PREPARED_STATEMENT_H_

