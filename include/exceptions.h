
#ifndef __wxDATABASE_EXCEPTION_H__
#define __wxDATABASE_EXCEPTION_H__

#ifndef DONT_USE_DATABASE_EXCEPTIONS

#include <wx/string.h>

class wxDatabaseException
{
public:
  wxDatabaseException(int nCode, const wxString& strError)
  {
    m_nErrorCode = nCode;
    m_strErrorMessage = strError;
  }

  const wxString& GetErrorMessage() const { return m_strErrorMessage; }
  const int GetErrorCode() const { return m_nErrorCode; }
  
  // Add functions for stack traces ??
private:
  wxString m_strErrorMessage;
  int m_nErrorCode;  
};

#endif // DONT_USE_DATABASE_EXCEPTIONS

#endif // __wxDATABASE_EXCEPTION_H__
