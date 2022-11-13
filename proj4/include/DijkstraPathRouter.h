#ifndef DIJKSTRAPATHROUTER_H
#define DIJKSTRAPATHROUTER_H

#include "PathRouter.h"
#include <memory>

class CDijkstraPathRouter : public CPathRouter{
    private:
        struct SImplementation;
        std::unique_ptr<SImplementation> DImplementation;
    public:
        CDijkstraPathRouter();
        ~CDijkstraPathRouter();

        std::size_t VertexCount() const noexcept;
        TVertexID AddVertex(std::any tag) noexcept;
        std::any GetVertexTag(TVertexID id) const noexcept;
        bool AddEdge(TVertexID src, TVertexID dest, double weight, bool bidir = false) noexcept;
        bool Precompute(std::chrono::steady_clock::time_point deadline) noexcept;
        double FindShortestPath(TVertexID src, TVertexID dest, std::vector<TVertexID> &path) noexcept;
};

#endif
