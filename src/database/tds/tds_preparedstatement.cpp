#include "wx/database/wxprec.h"

#if wxUSE_DATABASE_TDS

//#include <time.h>

// ctor
wxTdsPreparedStatement::wxTdsPreparedStatement(TDSSOCKET* pDatabase, const wxString& strQuery/*AML start*/, int nTdsVersion/*AML end*/)
: wxPreparedStatement()
{
	m_pDatabase = pDatabase;
	m_pStatement = NULL;
	m_pParameters = NULL;
	m_strOriginalQuery = strQuery;

	//AML start
	m_nTdsVersion = nTdsVersion;
	//AML end
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
	/* -- It looks like tds_free_bcp_column_data is taken care of by tds_free_dynamic
	if (m_pParameters != NULL)
	{
	for (int i=0; i<m_pParameters->num_cols; i++)
	{
	TDSCOLUMN* curcol = m_pParameters->columns[i];
	if ((curcol != NULL) && (curcol->bcp_column_data != NULL))
	tds_free_bcp_column_data(curcol->bcp_column_data);
	}
	}
	*/

	//AML start
	if (m_pStatement != NULL)
	//AML end
	tds_free_dynamic(m_pDatabase, m_pStatement);

	// Double check that result sets are de-allocated
	FreeAllocatedResultSets();
}

