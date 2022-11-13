#include <gtest/gtest.h>
#include "XMLReader.h"
#include "XMLWriter.h"
#include "StringUtils.h"
#include "StringDataSource.h"
#include "StringDataSink.h"

TEST(XMLReaderTest, SimpleTest){
    auto InStream = std::make_shared<CStringDataSource>("<element name=\"val\"></element>");
    CXMLReader Reader(InStream);
    SXMLEntity Entity;
    
    EXPECT_TRUE(Reader.ReadEntity(Entity));
    EXPECT_EQ(Entity.DType, SXMLEntity::EType::StartElement);
    EXPECT_EQ(Entity.DNameData, "element");
    EXPECT_EQ(Entity.DAttributes.size(), 1);
    EXPECT_TRUE(Entity.AttributeExists("name"));
    EXPECT_EQ(Entity.AttributeValue("name"), "val");
    
    EXPECT_TRUE(Reader.ReadEntity(Entity));
    EXPECT_EQ(Entity.DType, SXMLEntity::EType::EndElement);
    EXPECT_EQ(Entity.DNameData, "element");
    EXPECT_EQ(Entity.DAttributes.size(), 0);
    
}

TEST(XMLReaderTest, ElementTest){
    auto InStream = std::make_shared<CStringDataSource>("<?xml version='1.0' encoding='UTF-8'?>\n"
                                                        "<osm version=\"0.6\" generator=\"osmconvert 0.8.5\">\n"
                                                        "	<node id=\"62208369\" lat=\"38.5178523\" lon=\"-121.7712408\"/>\n"
                                                        "	<node id=\"62209104\" lat=\"38.535052\" lon=\"-121.7408606\"/>\n"
                                                        "</osm>");
    CXMLReader Reader(InStream);
    SXMLEntity Entity;
    
    EXPECT_TRUE(Reader.ReadEntity(Entity, true));
    EXPECT_EQ(Entity.DType, SXMLEntity::EType::StartElement);
    EXPECT_EQ(Entity.DNameData, "osm");
    EXPECT_EQ(Entity.DAttributes.size(), 2);
    if(Entity.DAttributes.size() == 2){
        EXPECT_TRUE(Entity.AttributeExists("version"));
        EXPECT_EQ(Entity.AttributeValue("version"), "0.6");
        EXPECT_TRUE(Entity.AttributeExists("generator"));
        EXPECT_EQ(Entity.AttributeValue("generator"), "osmconvert 0.8.5");
    }
    
    EXPECT_TRUE(Reader.ReadEntity(Entity, true));
    EXPECT_EQ(Entity.DType, SXMLEntity::EType::StartElement);
    EXPECT_EQ(Entity.DNameData, "node");
    EXPECT_EQ(Entity.DAttributes.size(), 3);
    if(Entity.DAttributes.size() == 3){
        EXPECT_TRUE(Entity.AttributeExists("id"));
        EXPECT_EQ(Entity.AttributeValue("id"), "62208369");
        EXPECT_TRUE(Entity.AttributeExists("lat"));
        EXPECT_EQ(Entity.AttributeValue("lat"), "38.5178523");
        EXPECT_TRUE(Entity.AttributeExists("lon"));
        EXPECT_EQ(Entity.AttributeValue("lon"), "-121.7712408");
    }
    
    EXPECT_TRUE(Reader.ReadEntity(Entity, true));
    EXPECT_EQ(Entity.DType, SXMLEntity::EType::EndElement);
    EXPECT_EQ(Entity.DNameData, "node");
    EXPECT_EQ(Entity.DAttributes.size(), 0);
    
    
    EXPECT_TRUE(Reader.ReadEntity(Entity, true));
    EXPECT_EQ(Entity.DType, SXMLEntity::EType::StartElement);
    EXPECT_EQ(Entity.DNameData, "node");
    EXPECT_EQ(Entity.DAttributes.size(), 3);
    if(Entity.DAttributes.size() == 3){
        EXPECT_TRUE(Entity.AttributeExists("id"));
        EXPECT_EQ(Entity.AttributeValue("id"), "62209104");
        EXPECT_TRUE(Entity.AttributeExists("lat"));
        EXPECT_EQ(Entity.AttributeValue("lat"), "38.535052");
        EXPECT_TRUE(Entity.AttributeExists("lon"));
        EXPECT_EQ(Entity.AttributeValue("lon"), "-121.7408606");
    }
    
    EXPECT_TRUE(Reader.ReadEntity(Entity, true));
    EXPECT_EQ(Entity.DType, SXMLEntity::EType::EndElement);
    EXPECT_EQ(Entity.DNameData, "node");
    EXPECT_EQ(Entity.DAttributes.size(), 0);

    EXPECT_TRUE(Reader.ReadEntity(Entity, true));
    EXPECT_EQ(Entity.DType, SXMLEntity::EType::EndElement);
    EXPECT_EQ(Entity.DNameData, "osm");
    EXPECT_EQ(Entity.DAttributes.size(), 0);
    
    EXPECT_TRUE(Reader.End());
}

