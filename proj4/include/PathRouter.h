#ifndef PATHROUTER_H
#define PATHROUTER_H

#include <vector>
#include <limits>
#include <any>
#include <chrono>

class CPathRouter{
    public:
        using TVertexID = std::size_t;

        static constexpr TVertexID InvalidVertexID = std::numeric_limits<TVertexID>::max();
        static constexpr double NoPathExists = std::numeric_limits<double>::max();

        virtual ~CPathRouter(){};

        virtual std::size_t VertexCount() const noexcept = 0;
        virtual TVertexID AddVertex(std::any tag) noexcept = 0;
        virtual std::any GetVertexTag(TVertexID id) const noexcept = 0;
        virtual bool AddEdge(TVertexID src, TVertexID dest, double weight, bool bidir = false) noexcept = 0;
        virtual bool Precompute(std::chrono::steady_clock::time_point deadline) noexcept = 0;
        virtual double FindShortestPath(TVertexID src, TVertexID dest, std::vector<TVertexID> &path) noexcept = 0;
};

#endif
