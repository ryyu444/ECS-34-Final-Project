CXX = g++

CXX_FLAGS = -std=gnu++17 -I $(INC_DIR) -I $(GTEST_HEADER_DIR) -I ${LEXPAT_HEADER_DIR}
LDFLAGS = -L $(GTEST_LIB_DIR) -lgtest -lgtest_main -lpthread
XMLFLAGS = -L ${GTEST_LIB_DIR} -L ${LEXPAT_LIB_DIR} -lgtest -lgtest_main -lpthread -lexpat

GTEST_HEADER_DIR = ../../googletest/googletest/include
# ./opt/local/include
GTEST_LIB_DIR = ../../googletest/googletest/lib
# ./opt/local/include
LEXPAT_HEADER_DIR = ../../../../../usr/local/include
LEXPAT_LIB_DIR = ../../../../../usr/local/lib
OBJ_DIR = ./obj
BIN_DIR = ./bin
INC_DIR = ./include
SRC_DIR = ./src
TESTSRC_DIR = ./testsrc

all: create_dirs run

run: $(BIN_DIR)/teststrutils $(BIN_DIR)/teststrdatasource $(BIN_DIR)/teststrdatasink $(BIN_DIR)/testdsv $(BIN_DIR)/testxml ${BIN_DIR}/testcsvbs ${BIN_DIR}/testosm
	$(BIN_DIR)/teststrutils && $(BIN_DIR)/teststrdatasource && $(BIN_DIR)/teststrdatasink && $(BIN_DIR)/testdsv && $(BIN_DIR)/testxml && ${BIN_DIR}/testcsvbs && ${BIN_DIR}/testosm

# Executables for DSV, XML, STRUTILS, STRDATASRC, & STRDATASINK
$(BIN_DIR)/testcsvbs: $(OBJ_DIR)/CSVBusSystem.o $(OBJ_DIR)/CSVBusSystemTest.o $(OBJ_DIR)/DSV_Reader.o ${OBJ_DIR}/StrUtils.o ${OBJ_DIR}/StrDataSource.o
	$(CXX) -o $(BIN_DIR)/testcsvbs $(OBJ_DIR)/CSVBusSystem.o $(OBJ_DIR)/CSVBusSystemTest.o $(OBJ_DIR)/DSV_Reader.o ${OBJ_DIR}/StrUtils.o ${OBJ_DIR}/StrDataSource.o $(LDFLAGS)

$(BIN_DIR)/testosm: $(OBJ_DIR)/OpenStreetMap.o $(OBJ_DIR)/OpenStreetMapTest.o $(OBJ_DIR)/XML_Reader.o ${OBJ_DIR}/StrDataSource.o
	$(CXX) -o $(BIN_DIR)/testosm $(OBJ_DIR)/OpenStreetMap.o $(OBJ_DIR)/OpenStreetMapTest.o $(OBJ_DIR)/XML_Reader.o ${OBJ_DIR}/StrDataSource.o $(XMLFLAGS)

$(BIN_DIR)/testdsv: $(OBJ_DIR)/DSV_Reader.o $(OBJ_DIR)/DSV_Writer.o ${OBJ_DIR}/DSV_Test.o ${OBJ_DIR}/StrUtils.o ${OBJ_DIR}/StrDataSink.o ${OBJ_DIR}/StrDataSource.o
	$(CXX) -o $(BIN_DIR)/testdsv $(OBJ_DIR)/DSV_Reader.o $(OBJ_DIR)/DSV_Writer.o $(OBJ_DIR)/DSV_Test.o ${OBJ_DIR}/StrUtils.o ${OBJ_DIR}/StrDataSink.o ${OBJ_DIR}/StrDataSource.o $(LDFLAGS)

$(BIN_DIR)/testxml: $(OBJ_DIR)/XML_Reader.o $(OBJ_DIR)/XML_Writer.o ${OBJ_DIR}/XML_Test.o ${OBJ_DIR}/StrUtils.o ${OBJ_DIR}/StrDataSink.o ${OBJ_DIR}/StrDataSource.o
	$(CXX) -o $(BIN_DIR)/testxml $(OBJ_DIR)/XML_Reader.o $(OBJ_DIR)/XML_Writer.o $(OBJ_DIR)/XML_Test.o ${OBJ_DIR}/StrUtils.o ${OBJ_DIR}/StrDataSink.o ${OBJ_DIR}/StrDataSource.o $(XMLFLAGS)

