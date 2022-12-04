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

        CDijkstraPathRouter DPRouter = CDijkstraPathRouter();

        EXPECT_EQ(DPRouter.AddVertex(nd1), 0);
        EXPECT_EQ(DPRouter.AddVertex(nd2), 1);
        EXPECT_EQ(DPRouter.AddVertex(nd3), 2);
        EXPECT_EQ(DPRouter.AddVertex(nd4), 3);
        EXPECT_EQ(DPRouter.AddVertex(nd5), 4);

        EXPECT_EQ(DPRouter.VertexCount(), 5);
    }

    {
        CDijkstraPathRouter DPRouter = CDijkstraPathRouter();
        EXPECT_EQ(DPRouter.VertexCount(), 0);
    }

    {
        CStreetMap::TNodeID nd1 = 1;
        CStreetMap::TNodeID nd2 = 5;
        CStreetMap::TNodeID nd3 = 3;
        CStreetMap::TNodeID nd4 = 15;

        CDijkstraPathRouter DPRouter = CDijkstraPathRouter();


        EXPECT_EQ(DPRouter.AddVertex(nd1), 0);
        EXPECT_EQ(DPRouter.AddVertex(nd2), 1);
        EXPECT_EQ(DPRouter.AddVertex(nd3), 2);
        EXPECT_EQ(DPRouter.AddVertex(nd4), 3);

        EXPECT_EQ(DPRouter.VertexCount(), 4);
    }

    {
        CDijkstraPathRouter DPRouter = CDijkstraPathRouter();

        for (int i = 0; i < 20; i++) {
            DPRouter.AddVertex((CStreetMap::TNodeID) i + 1);
        }

        EXPECT_EQ(DPRouter.VertexCount(), 20);
    }
}

TEST(DjikstraPathRouterTest, GetVertexTag) {
    
    {
        CStreetMap::TNodeID nd1 = 101;
        CStreetMap::TNodeID nd2 = 102;
        CStreetMap::TNodeID nd3 = 103;
        CStreetMap::TNodeID nd4 = 104;
        CStreetMap::TNodeID nd5 = 105;

        CDijkstraPathRouter DPRouter = CDijkstraPathRouter();

        EXPECT_EQ(DPRouter.AddVertex(nd1), 0);
        EXPECT_EQ(DPRouter.AddVertex(nd2), 1);
        EXPECT_EQ(DPRouter.AddVertex(nd3), 2);
        EXPECT_EQ(DPRouter.AddVertex(nd4), 3);
        EXPECT_EQ(DPRouter.AddVertex(nd5), 4);

        EXPECT_EQ(std::any_cast<CStreetMap::TNodeID>(DPRouter.GetVertexTag(0)), 101);
        EXPECT_EQ(std::any_cast<CStreetMap::TNodeID>(DPRouter.GetVertexTag(1)), 102);
        EXPECT_EQ(std::any_cast<CStreetMap::TNodeID>(DPRouter.GetVertexTag(2)), 103);
        EXPECT_EQ(std::any_cast<CStreetMap::TNodeID>(DPRouter.GetVertexTag(3)), 104);
        EXPECT_EQ(std::any_cast<CStreetMap::TNodeID>(DPRouter.GetVertexTag(4)), 105);
    }

    {
        CStreetMap::TNodeID nd1 = 1;
        CStreetMap::TNodeID nd2 = 2;
        CStreetMap::TNodeID nd3 = 3;
        CStreetMap::TNodeID nd4 = 4;

        CDijkstraPathRouter DPRouter = CDijkstraPathRouter();

        EXPECT_EQ(DPRouter.AddVertex(nd1), 0);
        EXPECT_EQ(DPRouter.AddVertex(nd2), 1);
        EXPECT_EQ(DPRouter.AddVertex(nd3), 2);
        EXPECT_EQ(DPRouter.AddVertex(nd4), 3);
        
        EXPECT_EQ(std::any_cast<CStreetMap::TNodeID>(DPRouter.GetVertexTag(0)), 1);
        EXPECT_EQ(std::any_cast<CStreetMap::TNodeID>(DPRouter.GetVertexTag(1)), 2);
        EXPECT_EQ(std::any_cast<CStreetMap::TNodeID>(DPRouter.GetVertexTag(2)), 3);
        EXPECT_EQ(std::any_cast<CStreetMap::TNodeID>(DPRouter.GetVertexTag(3)), 4);
    }

    {
        CDijkstraPathRouter DPRouter = CDijkstraPathRouter();

        // EXPECT_EQ(std::any(), DPRouter.GetVertexTag(100));
    }
}

