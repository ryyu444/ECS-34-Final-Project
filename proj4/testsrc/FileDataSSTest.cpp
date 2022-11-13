#include <gtest/gtest.h>
#include "FileDataFactory.h"
#include "FileDataSink.h"
#include "FileDataSource.h"
#include <cstdio>

// Assume being run from Makefile so testtmp is subdirectory

const std::string BaseDirectory = "./testtmp/";

TEST(FileDataSourceSink, EmptyTest){
    CFileDataFactory DataFactory(BaseDirectory);
    std::string Filename = "empty.txt";
    std::remove((BaseDirectory + Filename).c_str());
    {
        auto Sink = DataFactory.CreateSink(Filename);
    }
    auto Source = DataFactory.CreateSource(Filename);
    EXPECT_TRUE(Source->End());
}

TEST(FileDataSourceSink, PutTest){
    CFileDataFactory DataFactory(BaseDirectory);
    std::string Filename = "put.txt";
    std::remove((BaseDirectory + Filename).c_str());
    {
        auto Sink = DataFactory.CreateSink(Filename);
        for(char Ch = ' '; Ch < '~'; Ch++){
            EXPECT_TRUE(Sink->Put(Ch));
        }
    }
    auto Source = DataFactory.CreateSource(Filename);
    for(char Ch = ' '; Ch < '~'; Ch++){
        char TempCh;
        EXPECT_TRUE(Source->Get(TempCh));
        EXPECT_EQ(Ch,TempCh);
    }
    EXPECT_TRUE(Source->End());
}

TEST(FileDataSourceSink, WriteTest){
    CFileDataFactory DataFactory(BaseDirectory);
    std::string Filename = "write.txt";
    std::remove((BaseDirectory + Filename).c_str());
    std::vector<char> OutBuffer, InBuffer;
    for(char Ch = ' '; Ch < '~'; Ch++){
        OutBuffer.push_back(Ch);
    }
    {
        auto Sink = DataFactory.CreateSink(Filename);
        EXPECT_TRUE(Sink->Write(OutBuffer));
    }
    auto Source = DataFactory.CreateSource(Filename);
    EXPECT_TRUE(Source->Read(InBuffer,OutBuffer.size()));
    EXPECT_EQ(InBuffer,OutBuffer);
    EXPECT_TRUE(Source->End());
}
