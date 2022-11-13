#include "StandardDataSink.h"
#include <iostream>

bool CStandardDataSink::Put(const char &ch) noexcept{
    std::cout.put(ch);
    return std::cout.good();
}

bool CStandardDataSink::Write(const std::vector<char> &buf) noexcept{
    std::cout.write(buf.data(),buf.size());
    return std::cout.good();
}