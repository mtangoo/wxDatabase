##
## Auto Generated makefile by CodeLite IDE
## any manual changes will be erased      
##
## Debug
ProjectName            :=databaselayer
ConfigurationName      :=Debug
WorkspacePath          := "/home/stefano/Documents/developer/Hosanna/Hosanna_IDE"
ProjectPath            := "/home/stefano/Documents/developer/Hosanna/Hosanna_IDE/database_manager/databaselayer"
IntermediateDirectory  :=../../working/$(ProjectName)
OutDir                 := $(IntermediateDirectory)
CurrentFileName        :=
CurrentFilePath        :=
CurrentFileFullPath    :=
User                   :=Stefano D. Mtangoo
Date                   :=11/26/2012
CodeLitePath           :="/home/stefano/.codelite"
LinkerName             :=g++
SharedObjectLinkerName :=g++ -shared -fPIC
ObjectSuffix           :=.o
DependSuffix           :=.o.d
PreprocessSuffix       :=.o.i
DebugSwitch            :=-gstab
IncludeSwitch          :=-I
LibrarySwitch          :=-l
OutputSwitch           :=-o 
LibraryPathSwitch      :=-L
PreprocessorSwitch     :=-D
SourceSwitch           :=-c 
OutputFile             :=../../libsoutput/lib$(ProjectName).a
Preprocessors          :=
ObjectSwitch           :=-o 
ArchiveOutputSwitch    := 
PreprocessOnlySwitch   :=-E 
ObjectsFileList        :="/home/stefano/Documents/developer/Hosanna/Hosanna_IDE/database_manager/databaselayer/databaselayer.txt"
PCHCompileFlags        :=
MakeDirCommand         :=mkdir -p
LinkOptions            :=  $(shell $(WX_TOOL) --libs --unicode=yes --debug=yes ) -rdynamic 
IncludePath            :=  $(IncludeSwitch). $(IncludeSwitch). $(IncludeSwitch)vendors/mysql/include $(IncludeSwitch)vendors/postgresql/include 
IncludePCH             := 
RcIncludePath          := 
Libs                   := $(LibrarySwitch)vendors/mysql/lib/libmysqlclient 
ArLibs                 :=  "vendors/mysql/lib/libmysqlclient.a" 
LibPath                := $(LibraryPathSwitch). 

##
## Common variables
## AR, CXX, CC, CXXFLAGS and CFLAGS can be overriden using an environment variables
##
AR       := ar rcus
CXX      := g++
CC       := gcc
CXXFLAGS :=  -g -Wall -O0 $(shell $(WX_TOOL) --cxxflags --unicode=yes --debug=yes ) $(Preprocessors)
CFLAGS   :=  -g -Wall -O0 $(shell $(WX_TOOL) --cflags --unicode=yes --debug=yes ) $(Preprocessors)


##
## User defined environment variables
##
CodeLiteDir:=/usr/share/codelite
WX_TOOL:=/home/stefano/Documents/developer/svn/wxwidgets/wx-config
MYSQL_INC:=`/home/stefano/Documents/developer/Hosanna/Hosanna_IDE/lib/compilelibs/mysql-connector-c-6.0.2/mysql_config/mysql_config --include`
Objects=$(IntermediateDirectory)/mysql_mysql_database$(ObjectSuffix) $(IntermediateDirectory)/mysql_mysql_interface$(ObjectSuffix) $(IntermediateDirectory)/mysql_mysql_param$(ObjectSuffix) $(IntermediateDirectory)/mysql_mysql_preparedstatement$(ObjectSuffix) $(IntermediateDirectory)/mysql_mysql_preparedstatement_parameter$(ObjectSuffix) $(IntermediateDirectory)/mysql_mysql_preparedstatement_parametercollection$(ObjectSuffix) $(IntermediateDirectory)/mysql_mysql_preparedStatement_resultSet$(ObjectSuffix) $(IntermediateDirectory)/mysql_mysql_preparedstatement_wrapper$(ObjectSuffix) $(IntermediateDirectory)/mysql_mysql_resultset_metadata$(ObjectSuffix) $(IntermediateDirectory)/src_database$(ObjectSuffix) \
	$(IntermediateDirectory)/src_error_reporter$(ObjectSuffix) $(IntermediateDirectory)/src_PreparedStatement$(ObjectSuffix) $(IntermediateDirectory)/src_query_paser$(ObjectSuffix) $(IntermediateDirectory)/src_resultset$(ObjectSuffix) $(IntermediateDirectory)/src_strconverter$(ObjectSuffix) $(IntermediateDirectory)/postgresql_postgresql_database$(ObjectSuffix) $(IntermediateDirectory)/postgresql_postgresql_interface$(ObjectSuffix) $(IntermediateDirectory)/postgresql_postgresql_param$(ObjectSuffix) $(IntermediateDirectory)/postgresql_postgresql_preparedstatement$(ObjectSuffix) $(IntermediateDirectory)/postgresql_postgresql_preparedstatement_parameter_collector$(ObjectSuffix) \
	$(IntermediateDirectory)/postgresql_postgresql_preparedstatement_wrapper$(ObjectSuffix) $(IntermediateDirectory)/postgresql_postgresql_resultset$(ObjectSuffix) $(IntermediateDirectory)/postgresql_postgresql_resultset_metadata$(ObjectSuffix) $(IntermediateDirectory)/sqlite3_sqlite_database$(ObjectSuffix) $(IntermediateDirectory)/sqlite3_sqlite_preparedstatement$(ObjectSuffix) $(IntermediateDirectory)/sqlite3_sqlite_resultset$(ObjectSuffix) $(IntermediateDirectory)/sqlite3_sqlite_resultset_metatdata$(ObjectSuffix) $(IntermediateDirectory)/odbc_odbc_database$(ObjectSuffix) $(IntermediateDirectory)/odbc_odbc_inteface$(ObjectSuffix) $(IntermediateDirectory)/odbc_odbc_param$(ObjectSuffix) \
	$(IntermediateDirectory)/odbc_odbc_preparedstatement$(ObjectSuffix) $(IntermediateDirectory)/odbc_odbc_resultset$(ObjectSuffix) $(IntermediateDirectory)/odbc_odbc_resultset_metadata$(ObjectSuffix) 

##
## Main Build Targets 
##
.PHONY: all clean PreBuild PrePreBuild PostBuild
all: $(IntermediateDirectory) $(OutputFile)

$(OutputFile): $(Objects)
	@$(MakeDirCommand) $(@D)
	@echo "" > $(IntermediateDirectory)/.d
	@echo $(Objects) > $(ObjectsFileList)
	$(AR) $(ArchiveOutputSwitch)$(OutputFile) @$(ObjectsFileList) $(ArLibs)
	@$(MakeDirCommand) "/home/stefano/Documents/developer/Hosanna/Hosanna_IDE/.build-debug"
	@echo rebuilt > "/home/stefano/Documents/developer/Hosanna/Hosanna_IDE/.build-debug/databaselayer"

