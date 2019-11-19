cmake_minimum_required(VERSION 3.1)

if(NOT CMAKE_VERSION VERSION_LESS "3.3")
    # Don't ignore visibility related properties for non-SHARED targets
    cmake_policy(SET CMP0063 NEW)
endif()

if (NOT CMAKE_VERSION VERSION_LESS "3.13")
    # CMP0077: option() honors normal variables
    # https://cmake.org/cmake/help/latest/policy/CMP0077.html
    cmake_policy(SET CMP0077 NEW)
endif()

project (wxDatabase)
string(TOLOWER ${PROJECT_NAME} LIB_OUTPUT_NAME) 

# The version number.
set (wxDATABASE_VERSION_MAJOR 1)
set (wxDATABASE_VERSION_MINOR 0)
 
#Set Options 
option (ENABLE_MYSQL "Enable MariaDB and MYSQL Database" OFF) 
option (ENABLE_SQLITE "Enable SQLite3 Database" OFF) 
option (ENABLE_PGS "Enable PostGreSQL Database" OFF) 
option (ENABLE_TDS "Enable MSSQL Access with TDS Database" ON) 
option (ENABLE_ODBC "Enable ODBC Access" ON) 
option (BUILD_SAMPLE "Build Sample Application" ON) 
#Build type
#set(CMAKE_BUILD_TYPE Debug)

#Set Directories
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_CURRENT_SOURCE_DIR}/cmake/Modules/")
#Core include directories 
INCLUDE_DIRECTORIES(${CMAKE_CURRENT_SOURCE_DIR}/include)

set(EXECUTABLE_OUTPUT_PATH ${CMAKE_BINARY_DIR}/bin)
set(LIBRARY_OUTPUT_PATH ${CMAKE_BINARY_DIR}/lib)

set(wxDATABASE_LIBRARIES ${LIB_OUTPUT_NAME} CACHE INTERNAL "")
set(wxDATABASE_INCLUDE_DIRS ${CMAKE_CURRENT_SOURCE_DIR}/include CACHE INTERNAL "")

#Core Sources  
file(GLOB CORE_SRC "src/base/*.cpp")
#append to main SRC
list(APPEND SOURCES ${CORE_SRC})  
file(GLOB CORE_DB_SRC "src/database/*.cpp")
#append to main SRC
list(APPEND SOURCES ${CORE_DB_SRC})   

#Check wxWidgets first and fail if does not exist
find_package(wxWidgets REQUIRED)
if(wxWidgets_FOUND) 
	include(${wxWidgets_USE_FILE})  
else(wxWidgets_FOUND)
    #This message does not seem to be sent, even if wxWidgets is not found.
	message(FATAL_ERROR "wxWidgets Not Found. Please add it to System path")
endif(wxWidgets_FOUND)

#Support for MySQL?
if (ENABLE_MYSQL)   
    find_package(MySQL)
    if(MYSQL_FOUND)
        #add MySQL include. libs are seemed to dealt with in find_package
        include_directories(${MYSQL_INCLUDE_DIR})         
		list(APPEND LINKER_DIRS  ${MYSQL_LIB_DIR}) 
        
        #add wxDatabase MySQL Include and sources for compiling
        INCLUDE_DIRECTORIES(${CMAKE_CURRENT_SOURCE_DIR}/include/database/mysql)
        
        #append to main SRC
        file(GLOB CORE_MYSQL_SRC "src/database/mysql/*.cpp")
        list(APPEND SOURCES ${CORE_MYSQL_SRC})  	
        #Tell CMake to Pass this variable_requires
        add_definitions(-DwxUSE_DATABASE_MYSQL=1)
    else()
        find_package(MariaDB REQUIRED)
        if(MARIADB_FOUND)
            include_directories(${MARIADB_INCLUDE_DIR})         
            list(APPEND LINKER_DIRS  ${MARIADB_LIBRARY_DIR}) 
            
            #add wxDatabase MySQL Include and sources for compiling
            INCLUDE_DIRECTORIES(${CMAKE_CURRENT_SOURCE_DIR}/include/database/mysql)
            
            #append to main SRC
            file(GLOB CORE_MYSQL_SRC "src/database/mysql/*.cpp")
            list(APPEND SOURCES ${CORE_MYSQL_SRC})  	
            #Tell CMake to Pass this variable_requires
            add_definitions(-DwxUSE_DATABASE_MYSQL=1)
        endif(MARIADB_FOUND)
    endif(MYSQL_FOUND) 	
