#include "StandardDataSource.h"
#include <iostream>

bool CStandardDataSource::End() const noexcept{
    return std::cin.eof();
}

bool CStandardDataSource::Get(char &ch) noexcept{
    if(!std::cin.good()){
        return false;
    }
    ch = std::cin.get();
    if((ch != EOF)&&(ch != '\n')){
        std::cin.peek();
    }
    return ch != EOF;
}

bool CStandardDataSource::Peek(char &ch) noexcept{
    if(!std::cin.good()){
        return false;
    }
    ch = std::cin.peek();
    return ch != EOF;
}

bool CStandardDataSource::Read(std::vector<char> &buf, std::size_t count) noexcept{
    if(!std::cin.good()){
        return false;
    }
    char TempCh;
    buf.clear();
    buf.reserve(count);
    while(buf.size() < count && Get(TempCh)){
        buf.push_back(TempCh);
    }
    return !buf.empty();
}
