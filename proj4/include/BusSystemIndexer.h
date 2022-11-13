#ifndef BUSSYSTEMINDEXER_H
#define BUSSYSTEMINDEXER_H
#include "BusSystem.h"
#include <unordered_set>

class CBusSystemIndexer{
    private:
        struct SImplementation;
        std::unique_ptr<SImplementation> DImplementation;
    public:
        using TNodeID = CStreetMap::TNodeID;
        using SStop = CBusSystem::SStop;
        using SRoute = CBusSystem::SRoute;
        CBusSystemIndexer(std::shared_ptr<CBusSystem> bussystem);
        ~CBusSystemIndexer();

        std::size_t StopCount() const noexcept;
        std::size_t RouteCount() const noexcept;
        std::shared_ptr<SStop> SortedStopByIndex(std::size_t index) const noexcept;
        std::shared_ptr<SRoute> SortedRouteByIndex(std::size_t index) const noexcept;
        std::shared_ptr<SStop> StopByNodeID(TNodeID id) const noexcept;
        bool RoutesByNodeIDs(TNodeID src, TNodeID dest, std::unordered_set<std::shared_ptr<SRoute> > &routes) const noexcept;
        bool RouteBetweenNodeIDs(TNodeID src, TNodeID dest) const noexcept;
};

#endif
