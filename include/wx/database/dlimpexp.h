#ifndef _WX_DATABASE_DLIMPEXP_H_
#define _WX_DATABASE_DLIMPEXP_H_

#ifdef WXMAKINGDLL_DATABASE
#    define WXDLLIMPEXP_DATABASE WXEXPORT
#    define WXDLLIMPEXP_DATA_DATABASE(type) WXEXPORT type
#    if defined(HAVE_VISIBILITY)
#        define WXDLLIMPEXP_INLINE_DATABASE WXEXPORT
#    else
#        define WXDLLIMPEXP_INLINE_DATABASE
#    endif
#elif defined(WXUSINGDLL)
#    define WXDLLIMPEXP_DATABASE WXIMPORT
#    define WXDLLIMPEXP_DATA_DATABASE(type) WXIMPORT type
#    if defined(HAVE_VISIBILITY)
#        define WXDLLIMPEXP_INLINE_DATABASE WXIMPORT
#    else
#        define WXDLLIMPEXP_INLINE_DATABASE
#    endif
#else /* not making nor using DLL */
#    define WXDLLIMPEXP_DATABASE
#    define WXDLLIMPEXP_DATA_DATABASE(type) type
#    define WXDLLIMPEXP_INLINE_DATABASE
#endif

/*
   GCC warns about using __attribute__ (and also __declspec in mingw32 case) on
   forward declarations while MSVC complains about forward declarations without
   __declspec for the classes later declared with it, so we need a separate set
   of macros for forward declarations to hide this difference:
 */
#if defined(HAVE_VISIBILITY) || (defined(__WINDOWS__) && defined(__GNUC__))
    #define WXDLLIMPEXP_FWD_DATABASE
#else
    #define WXDLLIMPEXP_FWD_DATABASE      WXDLLIMPEXP_DATABASE
#endif

/* for backwards compatibility, define suffix-less versions too */
//#define WXDLLEXPORT WXDLLIMPEXP_DATABASE
//#define WXDLLEXPORT_DATA WXDLLIMPEXP_DATA_DATABASE

/*
   MSVC up to 6.0 needs to be explicitly told to export template instantiations
   used by the DLL clients, use this macro to do it like this:

       template <typename T> class Foo { ... };
       WXDLLIMPEXP_TEMPLATE_INSTANCE_DATABASE( Foo<int> )

   (notice that currently we only need this for wxBase and wxCore libraries)
 */
#if defined(__VISUALC__) && (__VISUALC__ <= 1200)
    /*
       We need to disable this warning when using this macro, as
       recommended by Microsoft itself:

       http://support.microsoft.com/default.aspx?scid=kb%3ben-us%3b168958
     */
    #pragma warning(disable:4231)

    #define WXDLLIMPEXP_TEMPLATE_INSTANCE_DATABASE(decl) \
        extern template class WXDLLIMPEXP_DATABASE decl;
#else /* not VC <= 6 */
    #define WXDLLIMPEXP_TEMPLATE_INSTANCE_DATABASE(decl)
#endif /* VC6/others */

#endif//_WX_DATABASE_DLIMPEXP_H