../../working/$(ProjectName):
	@test -d ../../working/$(ProjectName) || $(MakeDirCommand) ../../working/$(ProjectName)

PreBuild:


##
## Objects
##
$(IntermediateDirectory)/mysql_mysql_database$(ObjectSuffix): src/mysql/mysql_database.cpp $(IntermediateDirectory)/mysql_mysql_database$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/stefano/Documents/developer/Hosanna/Hosanna_IDE/database_manager/databaselayer/src/mysql/mysql_database.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/mysql_mysql_database$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/mysql_mysql_database$(DependSuffix): src/mysql/mysql_database.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/mysql_mysql_database$(ObjectSuffix) -MF$(IntermediateDirectory)/mysql_mysql_database$(DependSuffix) -MM "/home/stefano/Documents/developer/Hosanna/Hosanna_IDE/database_manager/databaselayer/src/mysql/mysql_database.cpp"

$(IntermediateDirectory)/mysql_mysql_database$(PreprocessSuffix): src/mysql/mysql_database.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/mysql_mysql_database$(PreprocessSuffix) "/home/stefano/Documents/developer/Hosanna/Hosanna_IDE/database_manager/databaselayer/src/mysql/mysql_database.cpp"

$(IntermediateDirectory)/mysql_mysql_interface$(ObjectSuffix): src/mysql/mysql_interface.cpp $(IntermediateDirectory)/mysql_mysql_interface$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/stefano/Documents/developer/Hosanna/Hosanna_IDE/database_manager/databaselayer/src/mysql/mysql_interface.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/mysql_mysql_interface$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/mysql_mysql_interface$(DependSuffix): src/mysql/mysql_interface.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/mysql_mysql_interface$(ObjectSuffix) -MF$(IntermediateDirectory)/mysql_mysql_interface$(DependSuffix) -MM "/home/stefano/Documents/developer/Hosanna/Hosanna_IDE/database_manager/databaselayer/src/mysql/mysql_interface.cpp"

$(IntermediateDirectory)/mysql_mysql_interface$(PreprocessSuffix): src/mysql/mysql_interface.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/mysql_mysql_interface$(PreprocessSuffix) "/home/stefano/Documents/developer/Hosanna/Hosanna_IDE/database_manager/databaselayer/src/mysql/mysql_interface.cpp"

$(IntermediateDirectory)/mysql_mysql_param$(ObjectSuffix): src/mysql/mysql_param.cpp $(IntermediateDirectory)/mysql_mysql_param$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/stefano/Documents/developer/Hosanna/Hosanna_IDE/database_manager/databaselayer/src/mysql/mysql_param.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/mysql_mysql_param$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/mysql_mysql_param$(DependSuffix): src/mysql/mysql_param.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/mysql_mysql_param$(ObjectSuffix) -MF$(IntermediateDirectory)/mysql_mysql_param$(DependSuffix) -MM "/home/stefano/Documents/developer/Hosanna/Hosanna_IDE/database_manager/databaselayer/src/mysql/mysql_param.cpp"

$(IntermediateDirectory)/mysql_mysql_param$(PreprocessSuffix): src/mysql/mysql_param.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/mysql_mysql_param$(PreprocessSuffix) "/home/stefano/Documents/developer/Hosanna/Hosanna_IDE/database_manager/databaselayer/src/mysql/mysql_param.cpp"

$(IntermediateDirectory)/mysql_mysql_preparedstatement$(ObjectSuffix): src/mysql/mysql_preparedstatement.cpp $(IntermediateDirectory)/mysql_mysql_preparedstatement$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/stefano/Documents/developer/Hosanna/Hosanna_IDE/database_manager/databaselayer/src/mysql/mysql_preparedstatement.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/mysql_mysql_preparedstatement$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/mysql_mysql_preparedstatement$(DependSuffix): src/mysql/mysql_preparedstatement.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/mysql_mysql_preparedstatement$(ObjectSuffix) -MF$(IntermediateDirectory)/mysql_mysql_preparedstatement$(DependSuffix) -MM "/home/stefano/Documents/developer/Hosanna/Hosanna_IDE/database_manager/databaselayer/src/mysql/mysql_preparedstatement.cpp"

$(IntermediateDirectory)/mysql_mysql_preparedstatement$(PreprocessSuffix): src/mysql/mysql_preparedstatement.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/mysql_mysql_preparedstatement$(PreprocessSuffix) "/home/stefano/Documents/developer/Hosanna/Hosanna_IDE/database_manager/databaselayer/src/mysql/mysql_preparedstatement.cpp"

$(IntermediateDirectory)/mysql_mysql_preparedstatement_parameter$(ObjectSuffix): src/mysql/mysql_preparedstatement_parameter.cpp $(IntermediateDirectory)/mysql_mysql_preparedstatement_parameter$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/stefano/Documents/developer/Hosanna/Hosanna_IDE/database_manager/databaselayer/src/mysql/mysql_preparedstatement_parameter.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/mysql_mysql_preparedstatement_parameter$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/mysql_mysql_preparedstatement_parameter$(DependSuffix): src/mysql/mysql_preparedstatement_parameter.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/mysql_mysql_preparedstatement_parameter$(ObjectSuffix) -MF$(IntermediateDirectory)/mysql_mysql_preparedstatement_parameter$(DependSuffix) -MM "/home/stefano/Documents/developer/Hosanna/Hosanna_IDE/database_manager/databaselayer/src/mysql/mysql_preparedstatement_parameter.cpp"

$(IntermediateDirectory)/mysql_mysql_preparedstatement_parameter$(PreprocessSuffix): src/mysql/mysql_preparedstatement_parameter.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/mysql_mysql_preparedstatement_parameter$(PreprocessSuffix) "/home/stefano/Documents/developer/Hosanna/Hosanna_IDE/database_manager/databaselayer/src/mysql/mysql_preparedstatement_parameter.cpp"

$(IntermediateDirectory)/mysql_mysql_preparedstatement_parametercollection$(ObjectSuffix): src/mysql/mysql_preparedstatement_parametercollection.cpp $(IntermediateDirectory)/mysql_mysql_preparedstatement_parametercollection$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/stefano/Documents/developer/Hosanna/Hosanna_IDE/database_manager/databaselayer/src/mysql/mysql_preparedstatement_parametercollection.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/mysql_mysql_preparedstatement_parametercollection$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/mysql_mysql_preparedstatement_parametercollection$(DependSuffix): src/mysql/mysql_preparedstatement_parametercollection.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/mysql_mysql_preparedstatement_parametercollection$(ObjectSuffix) -MF$(IntermediateDirectory)/mysql_mysql_preparedstatement_parametercollection$(DependSuffix) -MM "/home/stefano/Documents/developer/Hosanna/Hosanna_IDE/database_manager/databaselayer/src/mysql/mysql_preparedstatement_parametercollection.cpp"

$(IntermediateDirectory)/mysql_mysql_preparedstatement_parametercollection$(PreprocessSuffix): src/mysql/mysql_preparedstatement_parametercollection.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/mysql_mysql_preparedstatement_parametercollection$(PreprocessSuffix) "/home/stefano/Documents/developer/Hosanna/Hosanna_IDE/database_manager/databaselayer/src/mysql/mysql_preparedstatement_parametercollection.cpp"

