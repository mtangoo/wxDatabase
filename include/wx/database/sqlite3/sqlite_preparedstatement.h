#ifndef _WX_DATABASE_SQLITE_PREPARED_STATEMENT_H_
#define _WX_DATABASE_SQLITE_PREPARED_STATEMENT_H_

#include "wx/database/wxprec.h"

WX_DEFINE_ARRAY_PTR(sqlite3_stmt*, wxSqliteStatementVector);

class wxDatabaseResultSet;

class wxSqlitePreparedStatement : public wxPreparedStatement
{
public:
  // ctor
  wxSqlitePreparedStatement(sqlite3* pDatabase);
  wxSqlitePreparedStatement(sqlite3* pDatabase, sqlite3_stmt* pStatement);
  wxSqlitePreparedStatement(sqlite3* pDatabase, wxSqliteStatementVector statements);

  // dtor
  virtual ~wxSqlitePreparedStatement();

  virtual void Close();
  
  void AddPreparedStatement(sqlite3_stmt* pStatement);

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

  sqlite3_stmt* GetLastStatement() { return (m_Statements.size() > 0) ? m_Statements[m_Statements.size()-1] : NULL; }

private:
  int FindStatementAndAdjustPositionIndex(int* pPosition);
 
  sqlite3* m_pDatabase; // Database pointer needed for error messages
  wxSqliteStatementVector m_Statements;
};

#endif//_WX_DATABASE_SQLITE_PREPARED_STATEMENT_H_

