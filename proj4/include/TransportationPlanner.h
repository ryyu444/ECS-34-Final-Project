#ifndef TRANSPORTATIONPLANNER_H
#define TRANSPORTATIONPLANNER_H

#include "StreetMap.h"
#include "BusSystem.h"
#include "PathRouter.h"

class CTransportationPlanner{
    public:
        using TNodeID = CStreetMap::TNodeID;
        enum class ETransportationMode {Walk, Bike, Bus};
        using TTripStep = std::pair<ETransportationMode, TNodeID>;

        struct SConfiguration{
            virtual ~SConfiguration(){};
            virtual std::shared_ptr<CStreetMap> StreetMap() const noexcept = 0;
            virtual std::shared_ptr<CBusSystem> BusSystem() const noexcept = 0;
            virtual double WalkSpeed() const noexcept = 0;
            virtual double BikeSpeed() const noexcept = 0;
            virtual double DefaultSpeedLimit() const noexcept = 0;
            virtual double BusStopTime() const noexcept = 0;
            virtual int PrecomputeTime() const noexcept = 0;
        };

        virtual ~CTransportationPlanner(){};

        virtual std::size_t NodeCount() const noexcept = 0;
        virtual std::shared_ptr<CStreetMap::SNode> SortedNodeByIndex(std::size_t index) const noexcept = 0;

        virtual double FindShortestPath(TNodeID src, TNodeID dest, std::vector< TNodeID > &path) = 0;
        virtual double FindFastestPath(TNodeID src, TNodeID dest, std::vector< TTripStep > &path) = 0;
        virtual bool GetPathDescription(const std::vector< TTripStep > &path, std::vector< std::string > &desc) const = 0;
};

#endif
