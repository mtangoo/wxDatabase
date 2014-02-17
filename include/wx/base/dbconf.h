#ifndef _WX_MLBASE_DBLCONF_H_
#define _WX_MLBASE_DBLCONF_H_

#include "wx/base/wxprec.h"

#include <wx/fileconf.h>
#ifdef wxUSE_STREAMS 
#include <wx/stream.h> 
#endif 

// std::
#include <vector>

class WXDLLIMPEXP_MLBASE wxDatabaseConfig : public wxConfigBase 
{ 
public: 
	wxDatabaseConfig
	(
		const wxString& appName = wxEmptyString, 
		const wxString& vendorName = wxEmptyString, 
		const wxString& viewName = wxEmptyString, 
		const wxString& settingsConf = wxEmptyString,
		long style = 0, 
		const wxMBConv &conv = wxConvAuto()		
	); 
#ifdef wxUSE_STREAMS 
	wxDatabaseConfig
	(
		wxInputStream& inStream,
		const wxString& viewName = wxEmptyString, 
		const wxMBConv &conv = wxConvAuto()		
	); 
#endif 

	virtual ~wxDatabaseConfig(); 

	virtual void SetPath(const wxString& strPath); 
	virtual const wxString& GetPath() const { return m_entry.path; } 

	virtual bool GetFirstGroup(wxString& str, long& lIndex) const; 
	virtual bool GetNextGroup(wxString& str, long& lIndex) const; 
	virtual bool GetFirstEntry(wxString& str, long& lIndex) const; 
	virtual bool GetNextEntry(wxString& str, long& lIndex) const; 

	virtual size_t GetNumberOfEntries(bool bRecursive = false) const; 
	virtual size_t GetNumberOfGroups(bool bRecursive = false) const; 

	virtual bool HasGroup(const wxString& strName) const; 
	virtual bool HasEntry(const wxString& strName) const; 

	virtual bool Flush(bool bCurrentOnly = false); 

	virtual bool RenameEntry(const wxString& oldName, const wxString& newName); 
	virtual bool RenameGroup(const wxString& oldName, const wxString& newName); 

	virtual bool DeleteEntry(const wxString& key, bool bGroupIfEmptyAlso = true); 
	virtual bool DeleteGroup(const wxString& key); 
	virtual bool DeleteAll(); 

protected: 
	virtual bool DoReadString(const wxString& key, wxString* pStr) const; 
	virtual bool DoReadLong(const wxString& key, long* pl) const; 

	virtual bool DoWriteString(const wxString& key, const wxString& szValue); 
	virtual bool DoWriteLong(const wxString& key, long lValue); 
#if wxCHECK_VERSION(2,9,0) && wxUSE_BASE64
	virtual bool DoReadBinary(const wxString& key, wxMemoryBuffer* buf) const; 
	virtual bool DoWriteBinary(const wxString& key, const wxMemoryBuffer& buf); 
#endif

private: 
	typedef struct
	{
		long id;
		wxString name;
		wxString value;
		wxString path;
		bool isgroup;
	} dbentry;
	typedef std::vector<dbentry> dbentries;

	wxDatabaseConfig* m_self;

	void CleanUp(); 

#ifdef wxUSE_STREAMS 
	bool LoadStream(wxInputStream& inStream, wxString* err = NULL);
#endif

	wxDatabase*				m_pDatabase; 
	wxPreparedStatement*	m_pStatementSqlAddEntry;
	wxPreparedStatement*	m_pStatementSqlDelEntry;
	wxPreparedStatement*	m_pStatementSqlEditEntry;
	wxPreparedStatement*	m_pStatementSqlFindEntries;

	wxPreparedStatement*	m_pStatementSqlDropAll;

	wxPreparedStatement*	PrepareStatement(const wxString& strQuery);
	wxPreparedStatement*	ExecuteStatement(wxPreparedStatement* pStatement, dbentries* pEntries = NULL);

	void DatabaseErrorCheck(wxDatabaseErrorReporter* reporter);
	void ProcessException(wxDatabaseException& e, bool fail = true);

	void SetRootPath(dbentry& entry); 

	bool FindEntries(const wxString& nameORpath, dbentries& entries); 
	bool AddEntry(dbentry& parent, const wxString& name, const wxString* value = NULL); 
	bool WriteEntry(dbentry& entry, const wxString& name, const wxString& value); 

	dbentry		m_entry; 
	wxString	m_viewName; 
	wxString	m_settingsConf; 

	void GetChildren(dbentry& parent, dbentries* groups = NULL, dbentries* entries = NULL, bool recursive = false);

	// following used to optimise/cache calls to Get[First|Next][Entry|Group]
	dbentries m_groups;
	dbentries m_entries;

	wxDECLARE_NO_COPY_CLASS(wxDatabaseConfig); 
	wxDECLARE_ABSTRACT_CLASS(wxDatabaseConfig);
};

#endif//_WX_MLBASE_DBLCONF_H_
