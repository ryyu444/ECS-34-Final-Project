#include <gtest/gtest.h>
#include "XMLReader.h"
#include "StringUtils.h"
#include "StringDataSource.h"
#include "OpenStreetMap.h"
#include "CSVBusSystem.h"
#include "TransportationPlannerConfig.h"
#include "DijkstraTransportationPlanner.h"
#include "GeographicUtils.h"

TEST(CSVOSMTransporationPlanner, SimpleTest){
    auto InStreamOSM = std::make_shared<CStringDataSource>( "<?xml version='1.0' encoding='UTF-8'?>"
                                                            "<osm version=\"0.6\" generator=\"osmconvert 0.8.5\">"
                                                            "</osm>");
    auto InStreamStops = std::make_shared<CStringDataSource>("stop_id,node_id");
    auto InStreamRoutes = std::make_shared<CStringDataSource>("route,stop_id");
    auto XMLReader = std::make_shared<CXMLReader>(InStreamOSM);
    auto CSVReaderStops = std::make_shared<CDSVReader>(InStreamStops,',');
    auto CSVReaderRoutes = std::make_shared<CDSVReader>(InStreamRoutes,',');
    auto StreetMap = std::make_shared<COpenStreetMap>(XMLReader);
    auto BusSystem = std::make_shared<CCSVBusSystem>(CSVReaderStops, CSVReaderRoutes);
    auto Config = std::make_shared<STransportationPlannerConfig>(StreetMap,BusSystem);
    CDijkstraTransportationPlanner Planner(Config);
    std::vector< CTransportationPlanner::TNodeID > ShortestPath;
    std::vector< CTransportationPlanner::TTripStep > FastestPath;
    EXPECT_EQ(Planner.FindShortestPath(0,1,ShortestPath),CPathRouter::NoPathExists);
    EXPECT_EQ(Planner.FindFastestPath(0,1,FastestPath),CPathRouter::NoPathExists);
}

TEST(CSVOSMTransporationPlanner, SortedNodeTest){
    auto InStreamOSM = std::make_shared<CStringDataSource>( "<?xml version='1.0' encoding='UTF-8'?>"
                                                            "<osm version=\"0.6\" generator=\"osmconvert 0.8.5\">"
                                                            "<node id=\"4\" lat=\"38.5\" lon=\"-121.7\"/>"
                                                            "<node id=\"2\" lat=\"38.6\" lon=\"-121.7\"/>"
                                                            "<node id=\"1\" lat=\"38.6\" lon=\"-121.8\"/>"
                                                            "<node id=\"3\" lat=\"38.5\" lon=\"-121.8\"/>"
                                                            "<way id=\"10\">"
                                                            "<nd ref=\"1\"/>"
                                                            "<nd ref=\"2\"/>"
                                                            "<nd ref=\"3\"/>"
                                                            "<tag k=\"oneway\" v=\"yes\"/>"
                                                            "</way>"
                                                            "<way id=\"11\">"
                                                            "<nd ref=\"3\"/>"
                                                            "<nd ref=\"4\"/>"
                                                            "<nd ref=\"1\"/>"
                                                            "<tag k=\"oneway\" v=\"yes\"/>"
                                                            "</way>"
                                                            "</osm>");
    // 6.9090909 mil 1 -> 2
    // 5.4 mile  2 -> 3
    // 6.9090909 mil 3 -> 4
    // 5.407386 mi 4 -> 1
    auto InStreamStops = std::make_shared<CStringDataSource>("stop_id,node_id");
    auto InStreamRoutes = std::make_shared<CStringDataSource>("route,stop_id");
    auto XMLReader = std::make_shared<CXMLReader>(InStreamOSM);
    auto CSVReaderStops = std::make_shared<CDSVReader>(InStreamStops,',');
    auto CSVReaderRoutes = std::make_shared<CDSVReader>(InStreamRoutes,',');
    auto StreetMap = std::make_shared<COpenStreetMap>(XMLReader);
    auto BusSystem = std::make_shared<CCSVBusSystem>(CSVReaderStops, CSVReaderRoutes);
    auto Config = std::make_shared<STransportationPlannerConfig>(StreetMap,BusSystem);
    CDijkstraTransportationPlanner Planner(Config);
    EXPECT_EQ(Planner.NodeCount(),4);
    for(std::size_t Index = 0; Index < Planner.NodeCount(); Index++){
        auto Node = Planner.SortedNodeByIndex(Index);
        ASSERT_TRUE(Node);
        EXPECT_EQ(Node->ID(),Index+1);
    }
}