TEST(XMLReaderTest, CDataTest){
    auto InStream = std::make_shared<CStringDataSource>( "<bold>Text</bold>");
    CXMLReader Reader(InStream);
    SXMLEntity Entity;
    
    EXPECT_TRUE(Reader.ReadEntity(Entity));
    EXPECT_EQ(Entity.DType, SXMLEntity::EType::StartElement);
    EXPECT_EQ(Entity.DNameData, "bold");
    EXPECT_EQ(Entity.DAttributes.size(), 0);
    
    EXPECT_TRUE(Reader.ReadEntity(Entity));
    EXPECT_EQ(Entity.DType, SXMLEntity::EType::CharData);
    EXPECT_EQ(Entity.DNameData, "Text");
    EXPECT_EQ(Entity.DAttributes.size(), 0);
    
    EXPECT_TRUE(Reader.ReadEntity(Entity));
    EXPECT_EQ(Entity.DType, SXMLEntity::EType::EndElement);
    EXPECT_EQ(Entity.DNameData, "bold");
    EXPECT_EQ(Entity.DAttributes.size(), 0);
    
    EXPECT_TRUE(Reader.End());
}

TEST(XMLReaderTest, LongCDataTest){
    auto InStream = std::make_shared<CStringDataSource>( "<elem>"
                                                        "0                               0"
                                                        "1                               1"
                                                        "2                               2"
                                                        "3                               3"
                                                        "4                               4"
                                                        "5                               5"
                                                        "6                               6"
                                                        "7                               7"
                                                        "8                               8"
                                                        "9                               9"
                                                        "A                               A"
                                                        "B                               B"
                                                        "C                               C"
                                                        "D                               D"
                                                        "E                               E"
                                                        "F                               F"
                                                        "</elem>");
    CXMLReader Reader(InStream);
    SXMLEntity Entity;
    
    EXPECT_TRUE(Reader.ReadEntity(Entity));
    EXPECT_EQ(Entity.DType, SXMLEntity::EType::StartElement);
    EXPECT_EQ(Entity.DNameData, "elem");
    EXPECT_EQ(Entity.DAttributes.size(), 0);
    
    EXPECT_TRUE(Reader.ReadEntity(Entity));
    EXPECT_EQ(Entity.DType, SXMLEntity::EType::CharData);
    EXPECT_EQ(Entity.DNameData, "0                               0"
                                "1                               1"
                                "2                               2"
                                "3                               3"
                                "4                               4"
                                "5                               5"
                                "6                               6"
                                "7                               7"
                                "8                               8"
                                "9                               9"
                                "A                               A"
                                "B                               B"
                                "C                               C"
                                "D                               D"
                                "E                               E"
                                "F                               F");
    EXPECT_EQ(Entity.DAttributes.size(), 0);
    
    EXPECT_TRUE(Reader.ReadEntity(Entity));
    EXPECT_EQ(Entity.DType, SXMLEntity::EType::EndElement);
    EXPECT_EQ(Entity.DNameData, "elem");
    EXPECT_EQ(Entity.DAttributes.size(), 0);
    
    EXPECT_TRUE(Reader.End());
}

TEST(XMLReaderTest, SpecialCharacterTest){
    auto InStream = std::make_shared<CStringDataSource>( "<elem attr=\"&amp;&quot;&apos;&lt;&gt;\">&amp;&quot;&apos;&lt;&gt;</elem>");
    CXMLReader Reader(InStream);
    SXMLEntity Entity;
    
    EXPECT_TRUE(Reader.ReadEntity(Entity));
    EXPECT_EQ(Entity.DType, SXMLEntity::EType::StartElement);
    EXPECT_EQ(Entity.DNameData, "elem");
    EXPECT_EQ(Entity.DAttributes.size(), 1);
    EXPECT_TRUE(Entity.AttributeExists("attr"));
    EXPECT_EQ(Entity.AttributeValue("attr"), "&\"'<>");
    
    EXPECT_TRUE(Reader.ReadEntity(Entity));
    EXPECT_EQ(Entity.DType, SXMLEntity::EType::CharData);
    EXPECT_EQ(Entity.DNameData, "&\"'<>");
    EXPECT_EQ(Entity.DAttributes.size(), 0);
    
    EXPECT_TRUE(Reader.ReadEntity(Entity));
    EXPECT_EQ(Entity.DType, SXMLEntity::EType::EndElement);
    EXPECT_EQ(Entity.DNameData, "elem");
    EXPECT_EQ(Entity.DAttributes.size(), 0);
    
    EXPECT_TRUE(Reader.End());
}

