// For compilers that support precompilation, includes "wx/wx.h".
#include <wx/wxprec.h>
#ifndef WX_PRECOMP
    #include <wx/wx.h>
#endif
 
#include <wx/database/postgresql/postgresql_database.h>


class wxDatabaseApp : public wxAppConsole
{
    public:
        virtual bool OnInit();
        virtual int OnRun();
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
        //Create table 
        wxString createSqlTB = "CREATE TABLE Names (ID INT PRIMARY KEY NOT NULL, Name VARCHAR(50) NOT NULL);";
		wxString strServer = "127.0.0.1";
		wxString strDatabase = "test"; 
		wxString strUser = "postgres"; 
		wxString  strPassword = "jesus"; 
        pDatabase = new wxPostgresDatabase(strServer, strDatabase, strUser, strPassword); 
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
                wxString id = pResults->GetResultString("ID");
                wxString name = pResults->GetResultString("Name");
                wxString info = wxString::Format("ID: %s \nName: %s\n===============", id.mb_str(), name.mb_str());
                wxPuts(info);
            }
            pDatabase->CloseResultSet(pResults);
        }
        
    }
    catch (wxDatabaseException& e)
    {
      wxString error = wxString::Format("Error (%d): %s", e.GetErrorCode(), e.GetErrorMessage().c_str());
      wxPuts(error);
      
      return e.GetErrorCode();
    }
    if(pDatabase)
        delete pDatabase;
    
   return 0;
}
