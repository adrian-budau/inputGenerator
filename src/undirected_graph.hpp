#ifndef INPUT_GENERATOR_UNDIRECTED_GRAPH_HPP_
#define INPUT_GENERATOR_UNDIRECTED_GRAPH_HPP_

#include <vector>

#include "exception.hpp"
#include "boolean.hpp"
#include "graph.hpp"
#include "vectors.hpp"
#include "tree.hpp"

namespace inputGenerator {

template<class NodeData = void, class EdgeData = void>
Graph<NodeData, EdgeData> undirectedGraph(const size_t& size,
                                          const size_t& edges) {
    Graph<NodeData, EdgeData> graph(size);
    int64_t max_edges_number =
        static_cast<int64_t>(size) * static_cast<int64_t>(size - 1) / 2;

    std::vector<int64_t> edges_ids =
        randomSample<int64_t>(edges, 0, max_edges_number);

    size_t node = 0;
    int64_t from = 0;
    for (auto &edge_id : edges_ids) {
        while (edge_id >= int64_t(from + size - node - 1)) {
            from += size - node - 1;
            ++node;
        }
        addEdge(graph[node], graph[edge_id - from + node + 1]);
    }

    return graph;
}

template<class NodeData = void, class EdgeData = void>
Graph<NodeData, EdgeData> undirectedGraph(const size_t& size,
                                          const size_t& edges,
                                          Boolean::Object connected) {
    if (!connected)
        return undirectedGraph(size, edges);

    auto graph = undirectedGraph(size, edges);
    auto connected_graph = tree(size);

    // the number of edges we should delete
    // that's the number of edges that are in in the tree
    // but not in the normal graph
    size_t bad_edges = 0;
    for (auto &edge : connected_graph.edges()) {
        int x = edge.from().index();
        int y = edge.to().index();

        if (!graph[x].hasEdge(graph[y])) {
            ++bad_edges;
            addEdge(graph[x], graph[y]);
        }
    }

    for (auto &edge : shuffle(graph.edges())) {
        if (bad_edges == 0)
            break;
        int x = edge.from().index();
        int y = edge.to().index();

        if (!connected_graph[x].hasEdge(connected_graph[y])) {
            eraseEdge(edge);
            --bad_edges;
        }
    }

    return graph;
}

template<class NodeData = void, class EdgeData = void>
Graph<NodeData, EdgeData> undirectedGraph(
        const size_t& size,
        Boolean::Object connected = Boolean::False) {
    if (connected)
        return undirectedGraph(
                size,
                randomInt<size_t>(size - 1, size * (size - 1) / 2), connected);

    return undirectedGraph(size, randomInt<size_t>(0, size * (size - 1) / 2));
}

}  // namespace inputGenerator

#endif  // INPUT_GENERATOR_UNDIRECTED_GRAPH_HPP_
