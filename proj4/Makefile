CXX = g++

OBJ = ./obj
BIN = ./bin
INC = ./include
SRC = ./src
TESTSRC = ./testsrc

CXXFLAGS = -std=c++17 -I $(INC) -I /opt/local/include
LDFLAGS = -L /opt/local/lib -lgtest -lgtest_main -lpthread -lexpat 

all: create_dirs run

run: $(BIN)/teststrutils $(BIN)/teststrdatasource $(BIN)/teststrdatasink $(BIN)/testdsv $(BIN)/testxml $(BIN)/testcsvb  $(BIN)/testosm
	$(BIN)/teststrutils && $(BIN)/teststrdatasource && $(BIN)/teststrdatasink && $(BIN)/testdsv && $(BIN)/testxml && $(BIN)/testcsvb && $(BIN)/testosm

# Executables
$(BIN)/testcsvb: $(OBJ)/CSVBusSystem.o $(OBJ)/CSVBusSystemTest.o $(OBJ)/DSV_Reader.o $(OBJ)/StrUtils.o $(OBJ)/StrDataSource.o
	$(CXX) -o $(BIN)/testcsvb $(OBJ)/CSVBusSystem.o $(OBJ)/CSVBusSystemTest.o $(OBJ)/DSV_Reader.o $(OBJ)/StrUtils.o $(OBJ)/StrDataSource.o $(LDFLAGS)

$(BIN)/testosm: $(OBJ)/OpenStreetMap.o $(OBJ)/OSMTest.o $(OBJ)/XML_Reader.o $(OBJ)/StrDataSource.o $(OBJ)/StrUtils.o
	$(CXX) -o $(BIN)/testosm $(OBJ)/OpenStreetMap.o $(OBJ)/OSMTest.o $(OBJ)/StrDataSource.o $(OBJ)/XML_Reader.o $(OBJ)/StrUtils.o $(LDFLAGS)

$(BIN)/testdsv: $(OBJ)/DSV_Reader.o $(OBJ)/DSV_Writer.o $(OBJ)/DSV_Test.o $(OBJ)/StrUtils.o $(OBJ)/StrDataSink.o $(OBJ)/StrDataSource.o
	$(CXX) -o $(BIN)/testdsv $(OBJ)/DSV_Reader.o $(OBJ)/DSV_Writer.o $(OBJ)/DSV_Test.o $(OBJ)/StrUtils.o $(OBJ)/StrDataSink.o $(OBJ)/StrDataSource.o $(LDFLAGS)

$(BIN)/testxml: $(OBJ)/XML_Reader.o $(OBJ)/XML_Writer.o $(OBJ)/XML_Test.o $(OBJ)/StrDataSink.o $(OBJ)/StrDataSource.o $(OBJ)/StrUtils.o
	$(CXX) -o $(BIN)/testxml $(OBJ)/XML_Reader.o $(OBJ)/XML_Writer.o $(OBJ)/XML_Test.o $(OBJ)/StrUtils.o $(OBJ)/StrDataSink.o $(OBJ)/StrDataSource.o $(LDFLAGS)

$(BIN)/teststrutils: $(OBJ)/TestStrUtils.o $(OBJ)/StrUtils.o
	$(CXX) -o $(BIN)/teststrutils $(OBJ)/TestStrUtils.o $(OBJ)/StrUtils.o $(LDFLAGS)

$(BIN)/teststrdatasource: $(OBJ)/StrDataSource.o $(OBJ)/StrDataSourceTest.o
	$(CXX) -o $(BIN)/teststrdatasource $(OBJ)/StrDataSource.o $(OBJ)/StrDataSourceTest.o $(LDFLAGS)

$(BIN)/teststrdatasink: $(OBJ)/StrDataSink.o $(OBJ)/StrDataSinkTest.o
	$(CXX) -o $(BIN)/teststrdatasink $(OBJ)/StrDataSink.o $(OBJ)/StrDataSinkTest.o $(LDFLAGS)

# DSV/XML Reader & Writer Object Files: Takes in src & header files
$(OBJ)/CSVBusSystem.o: $(SRC)/CSVBusSystem.cpp $(INC)/CSVBusSystem.h $(INC)/BusSystem.h $(INC)/DSVReader.h ${INC}/StringUtils.h
	$(CXX) -o $(OBJ)/CSVBusSystem.o $(CXXFLAGS) -c $(SRC)/CSVBusSystem.cpp

$(OBJ)/OpenStreetMap.o: $(SRC)/OpenStreetMap.cpp $(INC)/OpenStreetMap.h $(INC)/StreetMap.h $(INC)/XMLReader.h
	$(CXX) -o $(OBJ)/OpenStreetMap.o $(CXXFLAGS) -c $(SRC)/OpenStreetMap.cpp