$(IntermediateDirectory)/mysql_mysql_preparedStatement_resultSet$(ObjectSuffix): src/mysql/mysql_preparedStatement_resultSet.cpp $(IntermediateDirectory)/mysql_mysql_preparedStatement_resultSet$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/stefano/Documents/developer/Hosanna/Hosanna_IDE/database_manager/databaselayer/src/mysql/mysql_preparedStatement_resultSet.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/mysql_mysql_preparedStatement_resultSet$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/mysql_mysql_preparedStatement_resultSet$(DependSuffix): src/mysql/mysql_preparedStatement_resultSet.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/mysql_mysql_preparedStatement_resultSet$(ObjectSuffix) -MF$(IntermediateDirectory)/mysql_mysql_preparedStatement_resultSet$(DependSuffix) -MM "/home/stefano/Documents/developer/Hosanna/Hosanna_IDE/database_manager/databaselayer/src/mysql/mysql_preparedStatement_resultSet.cpp"

$(IntermediateDirectory)/mysql_mysql_preparedStatement_resultSet$(PreprocessSuffix): src/mysql/mysql_preparedStatement_resultSet.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/mysql_mysql_preparedStatement_resultSet$(PreprocessSuffix) "/home/stefano/Documents/developer/Hosanna/Hosanna_IDE/database_manager/databaselayer/src/mysql/mysql_preparedStatement_resultSet.cpp"

$(IntermediateDirectory)/mysql_mysql_preparedstatement_wrapper$(ObjectSuffix): src/mysql/mysql_preparedstatement_wrapper.cpp $(IntermediateDirectory)/mysql_mysql_preparedstatement_wrapper$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/stefano/Documents/developer/Hosanna/Hosanna_IDE/database_manager/databaselayer/src/mysql/mysql_preparedstatement_wrapper.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/mysql_mysql_preparedstatement_wrapper$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/mysql_mysql_preparedstatement_wrapper$(DependSuffix): src/mysql/mysql_preparedstatement_wrapper.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/mysql_mysql_preparedstatement_wrapper$(ObjectSuffix) -MF$(IntermediateDirectory)/mysql_mysql_preparedstatement_wrapper$(DependSuffix) -MM "/home/stefano/Documents/developer/Hosanna/Hosanna_IDE/database_manager/databaselayer/src/mysql/mysql_preparedstatement_wrapper.cpp"

$(IntermediateDirectory)/mysql_mysql_preparedstatement_wrapper$(PreprocessSuffix): src/mysql/mysql_preparedstatement_wrapper.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/mysql_mysql_preparedstatement_wrapper$(PreprocessSuffix) "/home/stefano/Documents/developer/Hosanna/Hosanna_IDE/database_manager/databaselayer/src/mysql/mysql_preparedstatement_wrapper.cpp"

$(IntermediateDirectory)/mysql_mysql_resultset_metadata$(ObjectSuffix): src/mysql/mysql_resultset_metadata.cpp $(IntermediateDirectory)/mysql_mysql_resultset_metadata$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/stefano/Documents/developer/Hosanna/Hosanna_IDE/database_manager/databaselayer/src/mysql/mysql_resultset_metadata.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/mysql_mysql_resultset_metadata$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/mysql_mysql_resultset_metadata$(DependSuffix): src/mysql/mysql_resultset_metadata.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/mysql_mysql_resultset_metadata$(ObjectSuffix) -MF$(IntermediateDirectory)/mysql_mysql_resultset_metadata$(DependSuffix) -MM "/home/stefano/Documents/developer/Hosanna/Hosanna_IDE/database_manager/databaselayer/src/mysql/mysql_resultset_metadata.cpp"

$(IntermediateDirectory)/mysql_mysql_resultset_metadata$(PreprocessSuffix): src/mysql/mysql_resultset_metadata.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/mysql_mysql_resultset_metadata$(PreprocessSuffix) "/home/stefano/Documents/developer/Hosanna/Hosanna_IDE/database_manager/databaselayer/src/mysql/mysql_resultset_metadata.cpp"

$(IntermediateDirectory)/src_database$(ObjectSuffix): src/database.cpp $(IntermediateDirectory)/src_database$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/stefano/Documents/developer/Hosanna/Hosanna_IDE/database_manager/databaselayer/src/database.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/src_database$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/src_database$(DependSuffix): src/database.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/src_database$(ObjectSuffix) -MF$(IntermediateDirectory)/src_database$(DependSuffix) -MM "/home/stefano/Documents/developer/Hosanna/Hosanna_IDE/database_manager/databaselayer/src/database.cpp"

$(IntermediateDirectory)/src_database$(PreprocessSuffix): src/database.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/src_database$(PreprocessSuffix) "/home/stefano/Documents/developer/Hosanna/Hosanna_IDE/database_manager/databaselayer/src/database.cpp"

$(IntermediateDirectory)/src_error_reporter$(ObjectSuffix): src/error_reporter.cpp $(IntermediateDirectory)/src_error_reporter$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/stefano/Documents/developer/Hosanna/Hosanna_IDE/database_manager/databaselayer/src/error_reporter.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/src_error_reporter$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/src_error_reporter$(DependSuffix): src/error_reporter.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/src_error_reporter$(ObjectSuffix) -MF$(IntermediateDirectory)/src_error_reporter$(DependSuffix) -MM "/home/stefano/Documents/developer/Hosanna/Hosanna_IDE/database_manager/databaselayer/src/error_reporter.cpp"

$(IntermediateDirectory)/src_error_reporter$(PreprocessSuffix): src/error_reporter.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/src_error_reporter$(PreprocessSuffix) "/home/stefano/Documents/developer/Hosanna/Hosanna_IDE/database_manager/databaselayer/src/error_reporter.cpp"

$(IntermediateDirectory)/src_PreparedStatement$(ObjectSuffix): src/PreparedStatement.cpp $(IntermediateDirectory)/src_PreparedStatement$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/stefano/Documents/developer/Hosanna/Hosanna_IDE/database_manager/databaselayer/src/PreparedStatement.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/src_PreparedStatement$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/src_PreparedStatement$(DependSuffix): src/PreparedStatement.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/src_PreparedStatement$(ObjectSuffix) -MF$(IntermediateDirectory)/src_PreparedStatement$(DependSuffix) -MM "/home/stefano/Documents/developer/Hosanna/Hosanna_IDE/database_manager/databaselayer/src/PreparedStatement.cpp"

$(IntermediateDirectory)/src_PreparedStatement$(PreprocessSuffix): src/PreparedStatement.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/src_PreparedStatement$(PreprocessSuffix) "/home/stefano/Documents/developer/Hosanna/Hosanna_IDE/database_manager/databaselayer/src/PreparedStatement.cpp"

