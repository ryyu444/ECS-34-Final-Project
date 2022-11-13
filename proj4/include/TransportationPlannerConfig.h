#ifndef TRANSPORTATIONPLANNERCONFIG_H
#define TRANSPORTATIONPLANNERCONFIG_H

#include "TransportationPlanner.h"

struct STransportationPlannerConfig : public CTransportationPlanner::SConfiguration{
    std::shared_ptr<CStreetMap> DStreetMap;
    std::shared_ptr<CBusSystem> DBusSystem;
    double DWalkSpeed;
    double DBikeSpeed;
    double DDefaultSpeedLimit;
    double DBusStopTime;
    int DPrecomputeTime;

    STransportationPlannerConfig(   std::shared_ptr<CStreetMap> streetmap, 
                                    std::shared_ptr<CBusSystem> bussystem,
                                    double walkspeed = 3.0,
                                    double bikespeed = 8.0,
                                    double speedlimit = 25.0,
                                    double busstoptime = 30.0,
                                    int precompute = 30){
        DStreetMap = streetmap;
        DBusSystem = bussystem;
        DWalkSpeed = walkspeed;
        DBikeSpeed = bikespeed;
        DDefaultSpeedLimit = speedlimit;
        DBusStopTime = busstoptime;
        DPrecomputeTime = precompute;

    }

    std::shared_ptr<CStreetMap> StreetMap() const noexcept{
        return DStreetMap;    
    }

    std::shared_ptr<CBusSystem> BusSystem() const noexcept{
        return DBusSystem;
    }

    double WalkSpeed() const noexcept{
        return DWalkSpeed;
    }

    double BikeSpeed() const noexcept{
        return DBikeSpeed;
    }

    double DefaultSpeedLimit() const noexcept{
        return DDefaultSpeedLimit;
    }

    double BusStopTime() const noexcept{
        return DBusStopTime;
    }

    int PrecomputeTime() const noexcept{
        return DPrecomputeTime;
    }
};

#endif
