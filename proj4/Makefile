CXX = g++

OBJ = ./obj
BIN = ./bin
INC = ./include
SRC = ./src
TESTSRC = ./testsrc

CXXFLAGS = -std=c++17 -I $(INC) -I /opt/local/include
LDFLAGS = -L /opt/local/lib -lgtest -lgmock -lgtest_main -lpthread -lexpat

all: create_dirs run

run: $(BIN)/testStringUtils $(BIN)/testStringDataSource $(BIN)/testStringDataSink $(BIN)/testdsv $(BIN)/testxml $(BIN)/testcsvb $(BIN)/testosm $(BIN)/testdpr $(BIN)/testcsvbsi $(BIN)/testtp $(BIN)/testtpcl $(BIN)/testkml $(BIN)/testfiledatass ./transplanner ./speedtest ./kmlout
	$(BIN)/testStringUtils && $(BIN)/testStringDataSource && $(BIN)/testStringDataSink && $(BIN)/testdsv && $(BIN)/testxml && $(BIN)/testcsvb && $(BIN)/testosm && $(BIN)/testdpr && $(BIN)/testcsvbsi && $(BIN)/testtp && $(BIN)/testtpcl && $(BIN)/testkml && $(BIN)/testfiledatass 

# Executables
./kmlout: $(OBJ)/kmlout.o $(OBJ)/OpenStreetMap.o $(OBJ)/DSVReader.o $(OBJ)/DSVWriter.o $(OBJ)/FileDataFactory.o $(OBJ)/FileDataSource.o $(OBJ)/FileDataSink.o $(OBJ)/StandardDataSource.o $(OBJ)/StandardDataSink.o $(OBJ)/StandardErrorDataSink.o $(OBJ)/StringUtils.o $(OBJ)/KMLWriter.o $(OBJ)/XMLReader.o $(OBJ)/XMLWriter.o
	$(CXX) -o ./kmlout $(OBJ)/kmlout.o $(OBJ)/OpenStreetMap.o $(OBJ)/DSVReader.o $(OBJ)/DSVWriter.o $(OBJ)/FileDataFactory.o $(OBJ)/FileDataSource.o $(OBJ)/FileDataSink.o $(OBJ)/StandardDataSource.o $(OBJ)/StandardDataSink.o $(OBJ)/StandardErrorDataSink.o $(OBJ)/StringUtils.o $(OBJ)/KMLWriter.o $(OBJ)/XMLReader.o $(OBJ)/XMLWriter.o $(LDFLAGS)

./transplanner: $(OBJ)/transplanner.o $(OBJ)/DijkstraTransportationPlanner.o $(OBJ)/DijkstraPathRouter.o $(OBJ)/GeographicUtils.o $(OBJ)/StringUtils.o $(OBJ)/BusSystemIndexer.o $(OBJ)/TransportationPlannerCommandLine.o $(OBJ)/StringDataSink.o $(OBJ)/StringDataSource.o $(OBJ)/XMLReader.o $(OBJ)/DSVReader.o $(OBJ)/FileDataFactory.o $(OBJ)/FileDataSink.o $(OBJ)/FileDataSource.o $(OBJ)/OpenStreetMap.o $(OBJ)/CSVBusSystem.o
	$(CXX) -o ./transplanner $(OBJ)/transplanner.o $(OBJ)/DijkstraTransportationPlanner.o $(OBJ)/DijkstraPathRouter.o $(OBJ)/GeographicUtils.o $(OBJ)/StringUtils.o $(OBJ)/BusSystemIndexer.o $(OBJ)/TransportationPlannerCommandLine.o $(OBJ)/StringDataSink.o $(OBJ)/StringDataSource.o $(OBJ)/XMLReader.o $(OBJ)/DSVReader.o $(OBJ)/FileDataFactory.o $(OBJ)/FileDataSink.o $(OBJ)/FileDataSource.o $(OBJ)/OpenStreetMap.o $(OBJ)/CSVBusSystem.o $(LDFLAGS)
	