$(IntermediateDirectory)/src_query_paser$(ObjectSuffix): src/query_paser.cpp $(IntermediateDirectory)/src_query_paser$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/stefano/Documents/developer/Hosanna/Hosanna_IDE/database_manager/databaselayer/src/query_paser.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/src_query_paser$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/src_query_paser$(DependSuffix): src/query_paser.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/src_query_paser$(ObjectSuffix) -MF$(IntermediateDirectory)/src_query_paser$(DependSuffix) -MM "/home/stefano/Documents/developer/Hosanna/Hosanna_IDE/database_manager/databaselayer/src/query_paser.cpp"

$(IntermediateDirectory)/src_query_paser$(PreprocessSuffix): src/query_paser.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/src_query_paser$(PreprocessSuffix) "/home/stefano/Documents/developer/Hosanna/Hosanna_IDE/database_manager/databaselayer/src/query_paser.cpp"

$(IntermediateDirectory)/src_resultset$(ObjectSuffix): src/resultset.cpp $(IntermediateDirectory)/src_resultset$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/stefano/Documents/developer/Hosanna/Hosanna_IDE/database_manager/databaselayer/src/resultset.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/src_resultset$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/src_resultset$(DependSuffix): src/resultset.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/src_resultset$(ObjectSuffix) -MF$(IntermediateDirectory)/src_resultset$(DependSuffix) -MM "/home/stefano/Documents/developer/Hosanna/Hosanna_IDE/database_manager/databaselayer/src/resultset.cpp"

$(IntermediateDirectory)/src_resultset$(PreprocessSuffix): src/resultset.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/src_resultset$(PreprocessSuffix) "/home/stefano/Documents/developer/Hosanna/Hosanna_IDE/database_manager/databaselayer/src/resultset.cpp"

$(IntermediateDirectory)/src_strconverter$(ObjectSuffix): src/strconverter.cpp $(IntermediateDirectory)/src_strconverter$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/stefano/Documents/developer/Hosanna/Hosanna_IDE/database_manager/databaselayer/src/strconverter.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/src_strconverter$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/src_strconverter$(DependSuffix): src/strconverter.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/src_strconverter$(ObjectSuffix) -MF$(IntermediateDirectory)/src_strconverter$(DependSuffix) -MM "/home/stefano/Documents/developer/Hosanna/Hosanna_IDE/database_manager/databaselayer/src/strconverter.cpp"

$(IntermediateDirectory)/src_strconverter$(PreprocessSuffix): src/strconverter.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/src_strconverter$(PreprocessSuffix) "/home/stefano/Documents/developer/Hosanna/Hosanna_IDE/database_manager/databaselayer/src/strconverter.cpp"

$(IntermediateDirectory)/postgresql_postgresql_database$(ObjectSuffix): src/postgresql/postgresql_database.cpp $(IntermediateDirectory)/postgresql_postgresql_database$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/stefano/Documents/developer/Hosanna/Hosanna_IDE/database_manager/databaselayer/src/postgresql/postgresql_database.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/postgresql_postgresql_database$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/postgresql_postgresql_database$(DependSuffix): src/postgresql/postgresql_database.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/postgresql_postgresql_database$(ObjectSuffix) -MF$(IntermediateDirectory)/postgresql_postgresql_database$(DependSuffix) -MM "/home/stefano/Documents/developer/Hosanna/Hosanna_IDE/database_manager/databaselayer/src/postgresql/postgresql_database.cpp"

$(IntermediateDirectory)/postgresql_postgresql_database$(PreprocessSuffix): src/postgresql/postgresql_database.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/postgresql_postgresql_database$(PreprocessSuffix) "/home/stefano/Documents/developer/Hosanna/Hosanna_IDE/database_manager/databaselayer/src/postgresql/postgresql_database.cpp"

$(IntermediateDirectory)/postgresql_postgresql_interface$(ObjectSuffix): src/postgresql/postgresql_interface.cpp $(IntermediateDirectory)/postgresql_postgresql_interface$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/stefano/Documents/developer/Hosanna/Hosanna_IDE/database_manager/databaselayer/src/postgresql/postgresql_interface.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/postgresql_postgresql_interface$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/postgresql_postgresql_interface$(DependSuffix): src/postgresql/postgresql_interface.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/postgresql_postgresql_interface$(ObjectSuffix) -MF$(IntermediateDirectory)/postgresql_postgresql_interface$(DependSuffix) -MM "/home/stefano/Documents/developer/Hosanna/Hosanna_IDE/database_manager/databaselayer/src/postgresql/postgresql_interface.cpp"

$(IntermediateDirectory)/postgresql_postgresql_interface$(PreprocessSuffix): src/postgresql/postgresql_interface.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/postgresql_postgresql_interface$(PreprocessSuffix) "/home/stefano/Documents/developer/Hosanna/Hosanna_IDE/database_manager/databaselayer/src/postgresql/postgresql_interface.cpp"

$(IntermediateDirectory)/postgresql_postgresql_param$(ObjectSuffix): src/postgresql/postgresql_param.cpp $(IntermediateDirectory)/postgresql_postgresql_param$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/stefano/Documents/developer/Hosanna/Hosanna_IDE/database_manager/databaselayer/src/postgresql/postgresql_param.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/postgresql_postgresql_param$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/postgresql_postgresql_param$(DependSuffix): src/postgresql/postgresql_param.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/postgresql_postgresql_param$(ObjectSuffix) -MF$(IntermediateDirectory)/postgresql_postgresql_param$(DependSuffix) -MM "/home/stefano/Documents/developer/Hosanna/Hosanna_IDE/database_manager/databaselayer/src/postgresql/postgresql_param.cpp"

$(IntermediateDirectory)/postgresql_postgresql_param$(PreprocessSuffix): src/postgresql/postgresql_param.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/postgresql_postgresql_param$(PreprocessSuffix) "/home/stefano/Documents/developer/Hosanna/Hosanna_IDE/database_manager/databaselayer/src/postgresql/postgresql_param.cpp"

$(IntermediateDirectory)/postgresql_postgresql_preparedstatement$(ObjectSuffix): src/postgresql/postgresql_preparedstatement.cpp $(IntermediateDirectory)/postgresql_postgresql_preparedstatement$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/stefano/Documents/developer/Hosanna/Hosanna_IDE/database_manager/databaselayer/src/postgresql/postgresql_preparedstatement.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/postgresql_postgresql_preparedstatement$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/postgresql_postgresql_preparedstatement$(DependSuffix): src/postgresql/postgresql_preparedstatement.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/postgresql_postgresql_preparedstatement$(ObjectSuffix) -MF$(IntermediateDirectory)/postgresql_postgresql_preparedstatement$(DependSuffix) -MM "/home/stefano/Documents/developer/Hosanna/Hosanna_IDE/database_manager/databaselayer/src/postgresql/postgresql_preparedstatement.cpp"

$(IntermediateDirectory)/postgresql_postgresql_preparedstatement$(PreprocessSuffix): src/postgresql/postgresql_preparedstatement.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/postgresql_postgresql_preparedstatement$(PreprocessSuffix) "/home/stefano/Documents/developer/Hosanna/Hosanna_IDE/database_manager/databaselayer/src/postgresql/postgresql_preparedstatement.cpp"