$(OBJ)/DSV_Reader.o: $(SRC)/DSVReader.cpp $(INC)/DSVReader.h $(INC)/DataSource.h $(INC)/StringUtils.h
	$(CXX) -o $(OBJ)/DSV_Reader.o $(CXXFLAGS) -c $(SRC)/DSVReader.cpp

$(OBJ)/DSV_Writer.o: $(SRC)/DSVWriter.cpp $(INC)/DSVWriter.h  $(INC)/DataSink.h $(INC)/StringUtils.h
	$(CXX) -o $(OBJ)/DSV_Writer.o $(CXXFLAGS) -c $(SRC)/DSVWriter.cpp

$(OBJ)/XML_Reader.o: $(SRC)/XMLReader.cpp $(INC)/DataSource.h $(INC)/XMLReader.h $(INC)/XMLEntity.h
	$(CXX) -o $(OBJ)/XML_Reader.o $(CXXFLAGS) -c $(SRC)/XMLReader.cpp

$(OBJ)/XML_Writer.o: $(SRC)/XMLWriter.cpp $(INC)/DataSink.h $(INC)/XMLWriter.h $(INC)/XMLEntity.h $(INC)/StringUtils.h
	$(CXX) -o $(OBJ)/XML_Writer.o $(CXXFLAGS) -c $(SRC)/XMLWriter.cpp

# DSV & XML Tests: Takes in testsrc & header files
$(OBJ)/CSVBusSystemTest.o: $(TESTSRC)/CSVBusSystemTest.cpp $(INC)/CSVBusSystem.h $(INC)/BusSystem.h $(INC)/DSVReader.h $(INC)/StringDataSource.h
	$(CXX) -o $(OBJ)/CSVBusSystemTest.o $(CXXFLAGS) -c $(TESTSRC)/CSVBusSystemTest.cpp

$(OBJ)/OSMTest.o: $(TESTSRC)/OSMTest.cpp $(INC)/OpenStreetMap.h $(INC)/StreetMap.h $(INC)/XMLReader.h $(INC)/StringDataSource.h
	$(CXX) -o $(OBJ)/OSMTest.o $(CXXFLAGS) -c $(TESTSRC)/OSMTest.cpp

$(OBJ)/DSV_Test.o: $(TESTSRC)/DSVTest.cpp $(INC)/DSVWriter.h $(INC)/DSVReader.h $(INC)/DataSink.h $(INC)/DataSource.h
	$(CXX) -o $(OBJ)/DSV_Test.o $(CXXFLAGS) -c $(TESTSRC)/DSVTest.cpp

$(OBJ)/XML_Test.o: $(TESTSRC)/XMLTest.cpp $(INC)/XMLWriter.h $(INC)/XMLReader.h $(INC)/XMLEntity.h $(INC)/DataSink.h $(INC)/DataSource.h
	$(CXX) -o $(OBJ)/XML_Test.o $(CXXFLAGS) -c $(TESTSRC)/XMLTest.cpp

# StrUtils, StrDataSource, & StrDataSink object & test obj files
$(OBJ)/StrUtils.o: $(SRC)/StringUtils.cpp $(INC)/StringUtils.h
	$(CXX) -o $(OBJ)/StrUtils.o $(CXXFLAGS) -c $(SRC)/StringUtils.cpp

$(OBJ)/TestStrUtils.o: $(TESTSRC)/StringUtilsTest.cpp $(INC)/StringUtils.h
	$(CXX) -o $(OBJ)/TestStrUtils.o $(CXXFLAGS) -c $(TESTSRC)/StringUtilsTest.cpp

$(OBJ)/StrDataSource.o: $(INC)/DataSource.h $(INC)/StringDataSource.h $(SRC)/StringDataSource.cpp
	$(CXX) -o $(OBJ)/StrDataSource.o $(CXXFLAGS) -c $(SRC)/StringDataSource.cpp

$(OBJ)/StrDataSourceTest.o: $(INC)/DataSource.h $(INC)/StringDataSource.h $(TESTSRC)/StringDataSourceTest.cpp
	$(CXX) -o $(OBJ)/StrDataSourceTest.o $(CXXFLAGS) -c $(TESTSRC)/StringDataSourceTest.cpp

$(OBJ)/StrDataSink.o: $(INC)/DataSink.h $(INC)/StringDataSink.h $(SRC)/StringDataSink.cpp
	$(CXX) -o $(OBJ)/StrDataSink.o $(CXXFLAGS) -c $(SRC)/StringDataSink.cpp

$(OBJ)/StrDataSinkTest.o: $(INC)/DataSink.h $(INC)/StringDataSink.h $(TESTSRC)/StringDataSinkTest.cpp
	$(CXX) -o $(OBJ)/StrDataSinkTest.o $(CXXFLAGS) -c $(TESTSRC)/StringDataSinkTest.cpp

create_dirs:
	mkdir -p $(BIN)
	mkdir -p $(OBJ)

.PHONY: clean

clean:
	rm -rf $(BIN) $(OBJ)