endif (ENABLE_MYSQL) 

#Support for ODBC?
if (ENABLE_ODBC)   
    find_package(ODBC REQUIRED)
    if(ODBC_FOUND)
        #add ODBC include.  
        include_directories(${ODBC_INCLUDE_DIR})    
        
        #add wxDatabase ODBC Include and sources for compiling
        INCLUDE_DIRECTORIES(${CMAKE_CURRENT_SOURCE_DIR}/include/database/odbc)
        
        #append to main SRC
        file(GLOB CORE_ODBC_SRC "src/database/odbc/*.cpp")
        list(APPEND SOURCES ${CORE_ODBC_SRC})  	
        #Tell CMake to Pass this variable_requires
        add_definitions(-DwxUSE_DATABASE_ODBC=1)
    endif(ODBC_FOUND) 	
endif (ENABLE_ODBC) 

#Support for SQLite3?
if (ENABLE_SQLITE)
    if(EXISTS ${CMAKE_CURRENT_SOURCE_DIR}/vendors/sqlite3/sqlite3.h)
        INCLUDE_DIRECTORIES(${CMAKE_CURRENT_SOURCE_DIR}/vendors/sqlite3/)
        #append to main SRC 
        list(APPEND SOURCES "${CMAKE_CURRENT_SOURCE_DIR}/vendors/sqlite3/sqlite3.c")
        message("Custom SQLite3 Found" )
        
        #append to main SRC
        file(GLOB CORE_SQLITE_SRC "src/database/sqlite3/*.cpp")
        list(APPEND SOURCES ${CORE_SQLITE_SRC})  	
        #Tell CMake to Pass this variable_requires
        add_definitions(-DwxUSE_DATABASE_SQLITE=1)

        
    else() 
    
        find_package(SQLite3 REQUIRED)
        if(SQLITE3_FOUND)
            #add SQLite3 include and libs
            include_directories(${SQLITE3_INCLUDE_DIRS})   
            
            #add wxDatabase SQLite3 Include and sources for compiling
            INCLUDE_DIRECTORIES(${CMAKE_CURRENT_SOURCE_DIR}/include/database/sqlite3)
            
            #append to main SRC
            file(GLOB CORE_SQLITE_SRC "src/database/sqlite3/*.cpp")
            list(APPEND SOURCES ${CORE_SQLITE_SRC})  	
            #Tell CMake to Pass this variable_requires
            add_definitions(-DwxUSE_DATABASE_SQLITE=1)
        endif(SQLITE3_FOUND)
        
    endif()        
endif (ENABLE_SQLITE) 

#Support for PostgreSQL?
if (ENABLE_PGS)   
    find_package(PostgreSQL REQUIRED)
    if(PostgreSQL_FOUND)
        #add PGS include. libs are seemed to dealt with in find_package
        include_directories(${PostgreSQL_INCLUDE_DIRS})   
        LINK_DIRECTORIES(${PostgreSQL_LIBRARY_DIRS})
        
        #add wxDatabase PGS Include and sources for compiling
        INCLUDE_DIRECTORIES(${CMAKE_CURRENT_SOURCE_DIR}/include/database/postgresql)
        
        #append to main SRC
        file(GLOB CORE_PGS_SRC "src/database/postgresql/*.cpp")
        list(APPEND SOURCES ${CORE_PGS_SRC})  	
        #Tell CMake to Pass this variable_requires
        add_definitions(-DwxUSE_DATABASE_POSTGRESQL=1)
        #Ignore the definition of timespec in pthread.h
        if (MINGW)
            add_definitions(-DHAVE_STRUCT_TIMESPEC)
        endif (MINGW)
    endif(PostgreSQL_FOUND) 	
