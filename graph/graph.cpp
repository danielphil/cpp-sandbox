#include <iostream>
#include "gtest/gtest.h"

class AdjacencyMatrixGraph
{
public:
    AdjacencyMatrixGraph(int number_of_vertices) {
        for (int i = 0; i < number_of_vertices; i++) {
            std::vector<int> row;
            row.resize(number_of_vertices, 0);

            m_matrix.push_back(row);
        }
    }

    void AddEdge(int from, int to, bool directed) {
        m_matrix[from][to] += 1;
        if (!directed) {
            AddEdge(to, from, true);
        }
    }

    void RemoveEdge(int from, int to, bool directed) {
        m_matrix[from][to] = std::max(m_matrix[from][to] - 1, 0);
        if (!directed) {
            m_matrix[to][from] = std::max(m_matrix[to][from] - 1, 0);
        }
    }

    std::vector<int> GetEdgesForVertex(int vertex) {
        std::vector<int> result;
        for (int i = 0; i < m_matrix.size(); i++) {
            if (m_matrix[vertex][i] > 0) {
                result.push_back(i);
            }
        }

        return result;
    }

private:

    std::vector<std::vector<int>> m_matrix;
};

class AdjacencyListGraph
{
private:
    class Edge {
    public:
        Edge(int to) : m_to(to) {            
        }

        int m_to;
    };
public:
    AdjacencyListGraph(int number_of_vertices) {
        for (int i = 0; i < number_of_vertices; i++) {
            std::vector<Edge> list;
            m_lists.push_back(list);
        }
    }

    void AddEdge(int from, int to, bool directed) {
        m_lists[from].push_back(Edge(to));
        if (!directed) {
            AddEdge(to, from, true);
        }
    }

    void RemoveEdge(int from, int to, bool directed) {
        auto& list = m_lists[from];
        auto to_remove = std::find_if(list.begin(), list.end(), [to] (const Edge& e) { return e.m_to == to; } );
        if (to_remove != list.end()) {
            list.erase(to_remove);
        }

        if (!directed) {
            RemoveEdge(to, from, true);
        }
    }

    std::vector<int> GetEdgesForVertex(int vertex) {
        std::vector<int> result;
        for (const auto& v : m_lists[vertex]) {
            result.push_back(v.m_to);
        }

        return result;
    }

private:

    std::vector<std::vector<Edge>> m_lists;
};

template <typename T>
class GraphTest : public ::testing::Test {

};

typedef ::testing::Types<AdjacencyListGraph, AdjacencyMatrixGraph> GraphTypes;
TYPED_TEST_CASE(GraphTest, GraphTypes);

TYPED_TEST(GraphTest, TestEmptyGraph) {
    TypeParam graph(5);
    
    for (int i = 0; i < 5; i++) {
        EXPECT_TRUE(graph.GetEdgesForVertex(i).empty());
    }
}

TYPED_TEST(GraphTest, TestBasicGraph) {
    TypeParam graph(5);
    graph.AddEdge(3, 4, false);

    auto edges3 = graph.GetEdgesForVertex(3);
    auto edges4 = graph.GetEdgesForVertex(4);

    auto edges3_expected = std::vector<int>({4});
    auto edges4_expected = std::vector<int>({3});

    EXPECT_EQ(edges3_expected, edges3);
    EXPECT_EQ(edges4_expected, edges4);
}

TYPED_TEST(GraphTest, TestRemoveEdge) {
    TypeParam graph(5);
    graph.AddEdge(3, 4, false);

    graph.RemoveEdge(4, 3, false);

    auto edges3 = graph.GetEdgesForVertex(3);
    auto edges4 = graph.GetEdgesForVertex(4);

    EXPECT_TRUE(edges3.empty());
    EXPECT_TRUE(edges4.empty());
}