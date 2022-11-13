#include "TransportationPlannerConfig.h"
#include "DijkstraTransportationPlanner.h"
#include "OpenStreetMap.h"
#include "CSVBusSystem.h"
#include "FileDataFactory.h"
#include "StandardDataSource.h"
#include "StandardDataSink.h"
#include "StandardErrorDataSink.h"
#include "StringUtils.h"
#include <iostream>
#include <iomanip>
#include <sstream>
#include <chrono>
#include <vector>
#include <cmath>

class CArgumentParser{
    private:
        std::string DDataDirectory;
        std::string DResultsDirectory;
        uint64_t DNumPoints;
        uint64_t DSeed;
        bool DArgumentsValid;
        bool DVerbose;
        
        void PrintSyntax() const;
    public:
        CArgumentParser(const std::vector<std::string> &args);

        bool ArgumentsValid() const;

        std::string DataDirectory() const;
        std::string ResultsDirectory() const;
        bool Verbose() const;
        uint64_t NumPoints() const;
        uint64_t Seed() const;
};

class CSpeedTest{
    private:
        std::shared_ptr<CTransportationPlanner> DPlanner;
        std::shared_ptr<CDataSink> DOutput;
        std::shared_ptr<CDataSink> DNotify;
        bool DViolatedPrecomputeTime;
        std::vector< std::vector< CStreetMap::TNodeID > > DShortestPaths;
        std::vector< double > DShortestDistance;
        std::vector< std::vector< CTransportationPlanner::TTripStep > > DFastestPaths;
        std::vector< double > DFastestTime;
        uint64_t DLoadDurationCount;
        uint64_t DProcessingDurationCount;

        static std::string DistanceToString(double dist);
        static std::string TimeToString(double dur);
        static std::string ShortestPathToNodeString(const std::vector< CStreetMap::TNodeID > &path);
        static std::string FastestPathToNodeString(const std::vector< CTransportationPlanner::TTripStep > &path);

        void OutputString(const std::string &str);
        void NotifyString(const std::string &str);
        void WriteStringToSink(std::shared_ptr<CDataSink> sink, const std::string &str);
    public:
        CSpeedTest(std::shared_ptr<CDataSink> out, std::shared_ptr<CDataSink> notify, std::shared_ptr<CTransportationPlanner::SConfiguration> config);

        bool RunTest(uint64_t seed, uint64_t numpoints, bool verbose);
        bool OutputResults(std::shared_ptr<CDataFactory> results, bool verbose);
};

int main(int argc, char *argv[]){
    std::vector<std::string> Arguments;
    const std::string OSMFilename = "city.osm";
    const std::string StopFilename = "stops.csv";
    const std::string RouteFilename = "routes.csv";

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
    auto RouteReader = std::make_shared<CDSVReader>(DataFactory->CreateSource(RouteFilename),',');
    auto BusSystem = std::make_shared<CCSVBusSystem>(StopReader, RouteReader);
    auto XMLReader = std::make_shared<CXMLReader>(DataFactory->CreateSource(OSMFilename));
    auto StreetMap = std::make_shared<COpenStreetMap>(XMLReader);
    auto PlannerConfig = std::make_shared<STransportationPlannerConfig>(StreetMap, BusSystem);

    CSpeedTest SpeedTester(StdOut,StdErr,PlannerConfig);

    if(SpeedTester.RunTest(Parser.Seed(),Parser.NumPoints(),Parser.Verbose())){
        if(SpeedTester.OutputResults(ResultsFactory,Parser.Verbose())){
            return EXIT_SUCCESS;        
        }
    }

    return EXIT_FAILURE;
}

CArgumentParser::CArgumentParser(const std::vector<std::string> &args){
    DDataDirectory = "./data";
    DResultsDirectory = "./results";
    DArgumentsValid = true;
    DNumPoints = 0;
    DSeed = 0;
    DVerbose = false;
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
        else if(Argument.find("--seed") == 0){
            auto SplitArg = StringUtils::Split(Argument,"=");
            if(SplitArg.size() != 2 || SplitArg[0] != "--seed"){
                DArgumentsValid = false;
                break;
            }
            DSeed = std::stoull(SplitArg[1]);
        }
        else if(Argument == "--verbose"){
            DVerbose = true;
        }
        else{
            if(DNumPoints){
                DArgumentsValid = false;
                break;
            }
            DNumPoints = std::stoull(Argument);
        }
    }
    if(!DArgumentsValid){
        PrintSyntax();
    }
    else{
        if(!DNumPoints){
            DNumPoints = 1000;
        }
        if(!DSeed){
            DSeed = 1234;
        }
    }
}

