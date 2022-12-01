#include "DijkstraPathRouter.h"
#include <unordered_map>
#include <deque>
#include <algorithm>

#include <iostream>

struct SVertex {
    CPathRouter::TVertexID m_id;
    std::any m_tag;
    std::vector<std::pair<SVertex, double>> m_adjacents;

    SVertex(CPathRouter::TVertexID id, std::any tag) {
        m_id = id;
        m_tag = tag;
        m_adjacents = {};
    }
};

struct CDijkstraPathRouter::SImplementation {
    public:

        std::vector<SVertex> m_graph; // collection of all vertices in graph
        
        SImplementation() {
            m_graph = {};
        };
};

CDijkstraPathRouter::CDijkstraPathRouter() {
    DImplementation = std::make_unique<SImplementation>(SImplementation());
}

CDijkstraPathRouter::~CDijkstraPathRouter() {}

std::size_t CDijkstraPathRouter::VertexCount() const noexcept {
    return DImplementation -> m_graph.size();
}

CPathRouter::TVertexID CDijkstraPathRouter::AddVertex(std::any tag) noexcept {
    DImplementation -> m_graph.push_back(SVertex(DImplementation -> m_graph.size(), tag));
    // DImplementation -> m_graph[tmpVertex.m_id].m_adjacents.push_back(std::make_pair(tmpVertex, 0));
    return DImplementation -> m_graph.back().m_id;
}

std::any CDijkstraPathRouter::GetVertexTag(CPathRouter::TVertexID id) const noexcept {
    if(id >= DImplementation -> m_graph.size()) {
        return std::any();
    }
    return DImplementation -> m_graph[id].m_tag;
}

bool CDijkstraPathRouter::AddEdge(CPathRouter::TVertexID src, CPathRouter::TVertexID dest, double weight, bool bidir) noexcept {
    
    if (src >= DImplementation -> m_graph.size() || dest >= DImplementation -> m_graph.size() || weight < 0) {
        return false;
    }
    
    DImplementation -> m_graph[src].m_adjacents.push_back(std::make_pair(DImplementation -> m_graph[dest], weight));
    
    if (bidir) {
        AddEdge(dest, src, weight, false);
    }

    return true;
}

bool CDijkstraPathRouter::Precompute(std::chrono::steady_clock::time_point deadline) noexcept {
    // std::chrono::steady_clock::duration dur;

    // while (std::chrono::steady_clock::now != deadline) {

    // }
    // TODO
    return false;
}

double CDijkstraPathRouter::FindShortestPath(CPathRouter::TVertexID src, CPathRouter::TVertexID dest, std::vector<CPathRouter::TVertexID> &path) noexcept {

    path.clear();

    if (src >= DImplementation -> m_graph.size() || dest >= DImplementation -> m_graph.size()) {
        return false;
    }

    // 1) Make min heap from m_graph w/ nodes as TVertexIDs & weights - Initialize w/ src vertex as root

    std::vector<SVertex> vertDistFromSource = {DImplementation -> m_graph[src]};
    std::vector<double> distances(DImplementation -> m_graph.size(), CPathRouter::NoPathExists);
    std::vector<TVertexID> prevVertex(DImplementation -> m_graph.size(), InvalidVertexID);
    std::vector<bool> visited(DImplementation -> m_graph.size(), false);
    distances[src] = 0;
    
    auto minHeapCmp = [distances](SVertex &vert1, SVertex &vert2) {
        return distances[vert1.m_id] > distances[vert2.m_id];
    };
    
    std::make_heap(vertDistFromSource.begin(), vertDistFromSource.end(), minHeapCmp);

    // 2) Extract the vertex with minimum distance value node from Min Heap
    // 3) 
    // Deal with duplicates - Check in distances if value is NOT NOPATHEXISTS
    while (!vertDistFromSource.empty()) {
        TVertexID currVertexID = vertDistFromSource.front().m_id;
        // std::cout << "Curr Vertex Id: " << currVertexID << std::endl;
        if (currVertexID == dest) {
            break;
        }

        std::pop_heap(vertDistFromSource.begin(), vertDistFromSource.end(), minHeapCmp);
        vertDistFromSource.pop_back();
        
        visited[currVertexID] = true;

        // std::cout << "Adj Size: " << DImplementation -> m_graph[currVertexID].m_adjacents.size() << std::endl;
        // std::cout << "real adj Size: " << DImplementation->m_graph[src].m_adjacents.size() << std::endl;

        for (int i = 0; i < DImplementation -> m_graph[currVertexID].m_adjacents.size(); i++) {
            // Vertex has been visited 
            // std::cout << " Adj ID: " << DImplementation -> m_graph[currVertexID].m_adjacents[i].first.m_id << std::endl;
            if (visited[DImplementation -> m_graph[currVertexID].m_adjacents[i].first.m_id]) {
                continue;
            }
            // Otherwise, add to min heap & calculate dist
            // Calculate dist w/ weight of currVertx - weight of adj + dist of currVertex
            else {
                // Cur Vertex dist + Adj vertex dist
                double dist = distances[currVertexID] + DImplementation -> m_graph[currVertexID].m_adjacents[i].second;
                
                if (dist < distances[DImplementation -> m_graph[currVertexID].m_adjacents[i].first.m_id]) {
                    distances[DImplementation -> m_graph[currVertexID].m_adjacents[i].first.m_id] = dist;
                    prevVertex[DImplementation -> m_graph[currVertexID].m_adjacents[i].first.m_id] = currVertexID;
                }

                vertDistFromSource.push_back(DImplementation -> m_graph[currVertexID].m_adjacents[i].first);
                std::push_heap(vertDistFromSource.begin(), vertDistFromSource.end(), minHeapCmp);

                // std::cout << "vertDistFromSource: ";
                // for( SVertex v : vertDistFromSource ) {std::cout << v.m_id << ' ' ;}
                // std::cout << '\n' ;
            }

            

        }
    }

    TVertexID currVertexID = dest;
    while (prevVertex[currVertexID] != InvalidVertexID) {
        path.push_back(currVertexID);
        currVertexID = prevVertex[currVertexID];
    }
    path.push_back(src);
    std::reverse(path.begin(), path.end());

    return distances[dest];

    // std::cout << "Distances:" << std::endl;
    // for (int i = 0; i < distances.size(); i++) {
    //     std::cout << distances[i] << ", ";;
    // }
    // std::cout << std::endl;

    // std::cout << "Previous:" << std::endl;
    // for (int i = 0; i < prevVertex.size(); i++) {
    //     std::cout << prevVertex[i] << ", ";;
    // }
    // std::cout << std::endl;
    
}
