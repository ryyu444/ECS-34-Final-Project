#include "BusSystemIndexer.h"

#include <iostream>
#include <vector>
#include <bits/stdc++.h>
#include <unordered_map>

bool compareByID(std::shared_ptr<CBusSystem::SStop> &stop1, std::shared_ptr<CBusSystem::SStop> &stop2) {
    return stop1->ID() < stop2->ID();
}

bool compareByName(std::shared_ptr<CBusSystem::SRoute> &route1, std::shared_ptr<CBusSystem::SRoute> &route2) {
    return route1->Name().compare(route2->Name()) <= 0 ? true : false;
}

struct CBusSystemIndexer::SImplementation {

    public: 
        std::shared_ptr<CBusSystem> m_busSystem;
        std::vector<std::shared_ptr<CBusSystem::SStop>> m_sortedStopsByID;
        std::vector<std::shared_ptr<CBusSystem::SRoute>> m_sortedRoutesByName;
        std::unordered_map<CStreetMap::TNodeID, std::shared_ptr<SStop>> m_stopsByNodeIDMap;
        
        SImplementation(std::shared_ptr<CBusSystem> bussystem) {
            m_busSystem = bussystem;

            /*
            this is probably very inefficient, just trying to get it to work at first
            */

            // put stops and routes into vector and then sort them by id/name
            for(int i = 0; i < m_busSystem->StopCount(); i++) {
                m_sortedStopsByID.push_back(m_busSystem->StopByIndex(i));
                m_stopsByNodeIDMap[m_busSystem->StopByIndex(i)->NodeID()] = m_busSystem->StopByIndex(i); 
            }
            std::sort(m_sortedStopsByID.begin(), m_sortedStopsByID.end(), compareByID);

            for(int i = 0; i < m_busSystem->RouteCount(); i++) {
                m_sortedRoutesByName.push_back(m_busSystem->RouteByIndex(i));
            }
            std::sort(m_sortedRoutesByName.begin(), m_sortedRoutesByName.end(), compareByName);

        };
};

CBusSystemIndexer::CBusSystemIndexer(std::shared_ptr<CBusSystem> bussystem) {
    DImplementation = std::make_unique<SImplementation>(SImplementation(bussystem));
}

CBusSystemIndexer::~CBusSystemIndexer() {}

std::size_t CBusSystemIndexer::StopCount() const noexcept {
    return DImplementation->m_sortedStopsByID.size();
}

std::size_t CBusSystemIndexer::RouteCount() const noexcept {
    return DImplementation->m_sortedRoutesByName.size();
}

std::shared_ptr<CBusSystem::SStop> CBusSystemIndexer::SortedStopByIndex(std::size_t index) const noexcept {
    if(index >= StopCount()) {
        return nullptr;
    }
    return DImplementation->m_sortedStopsByID[index];
}

std::shared_ptr<CBusSystem::SRoute> CBusSystemIndexer::SortedRouteByIndex(std::size_t index) const noexcept {
    if(index >= RouteCount()) {
        return nullptr;
    }
    return DImplementation->m_sortedRoutesByName[index];
}

std::shared_ptr<CBusSystem::SStop> CBusSystemIndexer::StopByNodeID(CStreetMap::TNodeID id) const noexcept {
    if(DImplementation->m_stopsByNodeIDMap.find(id) == DImplementation->m_stopsByNodeIDMap.end()) {
        return nullptr;
    }
    return DImplementation->m_stopsByNodeIDMap[id];
}

bool CBusSystemIndexer::RoutesByNodeIDs(CStreetMap::TNodeID src, CStreetMap::TNodeID dest, std::unordered_set<std::shared_ptr<CBusSystem::SRoute> > &routes) const noexcept {

    // Could be accessing invalid TNodeIDs
    if (DImplementation -> m_stopsByNodeIDMap.find(src) == DImplementation -> m_stopsByNodeIDMap.end() || 
        DImplementation -> m_stopsByNodeIDMap.find(dest) == DImplementation -> m_stopsByNodeIDMap.end()) {
        return false;
    }
    // check if routes empty or not
    if (DImplementation->m_sortedRoutesByName.size() == 0) {
        return false;
    }
    
    CBusSystem::TStopID stopIDSrc = DImplementation->m_stopsByNodeIDMap[src] -> ID();
    CBusSystem::TStopID stopIDDest = DImplementation->m_stopsByNodeIDMap[dest] -> ID();

    bool foundRoute = false;

    // first: route name, second: stop id
    std::vector<std::pair<std::string, CBusSystem::TStopID>> tmpPairs;

    for(int i = 0; i < RouteCount(); i++) {
        for(int j = 0; j < DImplementation->m_sortedRoutesByName[i]->StopCount() - 1; j++) {
            CBusSystem::TStopID curStopID = DImplementation->m_sortedRoutesByName[i]->GetStopID(j);
            CBusSystem::TStopID nextStopID = DImplementation->m_sortedRoutesByName[i]->GetStopID(j + 1);
            std::string RouteName =  DImplementation->m_sortedRoutesByName[i]->Name();

            if((curStopID == stopIDSrc && nextStopID == stopIDDest)) {
                // tmpPairs.push_back({tmpRouteName, tmpStopID});
                routes.insert(DImplementation -> m_sortedRoutesByName[i]);
                // std::cout << "Route: " << DImplementation -> m_sortedRoutesByName[i] -> Name() << std::endl;
                foundRoute = true;
                break;
            }
        }
    }

    // std::cout << "end" <<std::endl;

    return foundRoute;
}

bool CBusSystemIndexer::RouteBetweenNodeIDs(CStreetMap::TNodeID src, CStreetMap::TNodeID dest) const noexcept {
    std::unordered_set<std::shared_ptr<CBusSystem::SRoute>> dummyRoutes;
    return RoutesByNodeIDs(src, dest, dummyRoutes);
}