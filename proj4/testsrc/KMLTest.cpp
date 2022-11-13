#include <gtest/gtest.h>
#include "KMLWriter.h"
#include "StringUtils.h"
#include "StringDataSource.h"
#include "StringDataSink.h"

TEST(KMLWriterTest, SimpleTest){
    auto OutStream = std::make_shared<CStringDataSink>();
    {
        CKMLWriter KMLWriter(OutStream,"Simple","Simple KML test");
    }
    
    EXPECT_EQ(OutStream->String(),  "<?xml version='1.0' encoding='UTF-8'?>\n"
                                    "<kml xmlns=\"http://www.opengis.net/kml/2.2\">\n"
                                    "  <Document>\n"
                                    "    <name>Simple</name>\n"
                                    "    <description>Simple KML test</description>\n"
                                    "  </Document>\n"
                                    "</kml>");
}

TEST(KMLWriterTest, PointStyleTest){
    auto OutStream = std::make_shared<CStringDataSink>();
    {
        CKMLWriter KMLWriter(OutStream,"PointStyle","Point Style KML test");
        EXPECT_TRUE(KMLWriter.CreatePointStyle("PointStyleID",0xff123456));

    }
    
    EXPECT_EQ(OutStream->String(),  "<?xml version='1.0' encoding='UTF-8'?>\n"
                                    "<kml xmlns=\"http://www.opengis.net/kml/2.2\">\n"
                                    "  <Document>\n"
                                    "    <name>PointStyle</name>\n"
                                    "    <description>Point Style KML test</description>\n"
                                    "    <Style id=\"PointStyleID\">\n"
                                    "      <Point>\n"
                                    "        <color>ff123456</color>\n"
                                    "      </Point>\n"
                                    "    </Style>\n"
                                    "  </Document>\n"
                                    "</kml>");
}

TEST(KMLWriterTest, LineStyleTest){
    auto OutStream = std::make_shared<CStringDataSink>();
    {
        CKMLWriter KMLWriter(OutStream,"LineStyle","Line Style KML test");
        EXPECT_TRUE(KMLWriter.CreateLineStyle("LineStyleID",0xff123456,4));

    }
    
    EXPECT_EQ(OutStream->String(),  "<?xml version='1.0' encoding='UTF-8'?>\n"
                                    "<kml xmlns=\"http://www.opengis.net/kml/2.2\">\n"
                                    "  <Document>\n"
                                    "    <name>LineStyle</name>\n"
                                    "    <description>Line Style KML test</description>\n"
                                    "    <Style id=\"LineStyleID\">\n"
                                    "      <LineStyle>\n"
                                    "        <color>ff123456</color>\n"
                                    "        <width>4</width>\n"
                                    "      </LineStyle>\n"
                                    "    </Style>\n"
                                    "  </Document>\n"
                                    "</kml>");
}

TEST(KMLWriterTest, PointTest){
    auto OutStream = std::make_shared<CStringDataSink>();
    {
        CKMLWriter KMLWriter(OutStream,"Point","Point KML test");
        EXPECT_TRUE(KMLWriter.CreatePointStyle("PointStyleID",0xff123456));
        EXPECT_TRUE(KMLWriter.CreatePoint("PointName","This is a point.","PointStyleID",std::make_pair(38.5,-121.7)));
    }
    
    EXPECT_EQ(OutStream->String(),  "<?xml version='1.0' encoding='UTF-8'?>\n"
                                    "<kml xmlns=\"http://www.opengis.net/kml/2.2\">\n"
                                    "  <Document>\n"
                                    "    <name>Point</name>\n"
                                    "    <description>Point KML test</description>\n"
                                    "    <Style id=\"PointStyleID\">\n"
                                    "      <Point>\n"
                                    "        <color>ff123456</color>\n"
                                    "      </Point>\n"
                                    "    </Style>\n"
                                    "    <Placemark>\n"
                                    "      <name>PointName</name>\n"
                                    "      <description>This is a point.</description>\n"
                                    "      <styleUrl>#PointStyleID</styleUrl>\n"
                                    "      <Point>\n"
                                    "        <tessellate>1</tessellate>\n"
                                    "        <altitudeMode>relativeToGround</altitudeMode>\n"
                                    "        <coordinates>\n"
                                    "          -121.700000,38.500000\n"
                                    "        </coordinates>\n"
                                    "      </Point>\n"
                                    "    </Placemark>\n"
                                    "  </Document>\n"
                                    "</kml>");
}

TEST(KMLWriterTest, PathTest){
    auto OutStream = std::make_shared<CStringDataSink>();
    {
        CKMLWriter KMLWriter(OutStream,"Path","Path KML test");
        EXPECT_TRUE(KMLWriter.CreateLineStyle("LineStyleID",0xff123456,4));
        EXPECT_TRUE(KMLWriter.CreatePath("PathName","LineStyleID",{{38.5,-121.7},{38.6,-121.8},{38.7,-121.7}}));
    }
    
    EXPECT_EQ(OutStream->String(),  "<?xml version='1.0' encoding='UTF-8'?>\n"
                                    "<kml xmlns=\"http://www.opengis.net/kml/2.2\">\n"
                                    "  <Document>\n"
                                    "    <name>Path</name>\n"
                                    "    <description>Path KML test</description>\n"
                                    "    <Style id=\"LineStyleID\">\n"
                                    "      <LineStyle>\n"
                                    "        <color>ff123456</color>\n"
                                    "        <width>4</width>\n"
                                    "      </LineStyle>\n"
                                    "    </Style>\n"
                                    "    <Placemark>\n"
                                    "      <name>PathName</name>\n"
                                    "      <styleUrl>#LineStyleID</styleUrl>\n"
                                    "      <LineString>\n"
                                    "        <tessellate>1</tessellate>\n"
                                    "        <altitudeMode>relativeToGround</altitudeMode>\n"
                                    "        <coordinates>\n"
                                    "          -121.700000,38.500000\n"
                                    "          -121.800000,38.600000\n"
                                    "          -121.700000,38.700000\n"
                                    "        </coordinates>\n"
                                    "      </LineString>\n"
                                    "    </Placemark>\n"
                                    "  </Document>\n"
                                    "</kml>");
}