#ifndef DSVWRITER_H
#define DSVWRITER_H

#include <memory>
#include <string>
#include "DataSink.h"

class CDSVWriter{
    private:
        struct SImplementation;
        std::unique_ptr<SImplementation> DImplementation;

    public:
        CDSVWriter(std::shared_ptr< CDataSink > sink, char delimiter, bool quoteall = false);
        ~CDSVWriter();

        bool WriteRow(const std::vector<std::string> &row);
};

#endif