TEST(DjikstraPathRouterTest, AddEdge) {

    {
        CStreetMap::TNodeID nd1 = 101;
        CStreetMap::TNodeID nd2 = 102;
        CStreetMap::TNodeID nd3 = 103;
        CStreetMap::TNodeID nd4 = 104;
        CStreetMap::TNodeID nd5 = 105;

        CDijkstraPathRouter DPRouter = CDijkstraPathRouter();

        EXPECT_EQ(DPRouter.AddVertex(nd1), 0);
        EXPECT_EQ(DPRouter.AddVertex(nd2), 1);
        EXPECT_EQ(DPRouter.AddVertex(nd3), 2);
        EXPECT_EQ(DPRouter.AddVertex(nd4), 3);
        EXPECT_EQ(DPRouter.AddVertex(nd5), 4);

        EXPECT_TRUE(DPRouter.AddEdge(0, 1, 1, true));
        EXPECT_TRUE(DPRouter.AddEdge(1, 2, 1, true));
        EXPECT_TRUE(DPRouter.AddEdge(2, 3, 1, true));
        EXPECT_TRUE(DPRouter.AddEdge(3, 4, 1, true));
        EXPECT_TRUE(DPRouter.AddEdge(4, 0, 100));
    }

    {
        CStreetMap::TNodeID nd1 = 1;
        CStreetMap::TNodeID nd2 = 3;

        CDijkstraPathRouter DPRouter = CDijkstraPathRouter();

        EXPECT_EQ(DPRouter.AddEdge(1, 3, 5, true), false);
        EXPECT_EQ(DPRouter.AddEdge(3, 4, 2, true), false);

        EXPECT_EQ(DPRouter.AddVertex(nd1), 0);
        EXPECT_EQ(DPRouter.AddVertex(nd2), 1);
        EXPECT_EQ(DPRouter.AddEdge(0, 1, 5, true), true);
        EXPECT_EQ(DPRouter.AddEdge(0, 1, -3, true), false);

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

        CDijkstraPathRouter DPRouter = CDijkstraPathRouter();

        EXPECT_EQ(DPRouter.AddVertex(nd1), 0);
        EXPECT_EQ(DPRouter.AddVertex(nd2), 1);
        EXPECT_EQ(DPRouter.AddVertex(nd3), 2);
        EXPECT_EQ(DPRouter.AddVertex(nd4), 3);
        EXPECT_EQ(DPRouter.AddVertex(nd5), 4);

        EXPECT_TRUE(DPRouter.AddEdge(0, 1, 3, true));
        EXPECT_TRUE(DPRouter.AddEdge(1, 2, 1, true));
        EXPECT_TRUE(DPRouter.AddEdge(2, 3, 2, true));
        EXPECT_TRUE(DPRouter.AddEdge(3, 4, 5, true));
        EXPECT_TRUE(DPRouter.AddEdge(4, 0, 100));

        expectedPath = {1, 2, 3}; 
        EXPECT_EQ(DPRouter.FindShortestPath(1, 3, path), 3);
        EXPECT_EQ(path, expectedPath);

        expectedPath = {1}; 
        EXPECT_EQ(DPRouter.FindShortestPath(1, 1, path), 0);
        EXPECT_EQ(path, expectedPath);

        expectedPath = {4, 3, 2, 1, 0}; 
        EXPECT_EQ(DPRouter.FindShortestPath(4, 0, path), 11);
        EXPECT_EQ(path, expectedPath);
    }


}

TEST(DjikstraPathRouterTest, MoreTests) {

    std::vector<CPathRouter::TVertexID> path;
    std::vector<CPathRouter::TVertexID> expectedPath;
    
    {

        CDijkstraPathRouter DPRouter = CDijkstraPathRouter();
        
        for (int i = 0; i <= 3; i++) {
            DPRouter.AddVertex(i);
        }

        DPRouter.AddEdge(3, 0, 2);
        DPRouter.AddEdge(0, 2, 5);
        DPRouter.AddEdge(1, 2, 2);
        DPRouter.AddEdge(0, 1, 1);
        
        expectedPath = {0, 1, 2};
        EXPECT_EQ(DPRouter.FindShortestPath(0, 2, path), 3);
        EXPECT_EQ(path, expectedPath);

        expectedPath = {};
        EXPECT_EQ(DPRouter.FindShortestPath(1, 3, path), CPathRouter::NoPathExists);
        EXPECT_EQ(path, expectedPath);
        
    }

    {

        CDijkstraPathRouter DPRouter = CDijkstraPathRouter();
        
        for (int i = 0; i <= 3; i++) {
            DPRouter.AddVertex(i);
        }

        DPRouter.AddEdge(3, 0, 2);
        DPRouter.AddEdge(0, 2, 5);
        DPRouter.AddEdge(1, 2, 2);
        DPRouter.AddEdge(0, 1, 1);
        
        expectedPath = {0, 1, 2};
        EXPECT_EQ(DPRouter.FindShortestPath(0, 2, path), 3);
        EXPECT_EQ(path, expectedPath);

        expectedPath = {};
        EXPECT_EQ(DPRouter.FindShortestPath(1, 3, path), CPathRouter::NoPathExists);
        EXPECT_EQ(path, expectedPath);
        
    }

    {

        CDijkstraPathRouter DPRouter = CDijkstraPathRouter();
        
        for (int i = 0; i <= 3; i++) {
            DPRouter.AddVertex(i);
        }

        DPRouter.AddEdge(0, 1, 1, true);
        DPRouter.AddEdge(1, 3, 3, true);
        DPRouter.AddEdge(1, 2, 2, true);
        
        expectedPath = {0, 1, 2};
        EXPECT_EQ(DPRouter.FindShortestPath(0, 2, path), 3);
        EXPECT_EQ(path, expectedPath);

        expectedPath = {0, 1, 3};
        EXPECT_EQ(DPRouter.FindShortestPath(0, 3, path), 4);
        EXPECT_EQ(path, expectedPath);
        
    }
}
