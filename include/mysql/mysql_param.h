#ifndef __MYSQL_PARAMETER_H__
#define __MYSQL_PARAMETER_H__

// For compilers that support precompilation, includes "wx.h".
#include "wx/wxprec.h"

#ifdef __BORLANDC__
    #pragma hdrstop
#endif

#ifndef WX_PRECOMP
    #include "wx/wx.h"
#endif

#include "wx/datetime.h"
#include "include/strconverter.h"


#include "mysql.h"

class wxMysqlParameter : public wxDatabaseStringConverter
{
public:
  // ctor
  wxMysqlParameter();
  wxMysqlParameter(const wxString& strValue);
  wxMysqlParameter(int nValue);
  wxMysqlParameter(double dblValue);
  wxMysqlParameter(bool bValue);
  wxMysqlParameter(const wxDateTime& dateValue);
  wxMysqlParameter(const void* pData, long nDataLength);

  // dtor
  virtual ~wxMysqlParameter();

  enum {
    PARAM_STRING = 0,
    PARAM_INT,
    PARAM_DOUBLE,
    PARAM_DATETIME,
    PARAM_BOOL,
    PARAM_BLOB,
    PARAM_NULL
  };
   
  long unsigned int GetDataLength();
  long unsigned int* GetDataLengthPtr();
  
  const void* GetDataPtr();
  int GetParameterType();

  enum_field_types GetBufferType();

private:
  int m_nParameterType;
  
  // A union would probably be better here
  wxString m_strValue;
  int m_nValue;
  double m_dblValue;
  MYSQL_TIME* m_pDate;
  bool m_bValue;
  wxMemoryBuffer m_BufferValue;
  wxCharBuffer m_CharBufferValue;
  long unsigned int m_nBufferLength;
  
};


#endif // __MYSQL_PARAMETER_H__
