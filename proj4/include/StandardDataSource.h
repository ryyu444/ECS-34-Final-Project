#ifndef STANDARDDATASOURCE_H
#define STANDARDDATASOURCE_H

#include "DataSource.h"

class CStandardDataSource : public CDataSource{
    private:
    public:
        bool End() const noexcept override;
        bool Get(char &ch) noexcept override;
        bool Peek(char &ch) noexcept override;
        bool Read(std::vector<char> &buf, std::size_t count) noexcept override;
};

#endif
