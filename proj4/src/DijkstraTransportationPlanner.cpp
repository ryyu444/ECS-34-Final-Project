#include "DijkstraTransportationPlanner.h"
#include "DijkstraPathRouter.h"
#include "BusSystemIndexer.h"
#include "GeographicUtils.h"

#include "StringUtils.h"

#include <bits/stdc++.h>
#include <unordered_map>

bool compareByID(std::shared_ptr<CStreetMap::SNode> &node1, std::shared_ptr<CStreetMap::SNode> &node2) {
    return node1->ID() < node2->ID();
}

struct CDijkstraTransportationPlanner::SImplementation {

    public: 
        std::shared_ptr<SConfiguration> m_config;
        std::shared_ptr<CStreetMap> m_streetMap;
        std::shared_ptr<CBusSystem> m_busSystem;
        std::unique_ptr<CBusSystemIndexer> m_busSystemIndexer;
        std::vector<std::shared_ptr<CStreetMap::SNode>> m_sortedNodesByID;

        std::unique_ptr<CDijkstraPathRouter> m_shortestPathGraph;
        std::unique_ptr<CDijkstraPathRouter> m_fastestBusWalkGraph;
        std::unique_ptr<CDijkstraPathRouter> m_fastestBikeGraph;
        std::unordered_map<CStreetMap::TNodeID, CPathRouter::TVertexID> m_vertexNodesMap;
        bool m_nodesPopulated;

        double m_walkSpeedLimit, m_bikeSpeedLimit, m_busStopTime, m_defaultSpeedLimit;

        
        SImplementation(std::shared_ptr<SConfiguration> config) {
            m_config = config;
            m_streetMap = m_config -> StreetMap();
            m_busSystem = m_config -> BusSystem();
            m_busSystemIndexer = std::make_unique<CBusSystemIndexer>(m_busSystem);
            m_shortestPathGraph = std::make_unique<CDijkstraPathRouter>();
            m_fastestBusWalkGraph = std::make_unique<CDijkstraPathRouter>();
            m_fastestBikeGraph = std::make_unique<CDijkstraPathRouter>();
            m_nodesPopulated = false;

            m_walkSpeedLimit = m_config -> WalkSpeed();
            m_bikeSpeedLimit = m_config  -> BikeSpeed();
            m_busStopTime = m_config -> BusStopTime();
            m_defaultSpeedLimit = m_config -> DefaultSpeedLimit();

            // put nodes into vector and then sort them by id/name
            for (int i = 0; i < m_streetMap -> NodeCount(); i++) {
                m_sortedNodesByID.push_back(m_streetMap -> NodeByIndex(i));
            }
            std::sort(m_sortedNodesByID.begin(), m_sortedNodesByID.end(), compareByID);
        }

        
};

CDijkstraTransportationPlanner::CDijkstraTransportationPlanner(std::shared_ptr<SConfiguration> config) {
    DImplementation = std::make_unique<SImplementation>(SImplementation(config));   
}

CDijkstraTransportationPlanner::~CDijkstraTransportationPlanner() {}

std::size_t CDijkstraTransportationPlanner::NodeCount() const noexcept {
    return DImplementation -> m_streetMap -> NodeCount();
}

std::shared_ptr<CStreetMap::SNode> CDijkstraTransportationPlanner::SortedNodeByIndex(std::size_t index) const noexcept {
    if (index >= NodeCount()) {
        return nullptr;
    }
    return DImplementation->m_sortedNodesByID[index];
}

