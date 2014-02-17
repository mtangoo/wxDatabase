#include "wx/base/wxprec.h"

#if wxUSE_DATABASE_CONFIG

#include <wx/filename.h>
#include <wx/sstream.h>
#include <wx/stdpaths.h>

#include <wx/log.h> 
#include <wx/intl.h> 
#include <wx/utils.h> 

#define viewname_default			"dbconf"

#define viewname					(m_viewName)
#define tablename_path				(m_viewName+"path")
#define tablename_entry				(m_viewName+"entry")

#define triggername_insertviewpath	("insert_"+m_viewName)
#define triggername_updateviewpath	("update_"+m_viewName)
#define triggername_deleteviewpath	("delete_"+m_viewName)

wxIMPLEMENT_ABSTRACT_CLASS(wxDatabaseConfig, wxConfigBase);

wxDatabaseConfig::wxDatabaseConfig
(
 const wxString& appName,
 const wxString& vendorName,
 const wxString& viewName,
 const wxString& settingsConf,
 long WXUNUSED(style), 
 const wxMBConv& WXUNUSED(conv)		
 ) 
 : wxConfigBase(appName, vendorName), 
 m_viewName(viewName), 
 m_pDatabase(NULL),
 m_self(wx_const_cast(wxDatabaseConfig*, this))
{
	wxStringInputStream inStream(settingsConf);
	wxString err;
	if (!LoadStream(inStream, &err))
	{
		wxLogWarning(wxString::Format("%s in configuration settings '%s'", err, settingsConf));
	}
} 

#ifdef wxUSE_STREAMS 
wxDatabaseConfig::wxDatabaseConfig
(
 wxInputStream& inStream,
 const wxString& viewName, 
 const wxMBConv& WXUNUSED(conv)		
 ) 
 : wxConfigBase(), 
 m_viewName(viewName), 
 m_pDatabase(NULL),
 m_self(wx_const_cast(wxDatabaseConfig*, this))
{ 
	wxString warning(" configuration stream");
	wxString err;
	if (!LoadStream(inStream, &err))
	{
		wxLogWarning(wxString::Format("%s in configuration stream", err));
	}
} 
#endif 

wxDatabaseConfig::~wxDatabaseConfig() 
{ 
	Flush(); 
	CleanUp(); 
} 

void wxDatabaseConfig::SetRootPath(dbentry& entry) 
{ 
	entry.id = 0;
	entry.name = "";
	entry.value = "";
	entry.path = "";
	entry.isgroup = true;
} 

void wxDatabaseConfig::SetPath(const wxString& strPath) 
{ 
	if (strPath.empty() || strPath.IsSameAs(wxCONFIG_PATH_SEPARATOR)) 
	{ 
		SetRootPath(m_entry); 
		return; 
	} 

	dbentries entries;
	wxString path = strPath;

	// let's be optimistic, most calls should be to existing paths!
	if (!FindEntries(path, entries))
	{
		if (path[0] != wxCONFIG_PATH_SEPARATOR)
		{
			path = m_entry.path + wxCONFIG_PATH_SEPARATOR + path; 
		}
		wxArrayString paths(wxSplit(path, wxCONFIG_PATH_SEPARATOR)); 
		path.Empty();
		for (size_t n = 0; n < paths.Count(); n++) 
		{
			if (paths[n].IsSameAs(wxEmptyString)) continue;
			path += wxCONFIG_PATH_SEPARATOR+paths[n];
			if (!FindEntries(path, entries))
			{ 
				if (!AddEntry(m_entry, paths[n])) return;
				if (!FindEntries(path, entries)) return;
			}
			m_entry = entries[0];
			entries.clear();
		} 
	}
	else
	{
		m_entry = entries[0];
	}
} 

bool wxDatabaseConfig::GetFirstGroup(wxString& str, long& lIndex) const 
{ 
	m_self->m_groups.clear();
	m_self->GetChildren(m_self->m_entry, &m_self->m_groups, NULL);
	lIndex = 0; 
	return m_groups.size() == 0 ? false : GetNextGroup(str, lIndex); 
} 

bool wxDatabaseConfig::GetNextGroup(wxString& str, long& lIndex) const 
{ 
	if ((long)m_groups.size() <= lIndex) return false;
	str = m_groups[lIndex++].name;
	return true;
} 

