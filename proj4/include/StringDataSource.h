#ifndef STRINGDATASOURCE_H
#define STRINGDATASOURCE_H

#include "DataSource.h"
#include <string>

class CStringDataSource : public CDataSource{
    private:
        std::string DString;
        size_t DIndex;
    public:
        CStringDataSource(const std::string &str);

        bool End() const noexcept override;
        bool Get(char &ch) noexcept override;
        bool Peek(char &ch) noexcept override;
        bool Read(std::vector<char> &buf, std::size_t count) noexcept override;
};

#endif