$(BIN_DIR)/teststrutils: $(OBJ_DIR)/TestStrUtils.o $(OBJ_DIR)/StrUtils.o
	$(CXX) -o $(BIN_DIR)/teststrutils $(OBJ_DIR)/TestStrUtils.o $(OBJ_DIR)/StrUtils.o $(LDFLAGS)

$(BIN_DIR)/teststrdatasource: $(OBJ_DIR)/StrDataSource.o $(OBJ_DIR)/StrDataSourceTest.o ${OBJ_DIR}/StrUtils.o
	$(CXX) -o $(BIN_DIR)/teststrdatasource $(OBJ_DIR)/StrDataSource.o $(OBJ_DIR)/StrDataSourceTest.o ${OBJ_DIR}/StrUtils.o $(LDFLAGS)

$(BIN_DIR)/teststrdatasink: $(OBJ_DIR)/StrDataSink.o $(OBJ_DIR)/StrDataSinkTest.o ${OBJ_DIR}/StrUtils.o
	$(CXX) -o $(BIN_DIR)/teststrdatasink $(OBJ_DIR)/StrDataSink.o $(OBJ_DIR)/StrDataSinkTest.o ${OBJ_DIR}/StrUtils.o $(LDFLAGS)

# DSV/XML Reader & Writer Object Files: Takes in src & header files
$(OBJ_DIR)/CSVBusSystem.o: $(SRC_DIR)/CSVBusSystem.cpp $(INC_DIR)/CSVBusSystem.h $(INC_DIR)/BusSystem.h $(INC_DIR)/DSVReader.h ${INC_DIR}/DataSource.h ${INC_DIR}/StringUtils.h
	$(CXX) -o $(OBJ_DIR)/CSVBusSystem.o $(CXX_FLAGS) -c $(SRC_DIR)/CSVBusSystem.cpp

$(OBJ_DIR)/OpenStreetMap.o: $(SRC_DIR)/OpenStreetMap.cpp ${INC_DIR}/OpenStreetMap.h $(INC_DIR)/StreetMap.h $(INC_DIR)/XMLReader.h
	$(CXX) -o $(OBJ_DIR)/OpenStreetMap.o $(CXX_FLAGS) -c $(SRC_DIR)/OpenStreetMap.cpp

$(OBJ_DIR)/DSV_Reader.o: $(SRC_DIR)/DSVReader.cpp $(INC_DIR)/DataSource.h $(INC_DIR)/DSVReader.h ${INC_DIR}/StringUtils.h
	$(CXX) -o $(OBJ_DIR)/DSV_Reader.o $(CXX_FLAGS) -c $(SRC_DIR)/DSVReader.cpp

$(OBJ_DIR)/DSV_Writer.o: $(SRC_DIR)/DSVWriter.cpp $(INC_DIR)/DataSink.h $(INC_DIR)/DSVWriter.h ${INC_DIR}/StringUtils.h
	$(CXX) -o $(OBJ_DIR)/DSV_Writer.o $(CXX_FLAGS) -c $(SRC_DIR)/DSVWriter.cpp

$(OBJ_DIR)/XML_Reader.o: $(SRC_DIR)/XMLReader.cpp $(INC_DIR)/DataSource.h $(INC_DIR)/XMLReader.h $(INC_DIR)/XMLEntity.h ${INC_DIR}/StringUtils.h
	$(CXX) -o $(OBJ_DIR)/XML_Reader.o $(CXX_FLAGS) -c $(SRC_DIR)/XMLReader.cpp

$(OBJ_DIR)/XML_Writer.o: $(SRC_DIR)/XMLWriter.cpp $(INC_DIR)/DataSink.h $(INC_DIR)/XMLWriter.h $(INC_DIR)/XMLEntity.h ${INC_DIR}/StringUtils.h
	$(CXX) -o $(OBJ_DIR)/XML_Writer.o $(CXX_FLAGS) -c $(SRC_DIR)/XMLWriter.cpp

# DSV & XML Tests: Takes in testsrc & header files
$(OBJ_DIR)/CSVBusSystemTest.o: $(TESTSRC_DIR)/CSVBusSystemTest.cpp ${INC_DIR}/CSVBusSystem.h $(INC_DIR)/BusSystem.h $(INC_DIR)/DSVReader.h ${INC_DIR}/DataSource.h ${INC_DIR}/StringDataSource.h
	$(CXX) -o $(OBJ_DIR)/CSVBusSystemTest.o $(CXX_FLAGS) -c $(TESTSRC_DIR)/CSVBusSystemTest.cpp

