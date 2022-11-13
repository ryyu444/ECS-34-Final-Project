#ifndef STRINGUTILS_H
#define STRINGUTILS_H

#include <string>
#include <vector>

namespace StringUtils{
    
std::string Slice(const std::string &str, ssize_t start, ssize_t end=0) noexcept;
std::string Capitalize(const std::string &str) noexcept;
std::string Upper(const std::string &str) noexcept;
std::string Lower(const std::string &str) noexcept;
std::string LStrip(const std::string &str) noexcept;
std::string RStrip(const std::string &str) noexcept;
std::string Strip(const std::string &str) noexcept;
std::string Center(const std::string &str, int width, char fill = ' ') noexcept;
std::string LJust(const std::string &str, int width, char fill = ' ') noexcept;
std::string RJust(const std::string &str, int width, char fill = ' ') noexcept;
std::string Replace(const std::string &str, const std::string &old, const std::string &rep) noexcept;
std::vector< std::string > Split(const std::string &str, const std::string &splt = "") noexcept;
std::string Join(const std::string &str, const std::vector< std::string > &vect) noexcept;
std::string ExpandTabs(const std::string &str, int tabsize = 4) noexcept;
int EditDistance(const std::string &left, const std::string &right, bool ignorecase=false) noexcept;

}

#endif
