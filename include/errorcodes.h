#ifndef __wxDATABASE_ERROR_CODES_H__
#define __wxDATABASE_ERROR_CODES_H__

#define wxDATABASE_OK 0
#define wxDATABASE_ERROR 1
#define wxDATABASE_USER 2
#define wxDATABASE_PASSWORD 3
#define wxDATABASE_CONSTRAINT_VIOLATION 4
#define wxDATABASE_SQL_SYNTAX_ERROR 5
#define wxDATABASE_ALLOCATION_ERROR 6
#define wxDATABASE_INCOMPATIBLE_FIELD_TYPE 7
#define wxDATABASE_FIELD_NOT_IN_RESULTSET 8
#define wxDATABASE_NO_ROWS_FOUND 9
#define wxDATABASE_NON_UNIQUE_RESULTSET 10
#define wxDATABASE_UNSUPPORTED_OPERATION 11
#define wxDATABASE_ERROR_LOADING_LIBRARY 12

// Using 0 for now since this is replacing a
//  boolean for the return code and we don't want
//  to break existing code
#define wxDATABASE_QUERY_RESULT_ERROR 0

#endif // __wxDATABASE_ERROR_CODES_H__