void CArgumentParser::PrintSyntax() const{
    std::cerr<<"Syntax Error: speedtest [--data=path | --results=path | --seed=rngseed | --verbose] [numpoints]"<<std::endl;
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

bool CArgumentParser::Verbose() const{
    return DVerbose;
}

uint64_t CArgumentParser::NumPoints() const{
    return DNumPoints;
}

uint64_t CArgumentParser::Seed() const{
    return DSeed;
}

CSpeedTest::CSpeedTest(std::shared_ptr<CDataSink> out, std::shared_ptr<CDataSink> notify, std::shared_ptr<CTransportationPlanner::SConfiguration> config){
    const int MillisecondsPerSecond = 1000;
    DOutput = out;
    DNotify = notify;
    NotifyString("Loading\n");
    auto LoadStart = std::chrono::steady_clock::now();
    DPlanner = std::make_shared<CDijkstraTransportationPlanner>(config);
    auto LoadDuration = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::steady_clock::now()-LoadStart);
    NotifyString("Loaded\n");
    DViolatedPrecomputeTime = config->PrecomputeTime() * MillisecondsPerSecond < LoadDuration.count();
    if(DViolatedPrecomputeTime){
        NotifyString("Violated precompute time!!!\n");
    }
    DLoadDurationCount = LoadDuration.count();
}

std::string CSpeedTest::DistanceToString(double dist){
    if(CPathRouter::NoPathExists == dist){
        return "(N/A)";
    }
    std::stringstream TempStringStream;
    TempStringStream<<"("<<std::setprecision(2)<<dist<<" mi)";
    return TempStringStream.str();
}

std::string CSpeedTest::TimeToString(double dur){
    if(CPathRouter::NoPathExists == dur){
        return "(N/A)";
    }
    std::stringstream TempStringStream;
    int IntegralTime;
    double Remainder = dur;
    bool PrecededOutput = false;
    TempStringStream<<"(";
    IntegralTime = Remainder;
    if(IntegralTime){
        TempStringStream<<IntegralTime<<":";
        PrecededOutput = true;
    }
    Remainder -= IntegralTime;
    Remainder *= 60;
    IntegralTime = Remainder;
    if(IntegralTime or PrecededOutput){
        if(PrecededOutput){
            TempStringStream<<std::setw(2)<<std::setfill('0');
        }
        TempStringStream<<IntegralTime<<":";
        PrecededOutput = true;
    }
    Remainder -= IntegralTime;
    Remainder *= 60;
    IntegralTime = Remainder;
    if(PrecededOutput){
        TempStringStream<<std::setw(2)<<std::setfill('0');
    }
    TempStringStream<<IntegralTime;
    if(PrecededOutput){
        TempStringStream<<")";
    }
    else{
        Remainder -= IntegralTime;
        Remainder *= 100;
        IntegralTime = Remainder;
        TempStringStream<<"."<<std::setw(2)<<std::setfill('0')<<IntegralTime<<" s)";
    }
    return TempStringStream.str();
}

std::string CSpeedTest::ShortestPathToNodeString(const std::vector< CStreetMap::TNodeID > &path){
    std::string ReturnString;
    if(!path.empty()){
        ReturnString = std::to_string(path[0]);
        for(std::size_t Index = 1; Index < path.size(); Index++){
            ReturnString += " " + std::to_string(path[Index]);
        }
    }
    return ReturnString;
}

std::string CSpeedTest::FastestPathToNodeString(const std::vector< CTransportationPlanner::TTripStep > &path){
    std::string ReturnString;
    if(!path.empty()){
        ReturnString = std::to_string(std::get<1>(path[0]));
        for(std::size_t Index = 1; Index < path.size(); Index++){
            ReturnString += " " + std::to_string(std::get<1>(path[Index]));
        }
    }
    return ReturnString;
}

void CSpeedTest::OutputString(const std::string &str){
    WriteStringToSink(DOutput,str);
}

