#ifndef _WX_DATABASE_TDS_PREPARED_STATEMENT_H_
#define _WX_DATABASE_TDS_PREPARED_STATEMENT_H_

#include "wx/database/wxprec.h"

class wxDatabaseResultSet;

class wxTdsPreparedStatement : public wxPreparedStatement
{
public:
	// ctor
	wxTdsPreparedStatement(TDSSOCKET* pDatabase, TDSDYNAMIC* pStatement, const wxString& strQuery);

	// dtor
	virtual ~wxTdsPreparedStatement();

	virtual void Close();

	// get field
	virtual void SetParamInt(int nPosition, int nValue);
	virtual void SetParamDouble(int nPosition, double dblValue);
	virtual void SetParamString(int nPosition, const wxString& strValue);
	virtual void SetParamNull(int nPosition);
	virtual void SetParamBlob(int nPosition, const void* pData, long nDataLength);
	virtual void SetParamDate(int nPosition, const wxDateTime& dateValue);
	virtual void SetParamBool(int nPosition, bool bValue);
	virtual int GetParameterCount();

	virtual int RunQuery();
	virtual wxDatabaseResultSet* RunQueryWithResults();

private:
	void FreeAllocatedResultSets();
	void AllocateParameter(int nPosition);
	int FindStatementAndAdjustPositionIndex(int* pPosition);
	void SetErrorInformationFromDatabaseLayer();

	TDSSOCKET* m_pDatabase;
	TDSDYNAMIC* m_pStatement;
	wxString m_strOriginalQuery;
};

#endif//_WX_DATABASE_TDS_PREPARED_STATEMENT_H_

