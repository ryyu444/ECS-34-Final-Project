#include <gtest/gtest.h>
#include "StringDataSource.h"

TEST(StringDataSource, EndTest){
    CStringDataSource EmptySource("");
    CStringDataSource BaseSource("Hello");

    EXPECT_TRUE(EmptySource.End());
    EXPECT_FALSE(BaseSource.End());
}

TEST(StringDataSource, PeekTest){
    CStringDataSource EmptySource("");
    CStringDataSource Source1("Hello");
    CStringDataSource Source2("Bye");
    char TempCh = 'x';

    EXPECT_FALSE(EmptySource.Peek(TempCh));
    EXPECT_EQ(TempCh,'x');
    EXPECT_TRUE(Source1.Peek(TempCh));
    EXPECT_EQ(TempCh,'H');
    TempCh = 'x';
    EXPECT_TRUE(Source1.Peek(TempCh));
    EXPECT_EQ(TempCh,'H');
    EXPECT_TRUE(Source2.Peek(TempCh));
    EXPECT_EQ(TempCh,'B');
    TempCh = 'x';
    EXPECT_TRUE(Source2.Peek(TempCh));
    EXPECT_EQ(TempCh,'B');
}

TEST(StringDataSource, GetTest){
    CStringDataSource EmptySource("");
    CStringDataSource Source1("Hello");
    CStringDataSource Source2("Bye");
    char TempCh = 'x';

    EXPECT_FALSE(EmptySource.Get(TempCh));
    EXPECT_EQ(TempCh,'x');
    EXPECT_TRUE(Source1.Get(TempCh));
    EXPECT_EQ(TempCh,'H');
    TempCh = 'x';
    EXPECT_TRUE(Source1.Peek(TempCh));
    EXPECT_EQ(TempCh,'e');
    TempCh = 'x';
    EXPECT_TRUE(Source1.Peek(TempCh));
    EXPECT_EQ(TempCh,'e');
    EXPECT_TRUE(Source2.Get(TempCh));
    EXPECT_EQ(TempCh,'B');
    TempCh = 'x';
    EXPECT_TRUE(Source2.Peek(TempCh));
    EXPECT_EQ(TempCh,'y');
    TempCh = 'x';
    EXPECT_TRUE(Source2.Peek(TempCh));
    EXPECT_EQ(TempCh,'y');
}

TEST(StringDataSource, ReadTest){
    CStringDataSource EmptySource("");
    CStringDataSource Source1("Hello");
    CStringDataSource Source2("Bye");
    std::vector< char > TempVector;
    char TempCh = 'x';

    EXPECT_FALSE(EmptySource.Read(TempVector,3));
    EXPECT_EQ(TempVector.size(),0);
    EXPECT_TRUE(Source1.Read(TempVector,4));
    ASSERT_TRUE(TempVector.size() >= 4);
    EXPECT_EQ(TempVector[0],'H');
    EXPECT_EQ(TempVector[1],'e');
    EXPECT_EQ(TempVector[2],'l');
    EXPECT_EQ(TempVector[3],'l');
    EXPECT_TRUE(Source1.Peek(TempCh));
    EXPECT_EQ(TempCh,'o');
    EXPECT_TRUE(Source2.Read(TempVector,4));
    ASSERT_TRUE(TempVector.size() >= 3);
    EXPECT_EQ(TempVector[0],'B');
    EXPECT_EQ(TempVector[1],'y');
    EXPECT_EQ(TempVector[2],'e');
    TempCh = 'x';
    EXPECT_FALSE(Source2.Peek(TempCh));
    EXPECT_EQ(TempCh,'x');
}
