#
# Find the FreeTDS library 
#
# FreeTDS_INCLUDE_DIRS, where to find the FreeTDS include directories
# FreeTDS_LIBRARIES, the libraries to link against to use FreeTDS
# FreeTDS_FOUND - If false, you cannot build anything that requires TDS


if (WIN32)
    #the next line can be used to set an environment variable or it can be done inside a shell
    SET(FreeTDS "C:\\wxDev\\freetds-1.00.24")
else(WIN32)
    #pass it with option like this --> cmake -DFREETDS=/home/stefano/freetds-1.00.24   
endif(WIN32) 

#MESSAGE("FreeTDS = [" ${FreeTDS} "]")

IF (FreeTDS_INCLUDE_DIRS AND FreeTDS_LIBRARIES)

	# Already in cache
	SET(FREETDS_FOUND TRUE)

ELSE (FreeTDS_INCLUDE_DIRS AND FreeTDS_LIBRARIES)

	FIND_PATH(FreeTDS_INCLUDE NAMES dblib.h
		PATHS
			${FreeTDS}
		PATH_SUFFIXES
			include
			include/freetds
		NO_DEFAULT_PATH)

	FIND_PATH(FreeTDS_BUILD_INCLUDE NAMES tds_sysdep_public.h
		PATHS
			${FreeTDS}
		PATH_SUFFIXES
			build/include
		NO_DEFAULT_PATH)

	FIND_PATH(FreeTDS_INCLUDE_FREETDS NAMES tds.h
		PATHS
			${FreeTDS}
		PATH_SUFFIXES
			include
			include/freetds
		NO_DEFAULT_PATH)
	
	SET(FreeTDS_INCLUDE_DIRS ${FreeTDS_INCLUDE} ${FreeTDS_BUILD_INCLUDE})
	
#MESSAGE("FreeTDS_INCLUDE_DIRS = [" ${FreeTDS_INCLUDE_DIRS} "]")

	FIND_LIBRARY(FreeTDS_LIBRARY_TDS NAMES tds.lib
		PATHS 
			${FreeTDS}/build/src/tds
		PATH_SUFFIXES
IF (DEBUG)
			debug
ELSE (DEBUG)
			release
ENDIF (DEBUG)
		NO_DEFAULT_PATH)

	FIND_LIBRARY(FreeTDS_LIBRARY_REPLACEMENTS NAMES replacements.lib
		PATHS 
			${FreeTDS}/build/src/replacements
		PATH_SUFFIXES
IF (DEBUG)
			debug
ELSE (DEBUG)
			release
ENDIF (DEBUG)
		NO_DEFAULT_PATH)

	FIND_LIBRARY(FreeTDS_LIBRARY_DBLIB NAMES dblib.lib db-lib.lib
		PATHS 
			${FreeTDS}/build/src/dblib
		PATH_SUFFIXES
IF (DEBUG)
			debug
ELSE (DEBUG)
			release
ENDIF (DEBUG)
		NO_DEFAULT_PATH)
	
	SET(FreeTDS_LIBRARIES ${FreeTDS_LIBRARY_TDS} ${FreeTDS_LIBRARY_REPLACEMENTS} ${FreeTDS_LIBRARY_DBLIB})

if (WIN32)
	SET(FreeTDS_LIBRARIES ws2_32.lib ${FreeTDS_LIBRARIES})
endif(WIN32) 

#MESSAGE("FreeTDS_LIBRARIES = [" ${FreeTDS_LIBRARIES} "]")

	INCLUDE(FindPackageHandleStandardArgs)
	FIND_PACKAGE_HANDLE_STANDARD_ARGS(FreeTDS DEFAULT_MSG FreeTDS_INCLUDE_DIRS FreeTDS_LIBRARIES)

	MARK_AS_ADVANCED(FreeTDS_INCLUDE_DIRS FreeTDS_LIBRARIES)

ENDIF (FreeTDS_INCLUDE_DIRS AND FreeTDS_LIBRARIES)