void wxTdsPreparedStatement::FreeAllocatedResultSets()
{
	//fprintf(stderr, "In FreeAllocatedResultSets\n");
	int rc;
	int result_type;
	TDS_INT8 saved_rows_affected = 0; //AML
	while ((rc = tds_process_tokens(m_pDatabase, &result_type, NULL, TDS_TOKEN_RESULTS)) == TDS_SUCCEED)
	{
		switch (result_type)
		{
		case TDS_DONE_RESULT:
		case TDS_DONEPROC_RESULT:
			//AML case TDS_DONEINPROC_RESULT:
			/* ignore possible spurious result (TDS7+ send it) */
		case TDS_STATUS_RESULT:
			break;
			//AML start
		case TDS_DONEINPROC_RESULT:
			saved_rows_affected = m_pDatabase->rows_affected;
			break;
			//AML end
		case TDS_ROWFMT_RESULT:
		case TDS_COMPUTEFMT_RESULT:
		case TDS_DESCRIBE_RESULT:
			break;
		case TDS_ROW_RESULT:
			//fprintf(stderr, "Warning:  wxTdsPreparedStatement query should not return results.  Type: %d\n", result_type);
			if (m_pDatabase->current_results && m_pDatabase->current_results->num_cols > 0)
			{
				while (tds_process_tokens(m_pDatabase, &result_type, NULL, TDS_STOPAT_ROWFMT|TDS_RETURN_DONE|TDS_RETURN_ROW) == TDS_SUCCEED)
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

	//AML start
	if (m_pDatabase != NULL)
		m_pDatabase->rows_affected = saved_rows_affected;
	//AML end

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
	while ((m_pParameters == NULL) || (nPosition > (m_pParameters->num_cols)))
	{
		//m_pParameters = tds_alloc_param_result(m_pStatement->params);
		m_pParameters = tds_alloc_param_result(m_pParameters);
		if (m_pParameters == NULL)
		{
			//fprintf(stderr, "out of memory!\n");
			SetErrorCode(wxDATABASE_ALLOCATION_ERROR);
			SetErrorMessage(wxT("out of memory"));
			ThrowDatabaseException();
			return;
		}
		//m_pStatement->params = m_pParameters;
	}

}

// set parameter
void wxTdsPreparedStatement::SetParamInt(int nPosition, int nValue)
{
	//fprintf(stderr, "Setting param %d to int %d\n", nPosition, nValue);
	ResetErrorCodes();

	AllocateParameter(nPosition);
	TDSCOLUMN* curcol = m_pParameters->columns[nPosition-1];
	curcol->column_type = SYBINTN;
	curcol->on_server.column_type = SYBINTN;
	curcol->column_size = sizeof(TDS_INT);
	curcol->on_server.column_size = sizeof(TDS_INT);
	curcol->column_varint_size = 1;
	curcol->column_cur_size = sizeof(TDS_INT);

	//tds_alloc_param_data(m_pParameters, curcol);
	tds_alloc_param_data(curcol);
	memcpy(curcol->column_data, &nValue, sizeof(nValue));
}

void wxTdsPreparedStatement::SetParamDouble(int nPosition, double dblValue)
{
	//fprintf(stderr, "Setting param %d to double %f\n", nPosition, dblValue);
	ResetErrorCodes();

	AllocateParameter(nPosition);
	TDSCOLUMN* curcol = m_pParameters->columns[nPosition-1];
	curcol->column_type = SYBFLTN;
	curcol->on_server.column_type = SYBFLTN;
	curcol->column_size = sizeof(TDS_FLOAT);
	curcol->on_server.column_size = sizeof(TDS_FLOAT);
	curcol->column_varint_size = 1;
	curcol->column_cur_size = sizeof(TDS_FLOAT);

	//tds_alloc_param_data(m_pParameters, curcol);
	tds_alloc_param_data(curcol);
	memcpy(curcol->column_data, &dblValue, sizeof(dblValue));
}

void wxTdsPreparedStatement::SetParamString(int nPosition, const wxString& strValue)
{
	//fprintf(stderr, "Setting param %d to string '%s'\n", nPosition, strValue.c_str());
	ResetErrorCodes();

	AllocateParameter(nPosition);
	//wxCharBuffer valueBuffer = ConvertToUnicodeStream(strValue);
	//int nLength = GetEncodedStreamLength(strValue);
	const char* valueBuffer = strValue.mb_str();
	int nLength = strValue.Len();
	TDSCOLUMN* curcol = m_pParameters->columns[nPosition-1];
	//AML start
	if (m_nTdsVersion >= wxTdsDatabase::TDS_70)
	{
		curcol->column_type = XSYBVARCHAR;
		curcol->on_server.column_type = XSYBVARCHAR;
		curcol->column_size = nLength+1;
		curcol->column_varint_size = 2;
		curcol->column_cur_size = nLength;
	}
	else
	{
		//AML end
		curcol->column_type = SYBVARCHAR;
		curcol->on_server.column_type = SYBVARCHAR;
		curcol->column_size = 40;//nLength+1;//40;
		//curcol->on_server.column_size = 40;
		curcol->column_varint_size = 1;
		curcol->column_cur_size = nLength+1;
		//AML start
	}
	//AML end

	//tds_alloc_param_data(m_pParameters, curcol);
	tds_alloc_param_data(curcol);
	memcpy(curcol->column_data, valueBuffer, nLength+1);
}

void wxTdsPreparedStatement::SetParamNull(int nPosition)
{
	//fprintf(stderr, "Setting param %d to NULL\n", nPosition);
	ResetErrorCodes();

	AllocateParameter(nPosition);
	TDSCOLUMN* curcol = m_pParameters->columns[nPosition-1];
	//AML start
	if (m_nTdsVersion >= wxTdsDatabase::TDS_70)
	{
		curcol->column_type = XSYBVARCHAR;
		curcol->on_server.column_type = XSYBVARCHAR;
		curcol->column_varint_size = 2;
		curcol->column_cur_size = -1;
	}
	else
	{
		//AML end
		curcol->column_type = SYBVARCHAR;
		curcol->on_server.column_type = SYBVARCHAR;
		//curcol->column_size = sizeof(TDS_INT);
		//curcol->on_server.column_size = 40;
		curcol->column_varint_size = 1;
		//curcol->column_cur_size = nLength;
		//curcol->column_nullbind = -1;
		curcol->column_cur_size = -1;
		//AML start
	}
	//AML end

	//tds_alloc_param_data(m_pParameters, curcol);
	tds_alloc_param_data(curcol);
	//AMLcurcol->column_data = NULL;

	//tds_alloc_param_data(m_pParameters, curcol);
	//memcpy(curcol->column_data, valueBuffer, nLength);
}

void wxTdsPreparedStatement::SetParamBlob(int nPosition, const void* pData, long nDataLength)
{
	//fprintf(stderr, "Setting param %d to blob\n", nPosition);
	ResetErrorCodes();

	AllocateParameter(nPosition);
	/*
	TDSCOLUMN* curcol = m_pParameters->columns[nPosition-1];
	curcol->column_type = SYBIMAGE;
	curcol->on_server.column_type = SYBIMAGE;
	curcol->column_size = 40;
	//curcol->on_server.column_size = 40;
	curcol->column_varint_size = 1;
	curcol->column_cur_size = 40;
	fprintf(stderr, "Setting blob column size to %d\n", nDataLength);

	tds_alloc_param_data(m_pParameters, curcol);

	BCPCOLDATA* pBcpColData = tds_alloc_bcp_column_data(nDataLength);
	curcol->bcp_column_data = pBcpColData;
	curcol->bcp_column_data->datalen = nDataLength;
	memcpy(curcol->bcp_column_data->data, pData, nDataLength);
	*/
	CONV_RESULT cr;
	//fprintf(stderr, "data length = %ld\n", nDataLength);
	int ret = tds_convert(this->m_pDatabase->tds_ctx, SYBBINARY, (TDS_CHAR*)pData, nDataLength, SYBVARBINARY, &cr);
	//fprintf(stderr, "tds_convert returned %d, data length = %ld\n", ret, nDataLength);
	TDSCOLUMN* curcol = m_pParameters->columns[nPosition-1];
	//AML start
	if (m_nTdsVersion >= wxTdsDatabase::TDS_70)
	{
		curcol->column_type = XSYBVARBINARY;
		curcol->on_server.column_type = XSYBVARBINARY;
		curcol->column_size = ret;
		curcol->on_server.column_size = ret;
		curcol->column_varint_size = 2;
		curcol->column_cur_size = ret;

	}
	else
	{
		//AML end
		curcol->column_type = SYBVARBINARY;
		curcol->on_server.column_type = SYBVARBINARY;
		curcol->column_size = ret;
		curcol->on_server.column_size = ret;
		curcol->column_varint_size = 1;
		curcol->column_cur_size = ret;
		//AML start
	}
	//AML end

	//tds_alloc_param_data(m_pParameters, curcol);
	tds_alloc_param_data(curcol);

	//fprintf(stderr, "Ready for memcpy of %d bytes\n", ret);
	memcpy(curcol->column_data, cr.ib, ret);
	//fprintf(stderr, "Memcpy completed\n");

	//AML start
	free(cr.ib);
	//AML end
}

void wxTdsPreparedStatement::SetParamDate(int nPosition, const wxDateTime& dateValue)
{
	//fprintf(stderr, "Setting param %d to date %s\n", nPosition, dateValue.Format().c_str());
	ResetErrorCodes();

	AllocateParameter(nPosition);

	wxString dateAsString = dateValue.Format(_("%Y-%m-%d %H:%M:%S"));
	//fprintf(stderr, "Setting param %d to date %s\n", nPosition, dateAsString.c_str());
	CONV_RESULT cr;
	wxCharBuffer dateCharBuffer = ConvertToUnicodeStream(dateAsString);
	int bufferLength = GetEncodedStreamLength(dateAsString);
	int ret = tds_convert(this->m_pDatabase->tds_ctx, SYBVARCHAR, 
		(TDS_CHAR*)(const char*)dateCharBuffer, bufferLength, SYBDATETIME, &cr);
	//fprintf(stderr, "tds_convert returned %d, sizeof TDS_DATETIME = %d\n", ret, sizeof(TDS_DATETIME));

	int valueSize = (ret < 0) ? sizeof(TDS_DATETIME) : ret;

	TDSCOLUMN* curcol = m_pParameters->columns[nPosition-1];
	curcol->column_type = SYBDATETIMN;
	curcol->on_server.column_type = SYBDATETIMN;
	curcol->column_size =  valueSize;
	curcol->on_server.column_size =  valueSize;
	curcol->column_varint_size = 1;
	curcol->column_cur_size = valueSize;

	//tds_alloc_param_data(m_pParameters, curcol);
	tds_alloc_param_data(curcol);
	memcpy(curcol->column_data, &cr.dt, valueSize);
}

void wxTdsPreparedStatement::SetParamBool(int nPosition, bool bValue)
{
	//fprintf(stderr, "Setting param %d to boolean %d\n", nPosition, bValue);
	ResetErrorCodes();

	AllocateParameter(nPosition);
	TDSCOLUMN* curcol = m_pParameters->columns[nPosition-1];
	curcol->column_type = SYBBITN;
	curcol->on_server.column_type = SYBBITN;
	curcol->column_size = sizeof(bool);
	curcol->on_server.column_size = sizeof(bool);
	curcol->column_varint_size = 1;
	curcol->column_cur_size = sizeof(TDS_DATETIME);

	//tds_alloc_param_data(m_pParameters, curcol);
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
	ResetErrorCodes();

	//fprintf(stderr, "Running statement\n");
	//fprintf(stderr, "m_pDatabase %d\n", m_pDatabase);
	//fprintf(stderr, "m_pStatement %d\n", m_pStatement);
	//fprintf(stderr, "m_pParameters %d\n", m_pParameters);
	// Prepare the statement
	wxCharBuffer sqlBuffer = ConvertToUnicodeStream(m_strOriginalQuery);
	//fprintf(stderr, "Preparing statement: '%s'\n", (const char*)sqlBuffer);
	//AML start
	if (m_pStatement != NULL)
	{
		m_pStatement->params = NULL;
	}
	//AML end
	m_pStatement = NULL;
	int nReturn = tds_submit_prepare(m_pDatabase, sqlBuffer, NULL, &m_pStatement, m_pParameters);
	if (nReturn != TDS_SUCCEED)
	{
		//fprintf(stderr, "tds_submit_prepare() failed for query '%s'\n", m_strOriginalQuery.c_str());

		if (m_pStatement != NULL)
			tds_free_dynamic(m_pDatabase, m_pStatement);
		if (m_pParameters != NULL)
			tds_free_param_results(m_pParameters);

		SetErrorInformationFromDatabaseLayer();
		FreeAllocatedResultSets();
		ThrowDatabaseException();
		return wxDATABASE_QUERY_RESULT_ERROR;
	}
	FreeAllocatedResultSets();
	tds_free_input_params(m_pStatement);
	m_pStatement->params = m_pParameters;

	// Execute the query
	nReturn = tds_submit_execute(m_pDatabase, m_pStatement);
	if (nReturn != TDS_SUCCEED)
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
	// Prepare the statement
	wxCharBuffer sqlBuffer = ConvertToUnicodeStream(m_strOriginalQuery);
	//fprintf(stderr, "Preparing statement: '%s'\n", (const char*)sqlBuffer);
	//AML start
	if (m_pStatement != NULL)
	{
		m_pStatement->params = NULL;
	}
	//AML end
	m_pStatement = NULL;
	int nReturn = tds_submit_prepare(m_pDatabase, sqlBuffer, NULL, &m_pStatement, m_pParameters);
	if (nReturn != TDS_SUCCEED)
	{
		//fprintf(stderr, "tds_submit_prepare() failed for query '%s'\n", m_strOriginalQuery.c_str());

		if (m_pStatement != NULL)
			tds_free_dynamic(m_pDatabase, m_pStatement);
		if (m_pParameters != NULL)
			tds_free_param_results(m_pParameters);

		SetErrorInformationFromDatabaseLayer();
		FreeAllocatedResultSets();
		ThrowDatabaseException();
		return NULL;
	}
	FreeAllocatedResultSets();
	tds_free_input_params(m_pStatement);
	m_pStatement->params = m_pParameters;

	// Execute the query
	//fprintf(stderr, "Statement prepared.  Ready to execute\n");
	nReturn = tds_submit_execute(m_pDatabase, m_pStatement);
	if (nReturn != TDS_SUCCEED)
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
	wxTdsDatabase* pDatabase = wxTdsDatabase::LookupTdsLayer(m_pDatabase->tds_ctx);
	if (pDatabase != NULL)
	{
		SetErrorCode(pDatabase->GetErrorCode());
		SetErrorMessage(pDatabase->GetErrorMessage());
	}
}

#endif//wxUSE_DATABASE_TDS
