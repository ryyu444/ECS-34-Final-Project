#include "BusSystemIndexer.h"

struct CBusSystemIndexer::SImplementation {

    public: 
        std::shared_ptr<CBusSystem> m_bussystem;
        
        SImplementation(std::shared_ptr<CBusSystem> bussystem) {
            m_bussystem = bussystem;
        };
};

CBusSystemIndexer::CBusSystemIndexer(std::shared_ptr<CBusSystem> bussystem) {
    DImplementation = std::make_unique<SImplementation>(SImplementation(bussystem));
}

CBusSystemIndexer::~CBusSystemIndexer() {}

std::size_t CBusSystemIndexer::StopCount() const noexcept {
    return DImplementation->m_bussystem->StopCount();
}

std::size_t CBusSystemIndexer::RouteCount() const noexcept {
    return DImplementation->m_bussystem->RouteCount();
}

std::shared_ptr<CBusSystem::SStop> CBusSystemIndexer::SortedStopByIndex(std::size_t index) const noexcept {
    return nullptr;
}

std::shared_ptr<CBusSystem::SRoute> CBusSystemIndexer::SortedRouteByIndex(std::size_t index) const noexcept {
    return nullptr;

}

std::shared_ptr<CBusSystem::SStop> CBusSystemIndexer::StopByNodeID(CStreetMap::TNodeID id) const noexcept {
    return nullptr;
}

bool CBusSystemIndexer::RoutesByNodeIDs(CStreetMap::TNodeID src, CStreetMap::TNodeID dest, std::unordered_set<std::shared_ptr<CBusSystem::SRoute> > &routes) const noexcept {
    return false;
}

bool CBusSystemIndexer::RouteBetweenNodeIDs(CStreetMap::TNodeID src, CStreetMap::TNodeID dest) const noexcept {
    return false;
}