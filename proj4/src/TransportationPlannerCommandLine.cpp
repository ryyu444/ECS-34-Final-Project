#include "TransportationPlannerCommandLine.h"

struct CTransportationPlannerCommandLine::SImplementation {

    public: 
        std::shared_ptr<CDataSource> m_cmdsrc;
        std::shared_ptr<CDataSink> m_outsink;
        std::shared_ptr<CDataSink> m_errsink;
        std::shared_ptr<CDataFactory> m_results;
        std::shared_ptr<CTransportationPlanner> m_planner;

        SImplementation(
            std::shared_ptr<CDataSource> cmdsrc, 
            std::shared_ptr<CDataSink> outsink, 
            std::shared_ptr<CDataSink> errsink, 
            std::shared_ptr<CDataFactory> results, 
            std::shared_ptr<CTransportationPlanner> planner) {

                m_cmdsrc = cmdsrc;
                m_outsink = outsink;
                m_errsink = errsink;
                m_results = results;
                m_planner = planner;

            }
};

CTransportationPlannerCommandLine::CTransportationPlannerCommandLine(
    std::shared_ptr<CDataSource> cmdsrc, 
    std::shared_ptr<CDataSink> outsink, 
    std::shared_ptr<CDataSink> errsink, 
    std::shared_ptr<CDataFactory> results, 
    std::shared_ptr<CTransportationPlanner> planner) {

        DImplementation = std::make_unique<SImplementation>(
            SImplementation(cmdsrc, outsink, errsink, results, planner));

    }

CTransportationPlannerCommandLine::~CTransportationPlannerCommandLine() {}

bool CTransportationPlannerCommandLine::ProcessCommands() {
    return false;
}