#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include "TransportationPlannerCommandLine.h"
#include "StringDataSink.h"
#include "StringDataSource.h"

class CMockTransportationPlanner : public CTransportationPlanner{
    public:
        MOCK_METHOD(std::size_t, NodeCount, (), (const, noexcept, override));
        MOCK_METHOD(std::shared_ptr<CStreetMap::SNode> , SortedNodeByIndex, (std::size_t index), (const, noexcept, override));
        MOCK_METHOD(double, FindShortestPath, (TNodeID src, TNodeID dest, std::vector< TNodeID > &path), (override));
        MOCK_METHOD(double, FindFastestPath, (TNodeID src, TNodeID dest, std::vector< TTripStep > &path), (override));
        MOCK_METHOD(bool, GetPathDescription, (const std::vector< TTripStep > &path, std::vector< std::string > &desc), (const, override));
};

struct SMockNode : public CStreetMap::SNode{
    MOCK_METHOD(CStreetMap::TNodeID, ID, (), (const, noexcept, override));
    MOCK_METHOD(CStreetMap::TLocation, Location, (), (const, noexcept, override));
    MOCK_METHOD(std::size_t, AttributeCount, (), (const, noexcept, override));
    MOCK_METHOD(std::string, GetAttributeKey, (std::size_t index), (const, noexcept, override));
    MOCK_METHOD(bool, HasAttribute, (const std::string &key), (const, noexcept, override));
    MOCK_METHOD(std::string, GetAttribute, (const std::string &key), (const, noexcept, override));
};

class CMockFactory : public CDataFactory{
    public:
        MOCK_METHOD(std::shared_ptr< CDataSource >, CreateSource, (const std::string &name), (noexcept, override));
        MOCK_METHOD(std::shared_ptr< CDataSink >, CreateSink, (const std::string &name), (noexcept, override));
};

TEST(TransporationPlannerCommandLine, SimpleTest){
    auto InputSource = std::make_shared<CStringDataSource>("exit\n");
    auto OutputSink = std::make_shared<CStringDataSink>();
    auto ErrorSink = std::make_shared<CStringDataSink>();
    auto MockPlanner = std::make_shared<CMockTransportationPlanner>();
    auto MockFactory = std::make_shared<CMockFactory>();

    CTransportationPlannerCommandLine CommandLine(InputSource,OutputSink,ErrorSink,MockFactory,MockPlanner);

    EXPECT_TRUE(CommandLine.ProcessCommands());
    EXPECT_EQ(OutputSink->String(),"> ");
    EXPECT_TRUE(ErrorSink->String().empty());
}

TEST(TransporationPlannerCommandLine, HelpTest){
    auto InputSource = std::make_shared<CStringDataSource>( "help\n"
                                                            "exit\n");
    auto OutputSink = std::make_shared<CStringDataSink>();
    auto ErrorSink = std::make_shared<CStringDataSink>();
    auto MockPlanner = std::make_shared<CMockTransportationPlanner>();
    auto MockFactory = std::make_shared<CMockFactory>();

    CTransportationPlannerCommandLine CommandLine(InputSource,OutputSink,ErrorSink,MockFactory,MockPlanner);

    EXPECT_TRUE(CommandLine.ProcessCommands());
    EXPECT_EQ(OutputSink->String(),"> "
                                    "------------------------------------------------------------------------\n"
                                    "help     Display this help menu\n"
                                    "exit     Exit the program\n"
                                    "count    Output the number of nodes in the map\n"
                                    "node     Syntax \"node [0, count)\" \n"
                                    "         Will output node ID and Lat/Lon for node\n"
                                    "fastest  Syntax \"fastest start end\" \n"
                                    "         Calculates the time for fastest path from start to end\n"
                                    "shortest Syntax \"shortest start end\" \n"
                                    "         Calculates the distance for the shortest path from start to end\n"
                                    "save     Saves the last calculated path to file\n"
                                    "print    Prints the steps for the last calculated path\n"
                                    "> ");
    EXPECT_TRUE(ErrorSink->String().empty());
}

