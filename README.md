## About the Library 
wxDatabase is built on the excellent work of Joseph Blough called DatabaseLayer.  You can read its story on history file.

 The wxDatabase classes provide a database independent interface similar to JDBC (but only VERY basic functionality). So far, the only database backends supported are SQLite3, MySQL, PostGreSQL, ODBC, and TDS. Other Backends may be supported but they are not tested. We welcome you to help us testing them.
 
 Of course the no brainer part: Have a database server running ;) 
 
 Although the C++ interface can be common between database, it should be noted that the SQL understood by the different database backends may vary.

The main interfaces of the component are:

 - wxDatabase
 - wxPreparedStatement 
 - wxDatabaseResultSet

**Prerequites**:
wxDatabase is a library that add databae support to wxWidgets. So you will need wxWidgets build. wxDatabases works with any wx29 series but you will save yourself a lot of pain by using 3.x instead as we test against that version.

**BUILDING**
To build wxDatabase, you need to have the database development files for the database you want to connect (that is headers and library). 

## USING ODBC 
To Succesful compile with ODBC you need header and libs for your ODBC driver installed. Here are some places to download the files necessary for ODBC

**WINDOWS**
- [PostGreSQL](http://www.postgresql.org/ftp/odbc/versions/msi/) 
- [MySQL](http://dev.mysql.com/downloads/connector/odbc/)
- [Free TDS](http://www.freetds.org/)
- [MariaDB](https://downloads.mariadb.org/connector-odbc/)
- [SQLite3](http://www.ch-werner.de/sqliteodbc/)

**LINUX**

Since Linux have so many distros, I will use Ubuntu (on which I do my development works) as example. It should not be too hard to make your favorite distro work.

First install the ODBC Manager. The Library is tested again [UnixODBC](http://www.unixodbc.org/) but any other manager should work fine. To install UnixODBC in Ubuntu run `sudo apt-get install unixodbc-dev` 

Then install the driver of your choice
- PostGreSQL `sudo apt-get install odbc-postgresql`
- MySQL/MariaDB `sudo apt-get install libmyodbc`
- SQLite3 `sudo apt-get install libsqliteodbc `
- FreeTDS `sudo apt-get install freetds` 

## NATIVE LIBRARIES
### WINDOWS

In Windows, you just need to get development libraries and headers to compile wxDatabase. Here are places you can download the development files:
 1. [MYSQL](https://dev.mysql.com/downloads/connector/c/)
 2. [PostGreSQL](http://www.enterprisedb.com/products-services-training/pgdownload#windows)
 3. [Free TDS](http://www.freetds.org/)
 4. [MariaDB](https://downloads.mariadb.org/connector-c/)

### LINUX
- PostGreSQL `sudo apt-get install postgresql-server-dev-9.4`
- MySQL/MariaDB `sudo apt-get install libmysqlclient-dev` (check [MySQL](http://dev.mysql.com/downloads/repo/apt/) and [MariaDB](https://downloads.mariadb.org/mariadb/repositories/#mirror=webafrica&distro=Ubuntu) articles on how to setup repositories)
- SQLite3 `sudo apt-get install libsqlite3-dev `
- FreeTDS `sudo apt-get install freetds-dev` 
 
**A NOTE ON SQLite3**

SQLite3 comes with wxDatabase distribution. This is the recommended way of bundling the library. However you can use installed SQLite3 library and it should work just fine. However its currently untested and would like to hear from those using the alternative.

## BUILDING
1. Install your compiler tool-set (Library is tested against GCC/MinGW. So reports on other compilers are welcome). Make sure your tool-set bin paths are added to system path.
2. Install CMake and make sure its added to system path
3. Change CMakeList.txt to enable/disable libraries as you want
4. Compile wxWidgets and add the WXWIN environment variable pointing to root folder of wxWidgets to your System environment variables
5. CD to wxDatabase/build on your terminal/CMD window
6. Run CMake command to generate your Makefiles (e.g.`cmake Verbose=1 -G "MinGW Makefiles" ..`). 
7. Run compiler (e.g. `mingw32-make`). If the build finishes successfully the library and samples should be in build directory
8. Run (Optional) install command (e.g. `mingw32-make install`) to install the headers and library to prefix.

## EXAMPLES
See the samples folder for demonstration on how to use wxDatabase library.

## LICENSE
wxDatabase Library is released under wxWidgets License. If you want to give us credit, point URL to http://hosannahighertech.co.tz but its not mandatory.
If you have any question that cannot be asked in wxWidgets forum or mailing list or its personal, contact me at support<(a.t)>hosannahighertech.co.tz