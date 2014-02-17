#ifndef _WX_MLBASE_XMLCONF_H_
#define _WX_MLBASE_XMLCONF_H_

#include "wx/base/wxprec.h"

//***************************************************************************** 
// @file        wxXmlConfig.h 
// @author      Nicolas Dextraze 
// @web         http://www.nicdex.com 
// @date        February 17, 2007 
// @desc        wxXmlConfig class header 
//***************************************************************************** 
#pragma once 

#include <wx/textfile.h> 
#include <wx/confbase.h> 
#include <wx/xml/xml.h> 
#ifdef wxUSE_STREAMS 
#include <wx/stream.h> 
#endif 

class wxXmlConfig; 

/*AML
#ifdef wxConfig 
        #undef wxConfig 
#endif 
#define wxConfig wxXmlConfig 
AML*/

#define wxXmlConfigEntry        wxXmlNode 
#define wxXmlConfigGroup        wxXmlNode 

class WXDLLIMPEXP_MLBASE wxXmlConfig 
        : public wxConfigBase 
{ 
public: 
        wxXmlConfig( const wxString& appName = wxEmptyString, 
                                 const wxString& vendorName = wxEmptyString, 
                                 const wxString& localFilename = wxEmptyString, 
                                 const wxString& globalFilename = wxEmptyString, 
                                 long style = wxCONFIG_USE_LOCAL_FILE | wxCONFIG_USE_GLOBAL_FILE, 
                                 const wxMBConv& conv = wxConvAuto()); 
#ifdef wxUSE_STREAMS 
        wxXmlConfig( wxInputStream& inStream, const wxMBConv& conv = wxConvAuto() ); 
#endif 
        virtual ~wxXmlConfig(); 

        static wxString GetGlobalFileName( const wxChar *szFile ); 
        static wxString GetLocalFileName( const wxChar *szFile ); 

        virtual void SetPath( const wxString& strPath ); 
        virtual const wxString& GetPath() const { return m_strPath; } 

        virtual bool GetFirstGroup( wxString& str, long& lIndex ) const; 
        virtual bool GetNextGroup( wxString& str, long& lIndex ) const; 
        virtual bool GetFirstEntry( wxString& str, long& lIndex ) const; 
        virtual bool GetNextEntry( wxString& str, long& lIndex ) const; 

        virtual size_t GetNumberOfEntries( bool bRecursive = false ) const; 
        virtual size_t GetNumberOfGroups( bool bRecursive = false ) const; 

        virtual bool HasGroup( const wxString& strName ) const; 
        virtual bool HasEntry( const wxString& strName ) const; 

        virtual bool Flush( bool bCurrentOnly = false ); 

        virtual bool RenameEntry( const wxString& oldName, const wxString& newName ); 
        virtual bool RenameGroup( const wxString& oldName, const wxString& newName ); 

        virtual bool DeleteEntry( const wxString& key, bool bGroupIfEmptyAlso = true ); 
        virtual bool DeleteGroup( const wxString& key ); 
        virtual bool DeleteAll(); 

#if wxUSE_STREAMS 
        virtual bool Save( wxOutputStream& os, const wxMBConv& conv = wxConvAuto() ); 
#endif 

protected: 
        virtual bool DoReadString( const wxString& key, wxString* pStr ) const; 
        virtual bool DoReadLong( const wxString& key, long* pl ) const; 

        virtual bool DoWriteString( const wxString& key, const wxString& szValue ); 
        virtual bool DoWriteLong( const wxString& key, long lValue ); 
#if wxCHECK_VERSION(2,9,0) && wxUSE_BASE64
        virtual bool DoReadBinary( const wxString& key, wxMemoryBuffer* buf ) const; 
        virtual bool DoWriteBinary( const wxString& key, const wxMemoryBuffer& buf ); 
#endif
private: 
        static wxString GetGlobalDir(); 
        static wxString GetLocalDir(); 

        void Init(); 

        void CleanUp(); 

        void Parse( const wxXmlDocument& xmlDocument, bool bLocal ); 

        void SetRootPath(); 

        bool DoSetPath( const wxString& strPath, bool createMissingComponents ); 

        void SetDirty() { m_isDirty = true; } 
        void ResetDirty() { m_isDirty = false; } 
        bool IsDirty() { return m_isDirty; } 

        bool IsGroup( const wxXmlConfigGroup *group ) const; 
        bool IsEntry( const wxXmlConfigEntry *entry ) const; 

        wxXmlConfigEntry *FindEntry( const wxString& key ) const; 
        wxXmlConfigEntry *AddEntry( const wxString& key ); 
        wxXmlConfigGroup *FindGroup( const wxString& key ) const; 
        wxXmlConfigGroup *AddGroup( const wxString& key ); 

        wxXmlDocument      *m_xmlDoc; 
        wxXmlConfigGroup        *m_pCurrentGroup; 
        wxString                        m_strLocalFile; 
        wxString                        m_strGlobalFile; 
        wxString                        m_strPath; 
        bool                m_isDirty; 

        wxDECLARE_NO_COPY_CLASS(wxXmlConfig); 
		wxDECLARE_ABSTRACT_CLASS(wxXmlConfig);
};

#endif//_WX_MLBASE_XMLCONF_H_
