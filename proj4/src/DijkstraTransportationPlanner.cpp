#include "DijkstraTransportationPlanner.h"

struct CDijkstraTransportationPlanner::SImplementation {

    public: 
        std::shared_ptr<SConfiguration> m_config;

        SImplementation(std::shared_ptr<SConfiguration> config) {
            m_config = config;
        }
};

CDijkstraTransportationPlanner::CDijkstraTransportationPlanner(std::shared_ptr<SConfiguration> config) {
    DImplementation = std::make_unique<SImplementation>(SImplementation(config));
}

CDijkstraTransportationPlanner::~CDijkstraTransportationPlanner() {}

std::size_t CDijkstraTransportationPlanner::NodeCount() const noexcept {
    return 1;
}
std::shared_ptr<CStreetMap::SNode> CDijkstraTransportationPlanner::SortedNodeByIndex(std::size_t index) const noexcept {
    return nullptr;
}
double CDijkstraTransportationPlanner::FindShortestPath(CTransportationPlanner::TNodeID src, CTransportationPlanner::TNodeID dest, std::vector< CTransportationPlanner::TNodeID > &path) {
    return 1.1;
}
double CDijkstraTransportationPlanner::FindFastestPath(CTransportationPlanner::TNodeID src, CTransportationPlanner::TNodeID dest, std::vector< CTransportationPlanner::TTripStep > &path) {
    return 1.1;
}
bool CDijkstraTransportationPlanner::GetPathDescription(const std::vector< CTransportationPlanner::TTripStep > &path, std::vector< std::string > &desc) const {
    return false; 
}