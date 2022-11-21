#include <gtest/gtest.h>
#include "DijkstraPathRouter.h"
#include "StreetMap.h"

TEST(DjikstraPathRouterTest, VertexCount) {

    {
        CStreetMap::TNodeID nd1 = 101;
        CStreetMap::TNodeID nd2 = 102;
        CStreetMap::TNodeID nd3 = 103;
        CStreetMap::TNodeID nd4 = 104;
        CStreetMap::TNodeID nd5 = 105;

        std::shared_ptr<CDijkstraPathRouter> DPRouter = std::make_shared<CDijkstraPathRouter>();

        EXPECT_EQ(DPRouter->AddVertex(nd1), 0);
        EXPECT_EQ(DPRouter->AddVertex(nd2), 1);
        EXPECT_EQ(DPRouter->AddVertex(nd3), 2);
        EXPECT_EQ(DPRouter->AddVertex(nd4), 3);
        EXPECT_EQ(DPRouter->AddVertex(nd5), 4);

        EXPECT_EQ(DPRouter->VertexCount(), 5);
    }

    {
        std::shared_ptr<CDijkstraPathRouter> DPRouter = std::make_shared<CDijkstraPathRouter>();
        EXPECT_EQ(DPRouter->VertexCount(), 0);
    }
}

TEST(DjikstraPathRouterTest, GetVertexTag) {
    
    {
        CStreetMap::TNodeID nd1 = 101;
        CStreetMap::TNodeID nd2 = 102;
        CStreetMap::TNodeID nd3 = 103;
        CStreetMap::TNodeID nd4 = 104;
        CStreetMap::TNodeID nd5 = 105;

        std::shared_ptr<CDijkstraPathRouter> DPRouter = std::make_shared<CDijkstraPathRouter>();

        EXPECT_EQ(DPRouter->AddVertex(nd1), 0);
        EXPECT_EQ(DPRouter->AddVertex(nd2), 1);
        EXPECT_EQ(DPRouter->AddVertex(nd3), 2);
        EXPECT_EQ(DPRouter->AddVertex(nd4), 3);
        EXPECT_EQ(DPRouter->AddVertex(nd5), 4);

        EXPECT_EQ(std::any_cast<CStreetMap::TNodeID>(DPRouter->GetVertexTag(0)), 101);
        EXPECT_EQ(std::any_cast<CStreetMap::TNodeID>(DPRouter->GetVertexTag(1)), 102);
        EXPECT_EQ(std::any_cast<CStreetMap::TNodeID>(DPRouter->GetVertexTag(2)), 103);
        EXPECT_EQ(std::any_cast<CStreetMap::TNodeID>(DPRouter->GetVertexTag(3)), 104);
        EXPECT_EQ(std::any_cast<CStreetMap::TNodeID>(DPRouter->GetVertexTag(4)), 105);
    }
}

TEST(DjikstraPathRouterTest, AddEdge) {

    {
        CStreetMap::TNodeID nd1 = 101;
        CStreetMap::TNodeID nd2 = 102;
        CStreetMap::TNodeID nd3 = 103;
        CStreetMap::TNodeID nd4 = 104;
        CStreetMap::TNodeID nd5 = 105;

        std::shared_ptr<CDijkstraPathRouter> DPRouter = std::make_shared<CDijkstraPathRouter>();

        EXPECT_EQ(DPRouter->AddVertex(nd1), 0);
        EXPECT_EQ(DPRouter->AddVertex(nd2), 1);
        EXPECT_EQ(DPRouter->AddVertex(nd3), 2);
        EXPECT_EQ(DPRouter->AddVertex(nd4), 3);
        EXPECT_EQ(DPRouter->AddVertex(nd5), 4);

        EXPECT_TRUE(DPRouter->AddEdge(0, 1, 1, true));
        EXPECT_TRUE(DPRouter->AddEdge(1, 2, 1, true));
        EXPECT_TRUE(DPRouter->AddEdge(2, 3, 1, true));
        EXPECT_TRUE(DPRouter->AddEdge(3, 4, 1, true));
        EXPECT_TRUE(DPRouter->AddEdge(4, 0, 100));
    }

}

TEST(DjikstraPathRouterTest, Precompute) {
 // TODO?
}

TEST(DjikstraPathRouterTest, FindShortestPath) {

    std::vector<CPathRouter::TVertexID> path;
    std::vector<CPathRouter::TVertexID> expectedPath;

    {
        CStreetMap::TNodeID nd1 = 101;
        CStreetMap::TNodeID nd2 = 102;
        CStreetMap::TNodeID nd3 = 103;
        CStreetMap::TNodeID nd4 = 104;
        CStreetMap::TNodeID nd5 = 105;

        std::shared_ptr<CDijkstraPathRouter> DPRouter = std::make_shared<CDijkstraPathRouter>();

        EXPECT_EQ(DPRouter->AddVertex(nd1), 0);
        EXPECT_EQ(DPRouter->AddVertex(nd2), 1);
        EXPECT_EQ(DPRouter->AddVertex(nd3), 2);
        EXPECT_EQ(DPRouter->AddVertex(nd4), 3);
        EXPECT_EQ(DPRouter->AddVertex(nd5), 4);

        EXPECT_TRUE(DPRouter->AddEdge(0, 1, 1, true));
        EXPECT_TRUE(DPRouter->AddEdge(1, 2, 1, true));
        EXPECT_TRUE(DPRouter->AddEdge(2, 3, 1, true));
        EXPECT_TRUE(DPRouter->AddEdge(3, 4, 1, true));
        EXPECT_TRUE(DPRouter->AddEdge(4, 0, 100));

        expectedPath = {1, 2, 3}; 
        EXPECT_EQ(DPRouter->FindShortestPath(1, 3, path), 2);
        EXPECT_EQ(path, expectedPath);

        expectedPath = {1}; 
        EXPECT_EQ(DPRouter->FindShortestPath(1, 1, path), 0);
        EXPECT_EQ(path, expectedPath);

        expectedPath = {4, 0}; 
        EXPECT_EQ(DPRouter->FindShortestPath(4, 0, path), 100);
        EXPECT_EQ(path, expectedPath);
    }


}