TEST(TransporationPlannerCommandLine, CountTest){
    auto InputSource = std::make_shared<CStringDataSource>( "count\n"
                                                            "exit\n");
    auto OutputSink = std::make_shared<CStringDataSink>();
    auto ErrorSink = std::make_shared<CStringDataSink>();
    auto MockPlanner = std::make_shared<CMockTransportationPlanner>();
    auto MockFactory = std::make_shared<CMockFactory>();

    EXPECT_CALL(*MockPlanner, NodeCount())
        .Times(::testing::AtLeast(1))
        .WillRepeatedly(::testing::Return(4));

    CTransportationPlannerCommandLine CommandLine(InputSource,OutputSink,ErrorSink,MockFactory,MockPlanner);

    EXPECT_TRUE(CommandLine.ProcessCommands());
    EXPECT_EQ(OutputSink->String(),"> "
                                    "4 nodes\n"
                                    "> ");
    EXPECT_TRUE(ErrorSink->String().empty());
}

TEST(TransporationPlannerCommandLine, NodeTest){
    auto InputSource = std::make_shared<CStringDataSource>("node 0\nexit\n");
    auto OutputSink = std::make_shared<CStringDataSink>();
    auto ErrorSink = std::make_shared<CStringDataSink>();
    auto MockPlanner = std::make_shared<CMockTransportationPlanner>();
    auto MockNode = std::make_shared<SMockNode>();
    auto MockFactory = std::make_shared<CMockFactory>();

    EXPECT_CALL(*MockPlanner, NodeCount())
        .WillRepeatedly(::testing::Return(4));

    EXPECT_CALL(*MockPlanner, SortedNodeByIndex(0))
        .WillRepeatedly(::testing::Return(MockNode));

    EXPECT_CALL(*MockNode, ID())
        .WillRepeatedly(::testing::Return(1234));

    EXPECT_CALL(*MockNode, Location())
        .WillRepeatedly(::testing::Return(std::make_pair(38.6,-121.78)));

    CTransportationPlannerCommandLine CommandLine(InputSource,OutputSink,ErrorSink,MockFactory,MockPlanner);

    EXPECT_TRUE(CommandLine.ProcessCommands());
    EXPECT_EQ(OutputSink->String(),"> "
                                    "Node 0: id = 1234 is at 38d 36' 0\" N, 121d 46' 48\" W\n"
                                    "> ");
    EXPECT_TRUE(ErrorSink->String().empty());
}

TEST(TransporationPlannerCommandLine, ShortestTest){
    auto InputSource = std::make_shared<CStringDataSource>( "shortest 123 456\n"
                                                            "exit\n");
    auto OutputSink = std::make_shared<CStringDataSink>();
    auto ErrorSink = std::make_shared<CStringDataSink>();
    auto MockPlanner = std::make_shared<CMockTransportationPlanner>();
    auto MockNode = std::make_shared<SMockNode>();
    auto MockFactory = std::make_shared<CMockFactory>();
    std::vector<CTransportationPlanner::TNodeID> ExpectedPath = {123, 456};
    
    EXPECT_CALL(*MockPlanner, FindShortestPath(123, 456, ::testing::_))
        .WillRepeatedly(::testing::DoAll(::testing::SetArgReferee<2>(ExpectedPath),::testing::Return(5.2)));

    CTransportationPlannerCommandLine CommandLine(InputSource,OutputSink,ErrorSink,MockFactory,MockPlanner);

    EXPECT_TRUE(CommandLine.ProcessCommands());
    EXPECT_EQ(OutputSink->String(),"> "
                                    "Shortest path is 5.2 mi.\n"
                                    "> ");
    EXPECT_TRUE(ErrorSink->String().empty());
}

