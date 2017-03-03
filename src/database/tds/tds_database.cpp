#include "wx/database/wxprec.h"

#if wxUSE_DATABASE_TDS

TdsContextToDatabaseLayerMap wxTdsDatabase::m_ContextLookupMap;

//AML start
static int tsql_handle_common(const TDSCONTEXT * pContext, TDSSOCKET * WXUNUSED(pTdsSocket), TDSMESSAGE * pMessage, bool error)
{
	if (pMessage->msgno != 5701 && pMessage->msgno != 5703 && pMessage->msgno != 20018)
	{
		char message[1024];
		sprintf(message, "%s %d, Level %d, State %d, Server %s, Line %d\n%s", error ? "Err" : "Msg", pMessage->msgno, pMessage->severity, pMessage->state, pMessage->server, pMessage->line_number, pMessage->message);
		wxTdsDatabase* pLayer = wxTdsDatabase::LookupTdsLayer(pContext);
		if (pLayer == NULL)
		{
			fprintf(stderr, "%s\n", message);
		}
		else
		{
			pLayer->SetError(pMessage->msgno, wxDatabaseStringConverter::ConvertFromUnicodeStream(message, "UTF-8"));
		}
	}
	return TDS_INT_CANCEL;
}
//AML end

static int tsql_handle_message(const TDSCONTEXT * pContext, TDSSOCKET * pTdsSocket, TDSMESSAGE * pMessage)
{
	return tsql_handle_common(pContext, pTdsSocket, pMessage, false); // AML
	/*AML
	//fprintf(stderr, "In tsql_handle_message: %d\n", pMessage->msgno);
	if (pMessage->msgno != 5701 && pMessage->msgno != 5703 && pMessage->msgno != 20018)
	{
		wxTdsDatabase* pLayer = wxTdsDatabase::LookupTdsLayer(pContext);
		if (pLayer != NULL)
		{
			pLayer->SetError(pMessage->msgno, wxDatabaseStringConverter::ConvertFromUnicodeStream(pMessage->message, "UTF-8"));
		}
		else
		{
			//fprintf(stderr, "In tsql_handle_message (%d)\n", pMessage->msgno);
			if (pMessage->msgno == 0)
			{
				//fprintf(stderr, "%s\n", pMessage->message);
				return 0;
			}

		}
	}
	return 0;
	AML*/
}

static int tsql_handle_error(const TDSCONTEXT * pContext, TDSSOCKET * pTdsSocket, TDSMESSAGE * pMessage)
{
	return tsql_handle_common(pContext, pTdsSocket, pMessage, true); // AML
	/*AML
	//fprintf(stderr, "In tsql_handle_error: %d\n", pMessage->msgno);
	if (pMessage->msgno != 5701 && pMessage->msgno != 5703 && pMessage->msgno != 20018)
	{
		wxTdsDatabase* pLayer = wxTdsDatabase::LookupTdsLayer(pContext);
		if (pLayer != NULL)
		{
			pLayer->SetError(pMessage->msgno, wxDatabaseStringConverter::ConvertFromUnicodeStream(pMessage->message, "UTF-8"));
		}
		else
		{
			//fprintf(stderr, "In tsql_handle_error (%d)\n", pMessage->msgno);
			if (pMessage->msgno == 0)
			{
				//fprintf(stderr, "%s\n", pMessage->message);
				return 0;
			}
			fprintf(stderr, "Msg %d, Level %d, State %d, Server %s, Line %d\n%s\n",
				pMessage->msgno, pMessage->severity, pMessage->state, pMessage->server, pMessage->line_number, pMessage->message);
		}
	}
	return 0;
	AML*/
}

wxString RemoveLastSemiColon(const wxString& strvalue)
{
	wxString result = strvalue;

	if (result.size() == 0)
		return result;	

	if (result[result.size()-1] == ';' )
	{
		result.RemoveLast();
	}	
	return result;
}


// ctor()
wxTdsDatabase::wxTdsDatabase()
: wxDatabase()
{
	m_pDatabase = NULL;
	m_pLogin = NULL;
	m_pContext = NULL;
	m_nTdsVersion = TDS_80;
	wxCSConv conv(_("UTF-8"));
	SetEncoding(&conv);
}

