#ifndef DIJKSTRATRANSPORTATIONPLANNER_H
#define DIJKSTRATRANSPORTATIONPLANNER_H

#include "TransportationPlanner.h"

class CDijkstraTransportationPlanner : public CTransportationPlanner{
    private:
        struct SImplementation;
        std::unique_ptr<SImplementation> DImplementation;
    public:
        CDijkstraTransportationPlanner(std::shared_ptr<SConfiguration> config);
        ~CDijkstraTransportationPlanner();

        std::size_t NodeCount() const noexcept override;
        std::shared_ptr<CStreetMap::SNode> SortedNodeByIndex(std::size_t index) const noexcept override;

        double FindShortestPath(TNodeID src, TNodeID dest, std::vector< TNodeID > &path) override;
        double FindFastestPath(TNodeID src, TNodeID dest, std::vector< TTripStep > &path) override;
        bool GetPathDescription(const std::vector< TTripStep > &path, std::vector< std::string > &desc) const override;
};

#endif
