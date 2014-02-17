#ifndef _WX_DATABASE_WXPREC_H_
#define _WX_DATABASE_WXPREC_H_

#ifndef	_WINSOCKAPI_
#define	_WINSOCKAPI_
#endif

#include <wx/wx.h>

#if defined(_DEBUG) && defined(_MSC_VER)
    #include <crtdbg.h>
	#define debug_normal_new new(_NORMAL_BLOCK ,__FILE__, __LINE__)
	#define debug_ignore_new new(_IGNORE_BLOCK ,__FILE__, __LINE__)
#else
    #define debug_normal_new new
    #define debug_ignore_new new
#endif

#include "wx/database/setup.h"
#include "wx/database/dlimpexp.h"

#include "wx/hashset.h"

#include "wx/database/string_converter.h"
#include "wx/database/query_parser.h"
#include "wx/database/errorcodes.h"
#include "wx/database/exception.h"
#include "wx/database/error_reporter.h"

#include "wx/database/resultset_metadata.h"
#include "wx/database/resultset.h"
#include "wx/database/prepared_statement.h"

#include "wx/database/database.h"

#if defined(WXMAKINGDLL_DATABASE) ||  defined(WXMAKINGLIB_DATABASE)

#if wxUSE_DATABASE_SQLITE
	#include <sqlite3.h>
	#include "wx/database/sqlite3/sqlite_database.h"
	#include "wx/database/sqlite3/sqlite_preparedstatement.h"
	#include "wx/database/sqlite3/sqlite_resultset.h"
	#include "wx/database/sqlite3/sqlite_resultset_metatdata.h"
#endif
#if wxUSE_DATABASE_POSTGRESQL
	#include "libpq/libpq-fe.h"
	#include "libpq/libpq-fs.h"
	#include "wx/database/postgresql/postgresql_database.h"
	#include "wx/database/postgresql/postgresql_interface.h"
	#include "wx/database/postgresql/postgresql_param.h"
	#include "wx/database/postgresql/postgresql_preparedstatement.h"
	#include "wx/database/postgresql/postgresql_preparedstatement_parameter_collection.h"
	#include "wx/database/postgresql/postgresql_preparedstatement_wrapper.h"
	#include "wx/database/postgresql/postgresql_resultset.h"
	#include "wx/database/postgresql/postgresql_resultset_metadata.h"
#endif
#if wxUSE_DATABASE_MYSQL
	#include <mysql.h>
	#include <errmsg.h>
	#include "wx/database/mysql/mysql_database.h"
	#include "wx/database/mysql/mysql_interface.h"
	#include "wx/database/mysql/mysql_param.h"
	#include "wx/database/mysql/mysql_preparedstatement.h"
	#include "wx/database/mysql/mysql_preparedstatement_parameter.h"
	#include "wx/database/mysql/mysql_preparedstatement_parameter_collection.h"
	#include "wx/database/mysql/mysql_preparedStatement_resultset.h"
	#include "wx/database/mysql/mysql_preparedstatement_wrapper.h"
	#include "wx/database/mysql/mysql_resultset_metadata.h"
#endif
#if wxUSE_DATABASE_ODBC
	#include <sql.h>
	#include <sqlext.h>
	#include "wx/database/odbc/odbc_database.h" 
	#include "wx/database/odbc/odbc_inteface.h"
	#include "wx/database/odbc/odbc_param.h"
	#include "wx/database/odbc/odbc_preparedstatement.h" 
	#include "wx/database/odbc/odbc_resultset.h" 
	#include "wx/database/odbc/odbc_resultset_metadata.h" 
#endif
#if wxUSE_DATABASE_TDS
	#ifndef	_FREETDS_LIBRARY_SOURCE
	#define	_FREETDS_LIBRARY_SOURCE
	#endif
	#include <tds.h>
	#include <tdsconvert.h>
	#include "wx/database/tds/tds_columndata.h"
	#include "wx/database/tds/tds_database.h"
	#include "wx/database/tds/tds_preparedstatement.h"
	#include "wx/database/tds/tds_resultset.h"
	#include "wx/database/tds/tds_resultset_metadata.h"
#endif

#endif//defined(WXMAKINGDLL_DATABASE) ||  defined(WXMAKINGLIB_DATABASE)

#endif//_WX_DATABASE_WXPREC_H_