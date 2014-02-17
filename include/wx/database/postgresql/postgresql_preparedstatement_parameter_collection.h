#ifndef _WX_DATABASE_POSTGRESQL_PREPARED_STATEMENT_PARAMETER_COLLECTION_H_
#define _WX_DATABASE_POSTGRESQL_PREPARED_STATEMENT_PARAMETER_COLLECTION_H_

#include "wx/database/wxprec.h"

WX_DECLARE_OBJARRAY(wxPostgresParameter, ArrayOfPostgresParameters);

class wxPostgresPreparedStatementParameterCollection
{
public:
  // dtor
  virtual ~wxPostgresPreparedStatementParameterCollection();

  int GetSize();
  char** GetParamValues();
  int* GetParamLengths();
  int* GetParamFormats();

  void SetParam(int nPosition, int nValue);
  void SetParam(int nPosition, double dblValue);
  void SetParam(int nPosition, const wxString& strValue);
  void SetParam(int nPosition);
  void SetParam(int nPosition, const void* pData, long nDataLength);
  void SetParam(int nPosition, const wxDateTime& dateValue);
  void SetParam(int nPosition, bool bValue);
  void SetParam(int nPosition, wxPostgresParameter& Parameter);
  
  
private:
  ArrayOfPostgresParameters m_Parameters;
};

#endif//_WX_DATABASE_POSTGRESQL_PREPARED_STATEMENT_PARAMETER_COLLECTION_H_

