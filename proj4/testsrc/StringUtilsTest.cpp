#include <gtest/gtest.h>
#include "StringUtils.h"

TEST(StringUtilsTest, InstructorSliceTest){
    EXPECT_EQ(StringUtils::Slice("Hello World!",1), "ello World!");
    EXPECT_EQ(StringUtils::Slice("Hello World!",0,5), "Hello");
    EXPECT_EQ(StringUtils::Slice("Hello World!",0,-1), "Hello World");
    EXPECT_EQ(StringUtils::Slice("Hello World!",3,-2), "lo Worl");
    EXPECT_EQ(StringUtils::Slice("Hello World!",-5,-2), "orl");
    EXPECT_EQ(StringUtils::Slice("Hello World!",14), "");
}

TEST(StringUtilsTest, InstructorCapitalize){
    EXPECT_EQ(StringUtils::Capitalize("hello world!"), "Hello world!");
    EXPECT_EQ(StringUtils::Capitalize("HELLO WORLD!"), "Hello world!");
    EXPECT_EQ(StringUtils::Capitalize(" HELLO WORLD!"), " hello world!");
}

TEST(StringUtilsTest, InstructorUpper){
    EXPECT_EQ(StringUtils::Upper("hello world!"), "HELLO WORLD!");
    EXPECT_EQ(StringUtils::Upper("Hello World!"), "HELLO WORLD!");
    EXPECT_EQ(StringUtils::Upper("HELLO WORLD!"), "HELLO WORLD!");
    EXPECT_EQ(StringUtils::Upper(" HELLO WORLD!"), " HELLO WORLD!");
}

TEST(StringUtilsTest, InstructorLower){
    EXPECT_EQ(StringUtils::Lower("hello world!"), "hello world!");
    EXPECT_EQ(StringUtils::Lower("Hello World!"), "hello world!");
    EXPECT_EQ(StringUtils::Lower("HELLO WORLD!"), "hello world!");
    EXPECT_EQ(StringUtils::Lower(" HELLO WORLD!"), " hello world!");
}

TEST(StringUtilsTest, InstructorLStrip){
    std::string Str1 = "    Test String    ";
    std::string Str2 = " \t \r\n Test String";
    std::string Str3 = "Test String \t \r\n ";
    EXPECT_EQ(StringUtils::LStrip(Str1), "Test String    ");
    EXPECT_EQ(StringUtils::LStrip(Str2), "Test String");
    EXPECT_EQ(StringUtils::LStrip(Str3), Str3);
}

TEST(StringUtilsTest, InstructorRStrip){
    std::string Str1 = "    Test String    ";
    std::string Str2 = " \t \r\n Test String";
    std::string Str3 = "Test String \t \r\n ";
    EXPECT_EQ(StringUtils::RStrip(Str1), "    Test String");
    EXPECT_EQ(StringUtils::RStrip(Str2),Str2);
    EXPECT_EQ(StringUtils::RStrip(Str3), "Test String");
}

// TEST(StringUtilsTest, InstructorStrip){
//     std::string Str1 = "    Test String    ";
//     std::string Str2 = " \t \r\n Test String";
//     std::string Str3 = "Test String \t \r\n ";
//     EXPECT_EQ(StringUtils::Strip(Str1), "Test String");
//     EXPECT_EQ(StringUtils::Strip(Str2), "Test String");
//     EXPECT_EQ(StringUtils::Strip(Str3), "Test String");
//     EXPECT_EQ(StringUtils::Strip(Str1),StringUtils::RStrip(StringUtils::LStrip(Str1)));
//     EXPECT_EQ(StringUtils::Strip(Str2),StringUtils::RStrip(StringUtils::LStrip(Str2)));
//     EXPECT_EQ(StringUtils::Strip(Str3),StringUtils::RStrip(StringUtils::LStrip(Str3)));
// }

TEST(StringUtilsTest, InstructorCenter){
    EXPECT_EQ(StringUtils::Center("Test String",19), "    Test String    ");
    EXPECT_EQ(StringUtils::Center("Test String",16,'-'), "--Test String---");
    EXPECT_EQ(StringUtils::Center("Test String",5,'-'), "Test String");
}

TEST(StringUtilsTest, InstructorLJust){
    EXPECT_EQ(StringUtils::LJust("Test String",19), "Test String        ");
    EXPECT_EQ(StringUtils::LJust("Test String",5), "Test String");
}

TEST(StringUtilsTest, InstructorRJust){
    EXPECT_EQ(StringUtils::RJust("Test String",19), "        Test String");
    EXPECT_EQ(StringUtils::RJust("Test String",5), "Test String");
}