TEST(XMLWriterTest, SimpleTest){
    auto OutStream = std::make_shared<CStringDataSink>();
    CXMLWriter Writer(OutStream);
    
    EXPECT_TRUE(Writer.WriteEntity({SXMLEntity::EType::StartElement, "element", {{"name","val"}}}));
    EXPECT_TRUE(Writer.WriteEntity({SXMLEntity::EType::EndElement, "element", {}}));

    EXPECT_EQ(OutStream->String(), "<element name=\"val\"></element>");
}

TEST(XMLWriterTest, ElementTest){
    auto OutStream = std::make_shared<CStringDataSink>();
    CXMLWriter Writer(OutStream);
    
    EXPECT_TRUE(Writer.WriteEntity({SXMLEntity::EType::StartElement, "osm", {{"version","0.6"},{"generator","osmconvert 0.8.5"}}}));
    EXPECT_TRUE(Writer.WriteEntity({SXMLEntity::EType::CharData, "\n\t", {}}));
    
    EXPECT_TRUE(Writer.WriteEntity({SXMLEntity::EType::CompleteElement, "node", {{"id","62208369"},{"lat","38.5178523"},{"lon","-121.7712408"}}}));
    EXPECT_TRUE(Writer.WriteEntity({SXMLEntity::EType::CharData, "\n\t", {}}));
    
    EXPECT_TRUE(Writer.WriteEntity({SXMLEntity::EType::CompleteElement, "node", {{"id","62209104"},{"lat","38.535052"},{"lon","-121.7408606"}}}));
    EXPECT_TRUE(Writer.WriteEntity({SXMLEntity::EType::CharData, "\n", {}}));
    
    EXPECT_TRUE(Writer.WriteEntity({SXMLEntity::EType::EndElement, "osm", {}}));

    EXPECT_EQ(OutStream->String(),  "<osm version=\"0.6\" generator=\"osmconvert 0.8.5\">\n"
                                    "	<node id=\"62208369\" lat=\"38.5178523\" lon=\"-121.7712408\"/>\n"
                                    "	<node id=\"62209104\" lat=\"38.535052\" lon=\"-121.7408606\"/>\n"
                                    "</osm>");
}

TEST(XMLWriterTest, FlushTest){
    auto OutStream = std::make_shared<CStringDataSink>();
    CXMLWriter Writer(OutStream);
    
    EXPECT_TRUE(Writer.WriteEntity({SXMLEntity::EType::StartElement, "osm", {{"version","0.6"},{"generator","osmconvert 0.8.5"}}}));
    EXPECT_TRUE(Writer.WriteEntity({SXMLEntity::EType::CharData, "\n\t", {}}));
    
    EXPECT_TRUE(Writer.WriteEntity({SXMLEntity::EType::CompleteElement, "node", {{"id","62208369"},{"lat","38.5178523"},{"lon","-121.7712408"}}}));
    EXPECT_TRUE(Writer.WriteEntity({SXMLEntity::EType::CharData, "\n\t", {}}));
    
    EXPECT_TRUE(Writer.WriteEntity({SXMLEntity::EType::CompleteElement, "node", {{"id","62209104"},{"lat","38.535052"},{"lon","-121.7408606"}}}));
    EXPECT_TRUE(Writer.WriteEntity({SXMLEntity::EType::CharData, "\n", {}}));
    
    EXPECT_TRUE(Writer.Flush());

    EXPECT_EQ(OutStream->String(),  "<osm version=\"0.6\" generator=\"osmconvert 0.8.5\">\n"
                                    "	<node id=\"62208369\" lat=\"38.5178523\" lon=\"-121.7712408\"/>\n"
                                    "	<node id=\"62209104\" lat=\"38.535052\" lon=\"-121.7408606\"/>\n"
                                    "</osm>");
}


TEST(XMLWriterTest, SpecialCharacterTest){
    auto OutStream = std::make_shared<CStringDataSink>();
    CXMLWriter Writer(OutStream);
    
    EXPECT_TRUE(Writer.WriteEntity({SXMLEntity::EType::StartElement, "elem", {{"attr","&\"'<>"}}}));
    
    EXPECT_TRUE(Writer.WriteEntity({SXMLEntity::EType::CharData, "&\"'<>", {}}));
    
    EXPECT_TRUE(Writer.WriteEntity({SXMLEntity::EType::EndElement, "elem", {}}));

    EXPECT_EQ(OutStream->String(), "<elem attr=\"&amp;&quot;&apos;&lt;&gt;\">&amp;&quot;&apos;&lt;&gt;</elem>");
}
