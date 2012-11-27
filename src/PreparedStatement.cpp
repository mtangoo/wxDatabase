#include "include/prepared_statement.h"


// ctor()
wxPreparedStatement::wxPreparedStatement()
 : wxDatabaseErrorReporter()
{
}

// dtor()
wxPreparedStatement::~wxPreparedStatement()
{
  //wxPrintf(_("~PreparedStatement()\n"));
  CloseResultSets();
}

void wxPreparedStatement::CloseResultSets()
{
  // Iterate through all of the result sets and close them all
  StatementResultSetHashSet::iterator start = m_ResultSets.begin();
  StatementResultSetHashSet::iterator stop = m_ResultSets.end();
  while (start != stop)
  {
    //wxPrintf(_("Deleting statement result set %d\n"), (*start));
    wxLogDebug(_("ResultSet NOT closed and cleaned up by the PreparedStatement dtor"));
    delete (*start);
    start++;
  }
  m_ResultSets.clear();
}

bool wxPreparedStatement::CloseResultSet(wxDatabaseResultSet* pResultSet)
{
  if (pResultSet != NULL)
  {
    // Check if we have this result set in our list
    if (m_ResultSets.find(pResultSet) != m_ResultSets.end())
    {
      // Remove the result set pointer from the list and delete the pointer
      delete pResultSet;
      m_ResultSets.erase(pResultSet);
      return true;
    }
    // The result set wasn't found in the list, return false
    return false;
  }
  else
  {
    // Return false on NULL pointer
    return false;
  }
}

void wxPreparedStatement::SetParamBlob(int nPosition, const wxMemoryBuffer& buffer)
{
  SetParamBlob(nPosition, buffer.GetData(), buffer.GetBufSize());
}


