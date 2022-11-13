#include "OpenStreetMap.h"
#include "BusSystem.h"
#include "DSVReader.h"
#include "DSVWriter.h"
#include "FileDataFactory.h"
#include "FileDataSource.h"
#include "FileDataSink.h"
#include "StandardDataSource.h"
#include "StandardDataSink.h"
#include "StandardErrorDataSink.h"
#include "StringUtils.h"
#include "KMLWriter.h"
#include <iostream>
#include <unordered_set>
#include <unordered_map>

class CArgumentParser{
    private:
        std::string DDataDirectory;
        std::string DResultsDirectory;
        std::vector<std::string> DFilenames;
        bool DArgumentsValid;

        void PrintSyntax() const;
    public:
        CArgumentParser(const std::vector<std::string> &args);

        bool ArgumentsValid() const;

        std::string DataDirectory() const;
        std::string ResultsDirectory() const;
        std::vector<std::string> Filenames() const;
};

using TNodeIDPair = std::pair<CStreetMap::TNodeID,CStreetMap::TNodeID>;
struct SNodeIDPairHasher{
    std::size_t operator()(const TNodeIDPair &nodes) const{
        return nodes.first ^ nodes.second;
    }
};

class CKMLTranslator{
    private:
        std::unordered_map<CStreetMap::TNodeID,CStreetMap::TLocation> DNodeIDToLocation;
        std::unordered_map<CStreetMap::TNodeID,CBusSystem::TStopID> DNodeIDToStopID;
        std::unordered_map<TNodeIDPair,std::vector<CStreetMap::TLocation>,SNodeIDPairHasher> DBusSegmentToLocations;

        std::vector<std::pair<std::string,CStreetMap::TNodeID> > ParsePathFile(std::shared_ptr<CDSVReader> path);

    public:
        CKMLTranslator(std::shared_ptr<CStreetMap> map, std::shared_ptr<CDSVReader> stops, std::shared_ptr<CDSVReader> buspaths);

        bool TranslateFile(const std::string &filename);
};

int main(int argc, char *argv[]){
    std::vector<std::string> Arguments;
    const std::string OSMFilename = "city.osm";
    const std::string StopFilename = "stops.csv";
    const std::string BusPathFilename = "buspaths.csv";

    // Skip program name
    for(int Index = 1; Index < argc; Index++){
        Arguments.push_back(argv[Index]);
    }

    CArgumentParser Parser(Arguments);
    if(!Parser.ArgumentsValid()){
        return EXIT_FAILURE;
    }
    auto DataFactory = std::make_shared<CFileDataFactory>(Parser.DataDirectory());
    auto ResultsFactory = std::make_shared<CFileDataFactory>(Parser.ResultsDirectory());
    auto StdIn = std::make_shared<CStandardDataSource>();
    auto StdOut = std::make_shared<CStandardDataSink>();
    auto StdErr = std::make_shared<CStandardErrorDataSink>();
    auto StopReader = std::make_shared<CDSVReader>(DataFactory->CreateSource(StopFilename),',');
    auto BusPathReader = std::make_shared<CDSVReader>(DataFactory->CreateSource(BusPathFilename),',');
    auto XMLReader = std::make_shared<CXMLReader>(DataFactory->CreateSource(OSMFilename));
    auto StreetMap = std::make_shared<COpenStreetMap>(XMLReader);
    CKMLTranslator KMLTranslator(StreetMap,StopReader,BusPathReader);

    for(auto &Filename : Parser.Filenames()){
        KMLTranslator.TranslateFile(Filename);
    }

    return EXIT_SUCCESS;
}

CArgumentParser::CArgumentParser(const std::vector<std::string> &args){
    DDataDirectory = "./data";
    DResultsDirectory = "./results";
    DArgumentsValid = true;
    for(auto &Argument : args){
        if(Argument.find("--data") == 0){
            auto SplitArg = StringUtils::Split(Argument,"=");
            if(SplitArg.size() != 2 || SplitArg[0] != "--data"){
                DArgumentsValid = false;
                break;
            }
            DDataDirectory = SplitArg[1];
        }
        else if(Argument.find("--results") == 0){
            auto SplitArg = StringUtils::Split(Argument,"=");
            if(SplitArg.size() != 2 || SplitArg[0] != "--results"){
                DArgumentsValid = false;
                break;
            }
            DResultsDirectory = SplitArg[1];
        }
        else{
            DFilenames.push_back(Argument);
        }
    }
    DArgumentsValid = !DFilenames.empty();
    if(!DArgumentsValid){
        PrintSyntax();
    }
}

void CArgumentParser::PrintSyntax() const{
    std::cerr<<"Syntax Error: kmlout [--data=path | --results=path] file [file ...]"<<std::endl;
}