$(IntermediateDirectory)/postgresql_postgresql_preparedstatement_parameter_collector$(ObjectSuffix): src/postgresql/postgresql_preparedstatement_parameter_collector.cpp $(IntermediateDirectory)/postgresql_postgresql_preparedstatement_parameter_collector$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/stefano/Documents/developer/Hosanna/Hosanna_IDE/database_manager/databaselayer/src/postgresql/postgresql_preparedstatement_parameter_collector.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/postgresql_postgresql_preparedstatement_parameter_collector$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/postgresql_postgresql_preparedstatement_parameter_collector$(DependSuffix): src/postgresql/postgresql_preparedstatement_parameter_collector.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/postgresql_postgresql_preparedstatement_parameter_collector$(ObjectSuffix) -MF$(IntermediateDirectory)/postgresql_postgresql_preparedstatement_parameter_collector$(DependSuffix) -MM "/home/stefano/Documents/developer/Hosanna/Hosanna_IDE/database_manager/databaselayer/src/postgresql/postgresql_preparedstatement_parameter_collector.cpp"

$(IntermediateDirectory)/postgresql_postgresql_preparedstatement_parameter_collector$(PreprocessSuffix): src/postgresql/postgresql_preparedstatement_parameter_collector.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/postgresql_postgresql_preparedstatement_parameter_collector$(PreprocessSuffix) "/home/stefano/Documents/developer/Hosanna/Hosanna_IDE/database_manager/databaselayer/src/postgresql/postgresql_preparedstatement_parameter_collector.cpp"

$(IntermediateDirectory)/postgresql_postgresql_preparedstatement_wrapper$(ObjectSuffix): src/postgresql/postgresql_preparedstatement_wrapper.cpp $(IntermediateDirectory)/postgresql_postgresql_preparedstatement_wrapper$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/stefano/Documents/developer/Hosanna/Hosanna_IDE/database_manager/databaselayer/src/postgresql/postgresql_preparedstatement_wrapper.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/postgresql_postgresql_preparedstatement_wrapper$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/postgresql_postgresql_preparedstatement_wrapper$(DependSuffix): src/postgresql/postgresql_preparedstatement_wrapper.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/postgresql_postgresql_preparedstatement_wrapper$(ObjectSuffix) -MF$(IntermediateDirectory)/postgresql_postgresql_preparedstatement_wrapper$(DependSuffix) -MM "/home/stefano/Documents/developer/Hosanna/Hosanna_IDE/database_manager/databaselayer/src/postgresql/postgresql_preparedstatement_wrapper.cpp"

$(IntermediateDirectory)/postgresql_postgresql_preparedstatement_wrapper$(PreprocessSuffix): src/postgresql/postgresql_preparedstatement_wrapper.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/postgresql_postgresql_preparedstatement_wrapper$(PreprocessSuffix) "/home/stefano/Documents/developer/Hosanna/Hosanna_IDE/database_manager/databaselayer/src/postgresql/postgresql_preparedstatement_wrapper.cpp"

$(IntermediateDirectory)/postgresql_postgresql_resultset$(ObjectSuffix): src/postgresql/postgresql_resultset.cpp $(IntermediateDirectory)/postgresql_postgresql_resultset$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/stefano/Documents/developer/Hosanna/Hosanna_IDE/database_manager/databaselayer/src/postgresql/postgresql_resultset.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/postgresql_postgresql_resultset$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/postgresql_postgresql_resultset$(DependSuffix): src/postgresql/postgresql_resultset.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/postgresql_postgresql_resultset$(ObjectSuffix) -MF$(IntermediateDirectory)/postgresql_postgresql_resultset$(DependSuffix) -MM "/home/stefano/Documents/developer/Hosanna/Hosanna_IDE/database_manager/databaselayer/src/postgresql/postgresql_resultset.cpp"

$(IntermediateDirectory)/postgresql_postgresql_resultset$(PreprocessSuffix): src/postgresql/postgresql_resultset.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/postgresql_postgresql_resultset$(PreprocessSuffix) "/home/stefano/Documents/developer/Hosanna/Hosanna_IDE/database_manager/databaselayer/src/postgresql/postgresql_resultset.cpp"

$(IntermediateDirectory)/postgresql_postgresql_resultset_metadata$(ObjectSuffix): src/postgresql/postgresql_resultset_metadata.cpp $(IntermediateDirectory)/postgresql_postgresql_resultset_metadata$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/stefano/Documents/developer/Hosanna/Hosanna_IDE/database_manager/databaselayer/src/postgresql/postgresql_resultset_metadata.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/postgresql_postgresql_resultset_metadata$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/postgresql_postgresql_resultset_metadata$(DependSuffix): src/postgresql/postgresql_resultset_metadata.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/postgresql_postgresql_resultset_metadata$(ObjectSuffix) -MF$(IntermediateDirectory)/postgresql_postgresql_resultset_metadata$(DependSuffix) -MM "/home/stefano/Documents/developer/Hosanna/Hosanna_IDE/database_manager/databaselayer/src/postgresql/postgresql_resultset_metadata.cpp"

$(IntermediateDirectory)/postgresql_postgresql_resultset_metadata$(PreprocessSuffix): src/postgresql/postgresql_resultset_metadata.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/postgresql_postgresql_resultset_metadata$(PreprocessSuffix) "/home/stefano/Documents/developer/Hosanna/Hosanna_IDE/database_manager/databaselayer/src/postgresql/postgresql_resultset_metadata.cpp"

$(IntermediateDirectory)/sqlite3_sqlite_database$(ObjectSuffix): src/sqlite3/sqlite_database.cpp $(IntermediateDirectory)/sqlite3_sqlite_database$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/stefano/Documents/developer/Hosanna/Hosanna_IDE/database_manager/databaselayer/src/sqlite3/sqlite_database.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/sqlite3_sqlite_database$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/sqlite3_sqlite_database$(DependSuffix): src/sqlite3/sqlite_database.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/sqlite3_sqlite_database$(ObjectSuffix) -MF$(IntermediateDirectory)/sqlite3_sqlite_database$(DependSuffix) -MM "/home/stefano/Documents/developer/Hosanna/Hosanna_IDE/database_manager/databaselayer/src/sqlite3/sqlite_database.cpp"

$(IntermediateDirectory)/sqlite3_sqlite_database$(PreprocessSuffix): src/sqlite3/sqlite_database.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/sqlite3_sqlite_database$(PreprocessSuffix) "/home/stefano/Documents/developer/Hosanna/Hosanna_IDE/database_manager/databaselayer/src/sqlite3/sqlite_database.cpp"

