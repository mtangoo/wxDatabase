#To Do List
#Build Types from Options -- Debug Release
#making DLL/LIB
#Add Support for Other DB than MySQL on build
#Fix the redefined error visible when verbose=1

cmake_minimum_required (VERSION 2.8)
project (wxDatabase)

# The version number.
set (wxDATABASE_VERSION_MAJOR 1)
set (wxDATABASE_VERSION_MINOR 0)
#find_program(CMAKE_MAKE_PROGRAM  NAMES make mingw32-make make.exe     DOC "Find a suitable make program for building under Windows/MinGW" HINTS C:/MinGW/bin ) 


#Set Options 
option (ENABLE_MYSQL "Enable MySQL Database" ON) 
option (ENABLE_PGS "Enable PostGreSQL Database" OFF) 
option (ENABLE_TDS "Enable MSSQL Access with TDS Database" OFF) 
option (ENABLE_ODBC "Enable ODBC Access" OFF) 
#Build type
set(CMAKE_BUILD_TYPE Release)
#set (wxWidgets_ROOT_DIR, "Your wxDirHere")

#Set Directories
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake/Modules/")

#Core include directories 
INCLUDE_DIRECTORIES(${CMAKE_CURRENT_SOURCE_DIR}/include)

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

#Preprocessors
add_definitions(-DWXMAKINGDLL_DATABASE) #WXMAKINGLIB_DATABASE

#Compile library
add_library(${CMAKE_PROJECT_NAME} SHARED ${SOURCES}) 

#Check wxWidgets first and fail if does not exist
find_package(wxWidgets REQUIRED)
if(wxWidgets_FOUND)
	include(${wxWidgets_USE_FILE}) 
	target_link_libraries(${CMAKE_PROJECT_NAME} ${wxWidgets_LIBRARIES})
else(wxWidgets_FOUND)
	message( FATAL_ERROR "wxWidgets Not Found. Please define and set wxWidgets_ROOT_DIR or add it to System path" )
endif(wxWidgets_FOUND)