wxTdsDatabase::wxTdsDatabase(const wxString& strFreeTDS, const wxString& strServer, const wxString& strDatabase, const wxString& strUser, const wxString& strPassword, int nTdsVersion)
: wxDatabase()
{
	m_pDatabase = NULL;
	m_pLogin = NULL;
	m_pContext = NULL;
	wxCSConv conv(_("UTF-8"));
	SetEncoding(&conv);
	m_strFreeTDS = strFreeTDS;
	m_strServer = strServer;
	m_strLogin = strUser;
	m_strPassword = strPassword;
	m_nTdsVersion = nTdsVersion;
	Open(strDatabase);
}

// dtor()
wxTdsDatabase::~wxTdsDatabase()
{
	//wxPrintf(_("~wxTdsDatabase()\n"));
	Close();
	//wxDELETE(m_pDatabase);
}

// open database
bool wxTdsDatabase::Open(const wxString& strDatabase)
{
	ResetErrorCodes();

	m_strDatabase = strDatabase;

	if (!Connect())
	{
		//SetErrorCode(wxTdsDatabase::TranslateErrorCode(???));
		//SetErrorMessage(ConvertFromUnicodeStream(???));
		return false;
	}

	// Change the database
	if (!RunQuery(_("USE ") + strDatabase, false))
	{
		return false;
	}

	return true;
}

// Connect to the server
bool wxTdsDatabase::Connect()
{
	m_pLogin = tds_alloc_login(/*AML*/1);
	if (m_pLogin == NULL)
	{
		//fprintf(stderr, "tds_alloc_login() failed.\n");
		SetError(wxDATABASE_ERROR, wxT("Failed to allocate login"));
		// Reset the variables so there are not mistaken as initialized
		m_pDatabase = NULL;
		m_pLogin = NULL;
		if (m_pContext)
		{
			tds_free_context(m_pContext);
			m_pContext = NULL;
		}
		ThrowDatabaseException();
		return false;
	}
	wxCharBuffer freeTDSBuffer = ConvertToUnicodeStream(m_strFreeTDS);
	tds_set_interfaces_file_loc(freeTDSBuffer);
	wxCharBuffer serverBuffer = ConvertToUnicodeStream(m_strServer);
	tds_set_server(m_pLogin, serverBuffer);
	wxCharBuffer loginBuffer = ConvertToUnicodeStream(m_strLogin);
	tds_set_user(m_pLogin, loginBuffer);
	wxCharBuffer passwordBuffer = ConvertToUnicodeStream(m_strPassword);
	tds_set_passwd(m_pLogin, passwordBuffer);
	tds_set_app(m_pLogin, "wxDatabase");
	tds_set_library(m_pLogin, "TDS-Library");
	tds_set_client_charset(m_pLogin, "UTF-8");
	switch (m_nTdsVersion)
	{
	case TDS_42:
		tds_set_version(m_pLogin, 4, 2);
		break;
	case TDS_46:
		tds_set_version(m_pLogin, 4, 6);
		break;
	case TDS_50:
		tds_set_version(m_pLogin, 5, 0); // Sybase
		break;
	case TDS_70:
		tds_set_version(m_pLogin, 7, 0); // SQL Server 7.0
		break;
	case TDS_71:
	case TDS_80:
		tds_set_version(m_pLogin, 7, 1); // SQL Server 2000
		break;
	case TDS_72:
		tds_set_version(m_pLogin, 7, 2); // SQL Server 2005
		break;
	case TDS_73:
		tds_set_version(m_pLogin, 7, 3); // SQL Server 2008
		break;
	case TDS_74:
		tds_set_version(m_pLogin, 7, 4); // SQL Server 2012
		break;
	default:
		tds_set_version(m_pLogin, 0, 0);
		break;
	};

	m_pContext = tds_alloc_context(NULL);
	if (m_pContext == NULL)
	{
		//fprintf(stderr, "tds_alloc_context() failed.\n");
		SetError(wxDATABASE_ERROR, wxT("Failed to allocate context"));
		// Reset the variables so there are not mistaken as initialized
		m_pDatabase = NULL;
		if (m_pLogin)
		{
			tds_free_login(m_pLogin);
			m_pLogin = NULL;
		}
		if (m_pContext)
		{
			tds_free_context(m_pContext);
			m_pContext = NULL;
		}
		ThrowDatabaseException();
		return false;
	}
	m_pContext->msg_handler = tsql_handle_message;
	m_pContext->err_handler = tsql_handle_error;

	// Add the context (and this databaselayer) from the lookup map
	//  used by the error handler
	wxTdsDatabase::AddTdsLayer(m_pContext, this);

	m_pDatabase = tds_alloc_socket(m_pContext, 512);
	if (m_pDatabase == NULL)
	{
		//fprintf(stderr, "tds_alloc_socket() failed.\n");
		SetError(wxDATABASE_ERROR, wxT("Failed to allocate socket"));
		// Reset the variables so there are not mistaken as initialized
		m_pDatabase = NULL;
		if (m_pLogin)
		{
			tds_free_login(m_pLogin);
			m_pLogin = NULL;
		}
		if (m_pContext)
		{
			tds_free_context(m_pContext);
			m_pContext = NULL;
		}
		ThrowDatabaseException();
		return false;
	}
	tds_set_parent(m_pDatabase, NULL);

	TDSLOGIN* pConnection = tds_read_config_info(NULL, m_pLogin, m_pContext->locale);

	if (!pConnection || tds_connect_and_login(m_pDatabase, pConnection) == TDS_FAIL)
	{
		if (pConnection)
		{
			tds_free_socket(m_pDatabase);
			//m_pDatabase = NULL;
			tds_free_login(pConnection);
		}
		//fprintf(stderr, "tds_connect() failed\n");
		if (GetErrorCode() == wxDATABASE_OK)
		{
			SetError(wxDATABASE_ERROR, wxT("Database connection failed"));
		}
		// Reset the variables so there are not mistaken as initialized
		m_pDatabase = NULL;
		if (m_pLogin)
		{
			tds_free_login(m_pLogin);
			m_pLogin = NULL;
		}
		if (m_pContext)
		{
			tds_free_context(m_pContext);
			m_pContext = NULL;
		}
		ThrowDatabaseException();
		return false;
	}
	tds_free_login(pConnection);

	return true;
}

