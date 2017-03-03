#ifndef _WX_DATABASE_TDS_DATABASE_H_
#define _WX_DATABASE_TDS_DATABASE_H_

#include "wx/database/wxprec.h"

class wxPreparedStatement;
class wxTdsDatabase;

WX_DECLARE_HASH_MAP(TDSCONTEXT*, wxTdsDatabase*, wxPointerHash, wxPointerEqual, TdsContextToDatabaseLayerMap);

class WXDLLIMPEXP_DATABASE wxTdsDatabase : public wxDatabase
{
public:
	// ctor()
	wxTdsDatabase();
	wxTdsDatabase(const wxString& strFreeTDS, const wxString& strServer, const wxString& strDatabase, const wxString& strUser, const wxString& strPassword, int nTdsVersion = TDS_80);

	// dtor()
	virtual ~wxTdsDatabase();

	// open database
	virtual bool Open(const wxString& strDatabase);

	bool Connect();

	// close database  
	virtual bool Close();

	// Is the connection to the database open?
	virtual bool IsOpen();

	// transaction support
	virtual void BeginTransaction();
	virtual void Commit();
	virtual void RollBack();

	// query database
	virtual int RunQuery(const wxString& strQuery, bool bParseQuery);
	virtual wxDatabaseResultSet* RunQueryWithResults(const wxString& strQuery);

	// wxPreparedStatement support
	virtual wxPreparedStatement* PrepareStatement(const wxString& strQuery);

	void SetFreeTDS(const wxString& strFreeTDS) { m_strFreeTDS = strFreeTDS; }
	void SetServer(const wxString& strServer) { m_strServer = strServer; }
	void SetDatabase(const wxString& strDatabase) { m_strDatabase = strDatabase; }
	void SetLogin(const wxString& strLogin) { m_strLogin = strLogin; }
	void SetPassword(const wxString& strPassword) { m_strPassword = strPassword; }
	void SetTdsVersion(int nTdsVersion) { m_nTdsVersion = nTdsVersion; }

	enum
	{
		TDS_42 = 0,
		TDS_46,
		TDS_50,
		TDS_70,
		//AML start
		TDS_71,
		TDS_72,
		TDS_73,
		TDS_74,
		//AML end
		TDS_80
	};

	// Database schema API contributed by M. Szeftel (author of wxActiveRecordGenerator)
	virtual bool TableExists(const wxString& table);
	virtual bool ViewExists(const wxString& view);
	virtual wxArrayString GetTables();
	virtual wxArrayString GetViews();
	virtual wxArrayString GetColumns(const wxString& table);
	virtual wxArrayString GetPKColumns(const wxString& table);

	static wxTdsDatabase* LookupTdsLayer(const TDSCONTEXT* pContext);
	void SetError(int nCode, const wxString& strMessage);

	//AML start
	typedef struct 
	{
		TDS_TINYINT has;
		TDS_INT ret;
	} status_t;
	//AML end
private:
	static int TranslateErrorCode(int nCode);
	void FreeAllocatedResultSets(/*AML start*/status_t* status = NULL/*AML end*/);
	static void AddTdsLayer(TDSCONTEXT* pContext, wxTdsDatabase* pLayer);
	static void RemoveTdsLayer(TDSCONTEXT* pContext);

	TDSPARAMINFO* GetParameters(const wxString& strQuery);

	// Map of TDSCONTEXT* to wxTdsDatabase* to be used by the error handler
	static TdsContextToDatabaseLayerMap m_ContextLookupMap;
	
	wxString m_strFreeTDS;
	wxString m_strServer;
	wxString m_strDatabase;
	wxString m_strLogin;
	wxString m_strPassword;
	int m_nTdsVersion;

	TDSSOCKET* m_pDatabase;
	TDSLOGIN* m_pLogin;
	TDSCONTEXT* m_pContext;
};

#endif//_WX_DATABASE_TDS_DATABASE_H_