double CDijkstraTransportationPlanner::FindShortestPath(CTransportationPlanner::TNodeID src, CTransportationPlanner::TNodeID dest, std::vector< CTransportationPlanner::TNodeID > &path) {
    // return NoPathExists if no nodes exist
    if (!NodeCount()) {
        return CPathRouter::NoPathExists;
    }

    // Check if the graph is created already or not
    if(!DImplementation -> m_nodesPopulated) {
        // Adds all nodes as vertices in the graph
        for (int j = 0; j < NodeCount(); j++) {
            CPathRouter::TVertexID tmp_vert_id;
            
            CStreetMap::TNodeID curr_node_ID = SortedNodeByIndex(j) -> ID();
            
            tmp_vert_id = DImplementation -> m_shortestPathGraph -> AddVertex(curr_node_ID);

            DImplementation -> m_vertexNodesMap[curr_node_ID] = tmp_vert_id;
        }

        // std::cout << "Vertex count buswalking: " << DImplementation -> m_busWalking -> VertexCount() << std::endl;
        // std::cout << "Vertex count biking: " << DImplementation -> m_biking -> VertexCount() << std::endl;
        
        // Iterate through ways & add edges connecting vertices
        /* 
        1) Check tag for oneway
        2) Have variables that track whether its a one way or not
        3) Calculate the distance between the nodes
        4) Add edges between the vertices with the distance as the weight
        */
        for (int k = 0; k < DImplementation -> m_streetMap -> WayCount(); k++) {
            
            std::shared_ptr<CStreetMap::SWay> curr_way = DImplementation -> m_streetMap -> WayByIndex(k);
            
            // check if one way or not 
            bool isOneWay = curr_way -> GetAttribute("oneway") == "yes" ? true : false;

            // Iterates through ways & builds edges between nodes
            for (int l = 0; l < curr_way -> NodeCount() - 1; l++) {
                CStreetMap::TNodeID curr_nodeID = curr_way -> GetNodeID(l);
                CStreetMap::TNodeID next_nodeID = curr_way -> GetNodeID(l + 1);

                std::shared_ptr<CStreetMap::SNode> curr_node = DImplementation -> m_streetMap -> NodeByID(curr_nodeID);
                std::shared_ptr<CStreetMap::SNode> next_node = DImplementation -> m_streetMap -> NodeByID(next_nodeID);
                
                double dist = SGeographicUtils::HaversineDistanceInMiles(curr_node -> Location(), next_node -> Location());

                // std::cout << "currNode id: " << curr_node -> ID() << std::endl; 
                // std::cout << "nextNode id: " << next_node -> ID() << std::endl; 
                // std::cout << "dist: " << dist << std::endl;
                
                DImplementation -> m_shortestPathGraph -> AddEdge(DImplementation -> m_vertexNodesMap[curr_nodeID], 
                                                                  DImplementation -> m_vertexNodesMap[next_nodeID], 
                                                                  dist, !isOneWay);
            }
        }

        DImplementation -> m_nodesPopulated = true;
    }

    // Finds shortest path
    std::vector<CPathRouter::TVertexID> shortestPath;
    double shortestDist = DImplementation -> m_shortestPathGraph -> FindShortestPath(DImplementation -> m_vertexNodesMap[src], 
                                                                                     DImplementation -> m_vertexNodesMap[dest], 
                                                                                     shortestPath);
    
    // Pushes back the nodes in shortestPath to the path
    for (int m = 0; m < shortestPath.size(); m++) {
        path.push_back(std::any_cast<CStreetMap::TNodeID>(
            DImplementation -> m_shortestPathGraph -> GetVertexTag(shortestPath[m])));
    }
    
    return shortestDist;
}