// close database  
bool wxTdsDatabase::Close()
{
	//puts("Resetting error codes");
	ResetErrorCodes();

	//puts("Closing result sets");
	CloseResultSets();
	//puts("Closing statements");
	CloseStatements();

	if (m_pDatabase != NULL)
	{
		//puts("Freeing socket");
		tds_free_socket(m_pDatabase);
		m_pDatabase = NULL;
	}
	if (m_pLogin != NULL)
	{
		//puts("Freeing login");
		tds_free_login(m_pLogin);
		m_pLogin = NULL;
	}
	if (m_pContext != NULL)
	{
		// Remove the context (and this databaselayer) from the lookup map
		//  used by the error handler
		//puts("Removing TDS layer from hash");
		wxTdsDatabase::RemoveTdsLayer(m_pContext);

		// Free the context
		//puts("Freeing context");
		tds_free_context(m_pContext);
		m_pContext = NULL;
	}

	return true;
}

bool wxTdsDatabase::IsOpen()
{
	return (m_pDatabase != NULL);
}

void wxTdsDatabase::FreeAllocatedResultSets(/*AML start*/status_t* status/*AML end*/)
{
	//fprintf(stderr, "In FreeAllocatedResultSets\n");
	int rc;
	int result_type;
	//while ((rc = tds_process_tokens(m_pDatabase, &result_type, NULL, TDS_TOKEN_RESULTS)) == TDS_SUCCESS)
	while ((rc = tds_process_tokens(m_pDatabase, &result_type, NULL, TDS_RETURN_DONE|TDS_RETURN_ROW|TDS_RETURN_COMPUTE)) == TDS_SUCCESS)
	{
		switch (result_type)
		{
		case TDS_DONE_RESULT:
		case TDS_DONEPROC_RESULT:
		case TDS_DONEINPROC_RESULT:
		case TDS_STATUS_RESULT:
		case TDS_ROWFMT_RESULT:
		case TDS_COMPUTEFMT_RESULT:
		case TDS_DESCRIBE_RESULT:
			break;
		case TDS_ROW_RESULT:
			//fprintf(stderr, "Warning (%d):  wxTdsDatabase query should not return results.  Type: %d\n", result_type, result_type);
			if (m_pDatabase->current_results && m_pDatabase->current_results->num_cols > 0)
			{
				while (tds_process_tokens(m_pDatabase, &result_type, NULL, TDS_STOPAT_ROWFMT|TDS_RETURN_DONE|TDS_RETURN_ROW) == /*AML TDS_SUCCEED*/TDS_SUCCESS)
				{
					//fprintf(stderr, "Warning:  wxTdsDatabase TDS_ROW_RESULT query should not return results.  Type: %d\n", result_type);
					if (result_type != TDS_ROW_RESULT)
						break;

					if (!m_pDatabase->current_results)
						continue;
				}
			}
			if (m_pDatabase != NULL)
				tds_free_all_results(m_pDatabase);
			return;
			break;
		default:
			//fprintf(stderr, "Error:  wxTdsDatabase query should not return results.  Type: %d\n", result_type);
			return;
			//break;
		}
	}

	//AML start
	if (m_pDatabase != NULL && status != NULL)
	{
		status->has = m_pDatabase->has_status;
		status->ret = m_pDatabase->ret_status;
	}
	//AML end

	// Make sure to clean up after ourselves
	if (m_pDatabase != NULL)
		tds_free_all_results(m_pDatabase);

	if (rc == TDS_FAIL)
	{
		ThrowDatabaseException();
		//fprintf(stderr, "tds_process_tokens() returned TDS_FAIL\n");
		return;
	}
	else if (rc != TDS_NO_MORE_RESULTS)
	{
		ThrowDatabaseException();
		//fprintf(stderr, "tds_process_tokens() unexpected return\n");
		return;
	}
}

