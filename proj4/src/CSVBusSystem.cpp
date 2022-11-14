#include "CSVBusSystem.h"
#include "DSVReader.h"
#include "StringUtils.h"
#include <iostream>
#include <unordered_map>

struct CCSVBusSystem::SImplementation {
    public:
        std::shared_ptr<CDSVReader> m_stopsrc, m_routesrc;
        
        // Holds pointers to total stops & routes
        std::vector<std::shared_ptr<SStop>> t_stops;
        std::unordered_map<TStopID, std::shared_ptr<SStop>> t_stops_map;
        std::vector<std::shared_ptr<SRoute>> t_routes;
        std::unordered_map<std::string, std::shared_ptr<SRoute>> t_routes_map;
        
        // Create our own Stop by inherting from SStop
        struct S_Stop: public CBusSystem::SStop {
            // Stop Id & Node Ids
            TStopID stop_id;
            CStreetMap::TNodeID node_id;
            
            // Intializes stop_id & node_id
            S_Stop(const TStopID &stp_id, const CStreetMap::TNodeID &nde_id) {
                stop_id = stp_id;
                node_id = nde_id;
            }
            
            TStopID ID() const noexcept override {
                // Returns stop id
                return stop_id;
            }

            CStreetMap::TNodeID NodeID() const noexcept override {
                // Returns node id
                return node_id;
            }

            ~S_Stop() override {};
        };

        // Create our own Route by inheriting from SRoute
        struct S_Route: public CBusSystem::SRoute {
            // Route name & Route Stops
            std::string route_name;
            std::vector<TStopID> route_stops;

            // Initializes route_name & route_stops
            S_Route(const std::string &name, const std::vector<TStopID> &stops) {
                route_name = name;
                route_stops = stops;
            }

            std::string Name() const noexcept override {
                // Returns route name
                return route_name;
            }

            std::size_t StopCount() const noexcept override {
                // Returns num of route stops
                return (std::size_t) route_stops.size();
            }

            TStopID GetStopID(std::size_t index) const noexcept override {
                // Index is beyond num of stops --> Invalid Stop ID
                if (index >= StopCount()) {
                    return InvalidStopID;
                }
                return route_stops.at(index);
            }
        };

        // Constructor: Sets up sources & parses through src data
        SImplementation(std::shared_ptr<CDSVReader> stopsrc, std::shared_ptr<CDSVReader> routesrc) {
            m_stopsrc = stopsrc;
            m_routesrc = routesrc;

            // Reads in first line header (which is text)
            std::vector<std::string> header;
            m_stopsrc -> ReadRow(header);
            
            // Read from stop src until the end
            while (!m_stopsrc -> End()) {
                std::vector<std::string> s_row;
                m_stopsrc -> ReadRow(s_row);

                auto tmp = std::make_shared<S_Stop>(S_Stop((TStopID) std::stoi(s_row.at(0)), 
                            (CStreetMap::TNodeID) std::stoi(s_row.at(1))));

                // Creates new Stop & pushes into stops map and stops vector
                t_stops_map[(TStopID) std::stoi(s_row.at(0))] = tmp;
                t_stops.push_back(tmp);
            }
            
            // Store route name & stops for that route
            std::string r_name;
            std::vector<TStopID> r_stops;

            // Reads in first line header (which is text)
            m_routesrc -> ReadRow(header);

            // Need to count up stop ids for a route
            while (!m_routesrc -> End()) {
                std::vector<std::string> r_row;
                m_routesrc -> ReadRow(r_row);
                
                // Goes through each row & splits up
                // Keeps track of stops for a given name --> Pushes if a new name is encountered
                if (r_name == "") {
                    r_name = r_row.at(0);
                    r_stops.push_back((TStopID) std::stoi(r_row.at(1)));
                }
                // Equivalent name --> Push the stop id
                else if (r_row.at(0) == r_name) {
                    r_stops.push_back((TStopID) std::stoi(r_row.at(1)));
                }
                // Push back prior route & stops
                // Keep track of new route name + clear old stops --> Add new route stop
                else {

                    auto tmp = std::make_shared<S_Route>(S_Route(r_name, r_stops));

                    t_routes.push_back(tmp);
                    t_routes_map[r_name] = tmp;
                    r_name = r_row.at(0);
                    r_stops.clear();
                    r_stops.push_back((TStopID) std::stoi(r_row.at(1)));
                }
            }
            // Pushes current route even if a new route is not encountered & we reach the end
            // Clears name & stops
            if (r_name != "" && r_stops.size()) {

                auto tmp = std::make_shared<S_Route>(S_Route(r_name, r_stops));

                t_routes.push_back(tmp);
                t_routes_map[r_name] = tmp;
                r_name.clear();
                r_stops.clear();
            }
        };

        ~SImplementation() {};
};

CCSVBusSystem::CCSVBusSystem(std::shared_ptr<CDSVReader> stopsrc, std::shared_ptr<CDSVReader> routesrc) {
    DImplementation = std::make_unique<SImplementation>(SImplementation(stopsrc, routesrc));
}

CCSVBusSystem::~CCSVBusSystem() {};

std::size_t CCSVBusSystem::StopCount() const noexcept {
    // Returns size of total stops
    return DImplementation -> t_stops.size();
}

std::size_t CCSVBusSystem::RouteCount() const noexcept {
    // Returns size of total routes
    return DImplementation -> t_routes.size();
}

std::shared_ptr<CBusSystem::SStop> CCSVBusSystem::StopByIndex(std::size_t index) const noexcept {
    // Index greater than or equal to num of stops --> nullptr
    // Else return the stop at the queried index
    if (index >= StopCount()) {
        return nullptr;
    }
    return DImplementation -> t_stops.at(index);
}

std::shared_ptr<CBusSystem::SStop> CCSVBusSystem::StopByID(CBusSystem::TStopID id) const noexcept {
    // Return the stop if found else nullptr
    if(DImplementation->t_stops_map.find(id) == DImplementation->t_stops_map.end()) {
        return nullptr;
    }
    return DImplementation->t_stops_map[id];
}

std::shared_ptr<CBusSystem::SRoute> CCSVBusSystem::RouteByIndex(std::size_t index) const noexcept {
    // Index greater than or equal to num of routes --> nullptr
    // Else return the route at queried index
    if (index >= RouteCount()) {
        return nullptr;
    }
    return DImplementation -> t_routes.at(index);
}

std::shared_ptr<CBusSystem::SRoute> CCSVBusSystem::RouteByName(const std::string &name) const noexcept {
    // Returns route if found else nullptr

    if(DImplementation->t_routes_map.find(name) == DImplementation->t_routes_map.end()) {
        return nullptr;
    }
    return DImplementation->t_routes_map[name];

}