$(IntermediateDirectory)/sqlite3_sqlite_preparedstatement$(ObjectSuffix): src/sqlite3/sqlite_preparedstatement.cpp $(IntermediateDirectory)/sqlite3_sqlite_preparedstatement$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/stefano/Documents/developer/Hosanna/Hosanna_IDE/database_manager/databaselayer/src/sqlite3/sqlite_preparedstatement.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/sqlite3_sqlite_preparedstatement$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/sqlite3_sqlite_preparedstatement$(DependSuffix): src/sqlite3/sqlite_preparedstatement.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/sqlite3_sqlite_preparedstatement$(ObjectSuffix) -MF$(IntermediateDirectory)/sqlite3_sqlite_preparedstatement$(DependSuffix) -MM "/home/stefano/Documents/developer/Hosanna/Hosanna_IDE/database_manager/databaselayer/src/sqlite3/sqlite_preparedstatement.cpp"

$(IntermediateDirectory)/sqlite3_sqlite_preparedstatement$(PreprocessSuffix): src/sqlite3/sqlite_preparedstatement.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/sqlite3_sqlite_preparedstatement$(PreprocessSuffix) "/home/stefano/Documents/developer/Hosanna/Hosanna_IDE/database_manager/databaselayer/src/sqlite3/sqlite_preparedstatement.cpp"

$(IntermediateDirectory)/sqlite3_sqlite_resultset$(ObjectSuffix): src/sqlite3/sqlite_resultset.cpp $(IntermediateDirectory)/sqlite3_sqlite_resultset$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/stefano/Documents/developer/Hosanna/Hosanna_IDE/database_manager/databaselayer/src/sqlite3/sqlite_resultset.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/sqlite3_sqlite_resultset$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/sqlite3_sqlite_resultset$(DependSuffix): src/sqlite3/sqlite_resultset.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/sqlite3_sqlite_resultset$(ObjectSuffix) -MF$(IntermediateDirectory)/sqlite3_sqlite_resultset$(DependSuffix) -MM "/home/stefano/Documents/developer/Hosanna/Hosanna_IDE/database_manager/databaselayer/src/sqlite3/sqlite_resultset.cpp"

$(IntermediateDirectory)/sqlite3_sqlite_resultset$(PreprocessSuffix): src/sqlite3/sqlite_resultset.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/sqlite3_sqlite_resultset$(PreprocessSuffix) "/home/stefano/Documents/developer/Hosanna/Hosanna_IDE/database_manager/databaselayer/src/sqlite3/sqlite_resultset.cpp"

$(IntermediateDirectory)/sqlite3_sqlite_resultset_metatdata$(ObjectSuffix): src/sqlite3/sqlite_resultset_metatdata.cpp $(IntermediateDirectory)/sqlite3_sqlite_resultset_metatdata$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/stefano/Documents/developer/Hosanna/Hosanna_IDE/database_manager/databaselayer/src/sqlite3/sqlite_resultset_metatdata.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/sqlite3_sqlite_resultset_metatdata$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/sqlite3_sqlite_resultset_metatdata$(DependSuffix): src/sqlite3/sqlite_resultset_metatdata.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/sqlite3_sqlite_resultset_metatdata$(ObjectSuffix) -MF$(IntermediateDirectory)/sqlite3_sqlite_resultset_metatdata$(DependSuffix) -MM "/home/stefano/Documents/developer/Hosanna/Hosanna_IDE/database_manager/databaselayer/src/sqlite3/sqlite_resultset_metatdata.cpp"

$(IntermediateDirectory)/sqlite3_sqlite_resultset_metatdata$(PreprocessSuffix): src/sqlite3/sqlite_resultset_metatdata.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/sqlite3_sqlite_resultset_metatdata$(PreprocessSuffix) "/home/stefano/Documents/developer/Hosanna/Hosanna_IDE/database_manager/databaselayer/src/sqlite3/sqlite_resultset_metatdata.cpp"

$(IntermediateDirectory)/odbc_odbc_database$(ObjectSuffix): src/odbc/odbc_database.cpp $(IntermediateDirectory)/odbc_odbc_database$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/stefano/Documents/developer/Hosanna/Hosanna_IDE/database_manager/databaselayer/src/odbc/odbc_database.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/odbc_odbc_database$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/odbc_odbc_database$(DependSuffix): src/odbc/odbc_database.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/odbc_odbc_database$(ObjectSuffix) -MF$(IntermediateDirectory)/odbc_odbc_database$(DependSuffix) -MM "/home/stefano/Documents/developer/Hosanna/Hosanna_IDE/database_manager/databaselayer/src/odbc/odbc_database.cpp"

$(IntermediateDirectory)/odbc_odbc_database$(PreprocessSuffix): src/odbc/odbc_database.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/odbc_odbc_database$(PreprocessSuffix) "/home/stefano/Documents/developer/Hosanna/Hosanna_IDE/database_manager/databaselayer/src/odbc/odbc_database.cpp"

$(IntermediateDirectory)/odbc_odbc_inteface$(ObjectSuffix): src/odbc/odbc_inteface.cpp $(IntermediateDirectory)/odbc_odbc_inteface$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/stefano/Documents/developer/Hosanna/Hosanna_IDE/database_manager/databaselayer/src/odbc/odbc_inteface.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/odbc_odbc_inteface$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/odbc_odbc_inteface$(DependSuffix): src/odbc/odbc_inteface.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/odbc_odbc_inteface$(ObjectSuffix) -MF$(IntermediateDirectory)/odbc_odbc_inteface$(DependSuffix) -MM "/home/stefano/Documents/developer/Hosanna/Hosanna_IDE/database_manager/databaselayer/src/odbc/odbc_inteface.cpp"

$(IntermediateDirectory)/odbc_odbc_inteface$(PreprocessSuffix): src/odbc/odbc_inteface.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/odbc_odbc_inteface$(PreprocessSuffix) "/home/stefano/Documents/developer/Hosanna/Hosanna_IDE/database_manager/databaselayer/src/odbc/odbc_inteface.cpp"

$(IntermediateDirectory)/odbc_odbc_param$(ObjectSuffix): src/odbc/odbc_param.cpp $(IntermediateDirectory)/odbc_odbc_param$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/stefano/Documents/developer/Hosanna/Hosanna_IDE/database_manager/databaselayer/src/odbc/odbc_param.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/odbc_odbc_param$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/odbc_odbc_param$(DependSuffix): src/odbc/odbc_param.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/odbc_odbc_param$(ObjectSuffix) -MF$(IntermediateDirectory)/odbc_odbc_param$(DependSuffix) -MM "/home/stefano/Documents/developer/Hosanna/Hosanna_IDE/database_manager/databaselayer/src/odbc/odbc_param.cpp"

$(IntermediateDirectory)/odbc_odbc_param$(PreprocessSuffix): src/odbc/odbc_param.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/odbc_odbc_param$(PreprocessSuffix) "/home/stefano/Documents/developer/Hosanna/Hosanna_IDE/database_manager/databaselayer/src/odbc/odbc_param.cpp"

$(IntermediateDirectory)/odbc_odbc_preparedstatement$(ObjectSuffix): src/odbc/odbc_preparedstatement.cpp $(IntermediateDirectory)/odbc_odbc_preparedstatement$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/stefano/Documents/developer/Hosanna/Hosanna_IDE/database_manager/databaselayer/src/odbc/odbc_preparedstatement.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/odbc_odbc_preparedstatement$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/odbc_odbc_preparedstatement$(DependSuffix): src/odbc/odbc_preparedstatement.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/odbc_odbc_preparedstatement$(ObjectSuffix) -MF$(IntermediateDirectory)/odbc_odbc_preparedstatement$(DependSuffix) -MM "/home/stefano/Documents/developer/Hosanna/Hosanna_IDE/database_manager/databaselayer/src/odbc/odbc_preparedstatement.cpp"

