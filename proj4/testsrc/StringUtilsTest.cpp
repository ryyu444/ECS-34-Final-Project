#include <gtest/gtest.h>
#include "StringUtils.h"

TEST(StringUtilsTest, SliceTest){
    EXPECT_EQ(StringUtils::Slice("Hello World!",1), "ello World!");
    EXPECT_EQ(StringUtils::Slice("Hello World!",0,5), "Hello");
    EXPECT_EQ(StringUtils::Slice("Hello World!",0,-1), "Hello World");
    EXPECT_EQ(StringUtils::Slice("Hello World!",3,-2), "lo Worl");
    EXPECT_EQ(StringUtils::Slice("Hello World!",-5,-2), "orl");
    EXPECT_EQ(StringUtils::Slice("Hello World!",14), "");
}

TEST(StringUtilsTest, Capitalize){
    EXPECT_EQ(StringUtils::Capitalize("hello world!"), "Hello world!");
    EXPECT_EQ(StringUtils::Capitalize("HELLO WORLD!"), "Hello world!");
    EXPECT_EQ(StringUtils::Capitalize(" HELLO WORLD!"), " hello world!");
}

TEST(StringUtilsTest, Upper){
    EXPECT_EQ(StringUtils::Upper("hello world!"), "HELLO WORLD!");
    EXPECT_EQ(StringUtils::Upper("Hello World!"), "HELLO WORLD!");
    EXPECT_EQ(StringUtils::Upper("HELLO WORLD!"), "HELLO WORLD!");
    EXPECT_EQ(StringUtils::Upper(" HELLO WORLD!"), " HELLO WORLD!");
}

TEST(StringUtilsTest, Lower){
    EXPECT_EQ(StringUtils::Lower("hello world!"), "hello world!");
    EXPECT_EQ(StringUtils::Lower("Hello World!"), "hello world!");
    EXPECT_EQ(StringUtils::Lower("HELLO WORLD!"), "hello world!");
    EXPECT_EQ(StringUtils::Lower(" HELLO WORLD!"), " hello world!");
}

TEST(StringUtilsTest, LStrip){
    std::string Str1 = "    Test String    ";
    std::string Str2 = " \t \r\n Test String";
    std::string Str3 = "Test String \t \r\n ";
    EXPECT_EQ(StringUtils::LStrip(Str1), "Test String    ");
    EXPECT_EQ(StringUtils::LStrip(Str2), "Test String");
    EXPECT_EQ(StringUtils::LStrip(Str3), Str3);
}

TEST(StringUtilsTest, RStrip){
    std::string Str1 = "    Test String    ";
    std::string Str2 = " \t \r\n Test String";
    std::string Str3 = "Test String \t \r\n ";
    EXPECT_EQ(StringUtils::RStrip(Str1), "    Test String");
    EXPECT_EQ(StringUtils::RStrip(Str2),Str2);
    EXPECT_EQ(StringUtils::RStrip(Str3), "Test String");
}

TEST(StringUtilsTest, Strip){
    std::string Str1 = "    Test String    ";
    std::string Str2 = " \t \r\n Test String";
    std::string Str3 = "Test String \t \r\n ";
    EXPECT_EQ(StringUtils::Strip(Str1), "Test String");
    EXPECT_EQ(StringUtils::Strip(Str2), "Test String");
    EXPECT_EQ(StringUtils::Strip(Str3), "Test String");
    EXPECT_EQ(StringUtils::Strip(Str1),StringUtils::RStrip(StringUtils::LStrip(Str1)));
    EXPECT_EQ(StringUtils::Strip(Str2),StringUtils::RStrip(StringUtils::LStrip(Str2)));
    EXPECT_EQ(StringUtils::Strip(Str3),StringUtils::RStrip(StringUtils::LStrip(Str3)));
}

