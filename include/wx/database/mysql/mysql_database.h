#ifndef _WX_DATABASE_MYSQL_DATABASE_H_
#define _WX_DATABASE_MYSQL_DATABASE_H_

#include "wx/database/wxprec.h"

#ifndef DONT_USE_DYNAMIC_DATABASE_LINKING
class wxMysqlDynamicInterface;
#endif


WX_DECLARE_VOIDPTR_HASH_MAP(void*, PointerLookupMap);

class WXDLLIMPEXP_DATABASE wxMysqlDatabase : public wxDatabase
{
public:
  // Information that can be specified for a MySQL database
  //  host or hostaddr
  //  port
  //  dbname
  //  user
  //  password
  // ctor
  wxMysqlDatabase();
  wxMysqlDatabase(const wxString& strDatabase);
  wxMysqlDatabase(const wxString& strServer, const wxString& strDatabase);
  wxMysqlDatabase(const wxString& strDatabase, const wxString& strUser, const wxString& strPassword);
  wxMysqlDatabase(const wxString& strServer, const wxString& strDatabase, const wxString& strUser, const wxString& strPassword);
  wxMysqlDatabase(void* pDatabase) { m_pDatabase = pDatabase; }

  // dtor
  virtual ~wxMysqlDatabase();

  // open database
  virtual bool Open(const wxString& strDatabase);
  virtual bool Open(const wxString& strServer, const wxString& strDatabase);
  virtual bool Open(const wxString& strDatabase, const wxString& strUser, const wxString& strPassword);
  virtual bool Open(const wxString& strServer, const wxString& strDatabase, const wxString& strUser, const wxString& strPassword);

  // close database
  virtual bool Close();
  
  //change database
  virtual bool ChangeDatabase(const wxString& database) ;
  
  // Is the connection to the database open?
  virtual bool IsOpen();

  // transaction support
  // transaction support
  virtual void BeginTransaction();
  virtual void Commit();
  virtual void RollBack();
  
  // query database
  virtual int RunQuery(const wxString& strQuery, bool bParseQuery);
  virtual wxDatabaseResultSet* RunQueryWithResults(const wxString& strQuery);

  // wxPreparedStatement support
  virtual wxPreparedStatement* PrepareStatement(const wxString& strQuery);

  // Database schema API contributed by M. Szeftel (author of wxActiveRecordGenerator)
  virtual bool TableExists(const wxString& table);
  virtual bool ViewExists(const wxString& view);
  virtual wxArrayString GetTables();
  virtual wxArrayString GetViews();
  virtual wxArrayString GetColumns(const wxString& table);
  virtual wxArrayString GetPKColumns(const wxString& table);

  static int TranslateErrorCode(int nCode);
  static bool IsAvailable();

private:
  void InitDatabase();
  void ParseServerAndPort(const wxString& strServer);

#ifndef DONT_USE_DYNAMIC_DATABASE_LINKING
  wxMysqlDynamicInterface* m_pInterface;
#endif
  wxString m_strServer;
  wxString m_strDatabase;
  wxString m_strUser;
  wxString m_strPassword;
  int m_iPort;
    
  void* m_pDatabase;

#if wxUSE_UNICODE
  PointerLookupMap m_ResultSets;
#endif
};

#endif//_WX_DATABASE_MYSQL_DATABASE_H_