TEST(CSVOSMTransporationPlanner, ShortestPathTest){
    auto InStreamOSM = std::make_shared<CStringDataSource>( "<?xml version='1.0' encoding='UTF-8'?>"
                                                            "<osm version=\"0.6\" generator=\"osmconvert 0.8.5\">"
                                                            "<node id=\"1\" lat=\"38.5\" lon=\"-121.7\"/>"
                                                            "<node id=\"2\" lat=\"38.6\" lon=\"-121.7\"/>"
                                                            "<node id=\"3\" lat=\"38.6\" lon=\"-121.8\"/>"
                                                            "<node id=\"4\" lat=\"38.5\" lon=\"-121.8\"/>"
                                                            "<way id=\"10\">"
                                                            "<nd ref=\"1\"/>"
                                                            "<nd ref=\"2\"/>"
                                                            "<nd ref=\"3\"/>"
                                                            "<tag k=\"oneway\" v=\"yes\"/>"
                                                            "</way>"
                                                            "<way id=\"11\">"
                                                            "<nd ref=\"3\"/>"
                                                            "<nd ref=\"4\"/>"
                                                            "<nd ref=\"1\"/>"
                                                            "<tag k=\"oneway\" v=\"yes\"/>"
                                                            "</way>"
                                                            "</osm>");
    // 6.9090909 mil 1 -> 2
    // 5.4 mile  2 -> 3
    // 6.9090909 mil 3 -> 4
    // 5.407386 mi 4 -> 1
    auto InStreamStops = std::make_shared<CStringDataSource>("stop_id,node_id");
    auto InStreamRoutes = std::make_shared<CStringDataSource>("route,stop_id");
    auto XMLReader = std::make_shared<CXMLReader>(InStreamOSM);
    auto CSVReaderStops = std::make_shared<CDSVReader>(InStreamStops,',');
    auto CSVReaderRoutes = std::make_shared<CDSVReader>(InStreamRoutes,',');
    auto StreetMap = std::make_shared<COpenStreetMap>(XMLReader);
    auto BusSystem = std::make_shared<CCSVBusSystem>(CSVReaderStops, CSVReaderRoutes);
    auto Config = std::make_shared<STransportationPlannerConfig>(StreetMap,BusSystem);
    CDijkstraTransportationPlanner Planner(Config);
    std::vector< CTransportationPlanner::TNodeID > ShortestPath, ExpectedShortestPath = {1,2,3,4};
    double ExpectedDistance = SGeographicUtils::HaversineDistanceInMiles(std::make_pair(38.5,-121.7),std::make_pair(38.6,-121.7)) + 
                                SGeographicUtils::HaversineDistanceInMiles(std::make_pair(38.6,-121.7),std::make_pair(38.6,-121.8)) + 
                                SGeographicUtils::HaversineDistanceInMiles(std::make_pair(38.6,-121.8),std::make_pair(38.5,-121.8));
    EXPECT_EQ(Planner.FindShortestPath(1,4,ShortestPath),ExpectedDistance);
    EXPECT_EQ(ShortestPath,ExpectedShortestPath);
}

