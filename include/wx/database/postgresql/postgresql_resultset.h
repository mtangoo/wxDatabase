#ifndef _WX_DATABASE_POSTGRESQL_RESULT_SET_H_
#define _WX_DATABASE_POSTGRESQL_RESULT_SET_H_

#include "wx/database/wxprec.h"

class wxPostgresResultSet : public wxDatabaseResultSet
{
public:
  // ctor
  wxPostgresResultSet(wxDynamicPostgresInterface* pInterface);
  wxPostgresResultSet(wxDynamicPostgresInterface* pInterface, PGresult* pResult);

  // dtor
  virtual ~wxPostgresResultSet();
  
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
  wxDynamicPostgresInterface* m_pInterface; 
  PGresult* m_pResult;

  StringToIntMap m_FieldLookupMap;
  int m_nCurrentRow;
  int m_nTotalRows;
  bool m_bBinaryResults;
};

#endif//_WX_DATABASE_POSTGRESQL_RESULT_SET_H_