double CDijkstraTransportationPlanner::FindFastestPath(CTransportationPlanner::TNodeID src, CTransportationPlanner::TNodeID dest, std::vector< CTransportationPlanner::TTripStep > &path) {
    
    // return NoPathExists if no nodes exist
    if (!NodeCount()) {
        return CPathRouter::NoPathExists;
    }
    
    // Grab shortest path for bus and populate m_vertexNodeMap if needed
    std::vector<CTransportationPlanner::TNodeID> busShortestPath;

    struct NodeIDDefaultVal {
        CTransportationPlanner::TNodeID nodeID = CStreetMap::InvalidNodeID;
        double speedLimit;
        bool checked = false;
    };

    std::unordered_map<CTransportationPlanner::TNodeID, NodeIDDefaultVal> busShortestPathEdgesMap;
    FindShortestPath(src, dest, busShortestPath);

    std::unordered_map<CTransportationPlanner::TNodeID,
                       std::unordered_map<CTransportationPlanner::TNodeID, 
                                          std::pair<double, CTransportationPlanner::ETransportationMode>>> transportationModesMap;

                        // 1, 2 (5, Walk) --> {1: {2: (5, Walk)}}
                        // 1, 3 (2, Bus) --> {1: {2: (5, Walk), 3: (2, Bus)}}
                        // 1, 2 (3, Bus) --> {1: {2: (3, Bus), 3: (2, Bus)}}
                        // 2 is in adj list: Check time (3 < 5)

    for (int r = 0; r < busShortestPath.size() - 1; r++) {
        CTransportationPlanner::TNodeID curNodeID = busShortestPath[r];
        CTransportationPlanner::TNodeID nextNodeID = busShortestPath[r + 1];
        // std::cout << "Cur Node: " << curNodeID << std::endl;
        // std::cout << "Next Node: " << nextNodeID << std::endl;
        busShortestPathEdgesMap[curNodeID].nodeID = nextNodeID;
    }

    // Adds all nodes as vertices in the graph
    for (int j = 0; j < NodeCount(); j++) {
        CPathRouter::TVertexID tmp_vert_id;
        
        CStreetMap::TNodeID curr_node_ID = SortedNodeByIndex(j) -> ID();
        
        tmp_vert_id = DImplementation -> m_fastestBusWalkGraph -> AddVertex(curr_node_ID);
        DImplementation -> m_fastestBikeGraph -> AddVertex(curr_node_ID);
    }

    // Iterate through ways & add edges connecting vertices
    /* 
       1) Check tag for oneway, maxspeed, bicycle, & name (optional)
       2) Have variables that track the above items: Set to default values if not found
       3) Calculate the min time for biking & bus/walking from cur node to next
       4) Add the edge with the min time
    */
    // std::cout << "Speed limit: " << speedLimit << std::endl;

    // Ways - Used for biking & walking
    for (int k = 0; k < DImplementation -> m_streetMap -> WayCount(); k++) {
        
        std::shared_ptr<CStreetMap::SWay> curr_way = DImplementation -> m_streetMap -> WayByIndex(k);
        
        // check if one way or not 
        bool isOneWay = curr_way -> GetAttribute("oneway") == "yes" ? true : false;
        bool bikingAllowed = curr_way -> GetAttribute("bicycle") == "no" ? false : true;
        std::string rawSpeedLimit = curr_way -> GetAttribute("maxspeed");
        // std::cout << "Raw Speed: " << rawSpeedLimit << std::endl;
        double speedLimit = rawSpeedLimit != "" ? 
            std::stod(StringUtils::Split(rawSpeedLimit)[0]) : 
            DImplementation -> m_defaultSpeedLimit;
        // std::cout << "Speed Limit Double: " << speedLimit << std::endl;
        
        

        // Iterates through ways & builds edges between nodes for biking and walking
        for (int l = 0; l < curr_way -> NodeCount() - 1; l++) {
            CStreetMap::TNodeID curr_nodeID = curr_way -> GetNodeID(l);
            CStreetMap::TNodeID next_nodeID = curr_way -> GetNodeID(l + 1);

            std::shared_ptr<CStreetMap::SNode> curr_node = DImplementation -> m_streetMap -> NodeByID(curr_nodeID);
            std::shared_ptr<CStreetMap::SNode> next_node = DImplementation -> m_streetMap -> NodeByID(next_nodeID);
            
            // time = distance / velocity (speed)
            double dist = SGeographicUtils::HaversineDistanceInMiles(curr_node -> Location(), next_node -> Location());
            double bikingTime = dist / DImplementation -> m_bikeSpeedLimit;
            double walkingTime = dist / DImplementation -> m_walkSpeedLimit;
            
            // std::cout << "currNode id: " << curr_node -> ID() << std::endl; 
            // std::cout << "nextNode id: " << next_node -> ID() << std::endl; 
            // std::cout << "distance in miles: " << dist << std::endl;

            if (busShortestPathEdgesMap[curr_nodeID].nodeID == next_nodeID) {
                busShortestPathEdgesMap[curr_nodeID].speedLimit = speedLimit;
            }

            if (busShortestPathEdgesMap[next_nodeID].nodeID == curr_nodeID) {
                busShortestPathEdgesMap[next_nodeID].speedLimit = speedLimit;
            }
            
            if (bikingAllowed) {
                DImplementation -> m_fastestBikeGraph -> AddEdge(DImplementation -> m_vertexNodesMap[curr_nodeID], 
                                                                 DImplementation -> m_vertexNodesMap[next_nodeID], 
                                                                 bikingTime, !isOneWay);
            }
            // std::cout << "Walking Time: " << walkingTime << std::endl;
            DImplementation -> m_fastestBusWalkGraph -> AddEdge(DImplementation -> m_vertexNodesMap[curr_nodeID], 
                                                                DImplementation -> m_vertexNodesMap[next_nodeID], 
                                                                walkingTime, true);

            if (transportationModesMap.find(curr_node_ID) != )
        }
    }


    // std::unordered_set<std::shared_ptr<CBusSystem::SRoute> > busRoutes;
    // DImplementation -> m_busSystemIndexer -> RoutesByNodeIDs(src, dest, busRoutes);
    // std::cout << "Number of routes: " << busRoutes.size() << std::endl;
    // std::unordered_set<std::shared_ptr<CBusSystem::SRoute>>::iterator i;
    // for (i = busRoutes.begin(); i != busRoutes.end(); std::advance(i, 1)) {
    //     auto temp = *i;
    //     std::cout << "Route: " << temp -> Name() << std::endl;
    // }

    
    for (int n = 0; n < DImplementation -> m_busSystem -> RouteCount(); n++) {
        std::shared_ptr<CBusSystem::SRoute> curr_route = DImplementation -> m_busSystem -> RouteByIndex(n);

        // Duplicate issue
        for(int q = 0; q < curr_route -> StopCount() - 1; q++) {
            CTransportationPlanner::TNodeID curr_nodeID = DImplementation -> m_busSystem -> StopByID(curr_route -> GetStopID(q)) -> NodeID();
            CTransportationPlanner::TNodeID next_nodeID = DImplementation -> m_busSystem -> StopByID(curr_route -> GetStopID(q + 1)) -> NodeID();

            // std::cout << "(" << curr_nodeID << ", " << busShortestPathEdgesMap[curr_nodeID].nodeID << ")" << std::endl;
            // std::cout << "Speed Limit: " << busShortestPathEdgesMap[curr_nodeID].speedLimit << std::endl;
            if(busShortestPathEdgesMap[curr_nodeID].nodeID == next_nodeID && busShortestPathEdgesMap[curr_nodeID].checked == false) {
                // Calculate time for edge
                // std::cout << "Curr (real) Node: " << curr_nodeID << std::endl;
                // std::cout << "Next (real) Node: " << next_nodeID << std::endl;
                std::shared_ptr<CStreetMap::SNode> curr_node = DImplementation -> m_streetMap -> NodeByID(curr_nodeID);
                std::shared_ptr<CStreetMap::SNode> next_node = DImplementation -> m_streetMap -> NodeByID(next_nodeID);
                double busDist = SGeographicUtils::HaversineDistanceInMiles(curr_node -> Location(), next_node -> Location());;
                double busTime = busDist / busShortestPathEdgesMap[curr_nodeID].speedLimit + (DImplementation -> m_busStopTime / 3600);
                // std:: cout << "Speed Limit: " << busShortestPathEdgesMap[curr_nodeID].speedLimit << std::endl;
                // std::cout << "Bussing Time: " << busTime << std::endl;
                // Check if edge is being added correctly
                // std::cout << "Thing: " << busWalkTransportationMode[std::make_pair(curr_nodeID, next_nodeID)] << std::endl;

                DImplementation -> m_fastestBusWalkGraph -> AddEdge(DImplementation -> m_vertexNodesMap[curr_nodeID], 
                                                                    DImplementation -> m_vertexNodesMap[next_nodeID], 
                                                                    busTime, false);
                // std::cout << "Edge: " <<
                // std::any_cast<CStreetMap::TNodeID>(DImplementation -> m_fastestBusWalkGraph -> GetVertexTag(DImplementation -> m_vertexNodesMap[curr_nodeID])) << std::endl;
                busShortestPathEdgesMap[curr_nodeID].checked = true;

            }
        }

    }

    // Finds shortest path for biking
    std::vector<CPathRouter::TVertexID> bikingPath;
    double bikingTime = DImplementation -> m_fastestBikeGraph -> FindShortestPath(DImplementation -> m_vertexNodesMap[src], 
                                                   DImplementation -> m_vertexNodesMap[dest], 
                                                   bikingPath);

    std::vector<CTransportationPlanner::TTripStep> bikingFastestPath;
    for (int p = 0; p < bikingPath.size(); p++) {
        bikingFastestPath.push_back(CTransportationPlanner::TTripStep(CTransportationPlanner::ETransportationMode::Bike,
                                                         std::any_cast<CStreetMap::TNodeID>(DImplementation -> m_fastestBikeGraph 
                                                                                            -> GetVertexTag(bikingPath[p]))));
        // std::cout << "Node ID: " << std::any_cast<CStreetMap::TNodeID>(DImplementation -> m_fastestBikeGraph -> GetVertexTag(bikingPath[p])) << std::endl;
    }
    // std::cout << "Biking Time: " << bikingTime << std::endl;

    // Finds shortest path for walking/bussing
    std::vector<CPathRouter::TVertexID> walkBussingPath;
    double walkBussingTime = DImplementation -> m_fastestBusWalkGraph -> FindShortestPath(DImplementation -> m_vertexNodesMap[src], 
                                                   DImplementation -> m_vertexNodesMap[dest], 
                                                   walkBussingPath);

    std::vector<CTransportationPlanner::TTripStep> walkBussingFastestPath;

    for (int p = 0; p < walkBussingPath.size(); p++) {
        CTransportationPlanner::TNodeID currNodeID = std::any_cast<CStreetMap::TNodeID>(DImplementation -> m_fastestBusWalkGraph 
                                                                                        -> GetVertexTag(walkBussingPath[p]));
        CTransportationPlanner::ETransportationMode mode = CTransportationPlanner::ETransportationMode::Walk;

        walkBussingFastestPath.push_back(CTransportationPlanner::TTripStep(mode, currNodeID));
        std::cout << "Node ID: " << currNodeID << std::endl;
    }
    // std::cout << "walk/bus time: " << walkBussingTime << std::endl;

    if (walkBussingTime > bikingTime) {
        path = bikingFastestPath;
        return bikingTime;
    }
    else {
        path = walkBussingFastestPath;
        return walkBussingTime;
    }
    
}

bool CDijkstraTransportationPlanner::GetPathDescription(const std::vector< CTransportationPlanner::TTripStep > &path, std::vector< std::string > &desc) const {
    // TODO
    return false; 
}