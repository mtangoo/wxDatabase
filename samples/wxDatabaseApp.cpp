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


class wxDatabaseApp : public wxAppConsole
{
    public:
        virtual bool OnInit();
        virtual int OnRun();
	private:
		wxDatabase* GetDatabase(const wxString& conf = "");
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
		// opening a database via wxDatabase::GetDatabase() hides all the details of the varied database arguments

		// calling the sample GetDatabase() with a non-empty "config entry" string also avoids the need for the wxDatabaseApp.conf file

		// simply comment in one of the GetDatabase() calls below

		// use the wxDatabaseApp.conf file
		//pDatabase = GetDatabase();
		
		// SQLite3 direct
		//pDatabase = GetDatabase("[SQLite]\ndatabase=C:/wxDev/sqlite/test.sqlite");
		
		// SQLite3 via ODBC
		//pDatabase = GetDatabase("[ODBC]\nConnection=DRIVER=SQLite3 ODBC Driver;Database=C:/wxDev/sqlite/test.sqlite\nDbType=SQLITE\n");
		
		// PostgreSQL
		//pDatabase = GetDatabase("[PostgreSQL]\nlibrary_location=C:/Program Files/PostgreSQL/8.3/bin\nserver=127.0.0.1\ndatabase=test\nuser=postgres\npassword=jesus\nport=5432");
		
		// MySQL
		//pDatabase = GetDatabase("[MySQL]\nlibrary_location=C:/Program Files/MySQL/MySQL Server 5.1/bin\nserver=127.0.0.1\ndatabase=test\nuser=root\npassword=jesus");

		// TDS direct
		// The following lines were added to freetds.conf for server=manyleaves_sqlexpress
		//	[manyleaves_sqlexpress]
		//	host = manyleaves
		//	instance = sqlexpress
		//#	port = 1433
		// instance and port are mutually exclusive
		//pDatabase = GetDatabase("[TDS]\nfreetds=C:/wxDev/freetds-1.00.24/freetds.conf\nserver=manyleaves_sqlexpress\ndatabase=test\nuser=\npassword=\nversion=7.1");
	
		// TDS via ODBC
		//pDatabase = GetDatabase("[ODBC]\nConnection=DRIVER=SQL Server;SERVER=manyleaves\\\\sqlexpress;TRUSTED_CONNECTION=Yes;DATABASE=test\nDSN=\nDbType=TDS\n");
	
		// TDS via ODBC using MARS
		//pDatabase = GetDatabase("[ODBC]\nConnection=DRIVER={SQL Server Native Client 10.0};MARS_CONNECTION=Yes;SERVER=manyleaves\\\\sqlexpress;TRUSTED_CONNECTION=Yes;DATABASE=test\nDSN=\nDbType=TDS\n");

		// MS Access via ODBC
		//pDatabase = GetDatabase("[ODBC]\nConnection=DRIVER={microsoft access driver (*.mdb)};dbq=C:/wxDev/msaccess/test.mdb;\nDSN=\nDbType=TDS\n");

		wxFileName f(wxStandardPaths::Get().GetExecutablePath());
		wxString appPath(f.GetPath());
		
		//If it does not work for Windows, Install drivers from http://www.ch-werner.de/sqliteodbc/
		pDatabase = GetDatabase("[ODBC]\nConnection=DRIVER=SQLite3 ODBC Driver;Database="+appPath + wxFileName::GetPathSeparator() + "wxdatabase.db\nDbType=SQLITE\n");
		
		if (!pDatabase) throw(wxDatabaseException(-1, "Unable to establish connection to a database"));

		//Create table 
        wxString deleteSqlTB = "DROP TABLE Names;";
		try
		{
			pDatabase->RunQuery(deleteSqlTB);
		}
		catch(wxDatabaseException&)
		{
		}

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
		pDatabase->Close();        
    }
    catch (wxDatabaseException& e)
    {
      wxString error = wxString::Format("Error (%d): %s\nPress any key to continue ...", e.GetErrorCode(), e.GetErrorMessage().c_str());
      wxPuts(error);
      
	  wxGetchar();

      return e.GetErrorCode();
    }
    if (pDatabase)
        delete pDatabase;
    
   return 0;
}

wxDatabase* wxDatabaseApp::GetDatabase(const wxString& configString)
{
	wxDatabase* pDatabase = NULL;
	wxInputStream* configStream = NULL;

	if (configString.IsEmpty())
	{
		wxString configPath("wxDatabaseApp.conf");
		if (!wxFileName::FileExists(configPath))
		{
			wxPrintf("Configuration file \"%s\" not found", configPath);
			return NULL;
		}
 		configStream = new wxFileInputStream(configPath);
		if (!configStream->IsOk())
		{
			wxPrintf("Configuration file \"%s\" cannot be opened", configPath);
			return NULL;
		}
	}
	else
	{
 		configStream = new wxStringInputStream(configString);
	}
	wxFileConfig config(*configStream);

	wxString err = wxEmptyString;
	// returns the first non-empty entry in the config stream
	pDatabase = wxDatabase::GetDatabase(config, &err);
	if (pDatabase == NULL)
	{
		wxPrintf("Cannot establish database connection from %s", err); 
		return NULL;
	}

	wxPrintf("Running %s", pDatabase->GetTypeName()); 
	if (pDatabase->IsViaODBC()) wxPrintf("[ODBC]"); 
	wxPrintf("\n\n");

	return pDatabase;
}