bool CArgumentParser::ArgumentsValid() const{
    return DArgumentsValid;
}

std::string CArgumentParser::DataDirectory() const{
    return DDataDirectory;
}

std::string CArgumentParser::ResultsDirectory() const{
    return DResultsDirectory;
}

std::vector<std::string> CArgumentParser::Filenames() const{
    return DFilenames;
}

CKMLTranslator::CKMLTranslator(std::shared_ptr<CStreetMap> map, std::shared_ptr<CDSVReader> stops, std::shared_ptr<CDSVReader> buspaths){
    const std::string StopIDHeading = "stop_id";
    const std::string NodeIDHeading = "node_id";
    const std::string SourceIDHeading = "src_id";
    const std::string DestinationIDHeading = "dest_id";
    const std::string RoutesHeading = "routes";
    const std::string PathHeading = "path";
    for(std::size_t Index = 0; Index < map->NodeCount(); Index++){
        auto Node = map->NodeByIndex(Index);
        DNodeIDToLocation[Node->ID()] = Node->Location();
    }
    std::vector<std::string> TempRow;
    if(stops->ReadRow(TempRow)){
        auto StopIDIndex = TempRow.size();
        auto NodeIDIndex = StopIDIndex;
        for(size_t Index = 0; Index < TempRow.size(); Index++){
            if(TempRow[Index] == StopIDHeading){
                StopIDIndex = Index;
            }
            else if(TempRow[Index] == NodeIDHeading){
                NodeIDIndex = Index;
            }
        }
        if((StopIDIndex >= TempRow.size())||(NodeIDIndex >= TempRow.size())){
            throw std::runtime_error("Missing stops header!");
        }
        while(stops->ReadRow(TempRow)){
            auto StopID = std::stoull(TempRow[StopIDIndex]);
            auto NodeID = std::stoull(TempRow[NodeIDIndex]);
            DNodeIDToStopID[NodeID] = StopID;
        }
    }
    if(buspaths->ReadRow(TempRow)){
        auto SourceIDIndex = TempRow.size();
        auto DestinationIDIndex = SourceIDIndex;
        auto RoutesIndex = SourceIDIndex;
        auto PathIndex = SourceIDIndex;
        for(size_t Index = 0; Index < TempRow.size(); Index++){
            if(TempRow[Index] == SourceIDHeading){
                SourceIDIndex = Index;
            }
            else if(TempRow[Index] == DestinationIDHeading){
                DestinationIDIndex = Index;
            }
            else if(TempRow[Index] == RoutesHeading){
                RoutesIndex = Index;
            }
            else if(TempRow[Index] == PathHeading){
                PathIndex = Index;
            }
        }
        if((SourceIDIndex >= TempRow.size())||(DestinationIDIndex >= TempRow.size())||(RoutesIndex >= TempRow.size())||(PathIndex >= TempRow.size())){
            throw std::runtime_error("Missing buspath header!");
        }
        while(buspaths->ReadRow(TempRow)){
            auto SourceID = std::stoull(TempRow[SourceIDIndex]);
            auto DestinationID = std::stoull(TempRow[DestinationIDIndex]);
            auto Routes = StringUtils::Split(TempRow[RoutesIndex],"_");
            auto PathStrings = StringUtils::Split(TempRow[PathIndex],",");
            std::vector<CStreetMap::TLocation> LocationList;
            for(auto &NodeIDString : PathStrings){
                auto NodeID = std::stoull(NodeIDString);
                auto Node = map->NodeByID(NodeID);
                LocationList.push_back(Node->Location());
            }
            DBusSegmentToLocations[std::make_pair(SourceID,DestinationID)] = LocationList;
        }
    }
}

