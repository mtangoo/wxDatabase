#ifndef _WX_DATABASE_TDS_RESULT_SET_H_
#define _WX_DATABASE_TDS_RESULT_SET_H_

#include "wx/database/wxprec.h"

class wxResultSetMetaData;

class wxTdsResultSet : public wxDatabaseResultSet
{
public:
  // ctor
  wxTdsResultSet(TDSSOCKET* pDatabase);

  // dtor
  virtual ~wxTdsResultSet();
  
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
  void FreeResultSets();
  void PopulateFieldMap();
  void CreateResultSetMetaData();
  void SetErrorInformationFromDatabaseLayer();

  StringToIntMap m_FieldLookupMap;

  TDSSOCKET* m_pDatabase;
  TDSRESULTINFO* m_pResultInfo;
  wxResultSetMetaData* m_pResultSetMetaData;
};

#endif//_WX_DATABASE_TDS_RESULT_SET_H_

