#ifndef _WX_DATABASE_SETUP_H_
#define _WX_DATABASE_SETUP_H_

// Use Joseph Blount's database layer databases.  Recommended.
//
// Default is 0
//
// Recommended setting: 1
#define wxUSE_DATABASE_SQLITE		0
#define wxUSE_DATABASE_POSTGRESQL	0
#define wxUSE_DATABASE_MYSQL		   1
#define wxUSE_DATABASE_ODBC			0
#define wxUSE_DATABASE_TDS			0

// Use database exceptions.  Recommended.
//
// Default is 1
//
// Recommended setting: 1
#define wxUSE_DATABASE_EXCEPTIONS	1

#endif//_WX_DATABASE_SETUP_H_