endif (ENABLE_PGS)  

#Support for FreeTDS?
if (ENABLE_TDS)   
    find_package(FreeTDS REQUIRED)
    if(FreeTDS_FOUND)
        #add FreeTDS include.  
        include_directories(${FreeTDS_INCLUDE_DIRS})    
        
        #add wxDatabase TDS include and sources for compiling
        include_directories(${CMAKE_CURRENT_SOURCE_DIR}/include/database/tds)
        
        #append to main SRC
        file(GLOB CORE_TDS_SRC "src/database/tds/*.cpp")
        list(APPEND SOURCES ${CORE_TDS_SRC})  	

        #Tell CMake to Pass this variable_requires
        add_definitions(-DwxUSE_DATABASE_TDS=1)
    endif(FreeTDS_FOUND) 	
endif (ENABLE_TDS)  

#Compile library
LINK_DIRECTORIES(${LINKER_DIRS})
add_library(${LIB_OUTPUT_NAME} SHARED ${SOURCES}) 
target_link_libraries(${LIB_OUTPUT_NAME} ${wxWidgets_LIBRARIES} ${MYSQL_CLIENT_LIBS} ${PostgreSQL_LIBRARIES} ${SQLITE3_LIBRARIES} ${ODBC_LIBRARIES} ${FreeTDS_LIBRARIES}) 
target_compile_definitions(${LIB_OUTPUT_NAME} PRIVATE WXMAKINGDLL_DATABASE=1 WXMAKINGDLL_MLBASE=1)

if(BUILD_SAMPLE) #build Samples
	set(SAMPLE_CONF_FILENAME wxDatabaseApp.conf)
	set(SAMPLE_CONF_SOURCE ${CMAKE_CURRENT_SOURCE_DIR}/samples/${SAMPLE_CONF_FILENAME})
	set(SAMPLE_CONF_TARGET ${CMAKE_CURRENT_BINARY_DIR}/${SAMPLE_CONF_FILENAME})
	link_directories(${CMAKE_CURRENT_SOURCE_DIR}/build)
	add_executable(wxDatabaseApp ${CMAKE_CURRENT_SOURCE_DIR}/samples/wxDatabaseApp.cpp)
	target_sources(wxDatabaseApp PUBLIC ${SAMPLE_CONF_SOURCE})
	target_link_libraries(wxDatabaseApp ${LIB_OUTPUT_NAME} ${wxWidgets_LIBRARIES}) 
	add_custom_command(
			TARGET wxDatabaseApp POST_BUILD
			COMMAND ${CMAKE_COMMAND} -E copy
					${SAMPLE_CONF_SOURCE}
					${SAMPLE_CONF_TARGET}
	)
	if(WIN32)
		set_target_properties(wxDatabaseApp PROPERTIES LINK_FLAGS_DEBUG "/SUBSYSTEM:CONSOLE /ENTRY:wxDatabaseApp::OnRun")
		set_target_properties(wxDatabaseApp PROPERTIES LINK_FLAGS_RELEASE "/SUBSYSTEM:CONSOLE /ENTRY:wxDatabaseApp::OnRun")
	endif(WIN32)
endif(BUILD_SAMPLE) 

#Install 
install (DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/include/ DESTINATION include)
install (TARGETS ${LIB_OUTPUT_NAME} ARCHIVE DESTINATION lib  LIBRARY DESTINATION lib  RUNTIME DESTINATION lib COMPONENT library)

if(BUILD_SAMPLE)
    install (TARGETS wxDatabaseApp RUNTIME DESTINATION samples)
endif(BUILD_SAMPLE)
