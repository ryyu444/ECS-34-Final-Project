#include <gtest/gtest.h>
#include "XMLReader.h"
#include "StringUtils.h"
#include "StringDataSource.h"
#include "OpenStreetMap.h"

TEST(OSMTest, SimpleTest){
    auto InStream = std::make_shared<CStringDataSource>("<?xml version='1.0' encoding='UTF-8'?>"
                                                        "<osm version=\"0.6\" generator=\"osmconvert 0.8.5\">"
                                                        "</osm>");
    auto Reader = std::make_shared<CXMLReader>(InStream);
    COpenStreetMap StreetMap(Reader);
    
    EXPECT_EQ(StreetMap.NodeCount(),0);
    EXPECT_EQ(StreetMap.WayCount(),0);
}

TEST(OSMTest, SimpleNodeTest){
    auto InStream = std::make_shared<CStringDataSource>("<?xml version='1.0' encoding='UTF-8'?>"
                                                        "<osm version=\"0.6\" generator=\"osmconvert 0.8.5\">"
                                                        "<node id=\"1\" lat=\"38.5\" lon=\"-121.7\"/>"
                                                        "<node id=\"2\" lat=\"38.5\" lon=\"-121.71\"/>"
                                                        "</osm>");
    auto Reader = std::make_shared<CXMLReader>(InStream);
    COpenStreetMap StreetMap(Reader);
    
    EXPECT_EQ(StreetMap.NodeCount(),2);
    EXPECT_EQ(StreetMap.WayCount(),0);
    auto TempNode1 = StreetMap.NodeByIndex(0);
    auto TempNode2 = StreetMap.NodeByID(1);
    EXPECT_EQ(TempNode1,TempNode2);
    ASSERT_TRUE(bool(TempNode1));
    EXPECT_EQ(TempNode1->ID(),1);
    EXPECT_EQ(TempNode1->AttributeCount(),0);
    EXPECT_EQ(TempNode1->Location(),std::make_pair(38.5,-121.7));
    TempNode1 = StreetMap.NodeByIndex(1);
    TempNode2 = StreetMap.NodeByID(2);
    EXPECT_EQ(TempNode1,TempNode2);
    ASSERT_TRUE(bool(TempNode1));
    EXPECT_EQ(TempNode1->ID(),2);
    EXPECT_EQ(TempNode1->AttributeCount(),0);
    EXPECT_EQ(TempNode1->Location(),std::make_pair(38.5,-121.71));
}

TEST(OSMTest, NodeTest){
    auto InStream = std::make_shared<CStringDataSource>("<?xml version='1.0' encoding='UTF-8'?>"
                                                        "<osm version=\"0.6\" generator=\"osmconvert 0.8.5\">"
                                                        "<node id=\"1\" lat=\"38.5\" lon=\"-121.7\">"
                                                        "<tag k=\"foot\" v=\"yes\"/>"
                                                        "<tag k=\"bicycle\" v=\"yes\"/>"
                                                        "</node>"
                                                        "<node id=\"2\" lat=\"38.5\" lon=\"-121.71\"/>"
                                                        "</osm>");
    auto Reader = std::make_shared<CXMLReader>(InStream);
    COpenStreetMap StreetMap(Reader);
    
    EXPECT_EQ(StreetMap.NodeCount(),2);
    EXPECT_EQ(StreetMap.WayCount(),0);
    auto TempNode1 = StreetMap.NodeByIndex(0);
    auto TempNode2 = StreetMap.NodeByID(1);
    EXPECT_EQ(TempNode1,TempNode2);
    ASSERT_TRUE(bool(TempNode1));
    EXPECT_EQ(TempNode1->ID(),1);
    EXPECT_EQ(TempNode1->AttributeCount(),2);
    EXPECT_EQ(TempNode1->Location(),std::make_pair(38.5,-121.7));
    EXPECT_TRUE(TempNode1->HasAttribute("foot"));
    EXPECT_TRUE(TempNode1->HasAttribute("bicycle"));
    EXPECT_EQ(TempNode1->GetAttribute("foot"),"yes");
    EXPECT_EQ(TempNode1->GetAttribute("bicycle"),"yes");
    TempNode1 = StreetMap.NodeByIndex(1);
    TempNode2 = StreetMap.NodeByID(2);
    EXPECT_EQ(TempNode1,TempNode2);
    ASSERT_TRUE(bool(TempNode1));
    EXPECT_EQ(TempNode1->ID(),2);
    EXPECT_EQ(TempNode1->AttributeCount(),0);
    EXPECT_EQ(TempNode1->Location(),std::make_pair(38.5,-121.71));
}

