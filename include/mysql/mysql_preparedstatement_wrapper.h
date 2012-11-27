#ifndef __MYSQL_PREPARED_STATEMENT_WRAPPER_H__
#define __MYSQL_PREPARED_STATEMENT_WRAPPER_H__

// For compilers that support precompilation, includes "wx.h".
#include "wx/wxprec.h"

#ifdef __BORLANDC__
    #pragma hdrstop
#endif

#ifndef WX_PRECOMP
    #include "wx/wx.h"
#endif

#include "mysql.h"

#include "../error_reporter.h"
#include "../strconverter.h"
#include "mysql_preparedstatement_parametercollection.h"
#include "mysql_interface.h"

class wxDatabaseResultSet;

class wxMysqlPreparedStatementWrapper : public wxDatabaseErrorReporter, public wxDatabaseStringConverter
{
public:
  // ctor
  wxMysqlPreparedStatementWrapper(wxMysqlDynamicInterface* pInterface, MYSQL_STMT* pStatement);

  // dtor
  virtual ~wxMysqlPreparedStatementWrapper();

  void Close();

  // set field
  void SetParam(int nPosition, int nValue);
  void SetParam(int nPosition, double dblValue);
  void SetParam(int nPosition, const wxString& strValue);
  void SetParam(int nPosition);
  void SetParam(int nPosition, const void* pData, long nDataLength);
  void SetParam(int nPosition, const wxDateTime& dateValue);
  void SetParam(int nPosition, bool bValue);
  int GetParameterCount();
  
  int RunQuery();
  wxDatabaseResultSet* RunQueryWithResults();

private:  
  wxMysqlDynamicInterface* m_pInterface;
  MYSQL_STMT* m_pStatement;

  wxMysqlPreparedStatementParameterCollection m_Parameters;
};

#endif // __MYSQL_PREPARED_STATEMENT_WRAPPER_H__

