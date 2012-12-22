#ifndef INPUT_GENERATOR_CHAIN_HPP_
#define INPUT_GENERATOR_CHAIN_HPP_

#include "exception.hpp"
#include "graph.hpp"

namespace inputGenerator {

// you can generate a non-random graph if you really want by making the second parameter false
// this is undirected
template<class NodeData = int, class EdgeData = int>
Graph<NodeData, EdgeData> chain(const int &size = 1, const bool &random = true ) {
    if (size <= 0)
        throw Exception("Chains must have strictly positive sizes");

    Graph<NodeData, EdgeData> graph(size);
    for (int i = 1; i < size; ++i) {
        graph[i - 1].addEdge(graph[i]);
        graph[i].addEdge(graph[i - 1]);
    }

    if (random)
        graph.Index();

    return graph;
}

template<class NodeData = int, class EdgeData = int>
Graph<NodeData, EdgeData> path(const int &size = 1, const bool &random = true) {
    if (size <= 0)
        throw Exception("Paths must have strictly positive sizes");

    Graph<NodeData, EdgeData> graph(size);
    for (int i = 1; i < size; ++i)
        graph[i - 1].addEdge(graph[i]);

    if (random)
        graph.Index();

    return graph;
}

}

#endif // INPUT_GENERATOR_CHAIN_HPP_