TEST(OSMTest, SimpleWayTest){
    auto InStream = std::make_shared<CStringDataSource>("<?xml version='1.0' encoding='UTF-8'?>"
                                                        "<osm version=\"0.6\" generator=\"osmconvert 0.8.5\">"
                                                        "<node id=\"1\" lat=\"38.5\" lon=\"-121.7\">"
                                                        "<tag k=\"foot\" v=\"yes\"/>"
                                                        "<tag k=\"bicycle\" v=\"yes\"/>"
                                                        "</node>"
                                                        "<node id=\"2\" lat=\"38.5\" lon=\"-121.71\"/>"
                                                        "<way id=\"3\">"
                                                        "<nd ref=\"1\"/>"
                                                        "<nd ref=\"2\"/>"
                                                        "</way>"
                                                        "</osm>");
    auto Reader = std::make_shared<CXMLReader>(InStream);
    COpenStreetMap StreetMap(Reader);
    
    EXPECT_EQ(StreetMap.NodeCount(),2);
    EXPECT_EQ(StreetMap.WayCount(),1);
    auto TempWay = StreetMap.WayByIndex(0);
    ASSERT_TRUE(bool(TempWay));
    EXPECT_EQ(TempWay->NodeCount(),2);
    EXPECT_EQ(TempWay->GetNodeID(0),1);
    EXPECT_EQ(TempWay->GetNodeID(1),2);
    EXPECT_EQ(TempWay->AttributeCount(),0);
}

TEST(OSMTest, WayTest){
    auto InStream = std::make_shared<CStringDataSource>("<?xml version='1.0' encoding='UTF-8'?>"
                                                        "<osm version=\"0.6\" generator=\"osmconvert 0.8.5\">"
                                                        "<node id=\"1\" lat=\"38.5\" lon=\"-121.7\">"
                                                        "<tag k=\"foot\" v=\"yes\"/>"
                                                        "<tag k=\"bicycle\" v=\"yes\"/>"
                                                        "</node>"
                                                        "<node id=\"2\" lat=\"38.5\" lon=\"-121.71\"/>"
                                                        "<way id=\"3\">"
                                                        "<nd ref=\"1\"/>"
                                                        "<nd ref=\"2\"/>"
                                                        "<tag k=\"oneway\" v=\"yes\"/>"
                                                        "</way>"
                                                        "</osm>");
    auto Reader = std::make_shared<CXMLReader>(InStream);
    COpenStreetMap StreetMap(Reader);
    
    EXPECT_EQ(StreetMap.NodeCount(),2);
    EXPECT_EQ(StreetMap.WayCount(),1);
    auto TempWay = StreetMap.WayByIndex(0);
    ASSERT_TRUE(bool(TempWay));
    EXPECT_EQ(TempWay->NodeCount(),2);
    EXPECT_EQ(TempWay->GetNodeID(0),1);
    EXPECT_EQ(TempWay->GetNodeID(1),2);
    EXPECT_EQ(TempWay->AttributeCount(),1);
    EXPECT_TRUE(TempWay->HasAttribute("oneway"));
    EXPECT_EQ(TempWay->GetAttribute("oneway"),"yes");
}