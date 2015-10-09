#ifndef _WX_DATABASE_MYSQL_INTERFACES_H_
#define _WX_DATABASE_MYSQL_INTERFACES_H_

#include "wx/database/wxprec.h"
#include <wx/dynlib.h>

typedef void (STDCALL *MysqlServerEndType)(void);
typedef MYSQL* (STDCALL *MysqlInitType)(MYSQL*);
typedef MYSQL* (STDCALL *MysqlRealConnectType)(MYSQL*, const char*, const char*,
  const char*, const char*, unsigned int, const char*, unsigned long);
typedef int (STDCALL *MysqlRealQueryType)(MYSQL*, const char*, unsigned long);
typedef const char* (STDCALL *MysqlErrorType)(MYSQL*);
typedef unsigned int (STDCALL *MysqlErrnoType)(MYSQL*);
typedef void (STDCALL *MysqlCloseType)(MYSQL*);
typedef my_bool (STDCALL *MysqlAutoCommitType)(MYSQL*, my_bool);
typedef my_bool (STDCALL *MysqlCommitType)(MYSQL*);
typedef my_bool (STDCALL *MysqlRollbackType)(MYSQL*);
typedef int (STDCALL *MysqlQueryType)(MYSQL*, const char*);
typedef my_ulonglong(STDCALL *MysqlAffectedRowsType)(MYSQL*);
typedef MYSQL_STMT* (STDCALL *MysqlStmtInitType)(MYSQL*);
typedef int (STDCALL *MysqlStmtPrepareType)(MYSQL_STMT*, const char*, unsigned long);
typedef int (STDCALL *MysqlStmtExecuteType)(MYSQL_STMT*);
typedef const char* (STDCALL *MysqlStmtErrorType)(MYSQL_STMT*);
typedef unsigned int (STDCALL *MysqlStmtErrnoType)(MYSQL_STMT*);
typedef my_bool (STDCALL *MysqlStmtFreeResultType)(MYSQL_STMT*);
typedef my_bool (STDCALL *MysqlStmtCloseType)(MYSQL_STMT*);
typedef MYSQL_RES* (STDCALL *MysqlListTablesType)(MYSQL*, const char*);

typedef MYSQL_ROW (STDCALL *MysqlFetchRowType)(MYSQL_RES*);
typedef MYSQL_FIELD * (STDCALL *MysqlFetchFieldType)(MYSQL_RES*);

typedef void (STDCALL *MysqlFreeResultType)(MYSQL_RES*);
typedef unsigned long (STDCALL *MysqlGetServerVersionType)(MYSQL*);
typedef MYSQL_RES* (STDCALL *MysqlStmtResultMetadataType)(MYSQL_STMT*);
typedef unsigned int (STDCALL *MysqlNumFieldsType)(MYSQL_RES*);
typedef unsigned long (STDCALL *MysqlStmtParamCountType)(MYSQL_STMT*);
typedef my_bool (STDCALL *MysqlStmtBindParamType)(MYSQL_STMT*, MYSQL_BIND*);
typedef int (STDCALL *MysqlStmtFetchType)(MYSQL_STMT*);
typedef my_bool (STDCALL *MysqlStmtBindResultType)(MYSQL_STMT*, MYSQL_BIND*);
typedef int (STDCALL *MysqlSelectDatabaseType)(MYSQL *mysql, const char *db);
typedef int (STDCALL *MysqlStmtAffectedRows)(MYSQL_STMT*);


class wxMysqlDynamicInterface
{
public:
  wxMysqlDynamicInterface() { }
  bool Init();

