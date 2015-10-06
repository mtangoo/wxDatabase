About the Library
-----------------
wxDatabase is built on the excellent work of Joseph Blough called DatabaseLayer.  You can read its story on history file.

 The wxDatabase classes provide a database independent interface similar to JDBC (but only VERY basic functionality). So far, the only database backends supported are SQLite3, MySQL, PostGreSQL, and TDS. Other Backends may be supported but they are not tested. We welcome you to help us testing them.

EXAMPLE: TBD
 
 Although the C++ interface can be common between database, it should be noted that the SQL understood by the different database backends may vary.

The main interfaces of the component are:

 - wxDatabase
 - wxPreparedStatement 
 - wxDatabaseResultSet

**Prerequites**:
wxDatabase is a library that add databae support to wxWidgets. So you will need wxWidgets build. wxDatabases works with any wx29 series but you will save yourself a lot of pain by using 3.x instead as we test against that version.

**BUILDING**
To build wxDatabase, you need to have the database ou want to connect ready with headers and library. I will explain using MySQL but the process is the same with any other database.

**MYSQL**

 1. [Download](https://dev.mysql.com/downloads/connector/c/) MySQL    Connector C and Install it. In Linux you just need to install   development library of MySQL      
 2. Open your Console (CMD for windows for Windows) and cd to build sub-folder in wxDatabase root directory     
 3. Run CMake command to generate your flavour (e.g. cmake Verbose=1 -G   "MinGW Makefiles" ../)      
 4. Run make program (for MinGW files use mingw32-make)
 5. Enjoy your libraries in build folder)