./speedtest: $(OBJ)/speedtest.o $(OBJ)/DijkstraTransportationPlanner.o $(OBJ)/DijkstraPathRouter.o $(OBJ)/OpenStreetMap.o $(OBJ)/CSVBusSystem.o $(OBJ)/FileDataFactory.o $(OBJ)/StandardDataSource.o $(OBJ)/StandardDataSink.o $(OBJ)/StandardErrorDataSink.o $(OBJ)/StringUtils.o $(OBJ)/DSVReader.o $(OBJ)/XMLReader.o $(OBJ)/GeographicUtils.o $(OBJ)/BusSystemIndexer.o $(OBJ)/FileDataSink.o $(OBJ)/FileDataSource.o 
	$(CXX) -o ./speedtest $(OBJ)/speedtest.o $(OBJ)/DijkstraTransportationPlanner.o $(OBJ)/DijkstraPathRouter.o $(OBJ)/OpenStreetMap.o $(OBJ)/CSVBusSystem.o $(OBJ)/FileDataFactory.o $(OBJ)/StandardDataSource.o $(OBJ)/StandardDataSink.o $(OBJ)/StandardErrorDataSink.o $(OBJ)/StringUtils.o $(OBJ)/DSVReader.o $(OBJ)/XMLReader.o $(OBJ)/GeographicUtils.o $(OBJ)/BusSystemIndexer.o $(OBJ)/FileDataSink.o $(OBJ)/FileDataSource.o $(LDFLAGS)

$(BIN)/testkml: $(OBJ)/KMLWriter.o $(OBJ)/KMLTest.o $(OBJ)/XMLWriter.o $(OBJ)/StringDataSink.o $(OBJ)/StringDataSource.o $(OBJ)/StringUtils.o
	$(CXX) -o $(BIN)/testkml $(OBJ)/KMLWriter.o $(OBJ)/KMLTest.o $(OBJ)/XMLWriter.o $(OBJ)/StringDataSink.o $(OBJ)/StringDataSource.o $(OBJ)/StringUtils.o $(LDFLAGS)

# test file dat ass
$(BIN)/testfiledatass: $(OBJ)/FileDataSink.o $(OBJ)/FileDataSource.o $(OBJ)/FileDataFactory.o $(OBJ)/FileDataSSTest.o
	$(CXX) -o $(BIN)/testfiledatass $(OBJ)/FileDataSink.o $(OBJ)/FileDataSource.o $(OBJ)/FileDataFactory.o $(OBJ)/FileDataSSTest.o $(LDFLAGS)

$(BIN)/testdpr: $(OBJ)/DijkstraPathRouter.o $(OBJ)/DijkstraPathRouterTest.o
	$(CXX) -o $(BIN)/testdpr $(OBJ)/DijkstraPathRouter.o $(OBJ)/DijkstraPathRouterTest.o $(LDFLAGS)

$(BIN)/testcsvbsi: $(OBJ)/BusSystemIndexer.o $(OBJ)/CSVBusSystemIndexerTest.o $(OBJ)/StringDataSource.o $(OBJ)/DSVReader.o $(OBJ)/StringUtils.o $(OBJ)/XMLReader.o $(OBJ)/CSVBusSystem.o
	$(CXX) -o $(BIN)/testcsvbsi $(OBJ)/BusSystemIndexer.o $(OBJ)/CSVBusSystemIndexerTest.o $(OBJ)/StringDataSource.o $(OBJ)/DSVReader.o $(OBJ)/StringUtils.o $(OBJ)/XMLReader.o $(OBJ)/CSVBusSystem.o $(LDFLAGS)
 