void wxTdsDatabase::BeginTransaction()
{
	wxLogDebug(_("Beginning transaction"));
	RunQuery(_("begin transaction;"), false);
}

void wxTdsDatabase::Commit()
{
	wxLogDebug(_("Commiting transaction"));
	RunQuery(_("commit transaction;"), false);
}

void wxTdsDatabase::RollBack()
{
	wxLogDebug(_("Rolling back transaction"));
	RunQuery(_("rollback transaction;"), false);
}

// query database
int wxTdsDatabase::RunQuery(const wxString& strQuery, bool bParseQuery)
{
	ResetErrorCodes();

	if (m_pDatabase == NULL)
		return false;

	FreeAllocatedResultSets();

	wxArrayString QueryArray;
	if (bParseQuery)
		QueryArray = ParseQueries(strQuery);
	else
		QueryArray.push_back(strQuery);

	wxArrayString::iterator start = QueryArray.begin();
	wxArrayString::iterator stop = QueryArray.end();

	//AML start
	status_t status;
	status.has = false;
	status.ret = 0;
	//AML end

	while (start != stop)
	{
		//char* szErrorMessage = NULL;
		wxString strErrorMessage = _("");
		wxString sql = RemoveLastSemiColon(*start);
		wxCharBuffer sqlBuffer = ConvertToUnicodeStream(sql);

		//fprintf(stderr, "Running query '%s'\n", (const char*)sqlBuffer);
		int nReturn = tds_submit_query(m_pDatabase, sqlBuffer);
		if (nReturn != /*AML TDS_SUCCEED*/TDS_SUCCESS)
		{
			//fprintf(stderr, "tds_submit_query() failed for query '%s'\n", sqlBuffer);
			//fprintf(stderr, "tds_submit_query() failed for query '%s'\n", (sql).c_str());
			FreeAllocatedResultSets();
			ThrowDatabaseException();
			return false;
		}
		FreeAllocatedResultSets(/*AML start*/&status/*AML end*/);

		start++;
	}
	//AML start
	//AML return true;
	return (status.has ? status.ret : m_pDatabase->rows_affected);
	//AML end
}

