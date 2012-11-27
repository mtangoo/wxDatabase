#ifndef __POSTGRESQL_PREPARED_STATEMENT_PARAMETER_COLLECTION_H__
#define __POSTGRESQL_PREPARED_STATEMENT_PARAMETER_COLLECTION_H__

// For compilers that support precompilation, includes "wx.h".
#include "wx/wxprec.h"

#ifdef __BORLANDC__
    #pragma hdrstop
#endif

#ifndef WX_PRECOMP
    #include "wx/wx.h"
#endif

#include "include/postgresql/postgresql_param.h"

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

#endif // __POSTGRESQL_PREPARED_STATEMENT_PARAMETER_COLLECTION_H__

