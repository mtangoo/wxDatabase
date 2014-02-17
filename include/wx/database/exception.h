#ifndef _WX_DATABASE_EXCEPTION_H_
#define _WX_DATABASE_EXCEPTION_H_

#include "wx/database/wxprec.h"

#if wxUSE_DATABASE_EXCEPTIONS

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

#endif//wxUSE_DATABASE_EXCEPTIONS

#endif//_WX_DATABASE_EXCEPTION_H_