wxDatabaseResultSet* wxTdsDatabase::RunQueryWithResults(const wxString& strQuery)
{
	ResetErrorCodes();

	if (m_pDatabase != NULL)
	{
		FreeAllocatedResultSets();

		wxArrayString QueryArray = ParseQueries(strQuery);

		for (unsigned int i=0; i<(QueryArray.size()-1); i++)
		{
			//char* szErrorMessage = NULL;
			wxString strErrorMessage = _("");
			wxString sql = RemoveLastSemiColon(QueryArray[i]);
			wxCharBuffer sqlBuffer = ConvertToUnicodeStream(sql);

			//fprintf(stderr, "Running query '%s'\n", sqlBuffer);
			int nReturn = tds_submit_query(m_pDatabase, sqlBuffer);
			if (nReturn != TDS_SUCCESS)
			{
				//fprintf(stderr, "tds_submit_query() failed for query '%s'\n", sql.c_str());
				FreeAllocatedResultSets();
				ThrowDatabaseException();
				return NULL;
			}
			FreeAllocatedResultSets();
		}

		// Create a Prepared statement for the last SQL statement and get a result set from it
		wxString strQuery = RemoveLastSemiColon(QueryArray[QueryArray.size()-1]);
		wxCharBuffer sqlBuffer = ConvertToUnicodeStream(strQuery);
		//fprintf(stderr, "Running query (with results) '%s'\n", sqlBuffer);
		int nReturn = tds_submit_query(m_pDatabase, sqlBuffer);
		if (nReturn != /*AML TDS_SUCCEED*/TDS_SUCCESS)
		{
			//fprintf(stderr, "tds_submit_query() failed for query '%s'\n", sqlBuffer);
			//fprintf(stderr, "tds_submit_query() failed for query '%s'\n", strQuery.c_str());
			FreeAllocatedResultSets();
			ThrowDatabaseException();
			return NULL;
		}
		wxTdsResultSet* pResultSet = new wxTdsResultSet(m_pDatabase);
		if (pResultSet)
			pResultSet->SetEncoding(GetEncoding());

		LogResultSetForCleanup(pResultSet);
		//fprintf(stderr, "Returning result set\n");
		return pResultSet;
	}
	else
	{
		return NULL;
	}
}

wxPreparedStatement* wxTdsDatabase::PrepareStatement(const wxString& strQuery)
{
	ResetErrorCodes();

	if (m_pDatabase != NULL)
	{
		wxString sql = RemoveLastSemiColon(strQuery);	

		wxCharBuffer sqlBuffer = ConvertToUnicodeStream(sql);
		TDSDYNAMIC* pStatement = NULL;
		TDSPARAMINFO* pParameters = GetParameters(strQuery);
		int nReturn = tds_submit_prepare(m_pDatabase, sqlBuffer, NULL, &pStatement, pParameters);
		if (nReturn != TDS_SUCCESS)
		{
			//fprintf(stderr, "tds_submit_prepare() failed for query '%s'\n", strQuery.c_str());

			if (pStatement != NULL)
				tds_release_dynamic(&pStatement);

			FreeAllocatedResultSets();
			ThrowDatabaseException();
			return NULL;
		}
		if (pParameters != NULL)
			tds_free_param_results(pParameters);
		FreeAllocatedResultSets();

		wxTdsPreparedStatement* pReturnStatement = new wxTdsPreparedStatement(m_pDatabase, pStatement, strQuery);
		if (pReturnStatement)
			pReturnStatement->SetEncoding(GetEncoding());

		if (pReturnStatement != NULL)
			LogStatementForCleanup(pReturnStatement);
		return pReturnStatement;
	}
	else
	{
		return NULL;
	}
}

bool wxTdsDatabase::TableExists(const wxString& table)
{
	// Initialize variables
	bool bReturn = false;
	// Keep these variables outside of scope so that we can clean them up
	//  in case of an error
	wxPreparedStatement* pStatement = NULL;
	wxDatabaseResultSet* pResult = NULL;

#if wxUSE_DATABASE_EXCEPTIONS
	try
	{
#endif
		wxString query = _("exec sp_tables ?, NULL, NULL, '''TABLE'''");
		pStatement = PrepareStatement(query);
		if (pStatement)
		{
			pStatement->SetParamString(1, table);
			pResult = pStatement->ExecuteQuery();
			if (pResult)
			{
				if (pResult->Next())
				{
					//wxPrintf(_("Table found: '%s'\n"), pResult->GetResultString(_("TABLE_NAME")));
					if(table.CmpNoCase(pResult->GetResultString(_("TABLE_NAME"))) == 0)
					{
						bReturn = true;
					}
				}
			}
		}
#if wxUSE_DATABASE_EXCEPTIONS
	}
	catch (wxDatabaseException& e)
	{
		if (pResult != NULL)
		{
			CloseResultSet(pResult);
			pResult = NULL;
		}

		if (pStatement != NULL)
		{
			CloseStatement(pStatement);
			pStatement = NULL;
		}

		throw e;
	}
#endif

	if (pResult != NULL)
	{
		CloseResultSet(pResult);
		pResult = NULL;
	}

	if (pStatement != NULL)
	{
		CloseStatement(pStatement);
		pStatement = NULL;
	}

	return bReturn;
}

