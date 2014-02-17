#include "wx/database/wxprec.h"

// Default the encoding converter
wxDatabaseStringConverter::wxDatabaseStringConverter()
  : m_Encoding(wxT("UTF-8"))
{
}

wxDatabaseStringConverter::wxDatabaseStringConverter(const wxChar* charset)
  : m_Encoding(charset)
{
}

void wxDatabaseStringConverter::SetEncoding(wxFontEncoding encoding)
{
  m_Encoding = wxCSConv(encoding);
}

void wxDatabaseStringConverter::SetEncoding(const wxCSConv* conv)
{
  m_Encoding = *conv;
}

const wxCharBuffer wxDatabaseStringConverter::ConvertToUnicodeStream(const wxString& inputString)
{
#if wxUSE_UNICODE
	// all the following do not work for Asian characters
  //return wxConvUTF8.cWC2MB(inputString.wc_str(*wxConvCurrent)); 
  //return wxConvUTF8.cWX2MB(inputString.wc_str(*wxConvCurrent));
  //return wxCharBuffer(wxConvCurrent->cWX2WC(inputString.wc_str()));
  //return inputString.c_str(); //this only works under some test cases
  return wxSafeConvertWX2MB(inputString.wc_str());
  //from docs for easy reference only:
  //To print a wxChar buffer to a non-Unicode stdout:
  //printf("Data: %s\n", (const char*) wxConvCurrent->cWX2MB(unicode_data));
#else
  wxString str(inputString.wc_str(*wxConvCurrent), wxConvUTF8);
  return str.mb_str();
#endif
}

size_t wxDatabaseStringConverter::GetEncodedStreamLength(const wxString& inputString)
{
  if(inputString == _T(""))
    return 0;
#if wxUSE_UNICODE
  size_t length = wxConvUTF8.WC2MB(NULL, inputString.wc_str(), (unsigned int)0);
#else
  wxString str(inputString.wc_str(*wxConvCurrent), wxConvUTF8);
  size_t length = str.Length();
#endif
  if (length == 0)
  {
    wxCharBuffer tempCharBuffer = ConvertToUnicodeStream(inputString);
    length = wxStrlen((wxChar*)(const char*)tempCharBuffer);
  }

  return length;
}

wxString wxDatabaseStringConverter::ConvertFromUnicodeStream(const char* inputBuffer)
{
  wxString strReturn(wxConvUTF8.cMB2WC(inputBuffer), *wxConvCurrent);
  // If the UTF-8 conversion didn't return anything, then try the default unicode conversion
  if (strReturn == wxEmptyString)
    strReturn << wxString(inputBuffer, *wxConvCurrent);
  
  return strReturn;
}

wxString wxDatabaseStringConverter::ConvertFromUnicodeStream(const wxChar* inputBuffer)
{
    wxString strReturn( wxConvUTF8.cWC2WX(inputBuffer),*wxConvCurrent);
  // If the UTF-8 conversion didn't return anything, then try the default unicode conversion
    if(strReturn==wxEmptyString)
        strReturn<<wxString(inputBuffer,*wxConvCurrent);
    return strReturn;
}

const wxCharBuffer wxDatabaseStringConverter::ConvertToUnicodeStream(const wxString& inputString, const char* WXUNUSED(encoding))
{
#if wxUSE_UNICODE
	// all the following do not work for Asian character issue
  //return wxConvUTF8.cWC2MB(inputString.wc_str(*wxConvCurrent)); 
  //return wxConvUTF8.cWX2MB(inputString.wc_str(*wxConvCurrent));
  //return wxCharBuffer(wxConvCurrent->cWX2WC(inputString.wc_str()));
  //return inputString.c_str(); //this only works under some test cases
  return wxSafeConvertWX2MB(inputString.wc_str());
  //from docs for easy reference only:
  //To print a wxChar buffer to a non-Unicode stdout:
  //printf("Data: %s\n", (const char*) wxConvCurrent->cWX2MB(unicode_data));
#else
  wxString str(inputString.wc_str(*wxConvCurrent), wxConvUTF8);
  return str.mb_str();
#endif
}

size_t wxDatabaseStringConverter::GetEncodedStreamLength(const wxString& inputString, const char* encoding)
{
  if(inputString == _T(""))
    return 0;
#if wxUSE_UNICODE
  size_t length = wxConvUTF8.WC2MB(NULL, inputString.wc_str(), (unsigned int)0);
#else
  const wchar_t* str = inputString.wc_str(*wxConvCurrent);
  size_t length = wxConvUTF8.WC2MB(NULL, str, (unsigned int)0);
#endif
  if (length == 0)
  {
    wxCharBuffer tempCharBuffer = wxDatabaseStringConverter::ConvertToUnicodeStream(inputString, encoding);
    length = wxStrlen((wxChar*)(const char*)tempCharBuffer);
  }

  return length;
}

wxString wxDatabaseStringConverter::ConvertFromUnicodeStream(const char* inputBuffer, const char* WXUNUSED(encoding))
{
  wxString strReturn(wxConvUTF8.cMB2WC(inputBuffer), *wxConvCurrent);
  // If the UTF-8 conversion didn't return anything, then try the default unicode conversion
  if (strReturn == wxEmptyString)
    strReturn << wxString(inputBuffer, *wxConvCurrent);
  
  return strReturn;
}  

wxString wxDatabaseStringConverter::ConvertFromUnicodeStream(const wxChar* inputBuffer, const char*  WXUNUSED(encoding))
{
    wxString strReturn( wxConvUTF8.cWC2WX(inputBuffer),*wxConvCurrent);
  // If the UTF-8 conversion didn't return anything, then try the default unicode conversion
    if(strReturn==wxEmptyString)
        strReturn<<wxString(inputBuffer,*wxConvCurrent);
    return strReturn;
}



