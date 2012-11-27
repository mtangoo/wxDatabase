#ifndef __wxDATABASE_ERROR_REPORTER_H__
#define __wxDATABASE_ERROR_REPORTER_H__

// For compilers that support precompilation, includes "wx.h".
#include "wx/wxprec.h"

#ifdef __BORLANDC__
    #pragma hdrstop
#endif

#ifndef WX_PRECOMP
    #include "wx/wx.h"
#endif

#include "error_reporter.h"
#include "defs.h"



class WXDLLIMPEXP_DATABASELAYER wxDatabaseErrorReporter
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

#endif // __wxDATABASE_ERROR_REPORTER_H__

