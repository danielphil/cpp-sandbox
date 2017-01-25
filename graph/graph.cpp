#include <iostream>
#include <queue>
#include <unordered_set>
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

enum class VertexState
{
    Undiscovered,
    Discovered,
    Processed
};

void bfs(
    IGraph* graph,
    const std::function<void (int, int)>& process_edge,
    const std::function<void (int)>& process_vertex,
    int start_vertex = 0
) {
    // initialise the tables of processed and unprocessed vertices
    const auto vertex_count = graph->GetVertexCount();
    if (vertex_count <= 0) {
        return;
    }

    std::vector<VertexState> state(vertex_count, VertexState::Undiscovered);
    std::vector<int> parents(vertex_count, -1);

    std::queue<int> to_process;
    to_process.push(start_vertex);
    state[start_vertex] = VertexState::Discovered;

    while (!to_process.empty()) {
        const auto vertex = to_process.front();
        to_process.pop();
        state[vertex] = VertexState::Processed;
        process_vertex(vertex);

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

void dfs_internal(
    IGraph* graph,
    std::vector<VertexState>& vertex_states,
    std::vector<int>& parents,
    int start_vertex,
    const std::function<void (int)>& process_vertex,
    const std::function<void (int, int)>& process_edge
) {
    if (vertex_states[start_vertex] == VertexState::Processed) {
        return;
    }

    vertex_states[start_vertex] = VertexState::Discovered;

    auto edges = graph->GetEdgesForVertex(start_vertex);
    std::sort(edges.begin(), edges.end());

    for (auto edge : edges) {
        if (parents[edge] == start_vertex) {
            continue;
        }

        switch (vertex_states[edge]) {
        case VertexState::Undiscovered:
            parents[start_vertex] = edge;
            process_edge(start_vertex, edge);
            dfs_internal(graph, vertex_states, parents, edge, process_vertex, process_edge);
            break;
        case VertexState::Discovered:
            process_edge(start_vertex, edge);
            break;
        case VertexState::Processed:
            // deliberately don't do anything here
            break;
        }
    }

    process_vertex(start_vertex);
    vertex_states[start_vertex] = VertexState::Processed;
}

void dfs(
    IGraph* graph,
    const std::function<void (int)>& process_vertex,
    const std::function<void (int, int)>& process_edge,
    int start_vertex = 0
) {
    std::vector<int> parents(graph->GetVertexCount(), -1);
    std::vector<VertexState> vertex_states(graph->GetVertexCount(), VertexState::Undiscovered);
    dfs_internal(graph, vertex_states, parents, start_vertex, process_vertex, process_edge);
}

bool is_bipartite(IGraph* g) {
    std::unordered_set<int> to_process;
    for (auto i = 0; i < g->GetVertexCount(); i++) {
        to_process.insert(i);
    }

    enum class VertexColor {
        Uncoloured,
        White,
        Black
    };
    auto complement = [] (VertexColor c) {
        switch (c) {
        case VertexColor::White:
            return VertexColor::Black;
        case VertexColor::Black:
            return VertexColor::White;
        default:
            return VertexColor::Uncoloured;
        }
    };

    std::vector<VertexColor> colours(g->GetVertexCount(), VertexColor::Uncoloured);

    bool bipartite = true;
    while (!to_process.empty()) {
        colours[*(to_process.begin())] = VertexColor::White;

        auto process_vertex = [&] (int vertex) {
            to_process.erase(vertex);
        };

        auto process_edge = [&] (int start, int end) {
            if (colours[start] == colours[end]) {
                bipartite = false;
                return;
            }

            colours[end] = complement(colours[start]);
        };
        bfs(g, process_edge, process_vertex, *(to_process.begin()));
    }

    return bipartite;
}

std::vector<std::vector<int>> connected_components(IGraph* graph) {
    std::unordered_set<int> to_process;
    for (auto i = 0; i < graph->GetVertexCount(); i++) {
        to_process.insert(i);
    }

    std::vector<std::vector<int>> found_components;
    while (!to_process.empty()) {
        std::vector<int> current_component;
        auto process_vertex = [&] (int edge) {
            to_process.erase(edge);
            current_component.push_back(edge);
        };
        bfs(graph, [] (int, int) {}, process_vertex, *(to_process.begin()));
        found_components.push_back(current_component);
    }

    return found_components;
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
    }, [] (int) {});

    EXPECT_EQ(2, edges.size());
    EXPECT_EQ(std::make_pair(0, 4), edges[0]);
    EXPECT_EQ(std::make_pair(0, 4), edges[0]);
}

TYPED_TEST(GraphTest, TestConnectedComponents) {
    TypeParam graph(5);
    graph.AddEdge(0, 4, false);
    graph.AddEdge(2, 3, false);

    auto components = connected_components(&graph);
    EXPECT_EQ((std::vector<int> { 4, 0 }), components[0]);
    EXPECT_EQ((std::vector<int> { 3, 2 }), components[1]);
    EXPECT_EQ((std::vector<int> { 1 }), components[2]);
}

TYPED_TEST(GraphTest, TestBipartite) {
    TypeParam graph(5);
    graph.AddEdge(0, 4, false);
    graph.AddEdge(2, 3, false);

    EXPECT_TRUE(is_bipartite(&graph));
}

TYPED_TEST(GraphTest, TestNotBipartite) {
    TypeParam graph(3);
    graph.AddEdge(0, 1, false);
    graph.AddEdge(0, 2, false);
    graph.AddEdge(1, 2, false);

    EXPECT_FALSE(is_bipartite(&graph));
}

TYPED_TEST(GraphTest, TestDFS) {
    TypeParam graph(5);
    graph.AddEdge(0, 4, false);
    graph.AddEdge(0, 3, false);
    graph.AddEdge(3, 4, false);

    std::vector<int> vertices;
    std::vector<std::pair<int, int>> edges;
    dfs(
        &graph,
        [&] (int vertex) { vertices.push_back(vertex); },
        [&] (int from, int to) { edges.push_back(std::make_pair(from, to)); }
    );

    EXPECT_EQ(3, vertices.size());
    EXPECT_EQ(3, edges.size());
    EXPECT_EQ(std::vector<int>({ 4, 3, 0 }), vertices);
    EXPECT_EQ(std::make_pair(0, 3), edges[0]);
    EXPECT_EQ(std::make_pair(3, 4), edges[1]);
    EXPECT_EQ(std::make_pair(4, 0), edges[2]);
}