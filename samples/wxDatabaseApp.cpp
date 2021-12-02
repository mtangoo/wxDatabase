// For compilers that support precompilation, includes "wx/wx.h".
#include <wx/wxprec.h>
#ifndef WX_PRECOMP
    #include <wx/wx.h>
#endif

#include <wx/filename.h>
#include <wx/wfstream.h>
#include <wx/sstream.h>
 
#include <wx/base/wxprec.h>
#include <wx/database/wxprec.h>

#include <wx/stdpaths.h>

#include <wx/database/sqlite3/sqlite_database.h>

/**
 * Sample application showing how to use basics of wxDatabase
 * To test different databases, you will need to setup appropriate connection using one
 * of the method provided at the end of the file. Be sure to enable that specific database
 * support via ENABLE_*** CMake directives before build to add support for that database.
 * Improvements are welcome as pull requests to https://github.com/mtangoo/wxDatabase
 * 
*/


class wxDatabaseApp : public wxAppConsole
{
    public:
        virtual bool OnInit();
        virtual int OnRun();
	private:
		wxDatabase* GetSQLiteDatabase();
};

wxIMPLEMENT_APP(wxDatabaseApp);

bool wxDatabaseApp::OnInit()
{
    return true;
}

int wxDatabaseApp::OnRun()
{ 
    wxDatabase *pDatabase = NULL;
    try
    {
		wxDatabase *pDatabase = GetSQLiteDatabase(); //Use method appropriate to the database you want to test
		//Create table 
		wxString createSqlTB = "CREATE TABLE Names (ID INT PRIMARY KEY NOT NULL, Name VARCHAR(50) NOT NULL);";
	    pDatabase->RunQuery(createSqlTB);
        
        //insert into table
        wxString sqlStefano = "INSERT INTO Names VALUES(1, 'Stefano Mtangoo');";
        wxString sqlAndrew = "INSERT INTO Names VALUES(2, 'Andrew aka Many Leaves');";
        wxString sqlUpendo = "INSERT INTO Names VALUES(3, 'Upendo Stefano');";

        pDatabase->RunQuery(sqlStefano);
        pDatabase->RunQuery(sqlAndrew);
        pDatabase->RunQuery(sqlUpendo);

        //Prepared statement
        wxPreparedStatement* pStatement = pDatabase->PrepareStatement("INSERT INTO Names (ID, Name) VALUES(?, ?);");
        if (pStatement)
        {
            pStatement->SetParamInt(1, 4);
            pStatement->SetParamString(2, "Polyasi Dickson");
            pStatement->RunQuery();
            pDatabase->CloseStatement(pStatement);
        }
		
        //Select from table
        wxDatabaseResultSet *pResults = pDatabase->RunQueryWithResults("SELECT * FROM Names;");
        if (pResults)
        {
            while (pResults->Next())
            {
                int id = pResults->GetResultInt("ID");
                wxString name = pResults->GetResultString("Name");
                wxString info = wxString::Format("ID: %d \nName: %s\n===============", id, name.mb_str());
                wxPuts(info);
            }
            pDatabase->CloseResultSet(pResults);
        }
    
		//When done with the datbase connection, close it
		pDatabase->Close();   
		//Delete Pointer, or use smart pointer for db connection
        if (pDatabase) delete pDatabase;     
    }
    catch (wxDatabaseException& e)
    {
      wxString error = wxString::Format("Error (%d): %s\nPress any key to continue ...", e.GetErrorCode(), e.GetErrorMessage().c_str());
      wxPuts(error);
      
	  wxGetchar();

      return e.GetErrorCode();
    }
    
   return 0;
}

/**
 * Methods below shows how to connect different databases using wxDatabase.
 * Just replace GetSQLiteDatabase() in OnRun() with any of these to test specific database
 * Remember to enable that specific database in CMake before building to make it available
*/
wxDatabase* wxDatabaseApp::GetSQLiteDatabase()
{
    wxFileName f(wxStandardPaths::Get().GetExecutablePath());
    wxString dbPath = f.GetPath() + "wxDatabase.db";
    wxRemoveFile(dbPath);
	
    wxDatabase* pDatabase =  new wxSqliteDatabase(dbPath, false);
    
    return pDatabase;
}