$(BIN)/testtp: $(OBJ)/CSVOSMTransportationPlannerTest.o $(OBJ)/DijkstraTransportationPlanner.o $(OBJ)/DijkstraPathRouter.o $(OBJ)/BusSystemIndexer.o $(OBJ)/OpenStreetMap.o $(OBJ)/CSVBusSystem.o $(OBJ)/StringDataSource.o $(OBJ)/StringUtils.o $(OBJ)/XMLReader.o $(OBJ)/DSVReader.o $(OBJ)/GeographicUtils.o
	$(CXX) -o $(BIN)/testtp $(OBJ)/CSVOSMTransportationPlannerTest.o $(OBJ)/DijkstraTransportationPlanner.o $(OBJ)/DijkstraPathRouter.o $(OBJ)/BusSystemIndexer.o $(OBJ)/OpenStreetMap.o $(OBJ)/CSVBusSystem.o $(OBJ)/StringDataSource.o $(OBJ)/StringUtils.o $(OBJ)/XMLReader.o $(OBJ)/DSVReader.o $(OBJ)/GeographicUtils.o $(LDFLAGS)

$(BIN)/testtpcl: $(OBJ)/TransportationPlannerCommandLine.o $(OBJ)/TPCommandLineTest.o $(OBJ)/StringDataSink.o $(OBJ)/StringDataSource.o $(OBJ)/StringUtils.o $(OBJ)/GeographicUtils.o
	$(CXX) -o $(BIN)/testtpcl $(OBJ)/TransportationPlannerCommandLine.o $(OBJ)/TPCommandLineTest.o $(OBJ)/StringDataSink.o $(OBJ)/StringDataSource.o $(OBJ)/StringUtils.o $(OBJ)/GeographicUtils.o $(LDFLAGS)

$(BIN)/testcsvb: $(OBJ)/CSVBusSystem.o $(OBJ)/CSVBusSystemTest.o $(OBJ)/DSVReader.o $(OBJ)/StringUtils.o $(OBJ)/StringDataSource.o
	$(CXX) -o $(BIN)/testcsvb $(OBJ)/CSVBusSystem.o $(OBJ)/CSVBusSystemTest.o $(OBJ)/DSVReader.o $(OBJ)/StringUtils.o $(OBJ)/StringDataSource.o $(LDFLAGS)

$(BIN)/testosm: $(OBJ)/OpenStreetMap.o $(OBJ)/OSMTest.o $(OBJ)/XMLReader.o $(OBJ)/StringDataSource.o $(OBJ)/StringUtils.o
	$(CXX) -o $(BIN)/testosm $(OBJ)/OpenStreetMap.o $(OBJ)/OSMTest.o $(OBJ)/StringDataSource.o $(OBJ)/XMLReader.o $(OBJ)/StringUtils.o $(LDFLAGS)

$(BIN)/testdsv: $(OBJ)/DSVReader.o $(OBJ)/DSVWriter.o $(OBJ)/DSVTest.o $(OBJ)/StringUtils.o $(OBJ)/StringDataSink.o $(OBJ)/StringDataSource.o
	$(CXX) -o $(BIN)/testdsv $(OBJ)/DSVReader.o $(OBJ)/DSVWriter.o $(OBJ)/DSVTest.o $(OBJ)/StringUtils.o $(OBJ)/StringDataSink.o $(OBJ)/StringDataSource.o $(LDFLAGS)

$(BIN)/testxml: $(OBJ)/XMLReader.o $(OBJ)/XMLWriter.o $(OBJ)/XMLTest.o $(OBJ)/StringDataSink.o $(OBJ)/StringDataSource.o $(OBJ)/StringUtils.o
	$(CXX) -o $(BIN)/testxml $(OBJ)/XMLReader.o $(OBJ)/XMLWriter.o $(OBJ)/XMLTest.o $(OBJ)/StringUtils.o $(OBJ)/StringDataSink.o $(OBJ)/StringDataSource.o $(LDFLAGS)

$(BIN)/testStringUtils: $(OBJ)/TestStringUtils.o $(OBJ)/StringUtils.o
	$(CXX) -o $(BIN)/testStringUtils $(OBJ)/TestStringUtils.o $(OBJ)/StringUtils.o $(LDFLAGS)

