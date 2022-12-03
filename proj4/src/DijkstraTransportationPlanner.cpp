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

        struct EdgeData {
            TNodeID m_nextNodeID = CStreetMap::InvalidNodeID;
            std::string m_streetName;
            ETransportationMode m_transMode;
            double m_time;
            double m_speedLimit;
            double m_distance;
            std::string currRoute = "";

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

        std::shared_ptr<SConfiguration> m_config;
        std::shared_ptr<CStreetMap> m_streetMap;
        std::shared_ptr<CBusSystem> m_busSystem;
        std::unique_ptr<CBusSystemIndexer> m_busSystemIndexer;
        std::vector<std::shared_ptr<CStreetMap::SNode>> m_sortedNodesByID;

        std::unique_ptr<CDijkstraPathRouter> m_shortestPathGraph;
        std::unique_ptr<CDijkstraPathRouter> m_fastestBusWalkGraph;
        std::unique_ptr<CDijkstraPathRouter> m_fastestBikeGraph;
        std::unordered_map<TNodeID, std::vector<EdgeData>> m_bikeEdgeData;
        std::unordered_map<TNodeID, std::vector<EdgeData>> m_busWalkEdgeData;
        std::unordered_map<TNodeID, CPathRouter::TVertexID> m_vertexNodesMap;
        bool m_shortestNodesPopulated;
        bool m_fastestNodesPopulated;

        double m_walkSpeedLimit, m_bikeSpeedLimit, m_busStopTime, m_defaultSpeedLimit;


        
        SImplementation(std::shared_ptr<SConfiguration> config) {
            m_config = config;
            m_streetMap = m_config -> StreetMap();
            m_busSystem = m_config -> BusSystem();
            m_busSystemIndexer = std::make_unique<CBusSystemIndexer>(m_busSystem);
            m_shortestPathGraph = std::make_unique<CDijkstraPathRouter>();
            m_fastestBusWalkGraph = std::make_unique<CDijkstraPathRouter>();
            m_fastestBikeGraph = std::make_unique<CDijkstraPathRouter>();
            m_shortestNodesPopulated = false;
            m_fastestNodesPopulated = false;

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
    if(!DImplementation -> m_shortestNodesPopulated) {
        // Adds all nodes as vertices in the graph + Map TNodeIDs to TVertexIDs
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
        
        // Distance Vector to store min distances of each vertex
        // std::vector<double> distances(DImplementation -> m_vertexNodesMap.size(), CPathRouter::NoPathExists);
        
        for (int k = 0; k < DImplementation -> m_streetMap -> WayCount(); k++) {
            
            std::shared_ptr<CStreetMap::SWay> currWay = DImplementation -> m_streetMap -> WayByIndex(k);
            
            // check if one way or not 
            bool isOneWay = currWay -> GetAttribute("oneway") == "yes" ? true : false;

            // Iterates through ways & builds edges between nodes
            for (int l = 0; l < currWay -> NodeCount() - 1; l++) {
                TNodeID currNodeID = currWay -> GetNodeID(l);
                TNodeID nextNodeID = currWay -> GetNodeID(l + 1);

                std::shared_ptr<CStreetMap::SNode> curr_node = DImplementation -> m_streetMap -> NodeByID(currNodeID);
                std::shared_ptr<CStreetMap::SNode> next_node = DImplementation -> m_streetMap -> NodeByID(nextNodeID);
                
                double dist = SGeographicUtils::HaversineDistanceInMiles(curr_node -> Location(), next_node -> Location());

                // std::cout << "currVert id: " << DImplementation -> m_vertexNodesMap[currNodeID] << std::endl; 
                // std::cout << "nextVert id: " << DImplementation -> m_vertexNodesMap[nextNodeID] << std::endl; 
                // std::cout << "dist: " << dist << std::endl;

                // 1) Only add new edge to nextNode if the calculated dist is less than the existing min dist
                // 2) Update the min dist to that nextNode
                // if (dist < distances[DImplementation -> m_vertexNodesMap[nextNodeID]]) {                    
                DImplementation -> m_shortestPathGraph -> AddEdge(DImplementation -> m_vertexNodesMap[currNodeID], 
                                                                  DImplementation -> m_vertexNodesMap[nextNodeID], 
                                                                  dist, !isOneWay);
                    // distances[DImplementation -> m_vertexNodesMap[nextNodeID]] = dist;
                // }
            }
        }

        DImplementation -> m_shortestNodesPopulated = true;
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
    // std::cout << "Vertex Count: " << DImplementation -> m_fastestBusWalkGraph -> VertexCount() << std::endl;
    if (!DImplementation -> m_fastestNodesPopulated) {
        // Adds all nodes as vertices in the graph
        for (int j = 0; j < NodeCount(); j++) {
            CPathRouter::TVertexID tmp_vert_id;
            
            TNodeID curr_node_ID = SortedNodeByIndex(j) -> ID();
            
            tmp_vert_id = DImplementation -> m_fastestBusWalkGraph -> AddVertex(curr_node_ID);
            DImplementation -> m_fastestBikeGraph -> AddVertex(curr_node_ID);

            // IF shortest path has not populated the vertexNodeMap, do it here
            if (!DImplementation -> m_shortestNodesPopulated) {
                DImplementation -> m_vertexNodesMap[curr_node_ID] = tmp_vert_id;
            }
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
            
            std::shared_ptr<CStreetMap::SWay> currWay = DImplementation -> m_streetMap -> WayByIndex(k);
            
            // check if one way or not 
            bool isOneWay = currWay -> GetAttribute("oneway") == "yes" ? true : false;
            bool bikingAllowed = currWay -> GetAttribute("bicycle") == "no" ? false : true;
            std::string streetName = currWay -> GetAttribute("name");
            std::string rawSpeedLimit = currWay -> GetAttribute("maxspeed");
            // std::cout << "Raw Speed: " << rawSpeedLimit << std::endl;
            double speedLimit = rawSpeedLimit != "" ? 
                std::stod(StringUtils::Split(rawSpeedLimit)[0]) : 
                DImplementation -> m_defaultSpeedLimit;
            // std::cout << "Speed Limit Double: " << speedLimit << std::endl;
            
            

            // Iterates through ways & builds edges between nodes for biking and walking
            // Check if there are routes between the nodes
            for (int l = 0; l < currWay -> NodeCount() - 1; l++) {
                TNodeID currNodeID = currWay -> GetNodeID(l);
                TNodeID nextNodeID = currWay -> GetNodeID(l + 1);

                // std::cout << "Cur Node ID: " << currNodeID << std::endl;
                // std::cout << "Next Node ID: " << nextNodeID << std::endl;

                std::shared_ptr<CStreetMap::SNode> curr_node = DImplementation -> m_streetMap -> NodeByID(currNodeID);
                std::shared_ptr<CStreetMap::SNode> next_node = DImplementation -> m_streetMap -> NodeByID(nextNodeID);
                
                // Seg Fault here
                std::unordered_set<std::shared_ptr<CBusSystem::SRoute>> busRoutes;
                DImplementation -> m_busSystemIndexer -> RoutesByNodeIDs(currNodeID, nextNodeID, busRoutes);
                // for (auto i : busRoutes) {
                //     std::cout << "Route: " << i -> Name() << std::endl;
                // }
                // std::cout << "End" << std::endl;
                
                // time = distance / velocity (speed)
                double dist = SGeographicUtils::HaversineDistanceInMiles(curr_node -> Location(), next_node -> Location());
                double bikingTime = dist / DImplementation -> m_bikeSpeedLimit;
                double walkingTime = dist / DImplementation -> m_walkSpeedLimit;
                double busTime = dist / speedLimit + (DImplementation -> m_busStopTime / 3600);
                // std::cout << "Bus Time: " << busTime << std::endl;
                
                // std::cout << "currNode id: " << curr_node -> ID() << std::endl; 
                // std::cout << "nextNode id: " << next_node -> ID() << std::endl; 
                // std::cout << "distance in miles: " << dist << std::endl;
                
                if (bikingAllowed) {
                    DImplementation -> m_fastestBikeGraph -> AddEdge(DImplementation -> m_vertexNodesMap[currNodeID], 
                                                                    DImplementation -> m_vertexNodesMap[nextNodeID], 
                                                                    bikingTime, !isOneWay);
                    DImplementation -> m_bikeEdgeData[currNodeID].push_back(SImplementation::EdgeData(nextNodeID, streetName, bikingTime, 
                                                                DImplementation -> m_bikeSpeedLimit, dist, 
                                                                ETransportationMode::Bike));
                }

                // std::cout << "Walking Time: " << walkingTime << std::endl;
                
                // Bus Route exists so create edge between cur & next w/ bus time
                if (busRoutes.size()) {
                    // std::cout << "Adj Edges before: " << DImplementation -> m_busWalkEdgeData[currNodeID].size() << std::endl;
                    // std::cout << "currVert id: " << DImplementation -> m_vertexNodesMap[currNodeID] << std::endl; 
                    // std::cout << "nextVert id: " << DImplementation -> m_vertexNodesMap[nextNodeID] << std::endl;
                    
                    DImplementation -> m_fastestBusWalkGraph -> AddEdge(DImplementation -> m_vertexNodesMap[currNodeID], 
                                                                        DImplementation -> m_vertexNodesMap[nextNodeID], 
                                                                        busTime, false);
                    // std::cout << "Added!" << std::endl;
                    DImplementation -> m_busWalkEdgeData[currNodeID].push_back(SImplementation::EdgeData(nextNodeID, streetName, busTime, speedLimit, dist, 
                                                                    ETransportationMode::Bus));
                    // std::cout << "Adj Edges after: " << DImplementation -> m_busWalkEdgeData[currNodeID].size() << std::endl;
                    // std::cout << "currNode: " << currNodeID << ", EdgeData: (" << DImplementation -> m_busWalkEdgeData[currNodeID][0].m_nextNodeID << ", " << DImplementation -> m_busWalkEdgeData[currNodeID][0].m_time << ")\n";
                }

                else {
                    // std::cout << "Not added" << std::endl;
                    DImplementation -> m_fastestBusWalkGraph -> AddEdge(DImplementation -> m_vertexNodesMap[currNodeID], 
                                                                        DImplementation -> m_vertexNodesMap[nextNodeID], 
                                                                        walkingTime, true);
                    DImplementation -> m_busWalkEdgeData[currNodeID].push_back(SImplementation::EdgeData(nextNodeID, streetName, walkingTime, speedLimit, dist, 
                                                                    ETransportationMode::Walk));
                }
                // std::cout << "currNode: " << currNodeID << ", EdgeData: (" << busWalkEdgeData[currNodeID][0].m_nextNodeID 
                //     << ", " << busWalkEdgeData[currNodeID][0].m_distance
                //     << ")\n";
                // std::cout << speed
            }
        }

        DImplementation -> m_fastestNodesPopulated = true;
    }

    // Finds shortest path for biking
    std::vector<CPathRouter::TVertexID> bikingPath;
    double bikingTime = DImplementation -> m_fastestBikeGraph -> FindShortestPath(DImplementation -> m_vertexNodesMap[src], 
                                                   DImplementation -> m_vertexNodesMap[dest], 
                                                   bikingPath);

    // std::cout << "Biking Time: " << bikingTime << std::endl;
    // std::cout << "Biking Path Size: " << bikingPath.size() << std::endl;
    // for (auto i : bikingPath) {
    //     std::cout << "Node ID: " << std::any_cast<TNodeID>(DImplementation -> m_fastestBusWalkGraph 
    //                                                     -> GetVertexTag(i)) << std::endl;
    // }
    // Finds shortest path for walking/bussing
    std::vector<CPathRouter::TVertexID> walkBussingPath;
    double walkBussingTime = DImplementation -> m_fastestBusWalkGraph -> FindShortestPath(DImplementation -> m_vertexNodesMap[src], 
                                                   DImplementation -> m_vertexNodesMap[dest], 
                                                   walkBussingPath);

    // std::cout << "Bus-Walk Time: " << walkBussingTime << std::endl;
    // std::cout << "Walk-Bus Path Size: " << walkBussingPath.size() << std::endl;


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
        if (walkBussingTime == CPathRouter::NoPathExists) {
            return walkBussingTime;
        }

        std::vector<TTripStep> walkBussingFastestPath;
        walkBussingFastestPath.push_back(TTripStep(ETransportationMode::Walk, std::any_cast<TNodeID>(DImplementation -> m_fastestBusWalkGraph 
                                                -> GetVertexTag(walkBussingPath[0]))));
        for (int p = 1; p < walkBussingPath.size(); p++) {
            TNodeID currNodeID = std::any_cast<TNodeID>(DImplementation -> m_fastestBusWalkGraph 
                                                        -> GetVertexTag(walkBussingPath[p - 1]));
            TNodeID nextNodeID = std::any_cast<TNodeID>(DImplementation -> m_fastestBusWalkGraph 
                                                        -> GetVertexTag(walkBussingPath[p]));
            for (int v = 0; v < DImplementation -> m_busWalkEdgeData[currNodeID].size(); v++) {
                if (DImplementation -> m_busWalkEdgeData[currNodeID][v].m_nextNodeID == nextNodeID) {
                    ETransportationMode mode = DImplementation -> m_busWalkEdgeData[currNodeID][v].m_transMode;
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