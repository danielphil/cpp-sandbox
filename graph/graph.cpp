#include <iostream>
#include <queue>
#include "gtest/gtest.h"

class IGraph
{
public:
    virtual ~IGraph() = default;
    virtual void AddEdge(int from, int to, bool directed) = 0;
    virtual void RemoveEdge(int from, int to, bool directed) = 0;
    virtual std::vector<int> GetEdgesForVertex(int vertex) = 0;
    virtual int GetVertexCount() const = 0;
};

class AdjacencyMatrixGraph : public IGraph
{
public:
    AdjacencyMatrixGraph(int number_of_vertices) {
        for (int i = 0; i < number_of_vertices; i++) {
            std::vector<int> row;
            row.resize(number_of_vertices, 0);

            m_matrix.push_back(row);
        }
    }

    virtual void AddEdge(int from, int to, bool directed) override {
        m_matrix[from][to] += 1;
        if (!directed) {
            AddEdge(to, from, true);
        }
    }

    virtual void RemoveEdge(int from, int to, bool directed) override {
        m_matrix[from][to] = std::max(m_matrix[from][to] - 1, 0);
        if (!directed) {
            m_matrix[to][from] = std::max(m_matrix[to][from] - 1, 0);
        }
    }

    virtual std::vector<int> GetEdgesForVertex(int vertex) override {
        std::vector<int> result;
        for (int i = 0; i < m_matrix.size(); i++) {
            for (int j = 0; j < m_matrix[vertex][i]; j++) {
                result.push_back(i);
            }
        }

        return result;
    }

    virtual int GetVertexCount() const override {
        return m_matrix.size();
    }

private:

    std::vector<std::vector<int>> m_matrix;
};

class AdjacencyListGraph : public IGraph
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

    virtual void AddEdge(int from, int to, bool directed) override {
        m_lists[from].push_back(Edge(to));
        if (!directed) {
            AddEdge(to, from, true);
        }
    }

    virtual void RemoveEdge(int from, int to, bool directed) override {
        auto& list = m_lists[from];
        auto to_remove = std::find_if(list.begin(), list.end(), [to] (const Edge& e) { return e.m_to == to; } );
        if (to_remove != list.end()) {
            list.erase(to_remove);
        }

        if (!directed) {
            RemoveEdge(to, from, true);
        }
    }

    virtual std::vector<int> GetEdgesForVertex(int vertex) override {
        std::vector<int> result;
        for (const auto& v : m_lists[vertex]) {
            result.push_back(v.m_to);
        }

        return result;
    }

    virtual int GetVertexCount() const override {
        return m_lists.size();
    }

private:

    std::vector<std::vector<Edge>> m_lists;
};

void bfs(
    IGraph* graph,
    const std::function<void (int, int)>& process_edge
) {
    enum class VertexState
    {
        Undiscovered,
        Discovered,
        Processed
    };

    // initialise the tables of processed and unprocessed vertices
    const auto vertex_count = graph->GetVertexCount();
    if (vertex_count <= 0) {
        return;
    }

    std::vector<VertexState> state(vertex_count, VertexState::Undiscovered);
    std::vector<int> parents(vertex_count, -1);

    std::queue<int> to_process;
    to_process.push(0);
    state[0] = VertexState::Discovered;

    while (!to_process.empty()) {
        const auto vertex = to_process.front();
        to_process.pop();
        state[vertex] = VertexState::Processed;

        const auto edges = graph->GetEdgesForVertex(vertex);
        for (auto edge : edges) {
            if (state[edge] != VertexState::Processed) {
                process_edge(vertex, edge);
            }
            if (state[edge] == VertexState::Undiscovered) {
                state[edge] = VertexState::Discovered;
                to_process.push(edge);
                parents[edge] = vertex;
            }
        }
    }
}

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

TYPED_TEST(GraphTest, TestBFS) {
    TypeParam graph(5);
    graph.AddEdge(0, 4, false);
    graph.AddEdge(4, 0, false);

    std::vector<std::pair<int, int>> edges;
    bfs(&graph, [&] (int from, int to) {
        edges.push_back(std::make_pair(from, to));
    });

    EXPECT_EQ(2, edges.size());
    EXPECT_EQ(std::make_pair(0, 4), edges[0]);
    EXPECT_EQ(std::make_pair(0, 4), edges[0]);
}