void CSpeedTest::NotifyString(const std::string &str){
    WriteStringToSink(DNotify,str);
}

void CSpeedTest::WriteStringToSink(std::shared_ptr<CDataSink> sink, const std::string &str){
    sink->Write(std::vector<char>(str.begin(),str.end()));
}

bool CSpeedTest::RunTest(uint64_t seed, uint64_t numpoints, bool verbose){
    std::vector< CStreetMap::TNodeID > TempShortestPath;
    std::vector< CTransportationPlanner::TTripStep > TempFastestPath;
    std::vector< std::pair< CStreetMap::TNodeID , CStreetMap::TNodeID > > RandomNodePairs;
    srand(seed);
    NotifyString("Generating src/dest pairs\n");
    for(uint64_t Index = 0; Index < numpoints; Index++){
        auto SourceIndex = rand() % DPlanner->NodeCount();
        auto DestIndex = rand() %DPlanner->NodeCount();
        while(SourceIndex == DestIndex){
            DestIndex = rand() % DPlanner->NodeCount();
        }
        auto SourceNodeID = DPlanner->SortedNodeByIndex(SourceIndex)->ID();
        auto DestNodeID = DPlanner->SortedNodeByIndex(DestIndex)->ID();
        RandomNodePairs.push_back(std::make_pair(SourceNodeID,DestNodeID));
    }
    DShortestPaths.resize(numpoints);
    DShortestDistance.resize(numpoints);
    DFastestPaths.resize(numpoints);
    DFastestTime.resize(numpoints);
    NotifyString("Finding paths\n");
    auto ProcessingStart = std::chrono::steady_clock::now();
    for(uint64_t Index = 0; Index < numpoints; Index++){
        auto SourceNodeID = std::get<0>(RandomNodePairs[Index]);
        auto DestNodeID = std::get<1>(RandomNodePairs[Index]);
        std::vector< CStreetMap::TNodeID > &ShortestPath = verbose ? DShortestPaths[Index] : TempShortestPath;
        std::vector< CTransportationPlanner::TTripStep > &FastestPath = verbose ? DFastestPaths[Index] : TempFastestPath;
        DShortestDistance[Index] = DPlanner->FindShortestPath(SourceNodeID, DestNodeID, ShortestPath);
        DFastestTime[Index] = DPlanner->FindFastestPath(SourceNodeID, DestNodeID, FastestPath);
    }
    auto ProcessingDuration = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::steady_clock::now()-ProcessingStart);
    NotifyString("Paths found\n");
    DProcessingDurationCount = ProcessingDuration.count();
    return true;
}

bool CSpeedTest::OutputResults(std::shared_ptr<CDataFactory> results, bool verbose){
    NotifyString("Outputting Results\n");
    auto Brief = results->CreateSink("speed_test_brief.txt");
    for(std::size_t Index = 0; Index < DShortestPaths.size(); Index++){
        WriteStringToSink(Brief,std::to_string(Index) + " SP:" + DistanceToString(DShortestDistance[Index]));
        if(verbose){
            WriteStringToSink(Brief," " + ShortestPathToNodeString(DShortestPaths[Index]));
        }
        WriteStringToSink(Brief,std::string("\n") + std::to_string(Index) + " FP:" + TimeToString(DFastestTime[Index]));
        
        if(verbose){
            WriteStringToSink(Brief," " + FastestPathToNodeString(DFastestPaths[Index]));
        }
        WriteStringToSink(Brief,"\n");   
    }
    auto SamplesPerDay = long((86400000 - DLoadDurationCount) / (double(DProcessingDurationCount) / DShortestPaths.size()));
    auto MarginOfError = long(double(SamplesPerDay) / sqrt(DShortestPaths.size()));
    std::string Summary = "Duration (load): " + std::to_string(DLoadDurationCount) + "\n";
    Summary += "Duration (proc): " + std::to_string(DProcessingDurationCount) + "\n";
    Summary += "Queries per day: " + std::to_string(SamplesPerDay) + " (+-" + std::to_string(MarginOfError) + "), " + std::to_string(SamplesPerDay - MarginOfError) + " min\n";

    WriteStringToSink(Brief,Summary);
    NotifyString(Summary);
    return true;
}