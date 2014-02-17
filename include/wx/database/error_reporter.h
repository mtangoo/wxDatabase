#ifndef _WX_DATABASE_ERROR_REPORTER_H_
#define _WX_DATABASE_ERROR_REPORTER_H_

#include "wx/database/wxprec.h"

class WXDLLIMPEXP_DATABASE wxDatabaseErrorReporter
{
public:
  // ctor
  wxDatabaseErrorReporter();

  // dtor
  virtual ~wxDatabaseErrorReporter();

  const wxString& GetErrorMessage();
  int GetErrorCode();
 
  void ResetErrorCodes();
  
protected:
  void SetErrorMessage(const wxString& strErrorMessage);
  void SetErrorCode(int nErrorCode);

  void ThrowDatabaseException();
  
private:
  wxString m_strErrorMessage;
  int m_nErrorCode;
};

#endif//_WX_DATABASE_ERROR_REPORTER_H_