bool wxTdsDatabase::ViewExists(const wxString& view)
{
	bool bReturn = false;
	// Keep these variables outside of scope so that we can clean them up
	//  in case of an error
	wxPreparedStatement* pStatement = NULL;
	wxDatabaseResultSet* pResult = NULL;

#if wxUSE_DATABASE_EXCEPTIONS
	try
	{
#endif
		wxString query = _("exec sp_tables ?, NULL, NULL, '''VIEW'''");
		pStatement = PrepareStatement(query);
		if (pStatement)
		{
			pStatement->SetParamString(1, view);
			pResult = pStatement->ExecuteQuery();
			if (pResult)
			{
				if (pResult->Next())
				{
					//wxPrintf(_("View found: '%s'\n"), pResult->GetResultString(_("TABLE_NAME")));
					if(view.CmpNoCase(pResult->GetResultString(_("TABLE_NAME"))) == 0)
					{
						bReturn = true;
					}
				}
			}
		}
#if wxUSE_DATABASE_EXCEPTIONS
	}
	catch (wxDatabaseException& e)
	{
		if (pResult != NULL)
		{
			CloseResultSet(pResult);
			pResult = NULL;
		}

		if (pStatement != NULL)
		{
			CloseStatement(pStatement);
			pStatement = NULL;
		}

		throw e;
	}
#endif

	if (pResult != NULL)
	{
		CloseResultSet(pResult);
		pResult = NULL;
	}

	if (pStatement != NULL)
	{
		CloseStatement(pStatement);
		pStatement = NULL;
	}

	return bReturn;
}

wxArrayString wxTdsDatabase::GetTables()
{
	wxArrayString returnArray;

	wxDatabaseResultSet* pResult = NULL;
#if wxUSE_DATABASE_EXCEPTIONS
	try
	{
#endif
		wxString query = _("sp_tables NULL, NULL, NULL, '''TABLE'''");
		pResult = ExecuteQuery(query);

		while (pResult->Next())
		{
			//wxPrintf(_("Adding table: '%s'\n"), pResult->GetResultString(_("TABLE_NAME")).Trim());
			returnArray.Add(pResult->GetResultString(_("TABLE_NAME")).Trim());
		}
#if wxUSE_DATABASE_EXCEPTIONS
	}
	catch (wxDatabaseException& e)
	{
		if (pResult != NULL)
		{
			CloseResultSet(pResult);
			pResult = NULL;
		}

		throw e;
	}
#endif

	if (pResult != NULL)
	{
		CloseResultSet(pResult);
		pResult = NULL;
	}

	return returnArray;
}

wxArrayString wxTdsDatabase::GetViews()
{
	wxArrayString returnArray;

	wxDatabaseResultSet* pResult = NULL;
#if wxUSE_DATABASE_EXCEPTIONS
	try
	{
#endif
		wxString query = _("sp_tables NULL, NULL, NULL, '''VIEW'''");
		pResult = ExecuteQuery(query);

		while (pResult->Next())
		{
			//wxPrintf(_("Adding view: '%s'\n"), pResult->GetResultString(_("TABLE_NAME")).Trim());
			returnArray.Add(pResult->GetResultString(_("TABLE_NAME")).Trim());
		}
#if wxUSE_DATABASE_EXCEPTIONS
	}
	catch (wxDatabaseException& e)
	{
		if (pResult != NULL)
		{
			CloseResultSet(pResult);
			pResult = NULL;
		}

		throw e;
	}
#endif

	if (pResult != NULL)
	{
		CloseResultSet(pResult);
		pResult = NULL;
	}

	return returnArray;
}

wxArrayString wxTdsDatabase::GetColumns(const wxString& table)
{
	wxArrayString returnArray;

	wxDatabaseResultSet* pResult = NULL;
#if wxUSE_DATABASE_EXCEPTIONS
	try
	{
#endif
		wxString query = wxString::Format(_("sp_columns %s;"), table.c_str());
		pResult = ExecuteQuery(query);

		while (pResult->Next())
		{
			//wxPrintf(_("Adding column: '%s'\n"), pResult->GetResultString(_("COLUMN_NAME")).Trim());
			returnArray.Add(pResult->GetResultString(_("COLUMN_NAME")).Trim());
		}
#if wxUSE_DATABASE_EXCEPTIONS
	}
	catch (wxDatabaseException& e)
	{
		if (pResult != NULL)
		{
			CloseResultSet(pResult);
			pResult = NULL;
		}

		throw e;
	}
#endif

	if (pResult != NULL)
	{
		CloseResultSet(pResult);
		pResult = NULL;
	}

	return returnArray;
}