bool CKMLTranslator::TranslateFile(const std::string &filename){
    const std::string WalkStyle = "WalkStyle";
    const std::string BikeStyle = "BikeStyle";
    const std::string BusStyle = "BusStyle";
    const std::string PointStyle = "PointStyle";
    uint32_t WalkColor = 0xff313131;
    uint32_t BikeColor = 0xffbe7443;
    uint32_t BusColor = 0xffa5a5a5;
    uint32_t PointColor = 0xff8d5f24;
    int DefaultWidth = 4;
    //0xffa5a5a5, 0xffd09d5a, 0xff744525, 0xff636363, 0xff8d5f24
    auto TripReader = std::make_shared<CDSVReader>(std::make_shared<CFileDataSource>(filename),',');
    auto DotIndex = filename.rfind(".");
    auto KMLFilename = filename.substr(0,DotIndex) + ".kml";
    auto FilenameComponents = StringUtils::Split(filename,"/");
    auto SubComponents = StringUtils::Split(FilenameComponents.back(),"_");
    bool IsFastest = SubComponents.back().find("hr") != std::string::npos;
    auto KMLName = SubComponents[0] + " to " + SubComponents[1];
    auto KMLDescription = IsFastest ? "Fastest path" : "Shortest path";
    CKMLWriter KMLWriter(std::make_shared<CFileDataSink>(KMLFilename),KMLName,KMLDescription);
    KMLWriter.CreatePointStyle(PointStyle,PointColor);
    KMLWriter.CreateLineStyle(WalkStyle,WalkColor,DefaultWidth);
    KMLWriter.CreateLineStyle(BikeStyle,BikeColor,DefaultWidth);
    KMLWriter.CreateLineStyle(BusStyle,BusColor,DefaultWidth);
    CStreetMap::TNodeID CurrentNodeID = CStreetMap::InvalidNodeID;
    CStreetMap::TLocation LastLocation;
    std::vector<CStreetMap::TLocation> SubPathLocations;
    std::string Description;
    std::string LastMode;
    for(auto &PathStep : ParsePathFile(TripReader)){
        auto Mode = std::get<0>(PathStep);
        auto NodeID = std::get<1>(PathStep);
        auto Location = DNodeIDToLocation[NodeID];
        if(CurrentNodeID == CStreetMap::InvalidNodeID){
            Description = std::string("Start Point\nNode ID: ") + std::to_string(NodeID) + "\nLatitude: " + std::to_string(std::get<0>(Location))+ "\nLongitude: " + std::to_string(std::get<1>(Location));
            KMLWriter.CreatePoint("Start Point",Description,PointStyle,Location);
            SubPathLocations.push_back(Location);
        }
        else{
            if(Mode != LastMode){
                if(SubPathLocations.size() > 1){
                    KMLWriter.CreatePath(LastMode,LastMode + "Style",SubPathLocations);
                }
                SubPathLocations.clear();
                SubPathLocations.push_back(LastLocation);
            }
            if(Mode == "Bus"){
                if(Mode != LastMode){
                    Description = std::string("Bus Stop\nStop ID: ") + std::to_string(DNodeIDToStopID[CurrentNodeID]) + "\nLatitude: " + std::to_string(std::get<0>(LastLocation))+ "\nLongitude: " + std::to_string(std::get<1>(LastLocation));
                    KMLWriter.CreatePoint("Bus Stop",Description,PointStyle,LastLocation);
                }
                KMLWriter.CreatePath(Mode,BusStyle,DBusSegmentToLocations[std::make_pair(CurrentNodeID,NodeID)]);
                Description = std::string("Bus Stop\nStop ID: ") + std::to_string(DNodeIDToStopID[NodeID]) + "\nLatitude: " + std::to_string(std::get<0>(Location))+ "\nLongitude: " + std::to_string(std::get<1>(Location));
                KMLWriter.CreatePoint("Bus Stop",Description,PointStyle,Location);
            }
            else{
                SubPathLocations.push_back(Location);
            }
            // if mode == Bus
            // else if mode 
        }
        CurrentNodeID = NodeID;
        LastLocation = Location;
        LastMode = Mode;
    }
    if(SubPathLocations.size() > 1){
        KMLWriter.CreatePath(LastMode,LastMode + "Style",SubPathLocations);
    }
    Description = std::string("End Point\nNode ID: ") + std::to_string(CurrentNodeID) + "\nLatitude: " + std::to_string(std::get<0>(LastLocation))+ "\nLongitude: " + std::to_string(std::get<1>(LastLocation));
    KMLWriter.CreatePoint("End Point",Description,PointStyle,LastLocation);


    return true;
}


std::vector<std::pair<std::string,CStreetMap::TNodeID> > CKMLTranslator::ParsePathFile(std::shared_ptr<CDSVReader> path){
    const std::string ModeHeading = "mode";
    const std::string NodeIDHeading = "node_id";
    
    std::vector<std::string> TempRow;
    if(path->ReadRow(TempRow)){
        auto ModeIndex = TempRow.size();
        auto NodeIDIndex = ModeIndex;
        for(size_t Index = 0; Index < TempRow.size(); Index++){
            if(TempRow[Index] == ModeHeading){
                ModeIndex = Index;
            }
            else if(TempRow[Index] == NodeIDHeading){
                NodeIDIndex = Index;
            }
        }
        if((ModeIndex >= TempRow.size())||(NodeIDIndex >= TempRow.size())){
            return {};
        }
        std::vector<std::pair<std::string,CStreetMap::TNodeID> > ReturnVector;
        while(path->ReadRow(TempRow)){
            auto Mode = TempRow[ModeIndex];
            auto NodeID = std::stoull(TempRow[NodeIDIndex]);
            ReturnVector.push_back(std::make_pair(Mode,NodeID));
        }
        return ReturnVector;
    }
    return {};
}