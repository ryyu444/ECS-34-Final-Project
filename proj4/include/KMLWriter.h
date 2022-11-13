#ifndef KMLWRITER_H
#define KMLWRITER_H

#include <string>
#include <memory>
#include <cstdint>
#include "DataSink.h"
#include "StreetMap.h"

class CKMLWriter{
    private:
        struct SImplementation;
        std::unique_ptr<SImplementation> DImplementation;
        
    public:
        CKMLWriter(std::shared_ptr< CDataSink > sink, const std::string &name, const std::string &desc);
        ~CKMLWriter();
        
        bool CreatePointStyle(const std::string &stylename, unsigned int color);
        bool CreateLineStyle(const std::string &stylename, unsigned int color, int width);

        bool CreatePoint(const std::string &name, const std::string &desc, const std::string &stylename, CStreetMap::TLocation point);
        bool CreatePath(const std::string &name, const std::string &stylename, const std::vector< CStreetMap::TLocation > &points);
};

#endif
                                      