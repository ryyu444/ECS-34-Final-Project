#include "StringDataSource.h"

CStringDataSource::CStringDataSource(const std::string &str) : DString(str), DIndex(0){

}

bool CStringDataSource::End() const noexcept{
    return DIndex >= DString.length();
}

bool CStringDataSource::Get(char &ch) noexcept{
    if(DIndex < DString.length()){
        ch = DString[DIndex];
        DIndex++;
        return true;
    }
    return false;
}

bool CStringDataSource::Peek(char &ch) noexcept{
    if(DIndex < DString.length()){
        ch = DString[DIndex];
        return true;
    }
    return false;
}

bool CStringDataSource::Read(std::vector<char> &buf, std::size_t count) noexcept{
    buf.clear();
    while(buf.size() < count){
        char TempChar;
        if(Get(TempChar)){
            buf.push_back(TempChar);
        }
        else{
            break;
        }
    }
    return !buf.empty();
}