bool wxDatabaseConfig::GetFirstEntry(wxString& str, long& lIndex) const 
{ 
	m_self->m_entries.clear();
	m_self->GetChildren(m_self->m_entry, NULL, &m_self->m_entries);
	lIndex = 0; 
	return m_entries.size() == 0 ? false : GetNextEntry(str, lIndex); 
} 

bool wxDatabaseConfig::GetNextEntry(wxString& str, long& lIndex) const 
{ 
	if ((long)m_entries.size() <= lIndex) return false;
	str = m_entries[lIndex++].name;
	return true;
} 

void wxDatabaseConfig::GetChildren(dbentry& parent, dbentries* groups, dbentries* entries, bool recursive)
{ 
	dbentries wild;
	m_self->FindEntries(parent.path+wxCONFIG_PATH_SEPARATOR+'%', wild);

	for (dbentries::iterator it = wild.begin(); it != wild.end(); ++it)
	{
		wxString localpath(it->path.Right(it->path.Length()-parent.path.Length()-1));
		if (!recursive && wxStrchr(localpath, wxCONFIG_PATH_SEPARATOR)) continue;
		if (it->isgroup)
		{
			if (groups == NULL) continue;
			groups->push_back(*it);
		}
		else
		{
			if (entries == NULL) continue;
			entries->push_back(*it);
		}
	}
}

size_t wxDatabaseConfig::GetNumberOfEntries(bool bRecursive) const 
{ 
	dbentries entries;
	m_self->GetChildren(m_self->m_entry, NULL, &entries, bRecursive);
	return entries.size();
} 

size_t wxDatabaseConfig::GetNumberOfGroups(bool bRecursive) const 
{ 
	dbentries groups;
	m_self->GetChildren(m_self->m_entry, &groups, NULL, bRecursive);
	return groups.size();
} 

bool wxDatabaseConfig::HasGroup(const wxString& strName) const 
{ 
	if (strName.empty()) return false; 
	dbentries entries;
	if (!m_self->FindEntries(strName, entries)) return false;
	return entries[0].isgroup;
} 

bool wxDatabaseConfig::HasEntry(const wxString& strName) const 
{
	if (strName.empty()) return false; 
	dbentries entries;
	if (!m_self->FindEntries(strName, entries)) return false;
	return !entries[0].isgroup;
} 

bool wxDatabaseConfig::Flush(bool WXUNUSED(bCurrentOnly)) 
{ 
	// should probably enclose all changes in a db transaction and commit it here!
	return true; 
} 

bool wxDatabaseConfig::FindEntries(const wxString& nameORpath, dbentries& entries)
{
	if (nameORpath.empty()) return false;
	wxString path = nameORpath;
	if (path[0] != wxCONFIG_PATH_SEPARATOR)
	{
		path = m_entry.path + wxCONFIG_PATH_SEPARATOR + path; 
	}
	if (path.Contains("..") || path.Contains("."))
	{
		wxArrayString in(wxSplit(path, wxCONFIG_PATH_SEPARATOR)); 
		wxArrayString out;
		size_t count = in.GetCount();
		for (size_t n = 0; n < count; n++)
		{
			wxString name = in[n];
			if (name.IsSameAs(wxEmptyString)) continue;
			if (name == ".") continue;
			if (name == ".." && !out.empty())
			{
				out.pop_back();
				continue;
			}
			out.Add(name);
		}
		path = wxCONFIG_PATH_SEPARATOR+wxJoin(out, wxCONFIG_PATH_SEPARATOR);
	}
	if (path.IsSameAs(wxCONFIG_PATH_SEPARATOR))
	{
		dbentry root;
		SetRootPath(root);
		entries.push_back(root);
	}
	m_pStatementSqlFindEntries->SetParamString(1, path);
	m_self->ExecuteStatement(m_pStatementSqlFindEntries, &entries);
	return (entries.size() > 0);
}

bool wxDatabaseConfig::AddEntry(dbentry& parent, const wxString& name, const wxString* value)
{
	if (name.empty()) return false;
	wxASSERT_MSG(!wxStrchr(name, wxCONFIG_PATH_SEPARATOR), wxT("AddEntry(): paths are not supported")); 

	m_pStatementSqlAddEntry->SetParamInt(1, parent.id);
	m_pStatementSqlAddEntry->SetParamString(2, name);
	if (value == NULL)
	{
		m_pStatementSqlAddEntry->SetParamNull(3);
	}
	else
	{
		wxString strValue(*value);
		m_pStatementSqlAddEntry->SetParamString(3, strValue);
	}
	m_self->ExecuteStatement(m_pStatementSqlAddEntry);
	return true;
}

