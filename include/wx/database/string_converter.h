#ifndef _WX_DATABASE_STRING_CONVERTER_H_
#define _WX_DATABASE_STRING_CONVERTER_H_

#include "wx/database/wxprec.h"

class WXDLLIMPEXP_DATABASE wxDatabaseStringConverter
{
public:
  // ctor
  wxDatabaseStringConverter();
  wxDatabaseStringConverter(const wxChar* charset);

  // dtor
  virtual ~wxDatabaseStringConverter() { }
  
  void SetEncoding(wxFontEncoding encoding);
  void SetEncoding(const wxCSConv* conv);
  const wxCSConv* GetEncoding() { return &m_Encoding; }

  virtual const wxCharBuffer ConvertToUnicodeStream(const wxString& inputString);
  virtual size_t GetEncodedStreamLength(const wxString& inputString);
  virtual wxString ConvertFromUnicodeStream(const char* inputBuffer);
  virtual wxString ConvertFromUnicodeStream(const wxChar* inputBuffer);
 
  static const wxCharBuffer ConvertToUnicodeStream(const wxString& inputString, const char* encoding);
  static wxString ConvertFromUnicodeStream(const char* inputBuffer, const char* encoding);
  static size_t GetEncodedStreamLength(const wxString& inputString, const char* encoding);
  static wxString ConvertFromUnicodeStream(const wxChar* inputBuffer,const char* encoding);

private:
  wxCSConv m_Encoding;
};

#endif//_WX_DATABASE_STRING_CONVERTER_H_