$(IntermediateDirectory)/odbc_odbc_preparedstatement$(PreprocessSuffix): src/odbc/odbc_preparedstatement.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/odbc_odbc_preparedstatement$(PreprocessSuffix) "/home/stefano/Documents/developer/Hosanna/Hosanna_IDE/database_manager/databaselayer/src/odbc/odbc_preparedstatement.cpp"

$(IntermediateDirectory)/odbc_odbc_resultset$(ObjectSuffix): src/odbc/odbc_resultset.cpp $(IntermediateDirectory)/odbc_odbc_resultset$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/stefano/Documents/developer/Hosanna/Hosanna_IDE/database_manager/databaselayer/src/odbc/odbc_resultset.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/odbc_odbc_resultset$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/odbc_odbc_resultset$(DependSuffix): src/odbc/odbc_resultset.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/odbc_odbc_resultset$(ObjectSuffix) -MF$(IntermediateDirectory)/odbc_odbc_resultset$(DependSuffix) -MM "/home/stefano/Documents/developer/Hosanna/Hosanna_IDE/database_manager/databaselayer/src/odbc/odbc_resultset.cpp"

$(IntermediateDirectory)/odbc_odbc_resultset$(PreprocessSuffix): src/odbc/odbc_resultset.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/odbc_odbc_resultset$(PreprocessSuffix) "/home/stefano/Documents/developer/Hosanna/Hosanna_IDE/database_manager/databaselayer/src/odbc/odbc_resultset.cpp"

$(IntermediateDirectory)/odbc_odbc_resultset_metadata$(ObjectSuffix): src/odbc/odbc_resultset_metadata.cpp $(IntermediateDirectory)/odbc_odbc_resultset_metadata$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/stefano/Documents/developer/Hosanna/Hosanna_IDE/database_manager/databaselayer/src/odbc/odbc_resultset_metadata.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/odbc_odbc_resultset_metadata$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/odbc_odbc_resultset_metadata$(DependSuffix): src/odbc/odbc_resultset_metadata.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/odbc_odbc_resultset_metadata$(ObjectSuffix) -MF$(IntermediateDirectory)/odbc_odbc_resultset_metadata$(DependSuffix) -MM "/home/stefano/Documents/developer/Hosanna/Hosanna_IDE/database_manager/databaselayer/src/odbc/odbc_resultset_metadata.cpp"

$(IntermediateDirectory)/odbc_odbc_resultset_metadata$(PreprocessSuffix): src/odbc/odbc_resultset_metadata.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/odbc_odbc_resultset_metadata$(PreprocessSuffix) "/home/stefano/Documents/developer/Hosanna/Hosanna_IDE/database_manager/databaselayer/src/odbc/odbc_resultset_metadata.cpp"