bool wxDatabaseConfig::WriteEntry(dbentry& entry, const wxString& name, const wxString& value)
{
	if (name.empty()) return false;
	wxASSERT_MSG(!wxStrchr(name, wxCONFIG_PATH_SEPARATOR), wxT("WriteEntry(): paths are not supported")); 

	m_pStatementSqlEditEntry->SetParamInt(3, entry.id);
	m_pStatementSqlEditEntry->SetParamString(1, name);
	m_pStatementSqlEditEntry->SetParamString(2, value);
	m_self->ExecuteStatement(m_pStatementSqlEditEntry);
	return true;
}

bool wxDatabaseConfig::RenameEntry(const wxString& oldName, const wxString& newName) 
{ 
	wxASSERT_MSG(!wxStrchr(oldName, wxCONFIG_PATH_SEPARATOR), wxT("RenameEntry(): paths are not supported")); 

	dbentries entries;
	if ( FindEntries(newName, entries)) return false; 
	entries.clear();
	if (!FindEntries(oldName, entries)) return false; 

	return WriteEntry(entries[0], newName, entries[0].value); 
} 

bool wxDatabaseConfig::RenameGroup(const wxString& oldName, const wxString& newName) 
{ 
	wxASSERT_MSG(!wxStrchr(oldName, wxCONFIG_PATH_SEPARATOR), wxT("RenameGroup(): paths are not supported")); 
	return RenameEntry(oldName, newName);
} 

bool wxDatabaseConfig::DeleteEntry(const wxString& key, bool bGroupIfEmptyAlso) 
{ 
	wxString path = this->RemoveTrailingSeparator(key);
	dbentries entries;
	if (!m_self->FindEntries(path, entries)) return false; 
	dbentry entry = entries[0];

	path = this->RemoveTrailingSeparator(key)+wxCONFIG_PATH_SEPARATOR+"..";
	entries.clear();
	if (!m_self->FindEntries(path, entries)) return false; 
	dbentry parent = entries[0];

	m_pStatementSqlDelEntry->SetParamInt(1, entry.id);
	m_self->ExecuteStatement(m_pStatementSqlDelEntry);

	if (!m_entry.path.IsSameAs(parent.path) && m_entry.path.StartsWith(parent.path))
	{
		m_entry = parent;
	}

	entries.clear();
	m_self->GetChildren(parent, NULL, &entries);

	if (bGroupIfEmptyAlso && entries.size() == 0)
	{
		m_pStatementSqlDelEntry->SetParamInt(1, parent.id);
		m_self->ExecuteStatement(m_pStatementSqlDelEntry);
	}
	return true;
} 

bool wxDatabaseConfig::DeleteGroup(const wxString& key) 
{ 
	wxString path = this->RemoveTrailingSeparator(key);
	dbentries entries;
	if (!m_self->FindEntries(path, entries)) return false; 
	dbentry group = entries[0];

	path = this->RemoveTrailingSeparator(key)+wxCONFIG_PATH_SEPARATOR+"..";
	entries.clear();
	if (!m_self->FindEntries(path, entries)) return false; 
	dbentry parent = entries[0];

	m_pStatementSqlDelEntry->SetParamInt(1, group.id);
	m_self->ExecuteStatement(m_pStatementSqlDelEntry);

	if (!m_entry.path.IsSameAs(parent.path) && m_entry.path.StartsWith(parent.path))
	{
		m_entry = parent;
	}

	return true;
} 

bool wxDatabaseConfig::DeleteAll() 
{ 
	this->ExecuteStatement(m_pStatementSqlDropAll);
	SetRootPath(m_entry);
	return true; 
} 

bool wxDatabaseConfig::DoReadString(const wxString& key, wxString* pStr) const 
{ 
	dbentries entries;
	if (!m_self->FindEntries(key, entries)) return false; 
	*pStr = entries[0].value;
	return true;
} 

bool wxDatabaseConfig::DoReadLong(const wxString& key, long* pl) const 
{ 
	wxString str; 
	if (!Read(key, &str)) return false; 
	str.Trim(); 
	return str.ToLong(pl); 
} 