$(BIN)/testStringDataSource: $(OBJ)/StringDataSource.o $(OBJ)/StringDataSourceTest.o
	$(CXX) -o $(BIN)/testStringDataSource $(OBJ)/StringDataSource.o $(OBJ)/StringDataSourceTest.o $(LDFLAGS)

$(BIN)/testStringDataSink: $(OBJ)/StringDataSink.o $(OBJ)/StringDataSinkTest.o
	$(CXX) -o $(BIN)/testStringDataSink $(OBJ)/StringDataSink.o $(OBJ)/StringDataSinkTest.o $(LDFLAGS)

# DSV/XML Reader & Writer Object Files: Takes in src & header files
$(OBJ)/DijkstraPathRouter.o: $(SRC)/DijkstraPathRouter.cpp $(INC)/DijkstraPathRouter.h $(INC)/PathRouter.h
	$(CXX) -o $(OBJ)/DijkstraPathRouter.o $(CXXFLAGS) -c $(SRC)/DijkstraPathRouter.cpp

$(OBJ)/BusSystemIndexer.o: $(SRC)/BusSystemIndexer.cpp $(INC)/BusSystemIndexer.h $(INC)/BusSystem.h
	$(CXX) -o $(OBJ)/BusSystemIndexer.o $(CXXFLAGS) -c $(SRC)/BusSystemIndexer.cpp

$(OBJ)/DijkstraTransportationPlanner.o: $(SRC)/DijkstraTransportationPlanner.cpp $(INC)/DijkstraTransportationPlanner.h $(INC)/TransportationPlanner.h $(INC)/BusSystemIndexer.h $(INC)/DijkstraPathRouter.h $(INC)/GeographicUtils.h
	$(CXX) -o $(OBJ)/DijkstraTransportationPlanner.o $(CXXFLAGS) -c $(SRC)/DijkstraTransportationPlanner.cpp

$(OBJ)/GeographicUtils.o: $(SRC)/GeographicUtils.cpp $(INC)/GeographicUtils.h $(INC)/StreetMap.h
	$(CXX) -o $(OBJ)/GeographicUtils.o $(CXXFLAGS) -c $(SRC)/GeographicUtils.cpp

$(OBJ)/TransportationPlannerCommandLine.o: $(SRC)/TransportationPlannerCommandLine.cpp $(INC)/TransportationPlannerCommandLine.h $(INC)/TransportationPlanner.h $(INC)/DataFactory.h $(INC)/StreetMap.h $(INC)/BusSystem.h $(INC)/PathRouter.h $(INC)/DataSink.h $(INC)/DataSource.h $(INC)/GeographicUtils.h
	$(CXX) -o $(OBJ)/TransportationPlannerCommandLine.o $(CXXFLAGS) -c $(SRC)/TransportationPlannerCommandLine.cpp

# missing still: StandardDataSink, StandardDataSource, StandardErrorDataSink, transplanner, KMLWriter, kmlout, speedtest, 

$(OBJ)/StandardDataSink.o: $(SRC)/StandardDataSink.cpp $(INC)/StandardDataSink.h $(INC)/DataSink.h
	$(CXX) -o $(OBJ)/StandardDataSink.o $(CXXFLAGS) -c $(SRC)/StandardDataSink.cpp

$(OBJ)/StandardDataSource.o: $(SRC)/StandardDataSource.cpp $(INC)/StandardDataSource.h $(INC)/DataSource.h
	$(CXX) -o $(OBJ)/StandardDataSource.o $(CXXFLAGS) -c $(SRC)/StandardDataSource.cpp

$(OBJ)/StandardErrorDataSink.o: $(SRC)/StandardErrorDataSink.cpp $(INC)/StandardErrorDataSink.h $(INC)/DataSink.h
	$(CXX) -o $(OBJ)/StandardErrorDataSink.o $(CXXFLAGS) -c $(SRC)/StandardErrorDataSink.cpp