TEST(CSVOSMTransporationPlanner, FastestPathTest){
    auto InStreamOSM = std::make_shared<CStringDataSource>( "<?xml version='1.0' encoding='UTF-8'?>"
                                                            "<osm version=\"0.6\" generator=\"osmconvert 0.8.5\">"
                                                            "<node id=\"1\" lat=\"38.5\" lon=\"-121.7\"/>"
                                                            "<node id=\"2\" lat=\"38.6\" lon=\"-121.7\"/>"
                                                            "<node id=\"3\" lat=\"38.6\" lon=\"-121.8\"/>"
                                                            "<node id=\"4\" lat=\"38.5\" lon=\"-121.8\"/>"
                                                            "<way id=\"10\">"
                                                            "<nd ref=\"1\"/>"
                                                            "<nd ref=\"2\"/>"
                                                            "<nd ref=\"3\"/>"
                                                            "<nd ref=\"4\"/>"
                                                            "<tag k=\"maxspeed\" v=\"20 mph\"/>"
                                                            "</way>"
                                                            "<way id=\"11\">"
                                                            "<nd ref=\"4\"/>"
                                                            "<nd ref=\"1\"/>"
                                                            "</way>"
                                                            "</osm>");
    // 6.9090909 mil 1 <-> 2
    // 5.4 mile  2 <-> 3
    // 6.9090909 mil 3 <-> 4
    // 5.407386 mi 4 <-> 1
    auto InStreamStops = std::make_shared<CStringDataSource>("stop_id,node_id\n"
                                                            "101,1\n"
                                                            "102,2\n"
                                                            "103,3\n"
                                                            "104,4"
                                                            );
    auto InStreamRoutes = std::make_shared<CStringDataSource>("route,stop_id\n"
                                                             "A,101\n"
                                                             "A,102\n"
                                                             "A,103\n"
                                                             "A,104\n"
                                                             "A,101\n"
                                                             "B,104\n"
                                                             "B,103\n"
                                                             "B,102\n"
                                                             "B,103\n"
                                                             "B,104");
    auto XMLReader = std::make_shared<CXMLReader>(InStreamOSM);
    auto CSVReaderStops = std::make_shared<CDSVReader>(InStreamStops,',');
    auto CSVReaderRoutes = std::make_shared<CDSVReader>(InStreamRoutes,',');
    auto StreetMap = std::make_shared<COpenStreetMap>(XMLReader);
    auto BusSystem = std::make_shared<CCSVBusSystem>(CSVReaderStops, CSVReaderRoutes);
    auto Config = std::make_shared<STransportationPlannerConfig>(StreetMap,BusSystem);
    CDijkstraTransportationPlanner Planner(Config);
    std::vector< CTransportationPlanner::TTripStep > BusFastestPath, ExpectedBusFastestPath = {{CTransportationPlanner::ETransportationMode::Walk,1},
                                                                                        {CTransportationPlanner::ETransportationMode::Bus,2},
                                                                                        {CTransportationPlanner::ETransportationMode::Bus,3}};
    double ExpectedBusDistance = SGeographicUtils::HaversineDistanceInMiles(std::make_pair(38.5,-121.7),std::make_pair(38.6,-121.7)) + 
                                SGeographicUtils::HaversineDistanceInMiles(std::make_pair(38.6,-121.7),std::make_pair(38.6,-121.8));
    double ExpectedBusTime = ExpectedBusDistance / 20.0 + (60.0 / 3600.0);
    EXPECT_EQ(Planner.FindFastestPath(1,3,BusFastestPath),ExpectedBusTime);
    EXPECT_EQ(BusFastestPath,ExpectedBusFastestPath);
    std::vector< CTransportationPlanner::TTripStep > BikeFastestPath, ExpectedBikeFastestPath = {{CTransportationPlanner::ETransportationMode::Bike,1},
                                                                                        {CTransportationPlanner::ETransportationMode::Bike,4}};
    double ExpectedBikeDistance = SGeographicUtils::HaversineDistanceInMiles(std::make_pair(38.5,-121.7),std::make_pair(38.5,-121.8));
    double ExpectedBikeTime = ExpectedBikeDistance / 8.0;
    EXPECT_EQ(Planner.FindFastestPath(1,4,BikeFastestPath),ExpectedBikeTime);
    EXPECT_EQ(BikeFastestPath,ExpectedBikeFastestPath);

}

