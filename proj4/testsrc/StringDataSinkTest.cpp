#include <gtest/gtest.h>
#include "StringDataSink.h"

TEST(StringDataSink, EmptyTest){
    CStringDataSink EmptySink;
    
    EXPECT_TRUE(EmptySink.String().empty());
    EXPECT_EQ(EmptySink.String(),"");
}

TEST(StringDataSink, PutTest){
    CStringDataSink Sink;
    
    EXPECT_TRUE(Sink.Put('H'));
    EXPECT_TRUE(Sink.Put('e'));
    EXPECT_TRUE(Sink.Put('l'));
    EXPECT_TRUE(Sink.Put('l'));
    EXPECT_EQ(Sink.String(),"Hell");
    EXPECT_TRUE(Sink.Put('o'));
    EXPECT_EQ(Sink.String(),"Hello");
}

TEST(StringDataSink, WriteTest){
    std::vector<char> TempVector1 = {'H','e','l','l','o'};
    std::vector<char> TempVector2 = {' ','W','o','r','l','d'};
    CStringDataSink Sink;

    EXPECT_TRUE(Sink.Write(TempVector1));
    EXPECT_EQ(Sink.String(),"Hello");
    EXPECT_TRUE(Sink.Write(TempVector2));
    EXPECT_EQ(Sink.String(),"Hello World");   
}
