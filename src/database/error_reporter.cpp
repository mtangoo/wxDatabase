#include "wx/database/wxprec.h"

wxDatabaseErrorReporter::wxDatabaseErrorReporter()
{
  ResetErrorCodes();
}

wxDatabaseErrorReporter::~wxDatabaseErrorReporter()
{
}

const wxString& wxDatabaseErrorReporter::GetErrorMessage()
{
  return m_strErrorMessage;
}

int wxDatabaseErrorReporter::GetErrorCode()
{
  return m_nErrorCode;
}

void wxDatabaseErrorReporter::SetErrorMessage(const wxString& strErrorMessage)
{
  m_strErrorMessage = strErrorMessage;
}

void wxDatabaseErrorReporter::SetErrorCode(int nErrorCode)
{
  m_nErrorCode = nErrorCode;
}

void wxDatabaseErrorReporter::ResetErrorCodes()
{
  m_strErrorMessage = wxT("");
  m_nErrorCode = wxDATABASE_OK;
}

void wxDatabaseErrorReporter::ThrowDatabaseException()
{
#if wxUSE_DATABASE_EXCEPTIONS
  wxDatabaseException error(GetErrorCode(), GetErrorMessage());
  throw error;
#endif
}