$(OBJ)/transplanner.o: $(SRC)/transplanner.cpp $(INC)/TransportationPlannerCommandLine.h $(INC)/TransportationPlannerConfig.h $(INC)/StringUtils.h $(INC)/StringDataSink.h $(INC)/StringDataSource.h $(INC)/FileDataFactory.h $(INC)/DSVReader.h $(INC)/XMLReader.h $(INC)/CSVBusSystem.h $(INC)/OpenStreetMap.h $(INC)/DijkstraTransportationPlanner.h
	$(CXX) -o $(OBJ)/transplanner.o $(CXXFLAGS) -c $(SRC)/transplanner.cpp

$(OBJ)/KMLWriter.o: $(SRC)/KMLWriter.cpp $(INC)/KMLWriter.h $(INC)/XMLWriter.h $(INC)/StringUtils.h $(INC)/DataSink.h $(INC)/StreetMap.h $(INC)/XMLEntity.h
	$(CXX) -o $(OBJ)/KMLWriter.o $(CXXFLAGS) -c $(SRC)/KMLWriter.cpp

$(OBJ)/kmlout.o: $(SRC)/kmlout.cpp $(INC)/OpenStreetMap.h $(INC)/BusSystem.h $(INC)/DSVReader.h $(INC)/DSVWriter.h $(INC)/FileDataFactory.h $(INC)/FileDataSource.h $(INC)/FileDataSink.h $(INC)/StandardDataSource.h $(INC)/StandardDataSink.h $(INC)/StandardErrorDataSink.h $(INC)/StringUtils.h $(INC)/KMLWriter.h
	$(CXX) -o $(OBJ)/kmlout.o $(CXXFLAGS) -c $(SRC)/kmlout.cpp

$(OBJ)/speedtest.o: $(SRC)/speedtest.cpp $(INC)/TransportationPlannerConfig.h $(INC)/DijkstraTransportationPlanner.h $(INC)/OpenStreetMap.h $(INC)/CSVBusSystem.h $(INC)/FileDataFactory.h $(INC)/StandardDataSource.h $(INC)/StandardDataSink.h $(INC)/StandardErrorDataSink.h $(INC)/StringUtils.h
	$(CXX) -o $(OBJ)/speedtest.o $(CXXFLAGS) -c $(SRC)/speedtest.cpp

$(OBJ)/FileDataFactory.o: $(SRC)/FileDataFactory.cpp $(INC)/FileDataFactory.h $(INC)/FileDataSource.h $(INC)/FileDataSink.h
	$(CXX) -o $(OBJ)/FileDataFactory.o $(CXXFLAGS) -c $(SRC)/FileDataFactory.cpp
 
$(OBJ)/FileDataSink.o: $(SRC)/FileDataSink.cpp $(INC)/FileDataSink.h
	$(CXX) -o $(OBJ)/FileDataSink.o $(CXXFLAGS) -c $(SRC)/FileDataSink.cpp

$(OBJ)/FileDataSource.o: $(SRC)/FileDataSource.cpp $(INC)/FileDataSource.h
	$(CXX) -o $(OBJ)/FileDataSource.o $(CXXFLAGS) -c $(SRC)/FileDataSource.cpp

$(OBJ)/CSVBusSystem.o: $(SRC)/CSVBusSystem.cpp $(INC)/CSVBusSystem.h $(INC)/BusSystem.h $(INC)/DSVReader.h ${INC}/StringUtils.h
	$(CXX) -o $(OBJ)/CSVBusSystem.o $(CXXFLAGS) -c $(SRC)/CSVBusSystem.cpp

$(OBJ)/OpenStreetMap.o: $(SRC)/OpenStreetMap.cpp $(INC)/OpenStreetMap.h $(INC)/StreetMap.h $(INC)/XMLReader.h
	$(CXX) -o $(OBJ)/OpenStreetMap.o $(CXXFLAGS) -c $(SRC)/OpenStreetMap.cpp

$(OBJ)/DSVReader.o: $(SRC)/DSVReader.cpp $(INC)/DSVReader.h $(INC)/DataSource.h $(INC)/StringUtils.h
	$(CXX) -o $(OBJ)/DSVReader.o $(CXXFLAGS) -c $(SRC)/DSVReader.cpp

