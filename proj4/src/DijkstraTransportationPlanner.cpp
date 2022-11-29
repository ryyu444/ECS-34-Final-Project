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
        std::unordered_map<TNodeID, CPathRouter::TVertexID> m_vertexNodesMap;
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

double CDijkstraTransportationPlanner::FindShortestPath(TNodeID src, TNodeID dest, std::vector< TNodeID > &path) {
    // return NoPathExists if no nodes exist
    if (!NodeCount()) {
        return CPathRouter::NoPathExists;
    }

    // Check if the graph is created already or not
    if(!DImplementation -> m_nodesPopulated) {
        // Adds all nodes as vertices in the graph
        for (int j = 0; j < NodeCount(); j++) {
            CPathRouter::TVertexID tmp_vert_id;
            
            TNodeID curr_node_ID = SortedNodeByIndex(j) -> ID();
            
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
                TNodeID curr_nodeID = curr_way -> GetNodeID(l);
                TNodeID next_nodeID = curr_way -> GetNodeID(l + 1);

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
        path.push_back(std::any_cast<TNodeID>(
            DImplementation -> m_shortestPathGraph -> GetVertexTag(shortestPath[m])));
    }
    
    return shortestDist;
}

double CDijkstraTransportationPlanner::FindFastestPath(TNodeID src, TNodeID dest, std::vector< TTripStep > &path) {
    
    // return NoPathExists if no nodes exist
    if (!NodeCount()) {
        return CPathRouter::NoPathExists;
    }

    struct EdgeData {
        TNodeID m_nextNodeID = CStreetMap::InvalidNodeID;
        std::string m_streetName;
        ETransportationMode m_transMode;
        double m_time;
        double m_speedLimit;
        double m_distance;
        bool m_checked = false;

        EdgeData(TNodeID nextNodeID, std::string streetName, 
            double time, double speedLimit, double distance, ETransportationMode transMode) {
            m_nextNodeID = nextNodeID;
            m_streetName = streetName;
            m_time = time;
            m_speedLimit = speedLimit;
            m_distance = distance;
            m_transMode = transMode;
        }
    };

    std::unordered_map<TNodeID, std::vector<EdgeData>> bikeEdgeData;
    std::unordered_map<TNodeID, std::vector<EdgeData>> busWalkEdgeData;

    // Adds all nodes as vertices in the graph
    for (int j = 0; j < NodeCount(); j++) {
        CPathRouter::TVertexID tmp_vert_id;
        
        TNodeID curr_node_ID = SortedNodeByIndex(j) -> ID();
        
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
        std::string streetName = curr_way -> GetAttribute("name");
        std::string rawSpeedLimit = curr_way -> GetAttribute("maxspeed");
        // std::cout << "Raw Speed: " << rawSpeedLimit << std::endl;
        double speedLimit = rawSpeedLimit != "" ? 
            std::stod(StringUtils::Split(rawSpeedLimit)[0]) : 
            DImplementation -> m_defaultSpeedLimit;
        // std::cout << "Speed Limit Double: " << speedLimit << std::endl;
        
        

        // Iterates through ways & builds edges between nodes for biking and walking
        for (int l = 0; l < curr_way -> NodeCount() - 1; l++) {
            TNodeID curr_nodeID = curr_way -> GetNodeID(l);
            TNodeID next_nodeID = curr_way -> GetNodeID(l + 1);

            std::shared_ptr<CStreetMap::SNode> curr_node = DImplementation -> m_streetMap -> NodeByID(curr_nodeID);
            std::shared_ptr<CStreetMap::SNode> next_node = DImplementation -> m_streetMap -> NodeByID(next_nodeID);
            
            // time = distance / velocity (speed)
            double dist = SGeographicUtils::HaversineDistanceInMiles(curr_node -> Location(), next_node -> Location());
            double bikingTime = dist / DImplementation -> m_bikeSpeedLimit;
            double walkingTime = dist / DImplementation -> m_walkSpeedLimit;
            
            // std::cout << "currNode id: " << curr_node -> ID() << std::endl; 
            // std::cout << "nextNode id: " << next_node -> ID() << std::endl; 
            // std::cout << "distance in miles: " << dist << std::endl;
            
            if (bikingAllowed) {
                DImplementation -> m_fastestBikeGraph -> AddEdge(DImplementation -> m_vertexNodesMap[curr_nodeID], 
                                                                 DImplementation -> m_vertexNodesMap[next_nodeID], 
                                                                 bikingTime, !isOneWay);
                bikeEdgeData[curr_nodeID].push_back(EdgeData(next_nodeID, streetName, bikingTime, 
                                                             DImplementation -> m_bikeSpeedLimit, dist, 
                                                             ETransportationMode::Bike));
                // std::cout << "currNode: " << curr_nodeID << ", EdgeData: (" << bikeEdgeData[curr_nodeID][0].m_nextNodeID << ", " << bikeEdgeData[curr_nodeID][0].m_time << ")\n";
            }

            // std::cout << "Walking Time: " << walkingTime << std::endl;
            DImplementation -> m_fastestBusWalkGraph -> AddEdge(DImplementation -> m_vertexNodesMap[curr_nodeID], 
                                                                DImplementation -> m_vertexNodesMap[next_nodeID], 
                                                                walkingTime, true);
            busWalkEdgeData[curr_nodeID].push_back(EdgeData(next_nodeID, streetName, walkingTime, speedLimit, dist, 
                                                            ETransportationMode::Walk));
            // std::cout << "currNode: " << curr_nodeID << ", EdgeData: (" << busWalkEdgeData[curr_nodeID][0].m_nextNodeID 
            //     << ", " << busWalkEdgeData[curr_nodeID][0].m_distance
            //     << ")\n";
            // std::cout << speed
        }
    }



    
    for (int n = 0; n < DImplementation -> m_busSystem -> RouteCount(); n++) {
        std::shared_ptr<CBusSystem::SRoute> curr_route = DImplementation -> m_busSystem -> RouteByIndex(n);

        // Duplicate issue
        for (int q = 0; q < curr_route -> StopCount() - 1; q++) {
            TNodeID currStopNodeID = DImplementation -> m_busSystem -> StopByID(curr_route -> GetStopID(q)) -> NodeID();
            TNodeID nextStopNodeID = DImplementation -> m_busSystem -> StopByID(curr_route -> GetStopID(q + 1)) -> NodeID();

            // Check if currNodeID in busWalkEdgeData & check if nextNodeID == busWalkEdgeData[currNodeID].m_nextNodeID
            /* stops can be more than 1 node apart, whilst walking has to go from node to node
                (1) - > (3) is possible for bus, but while walking you have to go (1) -> (2) -> (3)

                Plan: 
                1) Find shortest path between consecutive stops
                2) Iterate through nodes in the found shortest path and calculate the time for buses
                3) replace walk data (time and transMode) in EdgeData vector with bus data
            */
            
            std::vector<TNodeID> shortestPathBetweenStops;
            FindShortestPath(currStopNodeID, nextStopNodeID, shortestPathBetweenStops);

            // g++ cameo 😳
            for (int g = 0; g < shortestPathBetweenStops.size() - 1; g++) {
                TNodeID currNode = shortestPathBetweenStops[g];
                TNodeID nextNode = shortestPathBetweenStops[g + 1];

                for (int j = 0; j < busWalkEdgeData[currNode].size(); j++) {
                    if(busWalkEdgeData[currNode][j].m_nextNodeID == nextNode) {
                        double speed = busWalkEdgeData[currNode][j].m_speedLimit;
                        double dist = busWalkEdgeData[currNode][j].m_distance;
                        double time = dist / speed;
                        // only add stop time to first edge
                        if (g == 0) {
                            time += (DImplementation -> m_busStopTime / 3600);
                        }
                        busWalkEdgeData[currNode][j].m_time = time;
                        // std::cout << "Curr Node " << currNode << " - " << busWalkEdgeData[currNode][j].m_nextNodeID << ": (" << busWalkEdgeData[currNode][j].m_time << ")" << std::endl;
                        busWalkEdgeData[currNode][j].m_transMode = ETransportationMode::Bus;

                        DImplementation -> m_fastestBusWalkGraph -> AddEdge(DImplementation -> m_vertexNodesMap[currNode], 
                                                                            DImplementation -> m_vertexNodesMap[nextNode], 
                                                                            time, false);
                    }
                }

            }
        }

    }

    // Finds shortest path for biking
    std::vector<CPathRouter::TVertexID> bikingPath;
    double bikingTime = DImplementation -> m_fastestBikeGraph -> FindShortestPath(DImplementation -> m_vertexNodesMap[src], 
                                                   DImplementation -> m_vertexNodesMap[dest], 
                                                   bikingPath);

    // std::cout << "Biking Time: " << bikingTime << std::endl;

    // Finds shortest path for walking/bussing
    std::vector<CPathRouter::TVertexID> walkBussingPath;
    double walkBussingTime = DImplementation -> m_fastestBusWalkGraph -> FindShortestPath(DImplementation -> m_vertexNodesMap[src], 
                                                   DImplementation -> m_vertexNodesMap[dest], 
                                                   walkBussingPath);


    if (walkBussingTime > bikingTime) {
        
        std::vector<TTripStep> bikingFastestPath;
        for (int p = 0; p < bikingPath.size(); p++) {
            bikingFastestPath.push_back(TTripStep(ETransportationMode::Bike,
                                                  std::any_cast<TNodeID>(DImplementation -> m_fastestBikeGraph 
                                                                         -> GetVertexTag(bikingPath[p]))));
        }
        
        path = bikingFastestPath;
        return bikingTime;
    }
    else {

        std::vector<TTripStep> walkBussingFastestPath;
        walkBussingFastestPath.push_back(TTripStep(ETransportationMode::Walk, std::any_cast<TNodeID>(DImplementation -> m_fastestBusWalkGraph 
                                                -> GetVertexTag(walkBussingPath[0]))));
        for (int p = 1; p < walkBussingPath.size(); p++) {
            TNodeID currNodeID = std::any_cast<TNodeID>(DImplementation -> m_fastestBusWalkGraph 
                                                        -> GetVertexTag(walkBussingPath[p - 1]));
            TNodeID nextNodeID = std::any_cast<TNodeID>(DImplementation -> m_fastestBusWalkGraph 
                                                        -> GetVertexTag(walkBussingPath[p]));
            std::cout << "Cur Node ID: " << currNodeID << std::endl;
            for (int v = 0; v < busWalkEdgeData[currNodeID].size(); v++) {
                if (busWalkEdgeData[currNodeID][v].m_nextNodeID == nextNodeID) {
                    ETransportationMode mode = busWalkEdgeData[currNodeID][v].m_transMode;
                    walkBussingFastestPath.push_back(TTripStep(mode, nextNodeID));
                }
            }
        }

        path = walkBussingFastestPath;
        return walkBussingTime;
    }
    
}

bool CDijkstraTransportationPlanner::GetPathDescription(const std::vector< TTripStep > &path, std::vector< std::string > &desc) const {
    // TODO
    return false; 
}