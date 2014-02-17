#ifndef _WX_DATABASE_RESULT_SET_METADATA_H_
#define _WX_DATABASE_RESULT_SET_METADATA_H_

#include "wx/database/wxprec.h"

class WXDLLIMPEXP_DATABASE wxResultSetMetaData : public wxDatabaseStringConverter
{
public:
  /// Retrieve a column's type
  virtual int GetColumnType(int i) = 0;
  /// Retrieve a column's size
  virtual int GetColumnSize(int i) = 0;
  /// Retrieve a column's name
  virtual wxString GetColumnName(int i) = 0;
  /// Retrieve the number of columns in the result set
  virtual int GetColumnCount() = 0;

  enum {
    COLUMN_UNKNOWN = 0,
    COLUMN_NULL,
    COLUMN_INTEGER,
    COLUMN_STRING,
    COLUMN_DOUBLE,
    COLUMN_BOOL,
    COLUMN_BLOB,
    COLUMN_DATE,
  };
};

#endif//_WX_DATABASE_RESULT_SET_METADATA_H_