TEST(CSVOSMTransporationPlanner, PathDescription){
    auto InStreamOSM = std::make_shared<CStringDataSource>( "<?xml version='1.0' encoding='UTF-8'?>"
                                                            "<osm version=\"0.6\" generator=\"osmconvert 0.8.5\">"
                                                            "<node id=\"1\" lat=\"38.5\" lon=\"-121.7\"/>"
                                                            "<node id=\"2\" lat=\"38.55\" lon=\"-121.7\"/>"
                                                            "<node id=\"3\" lat=\"38.6\" lon=\"-121.7\"/>"
                                                            "<node id=\"4\" lat=\"38.6\" lon=\"-121.78\"/>"
                                                            "<node id=\"5\" lat=\"38.6\" lon=\"-121.8\"/>"
                                                            "<node id=\"6\" lat=\"38.55\" lon=\"-121.8\"/>"
                                                            "<node id=\"7\" lat=\"38.5\" lon=\"-121.8\"/>"
                                                            "<node id=\"8\" lat=\"38.5\" lon=\"-121.72\"/>"
                                                            "<node id=\"9\" lat=\"38.45\" lon=\"-121.72\"/>"
                                                            "<node id=\"10\" lat=\"38.4\" lon=\"-121.72\"/>"
                                                            "<node id=\"11\" lat=\"38.7\" lon=\"-121.78\"/>"
                                                            "<way id=\"10\">"
                                                            "<nd ref=\"1\"/>"
                                                            "<nd ref=\"2\"/>"
                                                            "<nd ref=\"3\"/>"
                                                            "<tag k=\"name\" v=\"A St.\"/>"
                                                            "</way>"
                                                            "<way id=\"11\">"
                                                            "<nd ref=\"3\"/>"
                                                            "<nd ref=\"4\"/>"
                                                            "<nd ref=\"5\"/>"
                                                            "<tag k=\"name\" v=\"2nd St.\"/>"
                                                            "</way>"
                                                            "<way id=\"12\">"
                                                            "<nd ref=\"5\"/>"
                                                            "<nd ref=\"6\"/>"
                                                            "<nd ref=\"7\"/>"
                                                            "<tag k=\"name\" v=\"B St.\"/>"
                                                            "</way>"
                                                            "<way id=\"13\">"
                                                            "<nd ref=\"7\"/>"
                                                            "<nd ref=\"8\"/>"
                                                            "<nd ref=\"1\"/>"
                                                            "<tag k=\"name\" v=\"Main St.\"/>"
                                                            "</way>"
                                                            "<way id=\"14\">"
                                                            "<nd ref=\"8\"/>"
                                                            "<nd ref=\"9\"/>"
                                                            "<nd ref=\"10\"/>"
                                                            "</way>"
                                                            "<way id=\"15\">"
                                                            "<nd ref=\"4\"/>"
                                                            "<nd ref=\"11\"/>"
                                                            "</way>"
                                                            "</osm>");
    // 6.9090909 mil 1 <-> 2
    // 5.4 mile  2 <-> 3
    // 6.9090909 mil 3 <-> 4
    // 5.407386 mi 4 <-> 1
    auto InStreamStops = std::make_shared<CStringDataSource>("stop_id,node_id\n"
                                                            "101,1\n"
                                                            "102,3\n"
                                                            "103,5\n"
                                                            "104,7"
                                                            );
    auto InStreamRoutes = std::make_shared<CStringDataSource>("route,stop_id\n"
                                                             "A,101\n"
                                                             "A,102\n"
                                                             "A,103\n"
                                                             "A,104\n"
                                                             "A,101\n"
                                                             "B,104\n"
                                                             "B,103\n"
                                                             "B,102\n"
                                                             "B,103\n"
                                                             "B,104");
    auto XMLReader = std::make_shared<CXMLReader>(InStreamOSM);
    auto CSVReaderStops = std::make_shared<CDSVReader>(InStreamStops,',');
    auto CSVReaderRoutes = std::make_shared<CDSVReader>(InStreamRoutes,',');
    auto StreetMap = std::make_shared<COpenStreetMap>(XMLReader);
    auto BusSystem = std::make_shared<CCSVBusSystem>(CSVReaderStops, CSVReaderRoutes);
    auto Config = std::make_shared<STransportationPlannerConfig>(StreetMap,BusSystem);
    CDijkstraTransportationPlanner Planner(Config);
    std::vector< CTransportationPlanner::TTripStep > Path1 = {{CTransportationPlanner::ETransportationMode::Walk,8},
                                                                {CTransportationPlanner::ETransportationMode::Walk,1},
                                                                {CTransportationPlanner::ETransportationMode::Bus,3},
                                                                {CTransportationPlanner::ETransportationMode::Bus,5},
                                                                {CTransportationPlanner::ETransportationMode::Walk,4},
                                                                {CTransportationPlanner::ETransportationMode::Walk,11}};

    std::vector<std::string> Description1, ExpectedDescription1 = {"Start at 38d 30' 0\" N, 121d 43' 12\" W",
                                                                    "Walk E along Main St. for 1.1 mi",
                                                                    "Take Bus A from stop 101 to stop 103",
                                                                    "Walk E along 2nd St. for 1.1 mi",
                                                                    "Walk N toward End for 6.9 mi",
                                                                    "End at 38d 42' 0\" N, 121d 46' 48\" W"};
    EXPECT_TRUE(Planner.GetPathDescription(Path1,Description1));
    EXPECT_EQ(Description1, ExpectedDescription1);
    std::vector< CTransportationPlanner::TTripStep > Path2 = {{CTransportationPlanner::ETransportationMode::Bike,8},
                                                                {CTransportationPlanner::ETransportationMode::Bike,7},
                                                                {CTransportationPlanner::ETransportationMode::Bike,6},
                                                                {CTransportationPlanner::ETransportationMode::Bike,5},
                                                                {CTransportationPlanner::ETransportationMode::Bike,4}};

    std::vector<std::string> Description2, ExpectedDescription2 = {"Start at 38d 30' 0\" N, 121d 43' 12\" W",
                                                                    "Bike W along Main St. for 4.3 mi",
                                                                    "Bike N along B St. for 6.9 mi",
                                                                    "Bike E along 2nd St. for 1.1 mi",
                                                                    "End at 38d 36' 0\" N, 121d 46' 48\" W"};
    EXPECT_TRUE(Planner.GetPathDescription(Path2,Description2));
    EXPECT_EQ(Description2, ExpectedDescription2);

    std::vector< CTransportationPlanner::TTripStep > Path3 = {{CTransportationPlanner::ETransportationMode::Bike,10},
                                                                {CTransportationPlanner::ETransportationMode::Bike,9},
                                                                {CTransportationPlanner::ETransportationMode::Bike,8},
                                                                {CTransportationPlanner::ETransportationMode::Bike,7},
                                                                {CTransportationPlanner::ETransportationMode::Bike,6}};

    std::vector<std::string> Description3, ExpectedDescription3 = {"Start at 38d 23' 60\" N, 121d 43' 12\" W",
                                                                    "Bike N toward Main St. for 6.9 mi",
                                                                    "Bike W along Main St. for 4.3 mi",
                                                                    "Bike N along B St. for 3.5 mi",
                                                                    "End at 38d 32' 60\" N, 121d 47' 60\" W"};
    EXPECT_TRUE(Planner.GetPathDescription(Path3,Description3));
    EXPECT_EQ(Description3, ExpectedDescription3);

}