bool wxDatabaseConfig::DoWriteString(const wxString& key, const wxString& szValue) 
{ 
	wxString name = key.AfterLast(wxCONFIG_PATH_SEPARATOR);
	if (name.StartsWith(wxCONFIG_IMMUTABLE_PREFIX))
	{ 
		wxLogError("Immutable entries cannot be changed"); 
		return false; 
	} 

	dbentries entries;
	if (!FindEntries(key, entries))
	{ 
		wxConfigPathChanger path(this, key);
		//wxLogTrace(DBLCONF_TRACE_MASK, wxString::Format("Adding Entry '%s' = '%s' to Group '%s'", name, szValue, m_entry.path)); 
		AddEntry(m_entry, name, &szValue); 
	}
	else if (entries[0].isgroup)
	{
		wxLogError("Can't set value of a group!."); 
	}
	else
	{
		//wxLogTrace(DBLCONF_TRACE_MASK, wxString::Format("Writing Entry '%s' = '%s' in Group '%s'", name, szValue, entries[0].path)); 
		WriteEntry(entries[0], name, szValue); 
	}
	return true; 
} 

bool wxDatabaseConfig::DoWriteLong(const wxString& key, long lValue) 
{ 
	return Write(key, wxString::Format(_T("%ld"), lValue)); 
} 

#if wxCHECK_VERSION(2,9,0) && wxUSE_BASE64
bool wxDatabaseConfig::DoReadBinary(const wxString& key, wxMemoryBuffer* buf) const
{
	wxCHECK_MSG(buf, false, wxT("NULL buffer"));

	wxString str;
	if (!Read(key, &str)) return false;
	*buf = wxBase64Decode(str);
	return true;
}

bool wxDatabaseConfig::DoWriteBinary(const wxString& key, const wxMemoryBuffer& buf)
{
	return Write(key, wxBase64Encode(buf));
}
#endif

void wxDatabaseConfig::CleanUp() 
{ 
	m_pDatabase->CloseStatement(m_pStatementSqlAddEntry);
	m_pDatabase->CloseStatement(m_pStatementSqlDelEntry);
	m_pDatabase->CloseStatement(m_pStatementSqlEditEntry);
	m_pDatabase->CloseStatement(m_pStatementSqlFindEntries);

	m_pDatabase->CloseStatement(m_pStatementSqlDropAll);

	wxDELETE(m_pDatabase);
} 

