#include "DijkstraTransportationPlanner.h"

struct DerivedConfiguration : public CTransportationPlanner::SConfiguration {
    public:
        std::shared_ptr<CStreetMap> m_streetMap;
        std::shared_ptr<CBusSystem> m_busSystem;
        double m_walkSpeed = 0.0;
        double m_bikeSpeed = 0.0;
        double m_defaultSpeedLimit = 0.0;
        double m_busStopTime = 0.0;
        double m_precomputeTime = 0.0;

        DerivedConfiguration(std::shared_ptr<CStreetMap> streetMap, std::shared_ptr<CBusSystem> busSystem) {
            m_streetMap = streetMap;
            m_busSystem = busSystem;
        };
        
        ~DerivedConfiguration() {};

        std::shared_ptr<CStreetMap> StreetMap() const noexcept {
            return m_streetMap;
        };
        
        std::shared_ptr<CBusSystem> BusSystem() const noexcept {
            return m_busSystem;
        };
        
        double WalkSpeed() const noexcept {
            return m_walkSpeed;
        };
        
        double BikeSpeed() const noexcept {
            return m_bikeSpeed;
        };
        
        double DefaultSpeedLimit() const noexcept {
            return m_defaultSpeedLimit;
        };
        
        double BusStopTime() const noexcept {
            return m_busStopTime;
        };
        
        int PrecomputeTime() const noexcept {
            return m_precomputeTime;
        };
};

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