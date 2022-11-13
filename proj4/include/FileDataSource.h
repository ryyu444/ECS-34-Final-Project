#ifndef FILEDATASOURCE_H
#define FILEDATASOURCE_H

#include "DataSource.h"
#include <fstream>

class CFileDataSource : public CDataSource{
    private:
        std::ifstream DFile;
    public:
        CFileDataSource(const std::string &filename);

        bool End() const noexcept override;
        bool Get(char &ch) noexcept override;
        bool Peek(char &ch) noexcept override;
        bool Read(std::vector<char> &buf, std::size_t count) noexcept override;
};

#endif
