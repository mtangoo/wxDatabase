#ifndef _WX_DATABASE_ODBC_PREPARED_STATEMENT_H_
#define _WX_DATABASE_ODBC_PREPARED_STATEMENT_H_

#include "wx/database/wxprec.h"

WX_DEFINE_ARRAY_PTR(SQLHSTMT, wxOdbcStatementVector);
WX_DEFINE_ARRAY_PTR(wxOdbcParameter*, ArrayOfOdbcParameters);

class wxDatabaseResultSet;

class wxOdbcPreparedStatement : public wxPreparedStatement
{
public:
    // ctor
    wxOdbcPreparedStatement(wxOdbcInterface* pInterface, SQLHENV sqlEnvHandle, SQLHDBC sqlHDBC);
    wxOdbcPreparedStatement(wxOdbcInterface* pInterface, SQLHENV sqlEnvHandle, SQLHDBC sqlHDBC, SQLHSTMT sqlStatementHandle);
    wxOdbcPreparedStatement(wxOdbcInterface* pInterface, SQLHENV sqlEnvHandle, SQLHDBC sqlHDBC, wxOdbcStatementVector statements);

    // dtor
    virtual ~wxOdbcPreparedStatement();

    virtual void Close();
      
    void AddPreparedStatement(SQLHSTMT pStatement);

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
    virtual wxDatabaseResultSet* RunQueryWithResults(bool bLogForCleanup);

    SQLHSTMT GetLastStatement() { return (m_Statements.size() > 0) ? m_Statements[m_Statements.size()-1] : NULL; }
    void SetOneTimer(bool bOneTimer = true) { m_bOneTimeStatement = bOneTimer; }

private:
    void InterpretErrorCodes( long nCode, SQLHSTMT stmth_ptr=NULL );
    void FreeParameters();
    void BindParameters();
    void SetParam(int nPosition, wxOdbcParameter* pParameter);


    int FindStatementAndAdjustPositionIndex(int* pPosition);
 
    bool m_bOneTimeStatement; // Flag to indicate that statement ownership should be handed off to any generated OdbcResultSets
    SQLHENV m_sqlEnvHandle;
    SQLHDBC m_sqlHDBC;
    wxOdbcStatementVector m_Statements;

    ArrayOfOdbcParameters m_Parameters;
    wxOdbcInterface* m_pInterface;
};

#endif//_WX_DATABASE_ODBC_PREPARED_STATEMENT_H_

