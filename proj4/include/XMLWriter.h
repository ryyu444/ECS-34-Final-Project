#ifndef XMLWRITER_H
#define XMLWRITER_H

#include <memory>
#include "XMLEntity.h"
#include "DataSink.h"

class CXMLWriter{
    private:
        struct SImplementation;
        std::unique_ptr<SImplementation> DImplementation;
        
    public:
        CXMLWriter(std::shared_ptr< CDataSink > sink);
        ~CXMLWriter();
        
        bool Flush();
        bool WriteEntity(const SXMLEntity &entity);
};

#endif
                                      