TEST(StringUtilsTest, InstructorReplace){
    EXPECT_EQ(StringUtils::Replace("\\Path\\To\\A\\Directory","\\","/"), "/Path/To/A/Directory");
    EXPECT_EQ(StringUtils::Replace("a = b + c * b + d","b","e"), "a = e + c * e + d");
    EXPECT_EQ(StringUtils::Replace("aabbccaaabbbcccaaaa","aa","ee"), "eebbcceeabbbccceeee");
}

TEST(StringUtilsTest, InstructorSplit){
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

TEST(StringUtilsTest, InstructorJoin){
    std::vector<std::string> PathComponents = {"", "Path", "To", "A", "Directory"};
    EXPECT_EQ(StringUtils::Join("/",PathComponents), "/Path/To/A/Directory");
    
    std::vector<std::string>  SentenceComponents = {"A","tougher","test","to","pass!"};
    EXPECT_EQ(StringUtils::Join(" ",SentenceComponents), "A tougher test to pass!");
}

TEST(StringUtilsTest, InstructorExpandTabs){
    EXPECT_EQ(StringUtils::ExpandTabs("1\t2\t3\t4"), "1   2   3   4");
    EXPECT_EQ(StringUtils::ExpandTabs("1\t12\t123\t1234"), "1   12  123 1234");
    EXPECT_EQ(StringUtils::ExpandTabs("1234\t123\t12\t1"), "1234    123 12  1");
    EXPECT_EQ(StringUtils::ExpandTabs("1234\t123\t12\t1",3), "1234  123   12 1");
    EXPECT_EQ(StringUtils::ExpandTabs("1234\t123\t12\t1",0), "1234123121");
}

TEST(StringUtilsTest, InstructorEditDistance){
    EXPECT_EQ(StringUtils::EditDistance("1234","1234"), 0);
    EXPECT_EQ(StringUtils::EditDistance("Test","test"), 1);
    EXPECT_EQ(StringUtils::EditDistance("Test","test",true), 0);
    EXPECT_EQ(StringUtils::EditDistance("12345","52341"), 2);
    EXPECT_EQ(StringUtils::EditDistance("This is an example","This is a sample"), 3);
    EXPECT_EQ(StringUtils::EditDistance("int Var = Other + 3.4;","int x = y + 3.4;"), 8);
}

TEST(StringUtilsTest, SliceTest) {
    EXPECT_EQ(StringUtils::Slice("hello", 0), "hello");
    EXPECT_EQ(StringUtils::Slice("hello", 0, 0), "hello");
    EXPECT_EQ(StringUtils::Slice("hello", 0, 10), "hello");
    EXPECT_EQ(StringUtils::Slice("hello", 2, 4), "ll");
    EXPECT_EQ(StringUtils::Slice("hello", 2), "llo");
    EXPECT_EQ(StringUtils::Slice("hello", 3, -2), "");
    EXPECT_EQ(StringUtils::Slice("hello", -3, -1), "ll");
    EXPECT_EQ(StringUtils::Slice("hello", 3, -1), "l");
    EXPECT_EQ(StringUtils::Slice("hello", -2, 0), "lo");
    EXPECT_EQ(StringUtils::Slice("hello", -2), "lo");
    EXPECT_EQ(StringUtils::Slice("hello", -5), "hello");
}

TEST(StringUtilsTest, Capitalize) {
    EXPECT_EQ(StringUtils::Capitalize("hello world"), "Hello world");
    EXPECT_EQ(StringUtils::Capitalize("say my name"), "Say my name");
    EXPECT_EQ(StringUtils::Capitalize("domain expansion: chimera shadow Garden!!!"), "Domain expansion: chimera shadow garden!!!");
    EXPECT_EQ(StringUtils::Capitalize("have some more chicken\nHave Some More Pie"), "Have some more chicken\nhave some more pie");
}

TEST(StringUtilsTest, Upper) {
    EXPECT_EQ(StringUtils::Upper("hello world"), "HELLO WORLD");
    EXPECT_EQ(StringUtils::Upper("no, i am not angry!!!"), "NO, I AM NOT ANGRY!!!");
    EXPECT_EQ(StringUtils::Upper("a b C d EFg"), "A B C D EFG");
}

TEST(StringUtilsTest, Lower) {
    EXPECT_EQ(StringUtils::Lower("hello WOrld"), "hello world");
    EXPECT_EQ(StringUtils::Lower("AAAAAAAA\n\tHAHAHAHAHAHHA"), "aaaaaaaa\n\thahahahahahha");
    EXPECT_EQ(StringUtils::Lower("\n\ta b C d EFg"), "\n\ta b c d efg");
}

TEST(StringUtilsTest, LStrip) {
    EXPECT_EQ(StringUtils::LStrip("    how do you do"), "how do you do");
    EXPECT_EQ(StringUtils::LStrip("\nhow do you do"), "how do you do");
    EXPECT_EQ(StringUtils::LStrip("\thow do you do"), "how do you do");
    EXPECT_EQ(StringUtils::LStrip("\n  \te  \t\n   "), "e  \t\n   ");
    EXPECT_EQ(StringUtils::LStrip("     "), "");
    EXPECT_EQ(StringUtils::RStrip(" \n\t  \n "), "");

}

TEST(StringUtilsTest, RStrip) {
    EXPECT_EQ(StringUtils::RStrip("how do you do    "), "how do you do");
    EXPECT_EQ(StringUtils::RStrip("how do you do\n"), "how do you do");
    EXPECT_EQ(StringUtils::RStrip("how do you do\t"), "how do you do");
    EXPECT_EQ(StringUtils::RStrip("\n  \te   \n\t"), "\n  \te");
    EXPECT_EQ(StringUtils::RStrip("     "), "");
    EXPECT_EQ(StringUtils::RStrip(" \n\t  \n "), "");
}

TEST(StringUtilsTest, Strip) {
    EXPECT_EQ(StringUtils::Strip("   hello   "), "hello");
    EXPECT_EQ(StringUtils::Strip("\nhello\n"), "hello");
    EXPECT_EQ(StringUtils::Strip("\thello\t"), "hello");
    EXPECT_EQ(StringUtils::Strip("\t\n    hello\n  \t"), "hello");
    EXPECT_EQ(StringUtils::Strip("\n  \te  \t\n   "), "e");
    EXPECT_EQ(StringUtils::Strip("     "), "");
    EXPECT_EQ(StringUtils::RStrip(" \n\t  \n "), "");

}

TEST(StringUtilsTest, Center) {
    EXPECT_EQ(StringUtils::Center("hello", 10), "  hello   ");
    EXPECT_EQ(StringUtils::Center("hello", 2), "hello");
    EXPECT_EQ(StringUtils::Center("hello", 6), "hello ");
    EXPECT_EQ(StringUtils::Center(" my name is", 20), "     my name is     ");
    EXPECT_EQ(StringUtils::Center(" my name is", 12), " my name is ");
    EXPECT_EQ(StringUtils::Center(" my name is", 13), "  my name is ");
    EXPECT_EQ(StringUtils::Center(" my name is", 20, '.'), ".... my name is.....");
    EXPECT_EQ(StringUtils::Center(" my name is", 12, 'e'), " my name ise");
    EXPECT_EQ(StringUtils::Center(" my name is", 13, '~'), "~ my name is~");
}

TEST(StringUtilsTest, LJust) {
    EXPECT_EQ(StringUtils::LJust("hello", 10), "hello     ");
    EXPECT_EQ(StringUtils::LJust("hello", 2), "hello");
    EXPECT_EQ(StringUtils::LJust("hello", 6), "hello ");
    EXPECT_EQ(StringUtils::LJust(" my name is", 20), " my name is         ");
    EXPECT_EQ(StringUtils::LJust(" my name is", 12), " my name is ");
    EXPECT_EQ(StringUtils::LJust(" my name is", 13), " my name is  ");
    EXPECT_EQ(StringUtils::LJust(" my name is", 20, '.'), " my name is.........");
    EXPECT_EQ(StringUtils::LJust(" my name is", 12, 'e'), " my name ise");
    EXPECT_EQ(StringUtils::LJust(" my name is", 13, '~'), " my name is~~");
}

TEST(StringUtilsTest, RJust) {
    EXPECT_EQ(StringUtils::RJust("hello", 10), "     hello");
    EXPECT_EQ(StringUtils::RJust("hello", 2), "hello");
    EXPECT_EQ(StringUtils::RJust("hello", 6), " hello");
    EXPECT_EQ(StringUtils::RJust(" my name is", 20), "          my name is");
    EXPECT_EQ(StringUtils::RJust(" my name is", 12), "  my name is");
    EXPECT_EQ(StringUtils::RJust(" my name is", 13), "   my name is");
    EXPECT_EQ(StringUtils::RJust(" my name is", 20, '.'), "......... my name is");
    EXPECT_EQ(StringUtils::RJust(" my name is", 12, 'e'), "e my name is");
    EXPECT_EQ(StringUtils::RJust(" my name is", 13, '~'), "~~ my name is");
}

TEST(StringUtilsTest, Replace) {
    EXPECT_EQ(StringUtils::Replace("hello", "hello", "hello"), "hello");
    EXPECT_EQ(StringUtils::Replace("hello", "hell", "heaven"), "heaveno");
    EXPECT_EQ(StringUtils::Replace("hello", "he", "she"), "shello");
    EXPECT_EQ(StringUtils::Replace("hello", "something", "literally anything"), "hello");
    EXPECT_EQ(StringUtils::Replace("hello", "hellow", "x"), "hello");
    EXPECT_EQ(StringUtils::Replace("eeeeeeeeeee", "ee", "a"), "aaaaae");
    EXPECT_EQ(StringUtils::Replace("ee.eee.", "ee", "a"), "a.ae.");
    EXPECT_EQ(StringUtils::Replace("changeme", "changeme", "i have now been changed, poggers\n\n\t"), "i have now been changed, poggers\n\n\t");
}

TEST(StringUtilsTest, Split) {
    std::vector<std::string> tmpVec = {"word"};
    EXPECT_EQ(StringUtils::Split("word"), tmpVec);

    tmpVec = {"word"};
    EXPECT_EQ(StringUtils::Split("word "), tmpVec);

    tmpVec = {"word"};
    EXPECT_EQ(StringUtils::Split(" word "), tmpVec);

    tmpVec = {"word"};
    EXPECT_EQ(StringUtils::Split("   word "), tmpVec);

    tmpVec = {"word"};
    EXPECT_EQ(StringUtils::Split("   \n\tword "), tmpVec);

    tmpVec = {"", "word", ""};
    EXPECT_EQ(StringUtils::Split(" word ", " "), tmpVec);

    tmpVec = {"", "word", ""};
    EXPECT_EQ(StringUtils::Split("ccwordcc", "cc"), tmpVec);

    tmpVec = {" word "};
    EXPECT_EQ(StringUtils::Split(" word ", "\n"), tmpVec);

    tmpVec = {"hello,", "how", "are", "you", "?\n\t"};
    EXPECT_EQ(StringUtils::Split("hello, how are you ?\n\t", " "), tmpVec);

    tmpVec = {"hello,", "how", "are", "you", "?"};
    EXPECT_EQ(StringUtils::Split("hello, how are you ?  "), tmpVec);

    tmpVec = {"hello,", "", "how", "are", "you", "?\n\t"};
    EXPECT_EQ(StringUtils::Split("hello,  how are you ?\n\t", " "), tmpVec);

    tmpVec = {"word1", "\tword2"};
    EXPECT_EQ(StringUtils::Split("word1\n\tword2", "\n"), tmpVec);

    tmpVec = {"word1", "word2"};
    EXPECT_EQ(StringUtils::Split("word1word3word2", "word3"), tmpVec);
    
    tmpVec = {"word1", "word2"};
    EXPECT_EQ(StringUtils::Split("word1 word2"), tmpVec);
    EXPECT_EQ(StringUtils::Split("word1 word2", " "), tmpVec);

    
}

TEST(StringUtilsTest, Join) {
    std::vector<std::string> tmpVec = {"word1", "word2", "word3\n"};
    EXPECT_EQ(StringUtils::Join(" ", tmpVec), "word1 word2 word3\n");

    tmpVec = {"word1", "word2", "word3"};
    EXPECT_EQ(StringUtils::Join("word4", tmpVec), "word1word4word2word4word3");
}

TEST(StringUtilsTest, ExpandTabs) {

    // some examples taken from https://www.programiz.com/python-programming/methods/string/expandtabs 

    EXPECT_EQ(StringUtils::ExpandTabs("xyz\t12345\tabc", 8), "xyz     12345   abc");
    EXPECT_EQ(StringUtils::ExpandTabs("\txyz\t12345\tabc", 8), "        xyz     12345   abc");
    EXPECT_EQ(StringUtils::ExpandTabs("xyz\t12345\tabc", 2), "xyz 12345 abc");
    EXPECT_EQ(StringUtils::ExpandTabs("xyz\t12345\tabc", 3), "xyz   12345 abc");
    EXPECT_EQ(StringUtils::ExpandTabs("xyz\t12345\tabc", 4), "xyz 12345   abc");
    EXPECT_EQ(StringUtils::ExpandTabs("xyz\t12345\tabc"), "xyz 12345   abc");
    EXPECT_EQ(StringUtils::ExpandTabs("xyz\t12345\tabc", 5), "xyz  12345     abc");
    EXPECT_EQ(StringUtils::ExpandTabs("xyz\t12345\tabc", 6), "xyz   12345 abc");

}

TEST(StringUtilsTest, EditDistance) {
    
    EXPECT_EQ(StringUtils::EditDistance("right", "lefT"), 5);
    EXPECT_EQ(StringUtils::EditDistance("right", "lefT", true), 4);
    EXPECT_EQ(StringUtils::EditDistance("righT", "lefT"), 4);
    EXPECT_EQ(StringUtils::EditDistance("righT", "lefT", true), 4);
    EXPECT_EQ(StringUtils::EditDistance("my name is walter white", "say my name"), 18);
    EXPECT_EQ(StringUtils::EditDistance("cabbages", "rabbit"), 5);

}