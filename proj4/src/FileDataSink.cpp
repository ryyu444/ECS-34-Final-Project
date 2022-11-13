#include "FileDataSink.h"


CFileDataSink::CFileDataSink(const std::string &filename){
    DFile.open(filename);
}

bool CFileDataSink::Put(const char &ch) noexcept{
    DFile.put(ch);
    return DFile.good();
}

bool CFileDataSink::Write(const std::vector<char> &buf) noexcept{
    DFile.write(buf.data(),buf.size());
    return DFile.good();
}