TEST(TransporationPlannerCommandLine, FastestTest){
    auto InputSource = std::make_shared<CStringDataSource>( "fastest 123 456\n"
                                                            "exit\n");
    auto OutputSink = std::make_shared<CStringDataSink>();
    auto ErrorSink = std::make_shared<CStringDataSink>();
    auto MockPlanner = std::make_shared<CMockTransportationPlanner>();
    auto MockNode = std::make_shared<SMockNode>();
    auto MockFactory = std::make_shared<CMockFactory>();
    std::vector<CTransportationPlanner::TTripStep> ExpectedSteps = {{CTransportationPlanner::ETransportationMode::Bike, 123},
                                                                    {CTransportationPlanner::ETransportationMode::Bike, 456}};
    
    EXPECT_CALL(*MockPlanner, FindFastestPath(123, 456, ::testing::_))
        .WillRepeatedly(::testing::DoAll(::testing::SetArgReferee<2>(ExpectedSteps),::testing::Return(0.65)));

    CTransportationPlannerCommandLine CommandLine(InputSource,OutputSink,ErrorSink,MockFactory,MockPlanner);

    EXPECT_TRUE(CommandLine.ProcessCommands());
    EXPECT_EQ(OutputSink->String(),"> "
                                    "Fastest path takes 39 min.\n"
                                    "> ");
    EXPECT_TRUE(ErrorSink->String().empty());
}

TEST(TransporationPlannerCommandLine, PrintTest){
    auto InputSource = std::make_shared<CStringDataSource>( "fastest 123 456\n"
                                                            "print\n"
                                                            "exit\n");
    auto OutputSink = std::make_shared<CStringDataSink>();
    auto ErrorSink = std::make_shared<CStringDataSink>();
    auto MockPlanner = std::make_shared<CMockTransportationPlanner>();
    auto MockNode = std::make_shared<SMockNode>();
    auto MockFactory = std::make_shared<CMockFactory>();
    std::vector<CTransportationPlanner::TTripStep> ExpectedSteps = {{CTransportationPlanner::ETransportationMode::Bike,10},
                                                                    {CTransportationPlanner::ETransportationMode::Bike,9},
                                                                    {CTransportationPlanner::ETransportationMode::Bike,8},
                                                                    {CTransportationPlanner::ETransportationMode::Bike,7},
                                                                    {CTransportationPlanner::ETransportationMode::Bike,6}};
    std::vector< std::string > ExpectedDescription = {"Start at 38d 23' 60\" N, 121d 43' 12\" W",
                                                        "Bike N toward Main St. for 6.9 mi",
                                                        "Bike W along Main St. for 4.3 mi",
                                                        "Bike N along B St. for 3.5 mi",
                                                        "End at 38d 32' 60\" N, 121d 47' 60\" W"};

    EXPECT_CALL(*MockPlanner, FindFastestPath(123, 456, ::testing::_))
        .WillRepeatedly(::testing::DoAll(::testing::SetArgReferee<2>(ExpectedSteps),::testing::Return(0.65)));

    EXPECT_CALL(*MockPlanner, GetPathDescription(::testing::_, ::testing::_))
        .WillRepeatedly(::testing::DoAll(::testing::SetArgReferee<1>(ExpectedDescription),::testing::Return(true)));
        
    CTransportationPlannerCommandLine CommandLine(InputSource,OutputSink,ErrorSink,MockFactory,MockPlanner);

    EXPECT_TRUE(CommandLine.ProcessCommands());
    EXPECT_EQ(OutputSink->String(),"> "
                                    "Fastest path takes 39 min.\n"
                                    "> "
                                    "Start at 38d 23' 60\" N, 121d 43' 12\" W\n"
                                    "Bike N toward Main St. for 6.9 mi\n"
                                    "Bike W along Main St. for 4.3 mi\n"
                                    "Bike N along B St. for 3.5 mi\n"
                                    "End at 38d 32' 60\" N, 121d 47' 60\" W\n"
                                    "> ");
    EXPECT_TRUE(ErrorSink->String().empty());
}

