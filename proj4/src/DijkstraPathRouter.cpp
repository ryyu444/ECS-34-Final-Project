#include "DijkstraPathRouter.h"

struct CDijkstraPathRouter::SImplementation {

    public: 
        SImplementation() {}
};

CDijkstraPathRouter::CDijkstraPathRouter() {
    DImplementation = std::make_unique<SImplementation>(SImplementation());
}

CDijkstraPathRouter::~CDijkstraPathRouter() {}

std::size_t CDijkstraPathRouter::VertexCount() const noexcept {
    return 1;
}

CPathRouter::TVertexID CDijkstraPathRouter::AddVertex(std::any tag) noexcept {
    return 1;
}

std::any CDijkstraPathRouter::GetVertexTag(CPathRouter::TVertexID id) const noexcept {
    return std::any(1);
}

bool CDijkstraPathRouter::AddEdge(CPathRouter::TVertexID src, CPathRouter::TVertexID dest, double weight, bool bidir) noexcept {
    return false;
}

bool CDijkstraPathRouter::Precompute(std::chrono::steady_clock::time_point deadline) noexcept {
    return false;
}

double CDijkstraPathRouter::FindShortestPath(CPathRouter::TVertexID src, CPathRouter::TVertexID dest, std::vector<CPathRouter::TVertexID> &path) noexcept {
    return 0.0;
}