$(OBJ_DIR)/OpenStreetMapTest.o: $(TESTSRC_DIR)/OpenStreetMapTest.cpp ${INC_DIR}/OpenStreetMap.h $(INC_DIR)/StreetMap.h $(INC_DIR)/XMLReader.h ${INC_DIR}/StringDataSource.h ${INC_DIR}/DataSource.h
	$(CXX) -o $(OBJ_DIR)/OpenStreetMapTest.o $(CXX_FLAGS) -c $(TESTSRC_DIR)/OpenStreetMapTest.cpp

${OBJ_DIR}/DSV_Test.o: ${TESTSRC_DIR}/DSVTest.cpp ${INC_DIR}/DSVWriter.h ${INC_DIR}/DSVReader.h ${INC_DIR}/DataSink.h ${INC_DIR}/DataSource.h ${INC_DIR}/StringDataSink.h ${INC_DIR}/StringDataSource.h
	${CXX} -o ${OBJ_DIR}/DSV_Test.o ${CXX_FLAGS} -c ${TESTSRC_DIR}/DSVTest.cpp

${OBJ_DIR}/XML_Test.o: ${TESTSRC_DIR}/XMLTest.cpp ${INC_DIR}/XMLWriter.h ${INC_DIR}/XMLReader.h ${INC_DIR}/XMLEntity.h ${INC_DIR}/DataSink.h ${INC_DIR}/DataSource.h ${INC_DIR}/StringDataSink.h ${INC_DIR}/StringDataSource.h
	${CXX} -o ${OBJ_DIR}/XML_Test.o ${CXX_FLAGS} -c ${TESTSRC_DIR}/XMLTest.cpp

# StrUtils, StrDataSource, & StrDataSink object & test obj files
$(OBJ_DIR)/StrUtils.o: $(SRC_DIR)/StringUtils.cpp $(INC_DIR)/StringUtils.h
	$(CXX) -o $(OBJ_DIR)/StrUtils.o $(CXX_FLAGS) -c $(SRC_DIR)/StringUtils.cpp

$(OBJ_DIR)/TestStrUtils.o: $(TESTSRC_DIR)/StringUtilsTest.cpp $(INC_DIR)/StringUtils.h
	$(CXX) -o $(OBJ_DIR)/TestStrUtils.o $(CXX_FLAGS) -c $(TESTSRC_DIR)/StringUtilsTest.cpp

$(OBJ_DIR)/StrDataSource.o: $(INC_DIR)/DataSource.h $(INC_DIR)/StringDataSource.h $(SRC_DIR)/StringDataSource.cpp
	$(CXX) -o $(OBJ_DIR)/StrDataSource.o $(CXX_FLAGS) -c $(SRC_DIR)/StringDataSource.cpp

$(OBJ_DIR)/StrDataSourceTest.o: $(INC_DIR)/DataSource.h $(INC_DIR)/StringDataSource.h $(TESTSRC_DIR)/StringDataSourceTest.cpp
	$(CXX) -o $(OBJ_DIR)/StrDataSourceTest.o $(CXX_FLAGS) -c $(TESTSRC_DIR)/StringDataSourceTest.cpp

$(OBJ_DIR)/StrDataSink.o: $(INC_DIR)/DataSink.h $(INC_DIR)/StringDataSink.h $(SRC_DIR)/StringDataSink.cpp
	$(CXX) -o $(OBJ_DIR)/StrDataSink.o $(CXX_FLAGS) -c $(SRC_DIR)/StringDataSink.cpp

$(OBJ_DIR)/StrDataSinkTest.o: $(INC_DIR)/DataSink.h $(INC_DIR)/StringDataSink.h $(TESTSRC_DIR)/StringDataSinkTest.cpp
	$(CXX) -o $(OBJ_DIR)/StrDataSinkTest.o $(CXX_FLAGS) -c $(TESTSRC_DIR)/StringDataSinkTest.cpp

create_dirs:
	mkdir -p $(BIN_DIR)
	mkdir -p $(OBJ_DIR)

.PHONY: clean

clean:
	rm -rf $(BIN_DIR) $(OBJ_DIR)