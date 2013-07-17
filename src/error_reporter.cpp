#include "include/error_reporter.h"
#include "include/errorcodes.h"
#include "include/exceptions.h"

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
#ifndef DONT_USE_DATABASE_LAYER_EXCEPTIONS
  wxDatabaseException error(GetErrorCode(), GetErrorMessage());
  throw error;
#endif
}

