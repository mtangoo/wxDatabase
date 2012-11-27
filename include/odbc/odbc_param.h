#ifndef __ODBC_PARAMETER_H__
#define __ODBC_PARAMETER_H__

// For compilers that support precompilation, includes "wx.h".
#include "wx/wxprec.h"

#ifdef __BORLANDC__
    #pragma hdrstop
#endif

#ifndef WX_PRECOMP
    #include "wx/wx.h"
#endif

#include "wx/datetime.h"

#include "../strconverter.h"

#include <sql.h>

class wxOdbcParameter : public wxDatabaseStringConverter
{
public:
  // ctor
  wxOdbcParameter();
  wxOdbcParameter(const wxString& strValue);
  wxOdbcParameter(int nValue);
  wxOdbcParameter(double dblValue);
  wxOdbcParameter(bool bValue);
  wxOdbcParameter(const wxDateTime& dateValue);
  wxOdbcParameter(const void* pData, long nDataLength);

  // dtor
  virtual ~wxOdbcParameter()  { }

  enum {
    PARAM_STRING = 0,
    PARAM_INT,
    PARAM_DOUBLE,
    PARAM_DATETIME,
    PARAM_BOOL,
    PARAM_BLOB,
    PARAM_NULL
  };
   
  long GetDataLength();
  long* GetDataLengthPointer();
  
  void* GetDataPtr();
  SQLSMALLINT GetValueType();
  SQLSMALLINT GetParameterType();
  SQLSMALLINT GetDecimalDigits();
  SQLUINTEGER GetColumnSize();

  SQLINTEGER* GetParameterLengthPtr(); // ???

  bool IsBinary();

private:
  int m_nParameterType;
  
  // A union would probably be better here
  TIMESTAMP_STRUCT m_DateValue;
  wxString m_strValue;
  long m_nValue;
  double m_dblValue;
  wxString m_strDateValue;
  bool m_bValue;
  wxMemoryBuffer m_BufferValue;
  wxCharBuffer m_CharBufferValue;
  long m_nBufferLength;
  
};


#endif // __ODBC_PARAMETER_H__
