#ifndef INPUT_GENERATOR_BIPARTITE_HPP_
#define INPUT_GENERATOR_BIPARTITE_HPP_

#include <utility>
#include <algorithm>
#include <vector>

#include "exception.hpp"
#include "numbers.hpp"
#include "vectors.hpp"
#include "graph.hpp"

namespace inputGenerator {

template<class NodeData = int, class EdgeData = int>
std::pair<Graph<NodeData, EdgeData>, Graph<NodeData, EdgeData>> bipartite(
        const int &leftSize,
        const int &rightSize,
        const int &edges,
        const bool &multipleEdges = false) {
#ifdef INPUT_GENERATOR_DEBUG
    if (leftSize < 1)
        throw Exception("`leftSize` should be strictly positive");
    if (rightSize < 1)
        throw Exception("`rightSize` should be strictly positive");

    if (edges < 0)
        throw Exception("`edges` (the number of edges) should be positive");

    if (!multipleEdges && int64_t(leftSize) * rightSize < edges)
        throw Exception("If multiple edges are not accepted then the number of"
                        " edges should be strictly smaller than the product of"
                       " the number of nodes on the left and right size");
#endif
    Graph<NodeData, EdgeData> leftGraph(leftSize);
    Graph<NodeData, EdgeData> rightGraph(rightSize);

    if (multipleEdges) {
        for (int i = 0 ; i < edges; ++i) {
            int first = randomInt(0, leftSize - 1);
            int second = randomInt(0, rightSize - 1);
            addEdge(leftGraph[first], rightGraph[second]);
        }
        return std::make_pair(leftGraph, rightGraph);
    }

    auto takenEdges = randomSample(int64_t(edges),
                                   int64_t(0),
                                   int64_t(leftSize) * rightSize - 1);
    for (auto &edge : takenEdges) {
        int first = edge / rightSize;
        int second = edge % rightSize;
        addEdge(leftGraph[first], rightGraph[second]);
    }

    return std::make_pair(leftGraph, rightGraph);
}

template<class NodeData = int, class EdgeData = int>
std::pair<Graph<NodeData, EdgeData>, Graph<NodeData, EdgeData>> bipartite(
        const int &leftSize,
        const int &rightSize,
        const bool &multipleEdges = false) {
    return bipartite<NodeData, EdgeData>(leftSize,
                                         rightSize,
                                         randomInt(0, leftSize * rightSize),
                                         multipleEdges);
}

template<class NodeData = int, class EdgeData = int>
std::pair<Graph<NodeData, EdgeData>, Graph<NodeData, EdgeData>> bipartite(
        const int &nodes,
        const bool& multipleEdges = false) {
#ifdef INPUT_GENERATOR_DEBUG
    if (nodes < 1)
        throw Exception("`nodes` must be be bigger than or equal to 2");
#endif
    auto parts = randomPartition(nodes, 2);
    return bipartite<NodeData, EdgeData>(parts[0], parts[1], multipleEdges);
}

template<class NodeData = int, class EdgeData = int>
std::pair<Graph<NodeData, EdgeData>, Graph<NodeData, EdgeData>>
regularBipartite(const int &nodes, const int &degree) {
    Graph<NodeData, EdgeData> leftGraph(nodes), rightGraph(nodes);

    std::vector<std::vector<int>> nodesByDegree(degree + 1);
    std::vector<std::pair<int, int>> whereMatrix(nodes);

    std::vector<int> nodesArray(nodes * (degree > 1));
    std::vector<int> whereArray(nodes);
    for (int i = 0; i < nodes; ++i) {
        nodesByDegree[degree].push_back(i);
        whereMatrix[i] = {degree, i};

        if (degree != 1)
            nodesArray[i] = i;
        whereArray[i] = i;
    }

    auto update = [&](const int &node) -> int {
        int row = whereMatrix[node].first;
        int column = whereMatrix[node].second;
        int position = whereArray[node];

        std::swap(nodesByDegree[row][column], nodesByDegree[row].back());
        whereMatrix[nodesByDegree[row][column]] = {row, column};
        nodesByDegree[row].pop_back();

        if (row > 1) {
            std::swap(nodesArray[position], nodesArray.back());
            whereArray[nodesArray[position]] = position;
            nodesArray.pop_back();
        }

        return row;
    };

    for (int i = 0; i < nodes; ++i) {
        // first we have to pick all nodes of degree nodes - i
        std::vector<std::pair<int, int>> picked;
        picked.reserve(degree);

        // the maximum amount of nodes with degree 1 we can pick is
        // nodesArray.size() + nodesByDegre[1].size() - degree
        int maxOnes = std::min(
                nodesArray.size() + nodesByDegree[1].size() - degree,
                nodesByDegree[1].size());

        if (nodes - i <= degree)
            for (auto node : std::vector<int>(nodesByDegree[nodes - i]))
                // we need to make a copy so we don't invalidate anything
                picked.push_back({node, update(node)});

        // now we have left to pick degree  - picked.size()
        // and we have at our disposal maxOnes nodes with degree 1
        // and nodesArray.size() others
        // let's try to find out first how many with degree 1 would we pick
        int pickedOnes = 0;
        if (degree - picked.size()) {
            auto pickedDegree1 = randomSample<int>(degree - picked.size(),
                                                   1,
                                                   maxOnes + nodesArray.size());

            for (auto &number : pickedDegree1)
                if (number <= maxOnes)
                    ++pickedOnes;
        }

        // so let's pick those with degree 1
        for (auto &node : randomSubsequence(nodesByDegree[1], pickedOnes))
            picked.push_back({node, update(node)});

        for (auto &node : randomSubsequence(
                    nodesArray,
                    static_cast<int>(degree - picked.size())))
            picked.push_back({node, update(node)});

        for (auto &node : picked) {
            addEdge(leftGraph[i], rightGraph[node.first]);
            nodesByDegree[node.second - 1].push_back(node.first);
            whereMatrix[node.first] = std::make_pair(
                    node.second - 1,
                    nodesByDegree[node.second - 1].size() - 1);

            if (node.second > 2) {
                nodesArray.push_back(node.first);
                whereArray[node.first] = nodesArray.size() - 1;
            }
        }
    }

    return std::make_pair(leftGraph, rightGraph);
}

}  // namespace inputGenerator
#endif  // INPUT_GENERATOR_BIPARTITE_HPP_
