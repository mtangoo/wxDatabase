#To Do List
#Build Types from Options -- Debug Release
#making DLL/LIB
#Add Support for Other DB than MySQL on build 
cmake_minimum_required(VERSION 2.8.0 FATAL_ERROR) 
project (wxDatabase)
string(TOLOWER ${CMAKE_PROJECT_NAME} LIB_OUTPUT_NAME) 

cmake_policy(SET CMP0043 NEW)
# The version number.
set (wxDATABASE_VERSION_MAJOR 1)
set (wxDATABASE_VERSION_MINOR 0)
 
#Set Options 
option (ENABLE_MYSQL "Enable MariaDB and MYSQL Database" OFF) 
option (ENABLE_SQLITE "Enable SQLite3 Database" OFF) 
option (ENABLE_PGS "Enable PostGreSQL Database" ON) 
option (ENABLE_TDS "Enable MSSQL Access with TDS Database" OFF) 
option (ENABLE_ODBC "Enable ODBC Access" OFF) 
option (BUILD_SAMPLE "Build Sample Application" ON) 
#Build type
set(CMAKE_BUILD_TYPE Debug)
#to specify wxDir on non-Windows you need extra check for wxConfig

if(WIN32)
    #the next line can be used to set an environment variable or it can be done inside a shell
    #set (WXWIN, "home/stefano/wxwidgets")
else(WIN32)
    #pass it with option like this --> cmake -DWXWIN=/home/stefano/wxwidgets   
    set(wxWidgets_CONFIG_EXECUTABLE "${WXWIN}/wx-config")
endif(WIN32) 

#Set Directories
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake/Modules/")

#Core include directories 
INCLUDE_DIRECTORIES(${CMAKE_CURRENT_SOURCE_DIR}/include)

#without these they don't work
set(SOURCES) 
set(LINKER_DIRS, "") 

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
	message(FATAL_ERROR "wxWidgets Not Found. Please set WXWIN or add it to System path")
endif(wxWidgets_FOUND)

#Support for MySQL?
if (ENABLE_MYSQL)   
    find_package(MySQL REQUIRED)
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

#Preprocessors
add_definitions(-DWXMAKINGDLL_DATABASE) #WXMAKINGLIB_DATABASE

#Compile library
LINK_DIRECTORIES(${LINKER_DIRS})
add_library(${LIB_OUTPUT_NAME} SHARED ${SOURCES}) 
target_link_libraries(${LIB_OUTPUT_NAME} ${wxWidgets_LIBRARIES} ${MYSQL_CLIENT_LIBS} ${PostgreSQL_LIBRARIES} ${SQLITE3_LIBRARIES} ${ODBC_LIBRARIES}) 


if(BUILD_SAMPLE) #build Samples
	if(ENABLE_SQLITE)
		add_executable (wxDatabaseApp_SQLite3 ${CMAKE_CURRENT_SOURCE_DIR}/samples/sqlite3.cpp)
		link_directories(${CMAKE_CURRENT_SOURCE_DIR}/build)
		target_link_libraries(wxDatabaseApp_SQLite3 ${LIB_OUTPUT_NAME} ${wxWidgets_LIBRARIES}) 
	endif(ENABLE_SQLITE)
	
	if(ENABLE_MYSQL)
		add_executable(wxDatabaseApp_MySQL ${CMAKE_CURRENT_SOURCE_DIR}/samples/mysql.cpp)
		link_directories(${CMAKE_CURRENT_SOURCE_DIR}/build)
		target_link_libraries(wxDatabaseApp_MySQL ${LIB_OUTPUT_NAME} ${wxWidgets_LIBRARIES}) 
	endif(ENABLE_MYSQL)
	
	if(ENABLE_PGS)
		add_executable(wxDatabaseApp_PGS ${CMAKE_CURRENT_SOURCE_DIR}/samples/postgres.cpp)
		link_directories(${CMAKE_CURRENT_SOURCE_DIR}/build)
		target_link_libraries(wxDatabaseApp_PGS ${LIB_OUTPUT_NAME} ${wxWidgets_LIBRARIES}) 
	endif(ENABLE_PGS)
    
	if(ENABLE_ODBC)
		add_executable(wxDatabaseApp_ODBC ${CMAKE_CURRENT_SOURCE_DIR}/samples/odbc.cpp)
		link_directories(${CMAKE_CURRENT_SOURCE_DIR}/build)
		target_link_libraries(wxDatabaseApp_ODBC ${LIB_OUTPUT_NAME} ${wxWidgets_LIBRARIES}) 
	endif(ENABLE_ODBC)
	
endif(BUILD_SAMPLE) 

#Install 
install (DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/include/ DESTINATION include)
install (TARGETS ${LIB_OUTPUT_NAME} ARCHIVE DESTINATION lib  LIBRARY DESTINATION lib  COMPONENT library)
if(ENABLE_MYSQL)
    install (TARGETS wxDatabaseApp_MySQL RUNTIME DESTINATION samples )
endif(ENABLE_MYSQL)

if(ENABLE_SQLITE AND BUILD_SAMPLE)
    install (TARGETS wxDatabaseApp_SQLite3 RUNTIME DESTINATION samples)
endif(ENABLE_SQLITE)

if(ENABLE_PGS AND BUILD_SAMPLE)
    install (TARGETS wxDatabaseApp_PGS RUNTIME DESTINATION samples)
endif(ENABLE_PGS)

if(ENABLE_ODBC AND BUILD_SAMPLE)
    install (TARGETS wxDatabaseApp_ODBC RUNTIME DESTINATION samples)
endif(ENABLE_ODBC)
