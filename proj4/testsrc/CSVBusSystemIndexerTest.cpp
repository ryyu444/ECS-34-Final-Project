#include <gtest/gtest.h>
#include "XMLReader.h"
#include "StringUtils.h"
#include "StringDataSource.h"
#include "DSVReader.h"
#include "CSVBusSystem.h"
#include "BusSystemIndexer.h"

TEST(CSVBusSystemIndexer, SimpleTest){
    auto InStreamStops = std::make_shared<CStringDataSource>("stop_id,node_id");
    auto InStreamRoutes = std::make_shared<CStringDataSource>("route,stop_id");
    auto CSVReaderStops = std::make_shared<CDSVReader>(InStreamStops,',');
    auto CSVReaderRoutes = std::make_shared<CDSVReader>(InStreamRoutes,',');
    auto BusSystem = std::make_shared<CCSVBusSystem>(CSVReaderStops, CSVReaderRoutes);
    CBusSystemIndexer BusSystemIndexer(BusSystem);
    EXPECT_EQ(BusSystemIndexer.StopCount(),0);
    EXPECT_EQ(BusSystemIndexer.RouteCount(),0);
}

TEST(CSVBusSystemIndexer, StopTest){
    auto InStreamStops = std::make_shared<CStringDataSource>(   "stop_id,node_id\n"
                                                                "2,102\n"
                                                                "1,101");
    auto InStreamRoutes = std::make_shared<CStringDataSource>("route,stop_id");
    auto CSVReaderStops = std::make_shared<CDSVReader>(InStreamStops,',');
    auto CSVReaderRoutes = std::make_shared<CDSVReader>(InStreamRoutes,',');
    auto BusSystem = std::make_shared<CCSVBusSystem>(CSVReaderStops, CSVReaderRoutes);
    CBusSystemIndexer BusSystemIndexer(BusSystem);

    EXPECT_EQ(BusSystemIndexer.StopCount(),2);
    EXPECT_EQ(BusSystemIndexer.RouteCount(),0);
    auto Stop1Index = BusSystemIndexer.SortedStopByIndex(0);
    auto Stop1Node = BusSystemIndexer.StopByNodeID(101);
    EXPECT_EQ(Stop1Index,Stop1Node);
    ASSERT_TRUE(bool(Stop1Index));
    EXPECT_EQ(Stop1Index->ID(),1);
    EXPECT_EQ(Stop1Index->NodeID(),101);
    auto Stop2Index = BusSystemIndexer.SortedStopByIndex(1);
    auto Stop2Node = BusSystemIndexer.StopByNodeID(102);
    EXPECT_EQ(Stop2Index,Stop2Node);
    ASSERT_TRUE(bool(Stop2Index));
    EXPECT_EQ(Stop2Index->ID(),2);
    EXPECT_EQ(Stop2Index->NodeID(),102);
}

TEST(CSVBusSystemIndexer, RouteTest){
    auto InStreamStops = std::make_shared<CStringDataSource>(   "stop_id,node_id\n"
                                                                "1,101\n"
                                                                "2,102");
    auto InStreamRoutes = std::make_shared<CStringDataSource>(  "route,stop_id\n"
                                                                "B,1\n"
                                                                "B,2\n"
                                                                "B,1\n"
                                                                "A,2\n"
                                                                "A,1\n"
                                                                "A,2");
    auto CSVReaderStops = std::make_shared<CDSVReader>(InStreamStops,',');
    auto CSVReaderRoutes = std::make_shared<CDSVReader>(InStreamRoutes,',');
    auto BusSystem = std::make_shared<CCSVBusSystem>(CSVReaderStops, CSVReaderRoutes);
    CBusSystemIndexer BusSystemIndexer(BusSystem);

    EXPECT_EQ(BusSystemIndexer.StopCount(),2);
    EXPECT_EQ(BusSystemIndexer.RouteCount(),2);
    auto Route1Index = BusSystemIndexer.SortedRouteByIndex(0);
    ASSERT_TRUE(bool(Route1Index));
    EXPECT_EQ(Route1Index->Name(),"A");
    EXPECT_EQ(Route1Index->StopCount(),3);
    EXPECT_EQ(Route1Index->GetStopID(0),2);
    EXPECT_EQ(Route1Index->GetStopID(1),1);
    EXPECT_EQ(Route1Index->GetStopID(2),2);
    auto Route2Index = BusSystemIndexer.SortedRouteByIndex(1);
    ASSERT_TRUE(bool(Route2Index));
    EXPECT_EQ(Route2Index->Name(),"B");
    EXPECT_EQ(Route2Index->StopCount(),3);
    EXPECT_EQ(Route2Index->GetStopID(0),1);
    EXPECT_EQ(Route2Index->GetStopID(1),2);
    EXPECT_EQ(Route2Index->GetStopID(2),1);
    std::unordered_set< std::shared_ptr<CBusSystem::SRoute> > Routes;
    EXPECT_TRUE(BusSystemIndexer.RoutesByNodeIDs(101,102,Routes));
    EXPECT_EQ(Routes.size(),2);
    EXPECT_TRUE(Routes.find(Route1Index) != Routes.end());
    EXPECT_TRUE(Routes.find(Route2Index) != Routes.end());

}