TEST(TransporationPlannerCommandLine, SaveTest){
    auto InputSource = std::make_shared<CStringDataSource>( "fastest 123 456\n"
                                                            "save\n"
                                                            "exit\n");
    auto OutputSink = std::make_shared<CStringDataSink>();
    auto ErrorSink = std::make_shared<CStringDataSink>();
    auto MockPlanner = std::make_shared<CMockTransportationPlanner>();
    auto MockNode = std::make_shared<SMockNode>();
    auto MockFactory = std::make_shared<CMockFactory>();
    auto SaveSink = std::make_shared<CStringDataSink>();
    std::vector<CTransportationPlanner::TTripStep> ExpectedSteps = {{CTransportationPlanner::ETransportationMode::Walk,10},
                                                                    {CTransportationPlanner::ETransportationMode::Walk,9},
                                                                    {CTransportationPlanner::ETransportationMode::Bus,8},
                                                                    {CTransportationPlanner::ETransportationMode::Bus,7},
                                                                    {CTransportationPlanner::ETransportationMode::Walk,6}};

    EXPECT_CALL(*MockPlanner, FindFastestPath(123, 456, ::testing::_))
        .WillRepeatedly(::testing::DoAll(::testing::SetArgReferee<2>(ExpectedSteps),::testing::Return(1.375)));

    EXPECT_CALL(*MockFactory, CreateSink(std::string("123_456_1.375000hr.csv")))
        .WillRepeatedly(::testing::Return(SaveSink));
        
    CTransportationPlannerCommandLine CommandLine(InputSource,OutputSink,ErrorSink,MockFactory,MockPlanner);

    EXPECT_TRUE(CommandLine.ProcessCommands());
    EXPECT_EQ(OutputSink->String(),"> "
                                    "Fastest path takes 1 hr 22 min 30 sec.\n"
                                    "> "
                                    "Path saved to <results>/123_456_1.375000hr.csv\n"
                                    "> ");
    EXPECT_EQ(SaveSink->String(),"mode,node_id\n"
                                 "Walk,10\n"
                                 "Walk,9\n"
                                 "Bus,8\n"
                                 "Bus,7\n"
                                 "Walk,6");
    EXPECT_TRUE(ErrorSink->String().empty());
}

TEST(TransporationPlannerCommandLine, ErrorTest){
    auto InputSource = std::make_shared<CStringDataSource>( "foo\n"
                                                            "node\n"
                                                            "node oops\n"
                                                            "shortest\n"
                                                            "shortest   bar   123\n"
                                                            "fastest\n"
                                                            "fastest 123 nope\n"
                                                            "save\n"
                                                            "print\n"
                                                            "exit\n");
    auto OutputSink = std::make_shared<CStringDataSink>();
    auto ErrorSink = std::make_shared<CStringDataSink>();
    auto MockPlanner = std::make_shared<CMockTransportationPlanner>();
    auto MockNode = std::make_shared<SMockNode>();
    auto MockFactory = std::make_shared<CMockFactory>();

    EXPECT_CALL(*MockPlanner, NodeCount())
        .Times(::testing::AtLeast(0))
        .WillRepeatedly(::testing::Return(0));
        
    CTransportationPlannerCommandLine CommandLine(InputSource,OutputSink,ErrorSink,MockFactory,MockPlanner);

    EXPECT_TRUE(CommandLine.ProcessCommands());
    EXPECT_EQ(OutputSink->String(), "> "
                                    "> "
                                    "> "
                                    "> "
                                    "> "
                                    "> "
                                    "> "
                                    "> "
                                    "> "
                                    "> ");
    EXPECT_EQ(ErrorSink->String(),  "Unknown command \"foo\" type help for help.\n"
                                    "Invalid node command, see help.\n"
                                    "Invalid node parameter, see help.\n"
                                    "Invalid shortest command, see help.\n"
                                    "Invalid shortest parameter, see help.\n"
                                    "Invalid fastest command, see help.\n"
                                    "Invalid fastest parameter, see help.\n"
                                    "No valid path to save, see help.\n"
                                    "No valid path to print, see help.\n");
}