$(OBJ)/DSVWriter.o: $(SRC)/DSVWriter.cpp $(INC)/DSVWriter.h  $(INC)/DataSink.h $(INC)/StringUtils.h
	$(CXX) -o $(OBJ)/DSVWriter.o $(CXXFLAGS) -c $(SRC)/DSVWriter.cpp

$(OBJ)/XMLReader.o: $(SRC)/XMLReader.cpp $(INC)/DataSource.h $(INC)/XMLReader.h $(INC)/XMLEntity.h
	$(CXX) -o $(OBJ)/XMLReader.o $(CXXFLAGS) -c $(SRC)/XMLReader.cpp

$(OBJ)/XMLWriter.o: $(SRC)/XMLWriter.cpp $(INC)/DataSink.h $(INC)/XMLWriter.h $(INC)/XMLEntity.h $(INC)/StringUtils.h
	$(CXX) -o $(OBJ)/XMLWriter.o $(CXXFLAGS) -c $(SRC)/XMLWriter.cpp

# DSV & XML Tests: Takes in testsrc & header files
$(OBJ)/KMLTest.o: $(TESTSRC)/KMLTest.cpp $(INC)/KMLWriter.h $(INC)/StringUtils.h $(INC)/StringDataSource.h $(INC)/StringDataSink.h $(INC)/DataSink.h $(INC)/DataSource.h $(INC)/StreetMap.h
	$(CXX) -o $(OBJ)/KMLTest.o $(CXXFLAGS) -c $(TESTSRC)/KMLTest.cpp

$(OBJ)/FileDataSSTest.o: $(TESTSRC)/FileDataSSTest.cpp $(INC)/FileDataFactory.h $(INC)/FileDataSink.h $(INC)/FileDataSource.h $(INC)/DataFactory.h $(INC)/DataSink.h $(INC)/DataSource.h
	$(CXX) -o $(OBJ)/FileDataSSTest.o $(CXXFLAGS) -c $(TESTSRC)/FileDataSSTest.cpp

$(OBJ)/DijkstraPathRouterTest.o: $(TESTSRC)/DijkstraPathRouterTest.cpp $(INC)/DijkstraPathRouter.h $(INC)/PathRouter.h
	$(CXX) -o $(OBJ)/DijkstraPathRouterTest.o $(CXXFLAGS) -c $(TESTSRC)/DijkstraPathRouterTest.cpp

$(OBJ)/CSVBusSystemIndexerTest.o: $(TESTSRC)/CSVBusSystemIndexerTest.cpp $(INC)/BusSystemIndexer.h $(INC)/BusSystem.h $(INC)/DSVReader.h $(INC)/StringDataSource.h $(INC)/StringUtils.h $(INC)/XMLReader.h $(INC)/CSVBusSystem.h $(INC)/DataSource.h
	$(CXX) -o $(OBJ)/CSVBusSystemIndexerTest.o $(CXXFLAGS) -c $(TESTSRC)/CSVBusSystemIndexerTest.cpp

$(OBJ)/CSVOSMTransportationPlannerTest.o: $(TESTSRC)/CSVOSMTransportationPlannerTest.cpp $(INC)/XMLReader.h $(INC)/StringUtils.h $(INC)/StringDataSource.h $(INC)/OpenStreetMap.h $(INC)/CSVBusSystem.h $(INC)/TransportationPlannerConfig.h $(INC)/GeographicUtils.h
	$(CXX) -o $(OBJ)/CSVOSMTransportationPlannerTest.o $(CXXFLAGS) -c $(TESTSRC)/CSVOSMTransportationPlannerTest.cpp

