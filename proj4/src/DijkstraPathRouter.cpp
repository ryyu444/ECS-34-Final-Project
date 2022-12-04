#include "DijkstraPathRouter.h"
#include <unordered_map>
#include <algorithm>
#include <iostream>

// bool minHeapCompare()

struct CDijkstraPathRouter::SImplementation {
    public:

        struct SVertex {
            CPathRouter::TVertexID m_id;
            std::any m_tag;
            std::vector<std::shared_ptr<SImplementation::SVertex>> m_adjacents;
            // std::vector<std::vector<double>>
            // vector[VertexID][ADJVertexID] = weight;

            SVertex(CPathRouter::TVertexID id, std::any tag) {
                m_id = id;
                m_tag = tag;
                m_adjacents = {};
            }
        };

        std::vector<std::shared_ptr<SImplementation::SVertex>> m_vertices; // collection of all vertices in graph
        std::unordered_map<TVertexID, std::unordered_map<TVertexID, double>> m_edgeWeights;

        SImplementation() {
            m_vertices = {};
            m_edgeWeights = {};
        };
};

CDijkstraPathRouter::CDijkstraPathRouter() {
    DImplementation = std::make_unique<SImplementation>(SImplementation());
}

CDijkstraPathRouter::~CDijkstraPathRouter() {}

std::size_t CDijkstraPathRouter::VertexCount() const noexcept {
    return DImplementation -> m_vertices.size();
}

CPathRouter::TVertexID CDijkstraPathRouter::AddVertex(std::any tag) noexcept {
    std::shared_ptr<SImplementation::SVertex> tmpVertex = 
        std::make_shared<SImplementation::SVertex>(DImplementation -> m_vertices.size(), tag);
    DImplementation -> m_vertices.push_back(tmpVertex);
    return tmpVertex -> m_id;
}

std::any CDijkstraPathRouter::GetVertexTag(CPathRouter::TVertexID id) const noexcept {
    if(id >= DImplementation -> m_vertices.size()) {
        return std::any();
    }
    return DImplementation -> m_vertices[id] -> m_tag;
}

bool CDijkstraPathRouter::AddEdge(CPathRouter::TVertexID src, CPathRouter::TVertexID dest, double weight, bool bidir) noexcept {
    
    if (src >= DImplementation -> m_vertices.size() || dest >= DImplementation -> m_vertices.size() || weight < 0) {
        return false;
    }
    
    std::shared_ptr<SImplementation::SVertex> srcVertex = DImplementation -> m_vertices[src];
    std::shared_ptr<SImplementation::SVertex> destVertex = DImplementation -> m_vertices[dest];
    srcVertex -> m_adjacents.push_back(destVertex);
    DImplementation -> m_edgeWeights[src][dest] = weight;
    
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

    if (src >= DImplementation -> m_vertices.size() || dest >= DImplementation -> m_vertices.size()) {
        return NoPathExists;
    }

    std::shared_ptr<SImplementation::SVertex> srcVertex = DImplementation -> m_vertices[src];
    std::shared_ptr<SImplementation::SVertex> destVertex = DImplementation -> m_vertices[dest];

    std::vector<std::shared_ptr<SImplementation::SVertex>> toVisit = {srcVertex};
    std::vector<std::shared_ptr<SImplementation::SVertex>> prevVertex(DImplementation -> m_vertices.size(), nullptr);
    std::vector<bool> visited(DImplementation -> m_vertices.size(), false);
    std::vector<double> distances(DImplementation -> m_vertices.size(), CPathRouter::NoPathExists);
    // std::unordered_map<TVertexID, std::unordered_map<TVertexID, bool>> numEdges;
    distances[src] = 0;

    auto sortDistMin = [&distances](auto &vert1, auto &vert2) {
        // std::cout << "sorting: " << vert1->m_id << ", " << vert2->m_id << std::endl;
        // std::cout << "distances: " << distances[vert1 -> m_id] << ", " << distances[vert2->m_id] << std::endl;
        return distances[vert1 -> m_id] > distances[vert2 -> m_id];
    };
    std::make_heap(toVisit.begin(), toVisit.end(), sortDistMin);

    while (!toVisit.empty()) {
        
        std::shared_ptr<SImplementation::SVertex> currVertex = toVisit.front();
        // std::cout << "Heap: ";
        // for (int i = 0; i < toVisit.size(); i++) {
        //     std::cout << toVisit[i] -> m_id << " ";
        // }
        // std::cout << std::endl;
       
        std::pop_heap(toVisit.begin(), toVisit.end(), sortDistMin);
        toVisit.pop_back();
          
        visited[currVertex -> m_id] = true;

        // std::cout << "Curr Vertex ID: " << currVertex -> m_id << std::endl;
        
        for (int j = 0; j < currVertex->m_adjacents.size(); j++) {
            // calculate the distance from cur vertex to the adj
            // std::cout << "Adj Vertex ID: " << currVertex->m_adjacents[j] -> m_id << std::endl;
            TVertexID adjVertID = currVertex->m_adjacents[j] -> m_id;
            double tmpDist = distances[currVertex->m_id] + DImplementation -> m_edgeWeights[currVertex->m_id][adjVertID];
            // std::cout << "Distances: (" << tmpDist << ", " << distances[currVertex->m_adjacents[j].first->m_id] << ")" << std::endl;
            
            // Check if new calculated dist is shorter than existing path to adj vertex
            if (tmpDist < distances[adjVertID]) {
                distances[adjVertID] = tmpDist;
                prevVertex[adjVertID] = currVertex;
                // std::cout << "  tmpDist: " << tmpDist << std::endl;
                // std::cout << "  prevVertex: " << currVertex->m_adjacents[j] -> m_id << std::endl;
                // Adding duplicate edges
                if (!visited[adjVertID]) {
                    // std::cout << "Adj ID: " << currVertex->m_adjacents[j].first -> m_id << std::endl;
                    toVisit.push_back(currVertex -> m_adjacents[j]);
                    // numEdges[currVertex -> m_id][adjVertID] = true;
                }
            }

            // std::cout << "Visited: " << visited[currVertex -> m_adjacents[j] -> m_id] << std::endl;
        }

    }

    if (distances[dest] == NoPathExists) {
        return NoPathExists;
    }

    // Finds the path started from destination to the src vertex
    CPathRouter::TVertexID currID = dest;
    path.push_back(dest);
    while (prevVertex[currID]) {
        currID = prevVertex[currID]->m_id;
        path.push_back(currID);
    }

    std::reverse(path.begin(), path.end());
    
    return distances[dest];
}