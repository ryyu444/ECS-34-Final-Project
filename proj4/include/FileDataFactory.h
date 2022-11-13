#ifndef FILEDATAFACTORY_H
#define FILEDATAFACTORY_H

#include "DataFactory.h"

class CFileDataFactory : public CDataFactory{
    private:
        std::string DBasePath;

    public:
        CFileDataFactory(const std::string &path);
;
        std::shared_ptr< CDataSource > CreateSource(const std::string &name) noexcept override;
        std::shared_ptr< CDataSink > CreateSink(const std::string &name) noexcept override;
};

#endif
