#To Do List
#Build Types from Options -- Debug Release
#making DLL/LIB
#Add Support for Other DB than MySQL on build 

cmake_minimum_required (VERSION 2.8)
project (wxDatabase)
cmake_policy(SET CMP0043 NEW)
# The version number.
set (wxDATABASE_VERSION_MAJOR 1)
set (wxDATABASE_VERSION_MINOR 0)
 
#Set Options 
option (ENABLE_MYSQL "Enable MariaDB Database" ON) 
option (ENABLE_SQLITE "Enable SQLite3 Database" ON) 
option (ENABLE_PGS "Enable PostGreSQL Database" OFF) 
option (ENABLE_TDS "Enable MSSQL Access with TDS Database" OFF) 
option (ENABLE_ODBC "Enable ODBC Access" OFF) 
option (BUILD_SAMPLE "Build Sample Application" ON) 
#Build type
set(CMAKE_BUILD_TYPE Debug)
#to specify wxDir on non Windows you need extra check for wxWonfig

if(WIN32)
    #set(wxWidgets_ROOT_DIR, "YOUR wx Path")
else(WIN32)
    #pass it with option like this --> cmake -DwxWidgets_ROOT_DIR=/home/stefano/wxwidgets   
    set(wxWidgets_CONFIG_EXECUTABLE "${wxWidgets_ROOT_DIR}/wx-config")
endif(WIN32) 

#Set Directories
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake/Modules/")

#Core include directories 
INCLUDE_DIRECTORIES(${CMAKE_CURRENT_SOURCE_DIR}/include)
set(SOURCES) #without thise they dont work
#Core Sources  
file(GLOB CORE_SRC "src/base/*.cpp")
#append to main SRC
list(APPEND SOURCES ${CORE_SRC})  
file(GLOB CORE_DB_SRC "src/database/*.cpp")
#append to main SRC
list(APPEND SOURCES ${CORE_DB_SRC})  

#Support for MySQL?
if (ENABLE_MYSQL)   
    find_package(MySQL REQUIRED)
    if(MYSQL_FOUND)
        #add MySQL include and libs
        include_directories(${MYSQL_INCLUDE_DIR})  
        #Link MySQL Libraries
        #LINK_DIRECTORIES(${MYSQL_LIB_DIR})
        #TARGET_LINK_LIBRARIES(${MYSQL_CLIENT_LIBS})
        
        #add wxDatabase MySQL Include and sources for compiling
        INCLUDE_DIRECTORIES(${CMAKE_CURRENT_SOURCE_DIR}/include/database/mysql)
        
        #append to main SRC
        file(GLOB CORE_MYSQL_SRC "src/database/mysql/*.cpp")
        list(APPEND SOURCES ${CORE_MYSQL_SRC})  	
        #Tell CMake to Pass this variable_requires
        add_definitions(-DwxUSE_DATABASE_MYSQL=1)
    endif(MYSQL_FOUND) 	
endif (ENABLE_MYSQL) 


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
            #Link SQLite3 Library
            TARGET_LINK_LIBRARIES(${SQLITE3_LIBRARIES})
            
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
 
#Preprocessors
add_definitions(-DWXMAKINGDLL_DATABASE) #WXMAKINGLIB_DATABASE

#Compile library
string(TOLOWER ${CMAKE_PROJECT_NAME} LIB_OUTPUT_NAME)
add_library(${LIB_OUTPUT_NAME} SHARED ${SOURCES}) 

#Check wxWidgets first and fail if does not exist
find_package(wxWidgets REQUIRED)
if(wxWidgets_FOUND)
	include(${wxWidgets_USE_FILE}) 
	target_link_libraries(${LIB_OUTPUT_NAME} ${wxWidgets_LIBRARIES}) 
else(wxWidgets_FOUND)
	message( FATAL_ERROR "wxWidgets Not Found. Please define and set wxWidgets_ROOT_DIR or add it to System path" )
endif(wxWidgets_FOUND)

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
	
endif(BUILD_SAMPLE)

#Install 
install (DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/include/ DESTINATION include)
install (TARGETS wxDatabaseApp_MySQL RUNTIME DESTINATION samples)
install (TARGETS wxDatabaseApp_SQLite3 RUNTIME DESTINATION samples)
install (TARGETS ${LIB_OUTPUT_NAME} LIBRARY DESTINATION lib)
