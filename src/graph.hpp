#ifndef INPUT_GENERATOR_GRAPH_HPP_
#define INPUT_GENERATOR_GRAPH_HPP_

#include <vector>
#include <utility>
#include <unordered_set>
#include <algorithm>

#include "exception.hpp"
#include "node.hpp"
#include "edge.hpp"
#include "vectors.hpp"

namespace inputGenerator {

template<class NodeData = int, class EdgeData = int>
class Graph {
  public:
    typedef _Node<NodeData, EdgeData> NodeType;
    typedef _Edge<NodeData, EdgeData> EdgeType;
    typedef NodeWrapper<NodeData, EdgeData> Node;

    // basic iterators
    template<class VectorIteratorType>
    class Iterator {
      public:
        typedef int difference_type;
        typedef Node value_type;
        typedef typename VectorIteratorType::value_type pointer;
        typedef typename VectorIteratorType::reference reference;
        typedef std::random_access_iterator_tag iterator_category;

        Iterator() = default;

        Iterator(const Iterator& that): alpha(that.alpha) {
        }

        Iterator& operator=(const Iterator& that) {
            alpha = that.alpha;
            return *this;
        }

        difference_type operator-(const Iterator& that) {
            return alpha - that.alpha;
        }

        reference operator*() {
            return *alpha;
        }

        Iterator& operator++() {
            ++alpha;
            return *this;
        }

        Iterator operator++(int) {
            Iterator newIterator = *this;
            ++alpha;
            return newIterator;
        }

        bool operator==(const Iterator& that) {
            return alpha == that.alpha;
        }

        bool operator!=(const Iterator& that) {
            return alpha != that.alpha;
        }

      private:
        friend class Graph<NodeData, EdgeData>;

        Iterator(const VectorIteratorType &that) {
            alpha = that;
        }

        VectorIteratorType alpha;
    };

    // using small first letter just for consistency
    typedef Iterator<typename std::vector<Node>::iterator> iterator;
    typedef Iterator<typename std::vector<Node>::const_iterator> const_iterator;

    Graph(const int& _size = 1, const int& _indexStart = 0);

    // shallow copy
    Graph<NodeData, EdgeData>& operator=(const Graph<NodeData, EdgeData> &) = default;

    // shallow copy too
    Graph(const Graph&) = default;

    // and some deep copy
    Graph<NodeData, EdgeData> clone() const;

    // really important not to leak memory
    void reset();

    void Index(std::initializer_list< std::pair<Node, int> > fixed, const int &from = 0);

    void Index(const int &from = 0);

    void addNodes(std::initializer_list<Node>);

    iterator begin() {
        return iterator(nodes.begin());
    }

    const_iterator begin() const {
        return const_iterator(nodes.begin());
    }

    iterator end() {
        return iterator(nodes.end());
    }

    const_iterator end() const {
        return const_iterator(nodes.end());
    }

    Node& operator[](const int &position) {
        if (position < indexStart || position >= indexStart + size())
            throw Exception("index out of range in the graph");

        return nodes[position - indexStart];
    }

    const Node& operator[](const int &position) const {
        if (position < indexStart || position >= indexStart + size())
            throw Exception("index out of range in the graph");
        return nodes[position - indexStart];
    }

    int size() const {
        return nodes.size();
    }

    std::vector<EdgeType> arcs(const bool &forceSearch = true) const;

    std::vector<EdgeType> edges(const bool &forceSearch = true) const;

    bool hasNode(const Node&) const;

    int min() const {
        return indexStart;
    }

    int max() const {
        return indexStart + size() - 1;
    }

  private:
    std::vector<Node> nodes;