-include $(IntermediateDirectory)/*$(DependSuffix)
##
## Clean
##
clean:
	$(RM) $(IntermediateDirectory)/mysql_mysql_database$(ObjectSuffix)
	$(RM) $(IntermediateDirectory)/mysql_mysql_database$(DependSuffix)
	$(RM) $(IntermediateDirectory)/mysql_mysql_database$(PreprocessSuffix)
	$(RM) $(IntermediateDirectory)/mysql_mysql_interface$(ObjectSuffix)
	$(RM) $(IntermediateDirectory)/mysql_mysql_interface$(DependSuffix)
	$(RM) $(IntermediateDirectory)/mysql_mysql_interface$(PreprocessSuffix)
	$(RM) $(IntermediateDirectory)/mysql_mysql_param$(ObjectSuffix)
	$(RM) $(IntermediateDirectory)/mysql_mysql_param$(DependSuffix)
	$(RM) $(IntermediateDirectory)/mysql_mysql_param$(PreprocessSuffix)
	$(RM) $(IntermediateDirectory)/mysql_mysql_preparedstatement$(ObjectSuffix)
	$(RM) $(IntermediateDirectory)/mysql_mysql_preparedstatement$(DependSuffix)
	$(RM) $(IntermediateDirectory)/mysql_mysql_preparedstatement$(PreprocessSuffix)
	$(RM) $(IntermediateDirectory)/mysql_mysql_preparedstatement_parameter$(ObjectSuffix)
	$(RM) $(IntermediateDirectory)/mysql_mysql_preparedstatement_parameter$(DependSuffix)
	$(RM) $(IntermediateDirectory)/mysql_mysql_preparedstatement_parameter$(PreprocessSuffix)
	$(RM) $(IntermediateDirectory)/mysql_mysql_preparedstatement_parametercollection$(ObjectSuffix)
	$(RM) $(IntermediateDirectory)/mysql_mysql_preparedstatement_parametercollection$(DependSuffix)
	$(RM) $(IntermediateDirectory)/mysql_mysql_preparedstatement_parametercollection$(PreprocessSuffix)
	$(RM) $(IntermediateDirectory)/mysql_mysql_preparedStatement_resultSet$(ObjectSuffix)
	$(RM) $(IntermediateDirectory)/mysql_mysql_preparedStatement_resultSet$(DependSuffix)
	$(RM) $(IntermediateDirectory)/mysql_mysql_preparedStatement_resultSet$(PreprocessSuffix)
	$(RM) $(IntermediateDirectory)/mysql_mysql_preparedstatement_wrapper$(ObjectSuffix)
	$(RM) $(IntermediateDirectory)/mysql_mysql_preparedstatement_wrapper$(DependSuffix)
	$(RM) $(IntermediateDirectory)/mysql_mysql_preparedstatement_wrapper$(PreprocessSuffix)
	$(RM) $(IntermediateDirectory)/mysql_mysql_resultset_metadata$(ObjectSuffix)
	$(RM) $(IntermediateDirectory)/mysql_mysql_resultset_metadata$(DependSuffix)
	$(RM) $(IntermediateDirectory)/mysql_mysql_resultset_metadata$(PreprocessSuffix)
	$(RM) $(IntermediateDirectory)/src_database$(ObjectSuffix)
	$(RM) $(IntermediateDirectory)/src_database$(DependSuffix)
	$(RM) $(IntermediateDirectory)/src_database$(PreprocessSuffix)
	$(RM) $(IntermediateDirectory)/src_error_reporter$(ObjectSuffix)
	$(RM) $(IntermediateDirectory)/src_error_reporter$(DependSuffix)
	$(RM) $(IntermediateDirectory)/src_error_reporter$(PreprocessSuffix)
	$(RM) $(IntermediateDirectory)/src_PreparedStatement$(ObjectSuffix)
	$(RM) $(IntermediateDirectory)/src_PreparedStatement$(DependSuffix)
	$(RM) $(IntermediateDirectory)/src_PreparedStatement$(PreprocessSuffix)
	$(RM) $(IntermediateDirectory)/src_query_paser$(ObjectSuffix)
	$(RM) $(IntermediateDirectory)/src_query_paser$(DependSuffix)
	$(RM) $(IntermediateDirectory)/src_query_paser$(PreprocessSuffix)
	$(RM) $(IntermediateDirectory)/src_resultset$(ObjectSuffix)
	$(RM) $(IntermediateDirectory)/src_resultset$(DependSuffix)
	$(RM) $(IntermediateDirectory)/src_resultset$(PreprocessSuffix)
	$(RM) $(IntermediateDirectory)/src_strconverter$(ObjectSuffix)
	$(RM) $(IntermediateDirectory)/src_strconverter$(DependSuffix)
	$(RM) $(IntermediateDirectory)/src_strconverter$(PreprocessSuffix)
	$(RM) $(IntermediateDirectory)/postgresql_postgresql_database$(ObjectSuffix)
	$(RM) $(IntermediateDirectory)/postgresql_postgresql_database$(DependSuffix)
	$(RM) $(IntermediateDirectory)/postgresql_postgresql_database$(PreprocessSuffix)
	$(RM) $(IntermediateDirectory)/postgresql_postgresql_interface$(ObjectSuffix)
	$(RM) $(IntermediateDirectory)/postgresql_postgresql_interface$(DependSuffix)
	$(RM) $(IntermediateDirectory)/postgresql_postgresql_interface$(PreprocessSuffix)
	$(RM) $(IntermediateDirectory)/postgresql_postgresql_param$(ObjectSuffix)
	$(RM) $(IntermediateDirectory)/postgresql_postgresql_param$(DependSuffix)
	$(RM) $(IntermediateDirectory)/postgresql_postgresql_param$(PreprocessSuffix)
	$(RM) $(IntermediateDirectory)/postgresql_postgresql_preparedstatement$(ObjectSuffix)
	$(RM) $(IntermediateDirectory)/postgresql_postgresql_preparedstatement$(DependSuffix)
	$(RM) $(IntermediateDirectory)/postgresql_postgresql_preparedstatement$(PreprocessSuffix)
	$(RM) $(IntermediateDirectory)/postgresql_postgresql_preparedstatement_parameter_collector$(ObjectSuffix)
	$(RM) $(IntermediateDirectory)/postgresql_postgresql_preparedstatement_parameter_collector$(DependSuffix)
	$(RM) $(IntermediateDirectory)/postgresql_postgresql_preparedstatement_parameter_collector$(PreprocessSuffix)
	$(RM) $(IntermediateDirectory)/postgresql_postgresql_preparedstatement_wrapper$(ObjectSuffix)
	$(RM) $(IntermediateDirectory)/postgresql_postgresql_preparedstatement_wrapper$(DependSuffix)
	$(RM) $(IntermediateDirectory)/postgresql_postgresql_preparedstatement_wrapper$(PreprocessSuffix)
	$(RM) $(IntermediateDirectory)/postgresql_postgresql_resultset$(ObjectSuffix)
	$(RM) $(IntermediateDirectory)/postgresql_postgresql_resultset$(DependSuffix)
	$(RM) $(IntermediateDirectory)/postgresql_postgresql_resultset$(PreprocessSuffix)
	$(RM) $(IntermediateDirectory)/postgresql_postgresql_resultset_metadata$(ObjectSuffix)
	$(RM) $(IntermediateDirectory)/postgresql_postgresql_resultset_metadata$(DependSuffix)
	$(RM) $(IntermediateDirectory)/postgresql_postgresql_resultset_metadata$(PreprocessSuffix)
	$(RM) $(IntermediateDirectory)/sqlite3_sqlite_database$(ObjectSuffix)
	$(RM) $(IntermediateDirectory)/sqlite3_sqlite_database$(DependSuffix)
	$(RM) $(IntermediateDirectory)/sqlite3_sqlite_database$(PreprocessSuffix)
	$(RM) $(IntermediateDirectory)/sqlite3_sqlite_preparedstatement$(ObjectSuffix)
	$(RM) $(IntermediateDirectory)/sqlite3_sqlite_preparedstatement$(DependSuffix)
	$(RM) $(IntermediateDirectory)/sqlite3_sqlite_preparedstatement$(PreprocessSuffix)
	$(RM) $(IntermediateDirectory)/sqlite3_sqlite_resultset$(ObjectSuffix)
	$(RM) $(IntermediateDirectory)/sqlite3_sqlite_resultset$(DependSuffix)
	$(RM) $(IntermediateDirectory)/sqlite3_sqlite_resultset$(PreprocessSuffix)
	$(RM) $(IntermediateDirectory)/sqlite3_sqlite_resultset_metatdata$(ObjectSuffix)
	$(RM) $(IntermediateDirectory)/sqlite3_sqlite_resultset_metatdata$(DependSuffix)
	$(RM) $(IntermediateDirectory)/sqlite3_sqlite_resultset_metatdata$(PreprocessSuffix)
	$(RM) $(IntermediateDirectory)/odbc_odbc_database$(ObjectSuffix)
	$(RM) $(IntermediateDirectory)/odbc_odbc_database$(DependSuffix)
	$(RM) $(IntermediateDirectory)/odbc_odbc_database$(PreprocessSuffix)
	$(RM) $(IntermediateDirectory)/odbc_odbc_inteface$(ObjectSuffix)
	$(RM) $(IntermediateDirectory)/odbc_odbc_inteface$(DependSuffix)
	$(RM) $(IntermediateDirectory)/odbc_odbc_inteface$(PreprocessSuffix)
	$(RM) $(IntermediateDirectory)/odbc_odbc_param$(ObjectSuffix)
	$(RM) $(IntermediateDirectory)/odbc_odbc_param$(DependSuffix)
	$(RM) $(IntermediateDirectory)/odbc_odbc_param$(PreprocessSuffix)
	$(RM) $(IntermediateDirectory)/odbc_odbc_preparedstatement$(ObjectSuffix)
	$(RM) $(IntermediateDirectory)/odbc_odbc_preparedstatement$(DependSuffix)
	$(RM) $(IntermediateDirectory)/odbc_odbc_preparedstatement$(PreprocessSuffix)
	$(RM) $(IntermediateDirectory)/odbc_odbc_resultset$(ObjectSuffix)
	$(RM) $(IntermediateDirectory)/odbc_odbc_resultset$(DependSuffix)
	$(RM) $(IntermediateDirectory)/odbc_odbc_resultset$(PreprocessSuffix)
	$(RM) $(IntermediateDirectory)/odbc_odbc_resultset_metadata$(ObjectSuffix)
	$(RM) $(IntermediateDirectory)/odbc_odbc_resultset_metadata$(DependSuffix)
	$(RM) $(IntermediateDirectory)/odbc_odbc_resultset_metadata$(PreprocessSuffix)
	$(RM) $(OutputFile)
	$(RM) "/home/stefano/Documents/developer/Hosanna/Hosanna_IDE/.build-debug/databaselayer"


