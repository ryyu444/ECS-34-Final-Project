#include "TransportationPlannerCommandLine.h"
#include "TransportationPlannerConfig.h"
#include "StringUtils.h"
#include "StringDataSink.h"
#include "StringDataSource.h"
#include "FileDataFactory.h"
#include "DSVReader.h"
#include "XMLReader.h"
#include "CSVBusSystem.h"
#include "OpenStreetMap.h"
#include "DijkstraTransportationPlanner.h"


#include <iostream>

int main(int argc, char *argv[]) {
    
    std::vector<std::string> args;
    for (int i = 1; i < argc; i++) {
        args.push_back(argv[i]);
    }

    std::string dataPath = "./data/";    
    std::string resultsPath = "./results/";    
    // check for valid flag arguments before going into main loop and stores data and results paths
    for(int i = 0; i < args.size(); i++) {
        std::string tmp1 = StringUtils::Slice(args[i], 0, 7);
        std::string tmp2 = StringUtils::Slice(args[i], 0, 10);
        if (tmp1 != "--data=" && tmp2 != "--results=") {
                std::cerr << "Syntax Error: transplanner [--data=path | --results=path]" << std::endl;
                return EXIT_FAILURE;
        }
        else if (tmp1 == "--data=") {
            dataPath = StringUtils::Slice(args[i], 7);
            std::cout << "dataPath: " << dataPath << std::endl;
        }
        else if (tmp2 == "--results=") {
            resultsPath = StringUtils::Slice(args[i], 10);
            std::cout << "resultsPath: " << resultsPath << std::endl;
        }
    }

    // read in commands until 'exit' called
    // parse and process commands
    std::string input;
    while(true) {

        std::cout << "> ";

        getline(std::cin, input);
        input += "\n";

        // create command source
        auto cmdSrc = std::make_shared<CStringDataSource>(input);
        auto outSink = std::make_shared<CStringDataSink>();
        auto errSink = std::make_shared<CStringDataSink>();
        auto results = std::make_shared<CFileDataFactory>(resultsPath);
        auto data = std::make_shared<CFileDataFactory>(dataPath);
        auto stopReader = std::make_shared<CDSVReader>(data->CreateSource("stops.csv"), ',');
        auto routeReader = std::make_shared<CDSVReader>(data->CreateSource("routes.csv"), ',');
        auto busSystem = std::make_shared<CCSVBusSystem>(stopReader, routeReader);
        auto osmReader = std::make_shared<CXMLReader>(data->CreateSource("city.osm"));
        auto streetMap = std::make_shared<COpenStreetMap>(osmReader);
        auto config = std::make_shared<STransportationPlannerConfig>(streetMap, busSystem);
        auto planner = std::make_shared<CDijkstraTransportationPlanner>(config);
        CTransportationPlannerCommandLine cmdLine(cmdSrc, outSink, errSink, results, planner);

        if(cmdLine.ProcessCommands()) {
            return EXIT_SUCCESS;
        }

    }
    

}