    int indexStart;
};

template<class NodeData, class EdgeData>
Graph<NodeData, EdgeData>::Graph(const int& _size, const int& _indexStart) {
    // we create _size new nodes
    nodes.reserve(_size);
    for (int i = 0; i < _size; ++i)
        nodes.push_back(Node(i));

    // make it so it supports both possibilities(nodes from 0 to size - 1 or from 1 to size)
    indexStart = _indexStart;
}

template<class NodeData, class EdgeData>
Graph<NodeData, EdgeData> Graph<NodeData, EdgeData>::clone() const {
    Graph<NodeData, EdgeData> newGraph(size(), indexStart);

    for (auto &arc: arcs(true)) {
        newGraph[arc.from().index()].addEdge(newGraph[arc.to().index()], arc.data());
    }

    for (auto &edge: edges(true))
        auto edge2 = addEdge(newGraph[edge.from().index()], newGraph[edge.to().index()], edge.data());

    return newGraph;
}

template<class NodeData, class EdgeData>
void Graph<NodeData, EdgeData>::Index(std::initializer_list< std::pair<Node, int> > fixed, const int &from) {
    // we go through each fixed node and keep track of it
    std::unordered_set<unsigned> fixedNodes;
    // we also keep track of the values we have so we can assign demn easily
    std::unordered_set<int> usedValues;
    for (auto &node: fixed) {
        if (node.second < from || node.second >= from + int(nodes.size()))
            throw Exception("On Graph Reindexing nodes must be reindexed using only values between `from` and `from` + the_number_of_nodes - 1");

        // why would you give a node twice?
        // anyway we ignore following attempts, it's your duty to not have multiple such actions
        if (fixedNodes.find(node.first.getKey()) != fixedNodes.end())
            throw Exception("On Graph Reindexing no node is allowed to appear twice");

        // and why would you reuse values?
        // again we ignore such attempts
        if (usedValues.find(node.second) != usedValues.end())
            throw Exception("On Graph Reindexing no two nodes are allowed to have the same index");

        node.first.index() = node.second;
        fixedNodes.insert(node.first.getKey());
        usedValues.insert(node.second);
    }

    std::vector<int> unusedValues;
    unusedValues.reserve(nodes.size() - usedValues.size());
    for (int i = from; i < from + int(nodes.size()); ++i)
        if (usedValues.find(i) == usedValues.end())
            unusedValues.push_back(i);
    unusedValues = shuffle(unusedValues);

    for (auto &node: nodes) {
        if (fixedNodes.find(node.getKey()) != fixedNodes.end())
            continue;

        node.index() = unusedValues.back();
        unusedValues.pop_back();
    }

    // let's reorder the nodes by this index
    sort(nodes.begin(), nodes.end(), [](const Node &first, const Node &second) {
        return first.index() < second.index();
    });

    indexStart = from;
}

template<class NodeData, class EdgeData>
void Graph<NodeData, EdgeData>::Index(const int &from) {
    return Index({}, from);
}

template<class NodeData, class EdgeData>
void Graph<NodeData, EdgeData>::addNodes(std::initializer_list<Node> newNodes) {
    int newIndex = indexStart + nodes.size();
    for (auto &node: newNodes)
        node.index() = newIndex++;
    nodes.insert(nodes.end(), newNodes.begin(), newNodes.end());
}

template<class NodeData, class EdgeData>
std::vector<_Edge<NodeData, EdgeData>> Graph<NodeData, EdgeData>::arcs(const bool& forceSearch) const {
    std::vector<_Edge<NodeData, EdgeData>> result;
    for (auto &node: nodes) {
        auto toAdd = node.arcs(forceSearch);

        result.insert(result.end(), toAdd.begin(), toAdd.end());
    }
    return result;
}

template<class NodeData, class EdgeData>
std::vector<_Edge<NodeData, EdgeData>> Graph<NodeData, EdgeData>::edges(const bool& forceSearch) const {
    std::vector<_Edge<NodeData, EdgeData>> result;
    for (auto &node: nodes) {
        auto toAdd = node.edges(forceSearch);

        for (auto &edge: toAdd)
            if (hasNode(edge.to())) {
                if (edge.from().index() < edge.to().index())
                    result.push_back(edge);
            } else
                result.push_back(edge);

    }

    return result;
}

template<class NodeData, class EdgeData>
bool Graph<NodeData, EdgeData>::hasNode(const Node& that) const {
    if (that.index() < indexStart || that.index() >= indexStart + int(nodes.size()))
        return false;

    return (*this)[that.index()] == that;
}

// specialization for shuffle
template<class NodeData, class EdgeData>
Graph<NodeData, EdgeData> shuffle(const Graph<NodeData, EdgeData>& graph) {
    auto newGraph = graph.clone();

    newGraph.Index();

    return newGraph;
}

}
#endif // INPUT_GENERATOR_GRAPH_HPP_
