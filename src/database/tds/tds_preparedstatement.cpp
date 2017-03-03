#include "wx/database/wxprec.h"

#if wxUSE_DATABASE_TDS

// ctor
wxTdsPreparedStatement::wxTdsPreparedStatement(TDSSOCKET* pDatabase, TDSDYNAMIC* pStatement, const wxString& strQuery)
: wxPreparedStatement()
{
	m_pDatabase = pDatabase;
	m_pStatement = pStatement;
	m_strOriginalQuery = strQuery;
}

// dtor
wxTdsPreparedStatement::~wxTdsPreparedStatement()
{
	Close();
}

void wxTdsPreparedStatement::Close()
{
	CloseResultSets();

	FreeAllocatedResultSets();

	tds_submit_unprepare(m_pDatabase, m_pStatement);

	if (m_pStatement != NULL)
		tds_release_dynamic(&m_pStatement);

	// Double check that result sets are de-allocated
	FreeAllocatedResultSets();
}

void wxTdsPreparedStatement::FreeAllocatedResultSets()
{
	int rc;
	int result_type;
	TDS_INT8 saved_rows_affected = 0; //AML
	while ((rc = tds_process_tokens(m_pDatabase, &result_type, NULL, TDS_TOKEN_RESULTS)) == TDS_SUCCESS)
	{
		switch (result_type)
		{
		case TDS_DONE_RESULT:
		case TDS_DONEPROC_RESULT:
			/* ignore possible spurious result (TDS7+ send it) */
		case TDS_STATUS_RESULT:
			break;
		case TDS_DONEINPROC_RESULT:
			saved_rows_affected = m_pDatabase->rows_affected;
			break;
		case TDS_ROWFMT_RESULT:
		case TDS_COMPUTEFMT_RESULT:
		case TDS_DESCRIBE_RESULT:
			break;
		case TDS_ROW_RESULT:
			//fprintf(stderr, "Warning:  wxTdsPreparedStatement query should not return results.  Type: %d\n", result_type);
			if (m_pDatabase->current_results && m_pDatabase->current_results->num_cols > 0)
			{
				while (tds_process_tokens(m_pDatabase, &result_type, NULL, TDS_STOPAT_ROWFMT|TDS_RETURN_DONE|TDS_RETURN_ROW) == TDS_SUCCESS)
				{
					//fprintf(stderr, "Warning:  wxTdsPreparedStatement TDS_ROW_RESULT query should not return results.  Type: %d\n", result_type);
					if (result_type != TDS_ROW_RESULT)
						break;
					if (!m_pDatabase->current_results)
						continue;
				}
			}
			return;
		default:
			//fprintf(stderr, "Error:  wxTdsPreparedStatement query should not return results.  Type: %d\n", result_type);
			return;
		}
	}

	if (m_pDatabase != NULL)
		m_pDatabase->rows_affected = saved_rows_affected;

	// Clean up after ourselves
	if (m_pDatabase != NULL)
		tds_free_all_results(m_pDatabase);

	if (rc == TDS_FAIL)
	{
		//fprintf(stderr, "tds_process_tokens() returned TDS_FAIL\n");
		SetErrorInformationFromDatabaseLayer();
		ThrowDatabaseException();
		return;
	}
	else if (rc != TDS_NO_MORE_RESULTS)
	{
		//fprintf(stderr, "tds_process_tokens() unexpected return\n");
		SetErrorInformationFromDatabaseLayer();
		ThrowDatabaseException();
		return;
	}
}

void wxTdsPreparedStatement::AllocateParameter(int nPosition)
{
	TDSPARAMINFO* pParameters = m_pStatement->params;
	while ((pParameters == NULL) || (nPosition > (pParameters->num_cols)))
	{
		pParameters = tds_alloc_param_result(pParameters);
		if (pParameters == NULL)
		{
			//fprintf(stderr, "out of memory!\n");
			SetErrorCode(wxDATABASE_ALLOCATION_ERROR);
			SetErrorMessage(wxT("out of memory"));
			ThrowDatabaseException();
			return;
		}
	}
	m_pStatement->params = pParameters;
}

// set parameter
void wxTdsPreparedStatement::SetParamInt(int nPosition, int nValue)
{
	//fprintf(stderr, "Setting param %d to int %d\n", nPosition, nValue);
	ResetErrorCodes();

	AllocateParameter(nPosition);
	TDSCOLUMN* curcol = m_pStatement->params->columns[nPosition-1];
	tds_set_param_type(m_pDatabase->conn, curcol, SYBINTN);
	curcol->column_size = sizeof(TDS_INT);
	curcol->on_server.column_size = sizeof(TDS_INT);
	curcol->column_cur_size = sizeof(TDS_INT);

	tds_alloc_param_data(curcol);
	memcpy(curcol->column_data, &nValue, sizeof(nValue));
}

