#include "DijkstraPathRouter.h"
#include <unordered_map>
#include <deque>
#include <algorithm>

struct CDijkstraPathRouter::SImplementation {
    public:

        struct SVertex {
            CPathRouter::TVertexID m_id;
            std::any m_tag;
            std::vector<std::pair<std::shared_ptr<SImplementation::SVertex>, double>> m_adjacents;

            SVertex(CPathRouter::TVertexID id, std::any tag) {
                m_id = id;
                m_tag = tag;
                m_adjacents = {};
            }
        };

        std::vector<std::shared_ptr<SImplementation::SVertex>> m_graph; // collection of all vertices in graph
        
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
    std::shared_ptr<SImplementation::SVertex> tmpVertex = 
        std::make_shared<SImplementation::SVertex>(DImplementation -> m_graph.size(), tag);
    DImplementation -> m_graph.push_back(tmpVertex);
    return tmpVertex -> m_id;
}

std::any CDijkstraPathRouter::GetVertexTag(CPathRouter::TVertexID id) const noexcept {
    if(id >= DImplementation -> m_graph.size()) {
        return std::any();
    }
    return DImplementation -> m_graph[id] -> m_tag;
}

bool CDijkstraPathRouter::AddEdge(CPathRouter::TVertexID src, CPathRouter::TVertexID dest, double weight, bool bidir) noexcept {
    
    if (src >= DImplementation -> m_graph.size() || dest >= DImplementation -> m_graph.size() || weight < 0) {
        return false;
    }
    
    std::shared_ptr<SImplementation::SVertex> srcVertex = DImplementation -> m_graph[src];
    std::shared_ptr<SImplementation::SVertex> destVertex = DImplementation -> m_graph[dest];
    srcVertex -> m_adjacents.push_back(std::make_pair(destVertex, weight));
    
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

    std::shared_ptr<SImplementation::SVertex> srcVertex = DImplementation -> m_graph[src];
    std::shared_ptr<SImplementation::SVertex> destVertex = DImplementation -> m_graph[dest];

    std::deque<std::shared_ptr<SImplementation::SVertex>> toVisit = {srcVertex};
    std::vector<std::shared_ptr<SImplementation::SVertex>> prevVertex;
    std::vector<bool> visited;
    std::vector<double> distances;

    for (int i = 0; i < DImplementation -> m_graph.size(); i++) {
        prevVertex.push_back(nullptr);
        visited.push_back(false);
        distances.push_back(CPathRouter::NoPathExists);
    }

    distances[src] = 0;

    while (!toVisit.empty()) {
        std::shared_ptr<SImplementation::SVertex> currVertex = toVisit.front();
        toVisit.pop_front();
        visited[currVertex->m_id] = true;

        for (int j = 0; j < currVertex->m_adjacents.size(); j++) {
            if (!visited[currVertex->m_adjacents[j].first->m_id]) {
                // calculate the distance from cur vertex to the adj
                double tmpDist = distances[currVertex->m_id] + currVertex->m_adjacents[j].second;
                
                // Check if new calculated dist is shorter than existing path to adj vertex
                if (tmpDist < distances[currVertex->m_adjacents[j].first->m_id]) {
                    distances[currVertex->m_adjacents[j].first->m_id] = tmpDist;
                    prevVertex[currVertex->m_adjacents[j].first->m_id] = currVertex;
                }

                // break out early if destination processed
                // if(currVertex->m_adjacents[j].first->m_id == dest) {
                //     break;
                // }
                toVisit.push_back(currVertex->m_adjacents[j].first);
            }
        }
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