bool wxDatabaseConfig::LoadStream(wxInputStream& inStream, wxString* err) 
{ 
	SetRootPath(m_entry);

	if (m_viewName.IsEmpty())
	{
		m_viewName = m_self->GetAppName();
		if (m_viewName.IsEmpty())
		{
			m_viewName = viewname_default;
		}
	}
	m_viewName.Replace(wxCONFIG_PATH_SEPARATOR, wxEmptyString);

	if (!inStream.CanRead())
	{
		if (err) err->Append("can't read"); 
		return false;
	}

	wxFileConfig config(inStream);

	/*
		SQLite doesn't support stored procedures nor recursion.
		The best compromise between all the flavours of databases is to store the group paths as a closure set
		and to access the groups and entries via a view with triggers instead of accessing the underlying tables directly.

		table "tablename_entry" contains the rows for both group names and entry name/values
		table "tablename_path"  contains a closure set for the group paths

		view  "viewname" (and its associated triggers) provide the interface used by this code
	*/

	wxString sqlCreateTablePath;
	wxString sqlCreateTableEntry;
	wxString sqlCreateViewPath;

	wxString sqlCreateTriggerInsteadOfInsertViewPath;
	wxString sqlCreateTriggerInsteadOfUpdateViewPath;
	wxString sqlCreateTriggerInsteadOfDeleteViewPath;

	wxString sqlAddEntry;
	wxString sqlDelEntry;
	wxString sqlEditEntry;
	wxString sqlFindEntries;

	wxString sqlDropAll;

	// should work on all DBs?
	sqlCreateTablePath << 
		"IF NOT EXISTS (SELECT * FROM sys.views WHERE name = '" << viewname <<"') "
		"AND NOT EXISTS (SELECT * FROM sys.objects WHERE type IN ('U') AND name = '" << tablename_path << "') EXECUTE('"
		"CREATE TABLE " << tablename_path <<
		"("
		"pid INT NOT NULL,"
		"id  INT NOT NULL,"
		"PRIMARY KEY (pid, id),"
		"FOREIGN KEY (pid) REFERENCES " << tablename_entry << "(id),"
		"FOREIGN KEY (id)  REFERENCES " << tablename_entry << "(id)"
		")"
		"')"
		;
	sqlCreateTableEntry << 
		"IF NOT EXISTS (SELECT * FROM sys.views WHERE name = '" << viewname <<"') "
		"AND NOT EXISTS (SELECT * FROM sys.objects WHERE type IN ('U') AND name = '" << tablename_entry <<"') EXECUTE('"
		"CREATE TABLE " << tablename_entry <<
		"("
		"id INT IDENTITY PRIMARY KEY,"
		"name VARCHAR(255) NOT NULL,"
		"value VARCHAR(255) NULL"
		")"
		"')"
		;
	sqlAddEntry <<
		"INSERT INTO " << viewname <<" (id, name, value) VALUES (?, ?, ?)"
		;
	sqlDelEntry <<
		"DELETE FROM " << viewname <<" WHERE id = ?"
		;
	sqlEditEntry <<
		"UPDATE " << viewname <<" SET name = ?, value = ? WHERE id = ?"
		;
	sqlFindEntries <<
		"SELECT id, name, value, path, isgroup FROM " << viewname <<" WHERE path LIKE ?"
		;
	sqlDropAll <<
		"DROP VIEW " << viewname <<"\n"
		"DROP TABLE " << tablename_path <<"\n"
		"DROP TABLE " << tablename_entry <<"\n"
		;
	sqlCreateTriggerInsteadOfInsertViewPath <<
		"IF NOT EXISTS (SELECT * FROM sys.triggers WHERE name = '" << triggername_insertviewpath <<"') EXECUTE('\n"
		"CREATE TRIGGER [" << triggername_insertviewpath <<"] ON " << viewname <<" INSTEAD OF INSERT\n"
		"AS\n"
		"BEGIN\n"
		"\tINSERT INTO " << tablename_entry <<" (name, value) SELECT i.name, i.value FROM inserted i\n"
		"\tINSERT INTO " << tablename_path <<" (pid, id)\n"
		"\tSELECT p.pid, (SELECT MAX(id) FROM " << tablename_entry <<")\n"
		"\tFROM\n"
		"\t\t" << tablename_path <<" p\n"
		"\t\tJOIN inserted i ON p.id = i.id -- treat i.id as the pid p\n"
		"\tUNION\n"
		"\tSELECT MAX(id), MAX(id) FROM " << tablename_entry <<"\n"
		//"\tSELECT id, name, value FROM " << tablename_entry <<" WHERE id IN (SELECT MAX(id) FROM " << tablename_entry <<")\n"
		"END\n"
		"')"
		;
	sqlCreateTriggerInsteadOfUpdateViewPath <<
		"IF NOT EXISTS (SELECT * FROM sys.triggers WHERE name = '" << triggername_updateviewpath <<"') EXECUTE('\n"
		"CREATE TRIGGER [" << triggername_updateviewpath <<"] ON " << viewname <<" INSTEAD OF UPDATE\n"
		"AS\n"
		"BEGIN\n"
		"\tUPDATE " << tablename_entry <<" SET\n"
		"\t\t" << tablename_entry <<".name = i.name,\n"
		"\t\t" << tablename_entry <<".value = CASE WHEN " << tablename_entry <<".value IS NULL THEN NULL ELSE i.value END\n"
		"\tFROM\n"
		"\t\tinserted i\n"
		"\tWHERE\n"
		"\t\t" << tablename_entry <<".id = i.id\n"
		//"\tSELECT e.id, e.name, e.value FROM " << tablename_entry <<" e JOIN inserted i ON i.id = e.id\n"
		"END\n"
		"')"
		;
	sqlCreateTriggerInsteadOfDeleteViewPath <<
		"IF NOT EXISTS (SELECT * FROM sys.triggers WHERE name = '" << triggername_deleteviewpath <<"') EXECUTE('\n"
		"CREATE TRIGGER [" << triggername_deleteviewpath <<"] ON " << viewname <<" INSTEAD OF DELETE\n"
		"AS\n"
		"BEGIN\n"
		"\tDELETE p\n"
		"\tFROM\n"
		"\t\t" << tablename_path <<" c\n"
		"\t\tJOIN " << tablename_path <<" p ON p.id = c.id\n"
		"\t\tJOIN deleted d ON c.pid = d.id -- treat d.id as the id\n"
		"\tDELETE e\n"
		"\tFROM\n"
		"\t\t" << tablename_entry <<" e\n"
		"\tWHERE\n"
		"\t\te.id  NOT IN (SELECT id FROM " << tablename_path <<")\n"
		"END\n"
		"')"
		;

	m_pDatabase = wxDatabase::GetDatabase(config, err);
	if (m_pDatabase == NULL)
	{
		if (err) err->Append("cannot establish database connection from"); 
		return false;
	}

	// if necessary add the database library path to the system path
	if (!m_pDatabase->GetLibraryPath().IsEmpty())
	{
		wxString path;
		wxGetEnv("PATH", &path);
#ifdef WIN32
		path.Append(";");
#else
		path.Append(":");
#endif
		path.Append(m_pDatabase->GetLibraryPath());
		wxSetEnv("PATH", path);
	}

	if (false);
#if wxUSE_DATABASE_SQLITE
	else
	if (m_pDatabase->GetTypeName().IsSameAs("SQLITE"))
	{
		sqlCreateViewPath << 
			"IF NOT EXISTS (SELECT * FROM sys.views WHERE name = '" << viewname <<"') EXECUTE('"
			"CREATE VIEW [" << viewname <<"]\n"
			"AS\n"
			"SELECT\n"
			"\te.id,\n"
			"\te.name,\n"
			"\te.value,\n"
			"\tCONVERT(VARCHAR(255), (\n"
			"\t\tSELECT\n"
			"\t\t\t''/''+dbo.GROUP_CONCAT_D(ie.name, ''/'')\n"
			"\t\tFROM\n"
			"\t\t\t" << tablename_path <<" ic\n"
			"\t\t\tJOIN " << tablename_path <<" ip ON ip.id = ic.pid\n"
			"\t\t\tJOIN " << tablename_entry <<" ie ON ie.id = ip.pid\n"
			"\t\tWHERE\n"
			"\t\t\tic.id = ic.pid\n"
			"\t\t\tAND\n"
			"\t\t\tic.id = c.id\n"
			"\t\tGROUP BY\n"
			"\t\t\tic.id\n"
			"\t)) AS [path],\n"
			"\tCASE WHEN e.value IS NULL THEN 1 ELSE 0 END AS [isgroup]\n"
			"FROM\n"
			"\t" << tablename_path <<" c\n"
			"\tJOIN " << tablename_entry <<" e ON e.id = c.id\n"
			"WHERE\n"
			"\tc.id = c.pid\n"
			"')"
			;
	}
#endif
#if wxUSE_DATABASE_POSTGRESQL
	else
	if (m_pDatabase->GetTypeName().IsSameAs("POSTGRESQL"))
	{
	}
#endif
#if wxUSE_DATABASE_MYSQL
	else
	if (m_pDatabase->GetTypeName().IsSameAs("MYSQL"))
	{
	}
#endif
#if wxUSE_DATABASE_ODBC
	else
	if (m_pDatabase->GetTypeName().IsSameAs("ODBC"))
	{
	}
#endif
#if wxUSE_DATABASE_TDS
	else
	if (m_pDatabase->GetTypeName().IsSameAs("TDS"))
	{
		sqlCreateViewPath << 
			"IF NOT EXISTS (SELECT * FROM sys.views WHERE name = '" << viewname <<"') EXECUTE('"
			"CREATE VIEW [" << viewname <<"]\n"
			"AS\n"
			"SELECT\n"
			"\te.id,\n"
			"\te.name,\n"
			"\te.value,\n"
			"\tCONVERT(VARCHAR(255), (\n"
			"\t\tSELECT\n"
			"\t\t\t''/''+e.name AS [text()]\n"
			"\t\tFROM\n"
			"\t\t\t" << tablename_path <<" p\n"
			"\t\t\tJOIN " << tablename_entry <<" e ON e.id = p.pid\n"
			"\t\tWHERE\n"
			"\t\t\tp.id = c.pid\n"
			"\t\tFOR XML PATH('''')\n"
			"\t)) AS [path],\n"
			"\tCASE WHEN e.value IS NULL THEN 1 ELSE 0 END AS [isgroup]\n"
			"FROM\n"
			"\t" << tablename_path <<" c\n"
			"\tJOIN " << tablename_entry <<" e ON e.id = c.id\n"
			"WHERE\n"
			"\tc.id = c.pid\n"
			"')"
			;
	}
#endif

	m_pDatabase->CloseStatement(this->ExecuteStatement(this->PrepareStatement(sqlCreateTableEntry)));
	m_pDatabase->CloseStatement(this->ExecuteStatement(this->PrepareStatement(sqlCreateTablePath)));
	m_pDatabase->CloseStatement(this->ExecuteStatement(this->PrepareStatement(sqlCreateViewPath)));
	m_pDatabase->CloseStatement(this->ExecuteStatement(this->PrepareStatement(sqlCreateTriggerInsteadOfInsertViewPath)));
	m_pDatabase->CloseStatement(this->ExecuteStatement(this->PrepareStatement(sqlCreateTriggerInsteadOfUpdateViewPath)));
	m_pDatabase->CloseStatement(this->ExecuteStatement(this->PrepareStatement(sqlCreateTriggerInsteadOfDeleteViewPath)));

	m_pStatementSqlAddEntry = this->PrepareStatement(sqlAddEntry);
	m_pStatementSqlDelEntry = this->PrepareStatement(sqlDelEntry);
	m_pStatementSqlEditEntry = this->PrepareStatement(sqlEditEntry);
	m_pStatementSqlFindEntries = this->PrepareStatement(sqlFindEntries);

	m_pStatementSqlDropAll = this->PrepareStatement(sqlDropAll);

	return true;
} 

