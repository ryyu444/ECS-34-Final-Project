#ifndef DATAFACTORY_H
#define DATAFACTORY_H

#include <string>
#include <memory>
#include "DataSink.h"
#include "DataSource.h"

class CDataFactory{
    public:
        virtual ~CDataFactory(){};
        virtual std::shared_ptr< CDataSource > CreateSource(const std::string &name) noexcept = 0;
        virtual std::shared_ptr< CDataSink > CreateSink(const std::string &name) noexcept = 0;
};

#endif