void wxTdsPreparedStatement::SetParamDouble(int nPosition, double dblValue)
{
	//fprintf(stderr, "Setting param %d to double %f\n", nPosition, dblValue);
	ResetErrorCodes();

	AllocateParameter(nPosition);
	TDSCOLUMN* curcol = m_pStatement->params->columns[nPosition-1];
	tds_set_param_type(m_pDatabase->conn, curcol, SYBFLTN);
	curcol->column_size = sizeof(TDS_FLOAT);
	curcol->on_server.column_size = sizeof(TDS_FLOAT);
	curcol->column_cur_size = sizeof(TDS_FLOAT);

	tds_alloc_param_data(curcol);
	memcpy(curcol->column_data, &dblValue, sizeof(dblValue));
}

void wxTdsPreparedStatement::SetParamString(int nPosition, const wxString& strValue)
{
	//fprintf(stderr, "Setting param %d to string '%s'\n", nPosition, strValue.c_str());
	ResetErrorCodes();

	// AML
	// it will not be possible to set a UNICODE parameter string unless
	// we set up curcol

	AllocateParameter(nPosition);

	wxCharBuffer valueBuffer = ConvertToUnicodeStream(strValue);
	int nLength = GetEncodedStreamLength(strValue);

	//const wchar_t* valueBuffer = strValue.wc_str();

	//const char* valueBuffer = strValue.mb_str();
	//int nLength = strValue.Len();
	TDSCOLUMN* curcol = m_pStatement->params->columns[nPosition-1];
	tds_set_param_type(m_pDatabase->conn, curcol, XSYBNVARCHAR);
	curcol->column_size = nLength+1;
	curcol->column_cur_size = nLength;

	tds_alloc_param_data(curcol);
	memcpy(curcol->column_data, valueBuffer, nLength+1);
}

void wxTdsPreparedStatement::SetParamNull(int nPosition)
{
	//fprintf(stderr, "Setting param %d to NULL\n", nPosition);
	ResetErrorCodes();

	AllocateParameter(nPosition);
	TDSCOLUMN* curcol = m_pStatement->params->columns[nPosition-1];
	tds_set_param_type(m_pDatabase->conn, curcol, SYBVARCHAR);

	tds_alloc_param_data(curcol);

	//tds_alloc_param_data(m_pParameters, curcol);
	//memcpy(curcol->column_data, valueBuffer, nLength);
}

void wxTdsPreparedStatement::SetParamBlob(int nPosition, const void* pData, long nDataLength)
{
	//fprintf(stderr, "Setting param %d to blob\n", nPosition);
	ResetErrorCodes();

	AllocateParameter(nPosition);
	CONV_RESULT cr;
	//fprintf(stderr, "data length = %ld\n", nDataLength);
	int ret = tds_convert(tds_get_ctx(this->m_pDatabase), SYBBINARY, (TDS_CHAR*)pData, nDataLength, SYBVARBINARY, &cr);
	//fprintf(stderr, "tds_convert returned %d, data length = %ld\n", ret, nDataLength);
	TDSCOLUMN* curcol = m_pStatement->params->columns[nPosition-1];
	tds_set_param_type(m_pDatabase->conn, curcol, SYBVARBINARY);
	curcol->column_size = ret;
	curcol->on_server.column_size = ret;
	curcol->column_cur_size = ret;

	tds_alloc_param_data(curcol);
	//fprintf(stderr, "Ready for memcpy of %d bytes\n", ret);
	memcpy(curcol->column_data, cr.ib, ret);
	int x = sizeof(cr.ib);
	//fprintf(stderr, "Memcpy completed\n");
	free(cr.ib);
}

void wxTdsPreparedStatement::SetParamDate(int nPosition, const wxDateTime& dateValue)
{
	//fprintf(stderr, "Setting param %d to date %s\n", nPosition, dateValue.Format().c_str());
	ResetErrorCodes();

	AllocateParameter(nPosition);

	wxString dateAsString = dateValue.Format(_("%Y-%m-%d %H:%M:%S"));
	//fprintf(stderr, "Setting param %d to date %s\n", nPosition, dateAsString.c_str());

	// for some unknown reason when sending as a SYBDATETIME
	// sub-minute information is being lost (as if it is a SMALLDATETIME)
	// so send as a SYBVARCHAR instead

#if 0
	CONV_RESULT cr;
	wxCharBuffer dateCharBuffer = ConvertToUnicodeStream(dateAsString);
	int bufferLength = GetEncodedStreamLength(dateAsString);
	int ret = tds_convert(tds_get_ctx(this->m_pDatabase), SYBVARCHAR, (TDS_CHAR*)(const char*)dateCharBuffer, bufferLength, SYBDATETIME, &cr);
	//fprintf(stderr, "tds_convert returned %d, sizeof TDS_DATETIME = %d\n", ret, sizeof(TDS_DATETIME));

	int valueSize = (ret < 0) ? sizeof(TDS_DATETIME) : ret;

	TDSCOLUMN* curcol = m_pStatement->params->columns[nPosition-1];
	tds_set_param_type(m_pDatabase->conn, curcol, SYBDATETIMN);
	curcol->column_size =  valueSize;
	curcol->on_server.column_size =  valueSize;
	curcol->column_cur_size = valueSize;

	tds_alloc_param_data(curcol);
	memcpy(curcol->column_data, &cr.dt, valueSize);
#else
	SetParamString(nPosition, dateAsString);
#endif
}

