#ifndef _WX_MLBASE_WXPREC_H_
#define _WX_MLBASE_WXPREC_H_

#include "wx/database/wxprec.h"

#if defined(_DEBUG) && defined(_MSC_VER)
    #include <crtdbg.h>
	#define debug_normal_new new(_NORMAL_BLOCK ,__FILE__, __LINE__)
	#define debug_ignore_new new(_IGNORE_BLOCK ,__FILE__, __LINE__)
#else
    #define debug_normal_new new
    #define debug_ignore_new new
#endif

#include "wx/base/setup.h"
#include "wx/base/dlimpexp.h"

#include <wx/convauto.h> 
#include <wx/confbase.h> 

#if wxUSE_DATABASE_CONFIG
	#include "wx/base/dbconf.h"
#endif
#if wxUSE_XML_CONFIG
	#include "wx/base/xmlconf.h"
#endif

#endif//_WX_MLBASE_WXPREC_H_