  MysqlServerEndType GetMysqlServerEnd() { return m_pMysqlServerEnd; }
  MysqlInitType GetMysqlInit() { return m_pMysqlInit; }
  MysqlRealConnectType GetMysqlRealConnect() { return m_pMysqlRealConnect; }
  MysqlRealQueryType GetMysqlRealQuery() { return m_pMysqlRealQuery; }
  MysqlErrorType GetMysqlError() { return m_pMysqlError; }
  MysqlErrnoType GetMysqlErrno() { return m_pMysqlErrno; }
  MysqlCloseType GetMysqlClose() { return m_pMysqlClose; }
  MysqlAutoCommitType GetMysqlAutoCommit() { return m_pMysqlAutoCommit; }
  MysqlCommitType GetMysqlCommit() { return m_pMysqlCommit; }
  MysqlRollbackType GetMysqlRollback() { return m_pMysqlRollback; }
  MysqlQueryType GetMysqlQuery() { return m_pMysqlQuery; }
  MysqlAffectedRowsType GetMysqlAffectedRows() { return m_pMysqlAffectedRows; }
  MysqlStmtAffectedRows GetMysqlStmtAffectedRows() { return m_pMysqlStmtAffectedRows; }
  MysqlStmtInitType GetMysqlStmtInit() { return m_pMysqlStmtInit; }
  MysqlStmtPrepareType GetMysqlStmtPrepare() { return m_pMysqlStmtPrepare; }
  MysqlStmtExecuteType GetMysqlStmtExecute() { return m_pMysqlStmtExecute; }
  MysqlStmtErrorType GetMysqlStmtError() { return m_pMysqlStmtError; }
  MysqlStmtErrnoType GetMysqlStmtErrno() { return m_pMysqlStmtErrno; }
  MysqlStmtFreeResultType GetMysqlStmtFreeResult() { return m_pMysqlStmtFreeResult; }
  MysqlStmtCloseType GetMysqlStmtClose() { return m_pMysqlStmtClose; }
  MysqlListTablesType GetMysqlListTables() { return m_pMysqlListTables; }
  
  MysqlFetchRowType GetMysqlFetchRow() { return m_pMysqlFetchRow; }
  MysqlFetchFieldType GetMysqlFetchField() { return m_pMysqlFetchField; }
  
  MysqlFreeResultType GetMysqlFreeResult() { return m_pMysqlFreeResult; }
  MysqlGetServerVersionType GetMysqlGetServerVersion() { return m_pMysqlGetServerVersion; }
  MysqlStmtResultMetadataType GetMysqlStmtResultMetadata() { return m_pMysqlStmtResultMetadata; }
  MysqlNumFieldsType GetMysqlNumFields() { return m_pMysqlNumFields; }
  MysqlStmtParamCountType GetMysqlStmtParamCount() { return m_pMysqlStmtParamCount; }
  MysqlStmtBindParamType GetMysqlStmtBindParam() { return m_pMysqlStmtBindParam; }
  MysqlStmtFetchType GetMysqlStmtFetch() { return m_pMysqlStmtFetch; }
  MysqlStmtBindResultType GetMysqlStmtBindResult() { return m_pMysqlStmtBindResult; }
  MysqlSelectDatabaseType GetMysqlSelectDatabase() { return m_pMysqlSelectDatabase; }
  
private:
  wxDynamicLibrary m_MysqlDLL;

  MysqlServerEndType m_pMysqlServerEnd;
  MysqlInitType m_pMysqlInit;
  MysqlRealConnectType m_pMysqlRealConnect;
  MysqlRealQueryType m_pMysqlRealQuery;
  MysqlErrorType m_pMysqlError;
  MysqlErrnoType m_pMysqlErrno;
  MysqlCloseType m_pMysqlClose;
  MysqlAutoCommitType m_pMysqlAutoCommit;
  MysqlCommitType m_pMysqlCommit;
  MysqlRollbackType m_pMysqlRollback;
  MysqlQueryType m_pMysqlQuery;
  MysqlAffectedRowsType m_pMysqlAffectedRows;
  MysqlStmtAffectedRows m_pMysqlStmtAffectedRows;
  MysqlStmtInitType m_pMysqlStmtInit;
  MysqlStmtPrepareType m_pMysqlStmtPrepare;
  MysqlStmtExecuteType m_pMysqlStmtExecute;
  MysqlStmtErrorType m_pMysqlStmtError;
  MysqlStmtErrnoType m_pMysqlStmtErrno;
  MysqlStmtFreeResultType m_pMysqlStmtFreeResult;
  MysqlStmtCloseType m_pMysqlStmtClose;
  MysqlListTablesType m_pMysqlListTables;
  MysqlFetchRowType m_pMysqlFetchRow;
  MysqlFetchFieldType m_pMysqlFetchField;
  MysqlFreeResultType m_pMysqlFreeResult;
  MysqlGetServerVersionType m_pMysqlGetServerVersion;
  MysqlStmtResultMetadataType m_pMysqlStmtResultMetadata;
  MysqlNumFieldsType m_pMysqlNumFields;
  MysqlStmtParamCountType m_pMysqlStmtParamCount;
  MysqlStmtBindParamType m_pMysqlStmtBindParam;
  MysqlStmtFetchType m_pMysqlStmtFetch;
  MysqlStmtBindResultType m_pMysqlStmtBindResult;
  MysqlSelectDatabaseType m_pMysqlSelectDatabase;
};

#endif//_WX_DATABASE_MYSQL_INTERFACES_H_
