#ifndef INPUT_GENERATOR_TREE_HPP_
#define INPUT_GENERATOR_TREE_HPP_

#include "exception.hpp"
#include "boolean.hpp"
#include "graph.hpp"
#include "vectors.hpp"
#include "chain.hpp"

namespace inputGenerator {

// TODO(adrian_budau): use pruffer sequences
template<class NodeData = int, class EdgeData = void>
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

    graph.Index();
    return graph;
}

template<class NodeData = int, class EdgeData = void>
Graph<NodeData, EdgeData> wideTree(const size_t &size,
                                   const size_t &minimumDiameter,
                                   Boolean::Object randomEnds = Boolean::True) {
#ifdef INPUT_GENERATOR_DEBUG
    if (size < 2)
        throw Exception("Wide trees must have at least two nodes");

    if (minimumDiameter < 2)
        throw Exception("Wide tree must have the diameter at least two");

    if (size < minimumDiameter)
        throw Exception("Diameter must be strictly less than the tree size");
#endif
    Graph<NodeData, EdgeData> graph = chain(minimumDiameter, randomEnds);
    typename Graph<NodeData, EdgeData>::Node start = graph[0],
                                             end = graph[minimumDiameter - 1];

    for (size_t i = minimumDiameter; i < size; ++i) {
        typename Graph<NodeData, EdgeData>::Node node;
        typename Graph<NodeData, EdgeData>::Node parent = randomElement(graph);

        addEdge(node, parent);
        graph.addNodes({node});
    }

    if (randomEnds)
        graph.Index();
    else
        graph.Index({{start, 0}, {end, 1}});
    return graph;
}

}  // namespace inputGenerator

#endif  // INPUT_GENERATOR_HPP_
