#ifndef BUSSYSTEM_H
#define BUSSYSTEM_H

#include "StreetMap.h"

class CBusSystem{
    public:
        using TStopID = uint64_t;

        static const TStopID InvalidStopID = std::numeric_limits<TStopID>::max();

        struct SStop{
            virtual ~SStop(){};
            virtual TStopID ID() const noexcept = 0;
            virtual CStreetMap::TNodeID NodeID() const noexcept= 0;
        };

        struct SRoute{
            virtual ~SRoute(){};
            virtual std::string Name() const noexcept = 0;
            virtual std::size_t StopCount() const noexcept = 0;
            virtual TStopID GetStopID(std::size_t index) const noexcept = 0;
        };

        virtual ~CBusSystem(){};

        virtual std::size_t StopCount() const noexcept = 0;
        virtual std::size_t RouteCount() const noexcept = 0;
        virtual std::shared_ptr<SStop> StopByIndex(std::size_t index) const noexcept = 0;
        virtual std::shared_ptr<SStop> StopByID(TStopID id) const noexcept = 0;
        virtual std::shared_ptr<SRoute> RouteByIndex(std::size_t index) const noexcept = 0;
        virtual std::shared_ptr<SRoute> RouteByName(const std::string &name) const noexcept = 0;
};

#endif
