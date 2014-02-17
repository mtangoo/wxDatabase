#ifndef _WX_DATABASE_MYSQL_PARAMETER_H_
#define _WX_DATABASE_MYSQL_PARAMETER_H_

#include "wx/database/wxprec.h"

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


#endif//_WX_DATABASE_MYSQL_PARAMETER_H_