wxArrayString wxTdsDatabase::GetPKColumns(const wxString& table)
{
	wxArrayString returnArray;

	// Keep these variables outside of scope so that we can clean them up
	//  in case of an error
	wxDatabaseResultSet* pResult = NULL;
#if wxUSE_DATABASE_EXCEPTIONS
	try
	{
#endif
		wxString query = wxString::Format(_("sp_pkeys %s;"), table.c_str());
		pResult = ExecuteQuery(query);

		while (pResult->Next())
		{
			//wxPrintf(_("Adding column: '%s'\n"), pResult->GetResultString(_("COLUMN_NAME")).Trim());
			returnArray.Add(pResult->GetResultString(_("COLUMN_NAME")).Trim());
		}
#if wxUSE_DATABASE_EXCEPTIONS
	}
	catch (wxDatabaseException& e)
	{
		if (pResult != NULL)
		{
			CloseResultSet(pResult);
			pResult = NULL;
		}

		throw e;
	}
#endif

	if (pResult != NULL)
	{
		CloseResultSet(pResult);
		pResult = NULL;
	}

	return returnArray;
}

void wxTdsDatabase::SetError(int nCode, const wxString& strMessage)
{
	SetErrorCode(TranslateErrorCode(nCode));
	wxString msg = GetErrorMessage();
	if (msg.Length() > 0) 
	{
		msg += "\n" + strMessage;
	}
	else
	{
		msg = strMessage;
	}
	SetErrorMessage(msg);

	// We really don't want to throw the exception here since it removes
	//  the ability of the calling code to cleanup any memory usage
	//wxPrintf(_("wxTdsDatabase SetError() throwing exception\n"));
	//ThrowDatabaseException();
}

int wxTdsDatabase::TranslateErrorCode(int nCode)
{
	// Ultimately, this will probably be a map of TDS database error code values to wxDatabase values
	// For now though, we'll just return error
	int nReturn = nCode;
	return nReturn;
}

wxTdsDatabase* wxTdsDatabase::LookupTdsLayer(const TDSCONTEXT* pContext)
{
	wxTdsDatabase* pLayer = NULL;
	TdsContextToDatabaseLayerMap::iterator finder = wxTdsDatabase::m_ContextLookupMap.find(pContext);
	if (finder != wxTdsDatabase::m_ContextLookupMap.end())
		pLayer = finder->second;

	return pLayer;
}

void wxTdsDatabase::AddTdsLayer(TDSCONTEXT* pContext, wxTdsDatabase* pLayer)
{
	wxTdsDatabase::m_ContextLookupMap[pContext] = pLayer;
}

void wxTdsDatabase::RemoveTdsLayer(TDSCONTEXT* pContext)
{
	wxTdsDatabase::m_ContextLookupMap.erase(pContext);
}

TDSPARAMINFO* wxTdsDatabase::GetParameters(const wxString& strQuery)
{
	TDSPARAMINFO* params = NULL;
	wxChar previous = '\0';
	bool bUnicode = false;
	bool bInStringLiteral = false;
	for (size_t i = 0; i < strQuery.length(); i++)
	{
		wxChar character = strQuery[i];
		if ('\'' == character)
		{
			// Signify that we are inside a string literal inside the SQL
			bInStringLiteral = !bInStringLiteral;
			// Is it a unicode string literal
			if (bInStringLiteral && previous == 'N') bUnicode = true;
		}
		else if (('?' == character) && !bInStringLiteral)
		{
			params = tds_alloc_param_result(params);
			TDSCOLUMN* col = params->columns[params->num_cols-1];
			if (bUnicode)
			{
				tds_set_param_type(m_pDatabase->conn, col, XSYBNVARCHAR);
				col->column_varint_size = 8;
			}
			else
			{
				tds_set_param_type(m_pDatabase->conn, col, XSYBVARCHAR);
				col->column_size = 8000;
				col->on_server.column_size = 8000;
				col->column_cur_size = 8000;
			}
			bUnicode = false;
		}
		previous = character;
	}
	return params;
}

#endif//wxUSE_DATABASE_TDS
