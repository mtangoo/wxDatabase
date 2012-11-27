#ifndef __SQLITE_RESULT_SET_H__
#define __SQLITE_RESULT_SET_H__

#include "../resultset.h"

#include "sqlite3.h"

class wxSqlitePreparedStatement;
class wxResultSetMetaData;

class wxSqliteResultSet : public wxDatabaseResultSet
{
public:
  // ctor
  wxSqliteResultSet();
  wxSqliteResultSet(wxSqlitePreparedStatement* pStatement, bool bManageStatement = false);

  // dtor
  virtual ~wxSqliteResultSet();
  
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
  
  wxSqlitePreparedStatement* m_pStatement;
  sqlite3_stmt* m_pSqliteStatement;

  StringToIntMap m_FieldLookupMap;

  bool m_bManageStatement;
};

#endif // __SQLITE_RESULT_SET_H__