wxPreparedStatement* wxDatabaseConfig::PrepareStatement(const wxString& sql)
{
	wxPreparedStatement* pStatement = m_pDatabase->PrepareStatement(sql);
	DatabaseErrorCheck(pStatement);
	return pStatement;
}

wxPreparedStatement* wxDatabaseConfig::ExecuteStatement(wxPreparedStatement* pStatement, dbentries* pEntries)
{
	wxASSERT(pStatement);
	m_self->DatabaseErrorCheck(pStatement);
	//m_self->DatabaseErrorCheck(m_pDatabase);
#ifndef DONT_USE_DATABASE_LAYER_EXCEPTIONS
	try
	{
#endif
		if (pEntries == NULL)
		{
			pStatement->RunQuery();
			m_self->DatabaseErrorCheck(pStatement);
		}
		else
		{
			wxDatabaseResultSet* pResultSet = pStatement->RunQueryWithResults();
			m_self->DatabaseErrorCheck(pStatement);
			if (pResultSet)
			{
				DatabaseErrorCheck(pResultSet);
				while (pResultSet->Next())
				{
					DatabaseErrorCheck(pResultSet);
					dbentry entry;
					entry.id = pResultSet->GetResultLong(1);
					entry.name = pResultSet->GetResultString(2);
					entry.value = pResultSet->GetResultString(3);
					entry.path = pResultSet->GetResultString(4);
					entry.isgroup = pResultSet->GetResultBool(5);
					pEntries->push_back(entry);
				}
				m_pDatabase->CloseResultSet(pResultSet);
			}
		}
		//m_self->DatabaseErrorCheck(m_pDatabase);
#ifndef DONT_USE_DATABASE_LAYER_EXCEPTIONS
	}
	catch (wxDatabaseException& e)
	{
		m_self->ProcessException(e);
	}
#endif
	return pStatement;
}

void wxDatabaseConfig::DatabaseErrorCheck(wxDatabaseErrorReporter* reporter)
{
	if (reporter->GetErrorCode() == wxDATABASE_OK) return;
	wxString message(wxString::Format("Error (%d): %s", reporter->GetErrorCode(), reporter->GetErrorMessage()));
	wxLogError(message);
}

void wxDatabaseConfig::ProcessException(wxDatabaseException& e, bool fail)
{
	wxString message = wxString::Format("Error (%d): %s", e.GetErrorCode(), e.GetErrorMessage());
	if (fail)
	{
		wxLogError(message);
	}
	else
	{
		wxLogWarning(message);
	}
}

#endif // wxUSE_MLBASE