TEST(StringUtilsTest, Center){
    EXPECT_EQ(StringUtils::Center("Test String",19), "    Test String    ");
    EXPECT_EQ(StringUtils::Center("Test String",16,'-'), "--Test String---");
    EXPECT_EQ(StringUtils::Center("Test String",5,'-'), "Test String");
}

TEST(StringUtilsTest, LJust){
    EXPECT_EQ(StringUtils::LJust("Test String",19), "Test String        ");
    EXPECT_EQ(StringUtils::LJust("Test String",5), "Test String");
}

TEST(StringUtilsTest, RJust){
    EXPECT_EQ(StringUtils::RJust("Test String",19), "        Test String");
    EXPECT_EQ(StringUtils::RJust("Test String",5), "Test String");
}

TEST(StringUtilsTest, Replace){
    EXPECT_EQ(StringUtils::Replace("\\Path\\To\\A\\Directory","\\","/"), "/Path/To/A/Directory");
    EXPECT_EQ(StringUtils::Replace("a = b + c * b + d","b","e"), "a = e + c * e + d");
    EXPECT_EQ(StringUtils::Replace("aabbccaaabbbcccaaaa","aa","ee"), "eebbcceeabbbccceeee");
}

TEST(StringUtilsTest, Split){
    auto PathComponents = StringUtils::Split("/Path/To/A/Directory","/");
    ASSERT_EQ(PathComponents.size(), 5);
    EXPECT_EQ(PathComponents[0], "");
    EXPECT_EQ(PathComponents[1], "Path");
    EXPECT_EQ(PathComponents[2], "To");
    EXPECT_EQ(PathComponents[3], "A");
    EXPECT_EQ(PathComponents[4], "Directory");
    
    auto SentenceComponents = StringUtils::Split("A  tougher\ttest\nto   pass!");
    ASSERT_EQ(SentenceComponents.size(), 5);
    EXPECT_EQ(SentenceComponents[0], "A");
    EXPECT_EQ(SentenceComponents[1], "tougher");
    EXPECT_EQ(SentenceComponents[2], "test");
    EXPECT_EQ(SentenceComponents[3], "to");
    EXPECT_EQ(SentenceComponents[4], "pass!");
}

TEST(StringUtilsTest, Join){
    std::vector<std::string> PathComponents = {"", "Path", "To", "A", "Directory"};
    EXPECT_EQ(StringUtils::Join("/",PathComponents), "/Path/To/A/Directory");
    
    std::vector<std::string>  SentenceComponents = {"A","tougher","test","to","pass!"};
    EXPECT_EQ(StringUtils::Join(" ",SentenceComponents), "A tougher test to pass!");
}

TEST(StringUtilsTest, ExpandTabs){
    EXPECT_EQ(StringUtils::ExpandTabs("1\t2\t3\t4"), "1   2   3   4");
    EXPECT_EQ(StringUtils::ExpandTabs("1\t12\t123\t1234"), "1   12  123 1234");
    EXPECT_EQ(StringUtils::ExpandTabs("1234\t123\t12\t1"), "1234    123 12  1");
    EXPECT_EQ(StringUtils::ExpandTabs("1234\t123\t12\t1",3), "1234  123   12 1");
    EXPECT_EQ(StringUtils::ExpandTabs("1234\t123\t12\t1",0), "1234123121");
}

TEST(StringUtilsTest, EditDistance){
    EXPECT_EQ(StringUtils::EditDistance("1234","1234"), 0);
    EXPECT_EQ(StringUtils::EditDistance("Test","test"), 1);
    EXPECT_EQ(StringUtils::EditDistance("Test","test",true), 0);
    EXPECT_EQ(StringUtils::EditDistance("12345","52341"), 2);
    EXPECT_EQ(StringUtils::EditDistance("This is an example","This is a sample"), 3);
    EXPECT_EQ(StringUtils::EditDistance("int Var = Other + 3.4;","int x = y + 3.4;"), 8);
}