void wxTdsPreparedStatement::SetParamBool(int nPosition, bool bValue)
{
	//fprintf(stderr, "Setting param %d to boolean %d\n", nPosition, bValue);
	ResetErrorCodes();

	AllocateParameter(nPosition);
	TDSCOLUMN* curcol = m_pStatement->params->columns[nPosition-1];
	tds_set_param_type(m_pDatabase->conn, curcol, SYBBITN);
	curcol->column_size = sizeof(bool);
	curcol->on_server.column_size = sizeof(bool);
	curcol->column_cur_size = sizeof(bool); // TDS_DATETIME

	tds_alloc_param_data(curcol);
	memcpy(curcol->column_data, &bValue , sizeof(bool));
}

int wxTdsPreparedStatement::GetParameterCount()
{
	ResetErrorCodes();

	int nReturn = 0;
	// It would probably be better to iterate through the query string to make sure that
	//  none of the '?' are in string literals
	//fprintf(stderr, "Freq of '%s' = %d\n", m_strOriginalQuery, m_strOriginalQuery.Freq('?'));
	//AML start
	//nReturn = m_strOriginalQuery.Freq('?');
	bool bInStringLiteral = false;
	for (size_t i = 0; i < m_strOriginalQuery.length(); i++)
	{
		wxChar character = m_strOriginalQuery[i];
		if ('\'' == character)
		{
			// Signify that we are inside a string literal inside the SQL
			bInStringLiteral = !bInStringLiteral;
		}
		else if (('?' == character) && !bInStringLiteral)
		{
			nReturn++;
		}
	}
	//AML end

	return nReturn;
}

int wxTdsPreparedStatement::RunQuery()
{
	//fprintf(stderr, "Running statement without results\n");
	ResetErrorCodes();

	FreeAllocatedResultSets();

	// Execute the query
	int nReturn = tds_submit_execute(m_pDatabase, m_pStatement);
	if (nReturn != TDS_SUCCESS)
	{
		//fprintf(stderr, "tds_submit_execute() failed for statement '%s'\n", m_pStatement->query);
		SetErrorInformationFromDatabaseLayer();
		FreeAllocatedResultSets();
		ThrowDatabaseException();
	}

	//fprintf(stderr, "Statement executed.  Freeing results\n");
	FreeAllocatedResultSets();

	return /*AML wxDATABASE_QUERY_RESULT_ERROR*/m_pDatabase->rows_affected;
}

wxDatabaseResultSet* wxTdsPreparedStatement::RunQueryWithResults()
{
	ResetErrorCodes();

	//fprintf(stderr, "Running statement with results\n");
	FreeAllocatedResultSets();

	// Execute the query
	int nReturn = tds_submit_execute(m_pDatabase, m_pStatement);
	if (nReturn != TDS_SUCCESS)
	{
		//fprintf(stderr, "tds_submit_execute() failed for query '%s'\n", m_pStatement->query);
		//fprintf(stderr, "tds_submit_execute() return code: %d\n", nReturn);
		SetErrorInformationFromDatabaseLayer();
		FreeAllocatedResultSets();
		ThrowDatabaseException();
		return NULL;
	}
	wxTdsResultSet* pResultSet = new wxTdsResultSet(m_pDatabase);
	if (pResultSet)
		pResultSet->SetEncoding(GetEncoding());

	LogResultSetForCleanup(pResultSet);
	//fprintf(stderr, "Returning prepared statement result set\n");
	return pResultSet;
}

int wxTdsPreparedStatement::FindStatementAndAdjustPositionIndex(int* WXUNUSED(pPosition))
{
	return 0;
}

void wxTdsPreparedStatement::SetErrorInformationFromDatabaseLayer()
{
	wxTdsDatabase* pDatabase = wxTdsDatabase::LookupTdsLayer(/*AML this->m_pDatabase->tds_ctx*/tds_get_ctx(this->m_pDatabase));
	if (pDatabase != NULL)
	{
		SetErrorCode(pDatabase->GetErrorCode());
		SetErrorMessage(pDatabase->GetErrorMessage());
	}
}

#endif//wxUSE_DATABASE_TDS
