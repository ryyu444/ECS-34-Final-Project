#include "DijkstraPathRouter.h"
#include <unordered_map>
#include <algorithm>

#include <iostream>

struct SVertex {
    CPathRouter::TVertexID m_id;
    std::any m_tag;
    std::unordered_map<CPathRouter::TVertexID, double> m_adjacents;

    SVertex(CPathRouter::TVertexID id, std::any tag) {
        m_id = id;
        m_tag = tag;
        m_adjacents = {};
    }
};

struct SEdge {
    CPathRouter::TVertexID m_parent;
    CPathRouter::TVertexID m_child;
    double m_weight;

    SEdge(CPathRouter::TVertexID parent, CPathRouter::TVertexID child, double weight) {
        m_parent = parent;
        m_child = child;
        m_weight = weight;
    };
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
    
    DImplementation -> m_graph[src].m_adjacents[dest] = weight;
    
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

    std::vector<SEdge> edgeMinHeap;
    std::vector<double> distances(DImplementation -> m_graph.size(), CPathRouter::NoPathExists);
    std::vector<TVertexID> prevVertex(DImplementation -> m_graph.size(), InvalidVertexID);
    std::vector<bool> visited(DImplementation -> m_graph.size(), false);
    distances[src] = 0;
    
    auto minHeapCmp = [](SEdge &edge1, SEdge &edge2) {
        return edge1.m_weight > edge2.m_weight;
    };

    // store src adjacents into minheap to init
    visited[src] = true;
    for (auto vert: DImplementation -> m_graph[src].m_adjacents) {
        edgeMinHeap.push_back(SEdge(src, vert.first, vert.second));
        distances[vert.first] = vert.second;
        prevVertex[vert.first] = src;
    }
    int count = 0;
    std::make_heap(edgeMinHeap.begin(), edgeMinHeap.end(), minHeapCmp);

    // 2) Extract the vertex with minimum distance value node from Min Heap
    // 3) 
    // Deal with duplicates - Check in distances if value is NOT NOPATHEXISTS
    while (!edgeMinHeap.empty()) {
        TVertexID currVertexID = edgeMinHeap.front().m_parent;
        TVertexID adjVertexID = edgeMinHeap.front().m_child;

        double dist = edgeMinHeap.front().m_weight + distances[currVertexID];
        if (dist < distances[adjVertexID]) {
            distances[adjVertexID] = dist;
            prevVertex[adjVertexID] = currVertexID;
            visited[adjVertexID] = true;
        }
        count++;
        // if (distances[])
        // prevVertex[adjVertexID] = currVertexID;
        // distances[adjVertexID] = 
        
        // std::cout << "Curr Vertex Id: " << currVertexID << std::endl;
        // std::cout << "Count: " << count << std::endl;
        // if (currVertexID == dest) {
        //     break;
        // }

        std::pop_heap(edgeMinHeap.begin(), edgeMinHeap.end(), minHeapCmp);
        edgeMinHeap.pop_back();
        

        // std::cout << "Adj Size: " << DImplementation -> m_graph[currVertexID].m_adjacents.size() << std::endl;
        // std::cout << "real adj Size: " << DImplementation->m_graph[src].m_adjacents.size() << std::endl;

        for (auto entry : DImplementation -> m_graph[adjVertexID].m_adjacents) {
            // Vertex has been visited
            auto key = entry.first;
            auto weight = entry.second;
            if(!visited[key]) {
                // std::cout << " Adj ID: " << key << std::endl;
                // std::cout << " Weight: " << weight << std::endl;
                edgeMinHeap.push_back(SEdge(adjVertexID, key, weight));
                std::push_heap(edgeMinHeap.begin(), edgeMinHeap.end(), minHeapCmp);
            }
            // Otherwise, add to min heap & calculate dist
            // Calculate dist w/ weight of currVertx - weight of adj + dist of currVertex
            // Cur Vertex dist + Adj vertex dist
        
            // std::cout << "edgeMinHeap: ";
            // for( SEdge v : edgeMinHeap ) {std::cout << v.m_parent << ' ' ;}
            // std::cout << '\n' ;

        }
    }

    if (distances[dest] == NoPathExists) {
        return NoPathExists;
    }

    TVertexID currVertexID = dest;
    while (prevVertex[currVertexID] != InvalidVertexID) {
        path.push_back(currVertexID);
        currVertexID = prevVertex[currVertexID];
    }
    path.push_back(src);
    std::reverse(path.begin(), path.end());

    return distances[dest];
    
}