$(OBJ)/TPCommandLineTest.o: $(TESTSRC)/TPCommandLineTest.cpp $(INC)/TransportationPlannerCommandLine.h $(INC)/StringDataSink.h $(INC)/StringDataSource.h $(INC)/DataFactory.h $(INC)/TransportationPlanner.h
	$(CXX) -o $(OBJ)/TPCommandLineTest.o $(CXXFLAGS) -c $(TESTSRC)/TPCommandLineTest.cpp

$(OBJ)/CSVBusSystemTest.o: $(TESTSRC)/CSVBusSystemTest.cpp $(INC)/CSVBusSystem.h $(INC)/BusSystem.h $(INC)/DSVReader.h $(INC)/StringDataSource.h
	$(CXX) -o $(OBJ)/CSVBusSystemTest.o $(CXXFLAGS) -c $(TESTSRC)/CSVBusSystemTest.cpp

$(OBJ)/OSMTest.o: $(TESTSRC)/OSMTest.cpp $(INC)/OpenStreetMap.h $(INC)/StreetMap.h $(INC)/XMLReader.h $(INC)/StringDataSource.h
	$(CXX) -o $(OBJ)/OSMTest.o $(CXXFLAGS) -c $(TESTSRC)/OSMTest.cpp

$(OBJ)/DSVTest.o: $(TESTSRC)/DSVTest.cpp $(INC)/DSVWriter.h $(INC)/DSVReader.h $(INC)/DataSink.h $(INC)/DataSource.h
	$(CXX) -o $(OBJ)/DSVTest.o $(CXXFLAGS) -c $(TESTSRC)/DSVTest.cpp

$(OBJ)/XMLTest.o: $(TESTSRC)/XMLTest.cpp $(INC)/XMLWriter.h $(INC)/XMLReader.h $(INC)/XMLEntity.h $(INC)/DataSink.h $(INC)/DataSource.h
	$(CXX) -o $(OBJ)/XMLTest.o $(CXXFLAGS) -c $(TESTSRC)/XMLTest.cpp

# StringUtils, StringDataSource, & StringDataSink object & test obj files
$(OBJ)/StringUtils.o: $(SRC)/StringUtils.cpp $(INC)/StringUtils.h
	$(CXX) -o $(OBJ)/StringUtils.o $(CXXFLAGS) -c $(SRC)/StringUtils.cpp

$(OBJ)/TestStringUtils.o: $(TESTSRC)/StringUtilsTest.cpp $(INC)/StringUtils.h
	$(CXX) -o $(OBJ)/TestStringUtils.o $(CXXFLAGS) -c $(TESTSRC)/StringUtilsTest.cpp

$(OBJ)/StringDataSource.o: $(INC)/DataSource.h $(INC)/StringDataSource.h $(SRC)/StringDataSource.cpp
	$(CXX) -o $(OBJ)/StringDataSource.o $(CXXFLAGS) -c $(SRC)/StringDataSource.cpp

$(OBJ)/StringDataSourceTest.o: $(INC)/DataSource.h $(INC)/StringDataSource.h $(TESTSRC)/StringDataSourceTest.cpp
	$(CXX) -o $(OBJ)/StringDataSourceTest.o $(CXXFLAGS) -c $(TESTSRC)/StringDataSourceTest.cpp

$(OBJ)/StringDataSink.o: $(INC)/DataSink.h $(INC)/StringDataSink.h $(SRC)/StringDataSink.cpp
	$(CXX) -o $(OBJ)/StringDataSink.o $(CXXFLAGS) -c $(SRC)/StringDataSink.cpp

$(OBJ)/StringDataSinkTest.o: $(INC)/DataSink.h $(INC)/StringDataSink.h $(TESTSRC)/StringDataSinkTest.cpp
	$(CXX) -o $(OBJ)/StringDataSinkTest.o $(CXXFLAGS) -c $(TESTSRC)/StringDataSinkTest.cpp

create_dirs:
	mkdir -p $(BIN)
	mkdir -p $(OBJ)
	mkdir -p results

.PHONY: clean

clean:
	rm -rf $(BIN) $(OBJ) results
	rm ./transplanner
	rm ./speedtest
	rm ./kmlout