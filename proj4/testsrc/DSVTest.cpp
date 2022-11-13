#include <gtest/gtest.h>
#include "DSVReader.h"
#include "DSVWriter.h"
#include "StringUtils.h"
#include "StringDataSource.h"
#include "StringDataSink.h"

TEST(DSVWriter, EmptyTest){
    auto DSVSink = std::make_shared<CStringDataSink>();
    CDSVWriter DSVWriter(DSVSink,',');

    EXPECT_EQ(DSVSink->String(),"");
}

TEST(DSVWriter, SimpleTest){
    auto DSVSink = std::make_shared<CStringDataSink>();
    CDSVWriter DSVWriter(DSVSink,',');
    std::vector<std::string> StringVector = {"How", "are", "you", "doing?"};

    EXPECT_TRUE(DSVWriter.WriteRow(StringVector));
    EXPECT_EQ(DSVSink->String(),StringUtils::Join(",",StringVector));
}

TEST(DSVWriter, NewlineTest){
    auto DSVSink = std::make_shared<CStringDataSink>();
    CDSVWriter DSVWriter(DSVSink,',');
    std::vector<std::string> StringVector = {"How", "are", "you", "doing?"};

    EXPECT_TRUE(DSVWriter.WriteRow(StringVector));
    std::string JoinedRow = StringUtils::Join(",",StringVector);
    EXPECT_TRUE(DSVWriter.WriteRow(StringVector));
    EXPECT_EQ(DSVSink->String(),StringUtils::Join("\n",{JoinedRow,JoinedRow}));
}

TEST(DSVWriter, QuotingTest){
    auto DSVSink = std::make_shared<CStringDataSink>();
    CDSVWriter DSVWriter(DSVSink,',');
    std::vector<std::string> StringVector = {"1,000", "My name is \"Bob\"!", "3.3"};

    EXPECT_TRUE(DSVWriter.WriteRow(StringVector));
    EXPECT_EQ(DSVSink->String(),"\"1,000\",\"My name is \"\"Bob\"\"!\",3.3");
}

TEST(DSVWriter, QuoteAllTest){
    auto DSVSink = std::make_shared<CStringDataSink>();
    CDSVWriter DSVWriter(DSVSink,',',true);
    std::vector<std::string> StringVector = {"a","b","c","d"};

    EXPECT_TRUE(DSVWriter.WriteRow(StringVector));
    EXPECT_EQ(DSVSink->String(),"\"a\",\"b\",\"c\",\"d\"");
}

TEST(DSVReader, EmptyTest){
    auto DSVSource = std::make_shared<CStringDataSource>("");
    CDSVReader DSVReader(DSVSource,',');
    std::vector<std::string> StringVector;

    EXPECT_FALSE(DSVReader.ReadRow(StringVector));
    EXPECT_TRUE(DSVReader.End());
}

TEST(DSVReader, SimpleTest){
    auto DSVSource = std::make_shared<CStringDataSource>("How,are,you,doing?");
    CDSVReader DSVReader(DSVSource,',');
    std::vector<std::string> StringVector;

    EXPECT_TRUE(DSVReader.ReadRow(StringVector));
    EXPECT_TRUE(DSVReader.End());
    ASSERT_EQ(StringVector.size(),4);
    EXPECT_EQ(StringVector[0],"How");
    EXPECT_EQ(StringVector[1],"are");
    EXPECT_EQ(StringVector[2],"you");
    EXPECT_EQ(StringVector[3],"doing?");
}

TEST(DSVReader, NewlineTest){
    auto DSVSource = std::make_shared<CStringDataSource>("How,are,you,doing?\nI'm,doing,great!");
    CDSVReader DSVReader(DSVSource,',');
    std::vector<std::string> StringVector;

    EXPECT_TRUE(DSVReader.ReadRow(StringVector));
    EXPECT_FALSE(DSVReader.End());
    ASSERT_EQ(StringVector.size(),4);
    EXPECT_EQ(StringVector[0],"How");
    EXPECT_EQ(StringVector[1],"are");
    EXPECT_EQ(StringVector[2],"you");
    EXPECT_EQ(StringVector[3],"doing?");
    EXPECT_TRUE(DSVReader.ReadRow(StringVector));
    ASSERT_EQ(StringVector.size(),3);
    EXPECT_EQ(StringVector[0],"I'm");
    EXPECT_EQ(StringVector[1],"doing");
    EXPECT_EQ(StringVector[2],"great!");
    EXPECT_TRUE(DSVReader.End());
}

TEST(DSVReader, QuotingTest){
    auto DSVSource = std::make_shared<CStringDataSource>("\"1,000\",\"My name is \"\"Bob\"\"!\",3.3");
    CDSVReader DSVReader(DSVSource,',');
    std::vector<std::string> StringVector;

    EXPECT_TRUE(DSVReader.ReadRow(StringVector));
    EXPECT_TRUE(DSVReader.End());
    ASSERT_EQ(StringVector.size(),3);
    EXPECT_EQ(StringVector[0],"1,000");
    EXPECT_EQ(StringVector[1],"My name is \"Bob\"!");
    EXPECT_EQ(StringVector[2],"3.3");    
}