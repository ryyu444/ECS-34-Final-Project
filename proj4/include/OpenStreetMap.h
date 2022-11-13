#ifndef OPENSTREETMAP_H
#define OPENSTREETMAP_H

#include "XMLReader.h"
#include "StreetMap.h"

class COpenStreetMap : public CStreetMap{
    private:
        struct SImplementation;
        std::unique_ptr<SImplementation> DImplementation;

    public:
        COpenStreetMap(std::shared_ptr<CXMLReader> src);
        ~COpenStreetMap();

        std::size_t NodeCount() const noexcept override;
        std::size_t WayCount() const noexcept override;
        std::shared_ptr<CStreetMap::SNode> NodeByIndex(std::size_t index) const noexcept override;
        std::shared_ptr<CStreetMap::SNode> NodeByID(TNodeID id) const noexcept override;
        std::shared_ptr<CStreetMap::SWay> WayByIndex(std::size_t index) const noexcept override;
        std::shared_ptr<CStreetMap::SWay> WayByID(TWayID id) const noexcept override;
};

#endif
