#ifndef INPUT_GENERATOR_TREE_HPP_
#define INPUT_GENERATOR_TREE_HPP_

#include "exception.hpp"
#include "graph.hpp"
#include "vectors.hpp"
#include "chain.hpp"

namespace inputGenerator {

// TODO: use pruffer sequences
template<class NodeData = int, class EdgeData = int>
Graph<NodeData, EdgeData> tree(const size_t &size = 1) {
#ifdef INPUT_GENERATOR_DEBUG
    if (size <= 0)
        throw Exception("Trees must have strictly positive sizes");
#endif
    Graph<NodeData, EdgeData> graph;
    for (size_t i = 1; i < size; ++i) {
        typename Graph<NodeData, EdgeData>::Node node;
        typename Graph<NodeData, EdgeData>::Node parent = randomElement(graph);

        addEdge(node, parent);
        graph.addNodes({node});
    }

    graph = shuffle(graph);

    return graph;
}

template<class NodeData = int, class EdgeData = int>
Graph<NodeData, EdgeData> wideTree(const size_t &size, const size_t &minimumDiameter) {
#ifdef INPUT_GENERATOR_DEBUG
    if (size <= 0)
        throw Exception("Trees must have strictly positive sizes");
    if (size < minimumDiameter)
        throw Exception("Diameter must be strictly less than the tree size");
#endif
    Graph<NodeData, EdgeData> graph = chain(minimumDiameter);
    for (size_t i = 1; i < size; ++i) {
        typename Graph<NodeData, EdgeData>::Node node;
        typename Graph<NodeData, EdgeData>::Node parent = randomElement(graph);

        addEdge(node, parent);
        graph.addNodes({node});
    }

    return shuffle(graph);
}

}

#endif // INPUT_GENERATOR_HPP_
