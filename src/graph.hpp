#ifndef INPUT_GENERATOR_GRAPH_HPP_
#define INPUT_GENERATOR_GRAPH_HPP_

#include <unordered_set>
#include <vector>
#include <utility>
#include <algorithm>

#include "exception.hpp"
#include "node.hpp"
#include "edge.hpp"
#include "vectors.hpp"

namespace inputGenerator {

template<class NodeData = void, class EdgeData = void>
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

        Iterator& operator+=(const size_t &difference) {
            alpha += difference;
            return *this;
        }

        bool operator==(const Iterator& that) {
            return alpha == that.alpha;
        }

        bool operator!=(const Iterator& that) {
            return alpha != that.alpha;
        }

      private:
        friend class Graph<NodeData, EdgeData>;

        explicit Iterator(const VectorIteratorType &that) {
            alpha = that;
        }

        VectorIteratorType alpha;
    };

    // using small first letter just for consistency
    typedef Iterator<typename std::vector<Node>::iterator> iterator;
    typedef Iterator<typename std::vector<Node>::const_iterator> const_iterator;

    Graph(const int& _size = 1, const int& _indexStart = 0);

    // shallow copy
    Graph(const Graph&) = default;

    // move constructor
    Graph(Graph&&);

    // shallow copy
    Graph<NodeData, EdgeData>& operator=(
            const Graph<NodeData, EdgeData> &) = default;

    // fast operator= using move semantics
    Graph<NodeData, EdgeData>& operator=(Graph<NodeData, EdgeData> &&);

    // and some deep copy
    Graph<NodeData, EdgeData> clone() const;

    void clear();

    void Index(std::initializer_list< std::pair<Node, int> > fixed,
               const int &from = 0);

    void Index(const int &from = 0);

    void addNodes(std::initializer_list<Node>);

    void mergeGraph(const Graph<NodeData, EdgeData> &);

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
#ifdef INPUT_GENERATOR_DEBUG
        if (position < indexStart || position >= indexStart + size())
            throw Exception("index out of range in the graph");
#endif
        return nodes[position - indexStart];
    }

    const Node& operator[](const int &position) const {
#ifdef INPUT_GENERATOR_DEBUG
        if (position < indexStart || position >= indexStart + size())
            throw Exception("index out of range in the graph");
#endif
        return nodes[position - indexStart];
    }

    int size() const {
        return nodes.size();
    }

    std::vector<EdgeType> arcs(const bool &forceSearch = true) const;

    std::vector<EdgeType> edges(const bool &forceSearch = true) const;

    void expandNode(const Node&, const Graph<NodeData, EdgeData>&);

    void expandNode(const Node&, const Graph<NodeData, EdgeData>&, const std::vector<Node>&);

    void expandEdge(const EdgeType&, Graph<NodeData, EdgeData>&, const Node&, const Node&);

    void expandArc(const EdgeType&, Graph<NodeData, EdgeData>&, const Node&, const Node&);

    void fuseGraph(Graph<NodeData, EdgeData>&, const std::vector< std::pair<Node, Node> >&, Boolean::Object simpleGraph = Boolean::False);

    void fuseGraph(Graph<NodeData, EdgeData>&, std::initializer_list< std::pair<Node, Node> >, Boolean::Object simpleGraph = Boolean::False);

    void fuseGraph(Graph<NodeData, EdgeData>&, int atLeast = 0, Boolean::Object simpleGraph = Boolean::False);

    bool hasNode(const Node&) const;

    int min() const {
        return indexStart;
    }

    int max() const {
        return indexStart + size() - 1;
    }

  protected:
    std::vector<Node> nodes;

    int indexStart;
};

template<class NodeData, class EdgeData>
Graph<NodeData, EdgeData>::Graph(const int& _size, const int& _indexStart) {
    // we create _size new nodes
    nodes.reserve(_size);
    for (int i = 0; i < _size; ++i)
        nodes.push_back(Node(i));

    // make it so it supports both possibilities
    // nodes from 0 to size - 1 or from 1 to size or etc
    indexStart = _indexStart;
}

template<class NodeData, class EdgeData>
Graph<NodeData, EdgeData>::Graph(Graph<NodeData, EdgeData> &&graph) {
    std::swap(nodes, graph.nodes);
    std::swap(indexStart, graph.indexStart);
}

template<class NodeData, class EdgeData>
Graph<NodeData, EdgeData>& Graph<NodeData, EdgeData>::operator=(
        Graph<NodeData, EdgeData> &&graph) {
    std::swap(nodes, graph.nodes);
    std::swap(indexStart, graph.indexStart);
    return *this;
}

template<class NodeData, class EdgeData>
Graph<NodeData, EdgeData> Graph<NodeData, EdgeData>::clone() const {
    Graph<NodeData, EdgeData> newGraph(size(), indexStart);

    for (auto &arc : arcs(true)) {
        newGraph[arc.from().index()].addEdge(newGraph[arc.to().index()],
                                             arc.data());
    }

    for (auto &edge : edges(true))
        auto edge2 = addEdge(newGraph[edge.from().index()],
                             newGraph[edge.to().index()],
                             edge.data());

    return newGraph;
}

template<class NodeData>
Graph<NodeData, void> cloneGraph(const Graph<NodeData, void>& graph) {
    Graph<NodeData, void> newGraph(graph.size(), graph.min());
    for (auto &arc : graph.arcs(true)) {
        newGraph[arc.from().index()].addEdge(newGraph[arc.to().index()]);
    }

    for (auto &edge: graph.edges(true))
        auto edge2 = addEdge(newGraph[edge.from().index()], newGraph[edge.to().index()]);

    return newGraph;
}


template<class NodeData, class EdgeData>
void Graph<NodeData, EdgeData>::clear() {
    std::vector<Node>().swap(nodes);
}

template<class NodeData, class EdgeData>
void Graph<NodeData, EdgeData>::Index(
        std::initializer_list< std::pair<Node, int> > fixed,
        const int &from) {
    // we go through each fixed node and keep track of it
    std::unordered_set<NodeType*> fixedNodes;
    // we also keep track of the values we have so we can assign demn easily
    std::unordered_set<int> usedValues;
    for (auto &node : fixed) {
#ifdef INPUT_GENERATOR_DEBUG
        if (node.second < from ||
            node.second >= from + static_cast<int>(nodes.size()))
            throw Exception("On Graph Reindexing nodes must be reindexed using "
                            "only values between `from` and `from` + the_number"
                            "_of_nodes - 1");

        // why would you give a node twice?
        // anyway we ignore following attempts
        // it's your duty to not have multiple such actions
        if (fixedNodes.find(node.first.internalNode.get()) != fixedNodes.end())
            throw Exception("On Graph Reindexing no node is allowed to appear "
                            "twice");

        // and why would you reuse values?
        // again we ignore such attempts
        if (usedValues.find(node.second) != usedValues.end())
            throw Exception("On Graph Reindexing no two nodes are allowed to ha"
                            "ve the same index");
#endif
        node.first.index() = node.second;
        fixedNodes.insert(node.first.internalNode.get());
        usedValues.insert(node.second);
    }

    std::vector<int> unusedValues;
    unusedValues.reserve(nodes.size() - usedValues.size());
    for (int i = from; i < from + static_cast<int>(nodes.size()); ++i)
        if (usedValues.find(i) == usedValues.end())
            unusedValues.push_back(i);
    unusedValues = shuffle(unusedValues);

    for (auto &node : nodes) {
        if (fixedNodes.find(node.internalNode.get()) != fixedNodes.end())
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
    for (auto &node : newNodes)
        node.index() = newIndex++;
    nodes.insert(nodes.end(), newNodes.begin(), newNodes.end());
}

template<class NodeData, class EdgeData>
void Graph<NodeData, EdgeData>::mergeGraph(
        const Graph<NodeData, EdgeData> &that) {
    for (auto &node : that)
        addNodes({node});
}

template<class NodeData, class EdgeData>
std::vector<_Edge<NodeData, EdgeData>> Graph<NodeData, EdgeData>::arcs(
        const bool& forceSearch) const {
    std::vector<_Edge<NodeData, EdgeData>> result;
    for (auto &node : nodes) {
        auto toAdd = node.arcs(forceSearch);

        result.insert(result.end(), toAdd.begin(), toAdd.end());
    }
    return result;
}

template<class NodeData, class EdgeData>
std::vector<_Edge<NodeData, EdgeData>> Graph<NodeData, EdgeData>::edges(
        const bool& forceSearch) const {
    std::vector<_Edge<NodeData, EdgeData>> result;
    for (auto &node : nodes) {
        auto toAdd = node.edges(forceSearch);

        for (auto &edge : toAdd)
            if (hasNode(edge.to())) {
                if (edge.from().index() <= edge.to().index())
                    result.push_back(edge);
            } else {
                result.push_back(edge);
            }
    }

    return result;
}

template<class NodeData, class EdgeData>
bool Graph<NodeData, EdgeData>::hasNode(const Node& that) const {
    if (that.index() < indexStart ||
        that.index() >= indexStart + static_cast<int>(nodes.size()))
        return false;

    return (*this)[that.index()] == that;
}

// specialization for shuffle
template<class NodeData, class EdgeData>
Graph<NodeData, EdgeData> shuffle(Graph<NodeData, EdgeData> && graph) {
    auto newGraph(std::move(graph));

    newGraph.Index();

    return newGraph;
}

template<class NodeData, class EdgeData>
Graph<NodeData, EdgeData> shuffle(const Graph<NodeData, EdgeData>& graph) {
    auto newGraph(std::move(graph.clone()));

    newGraph.Index();

    return newGraph;
}

template<class NodeData, class EdgeData>
void Graph<NodeData, EdgeData>::expandNode(const Node& node, const Graph<NodeData, EdgeData>& graph) {
    expandNode(node, graph, std::vector<Node>(graph.begin(), graph.end()));
}

namespace help {
    template<class NodeData, class EdgeData>
    void addEdge(Node<NodeData, EdgeData> from, Node<NodeData, EdgeData> to, _Edge<NodeData, EdgeData> original) {
        ::inputGenerator::addEdge(from, to, original.data());
    }

    template<class NodeData>
    void addEdge(Node<NodeData, void> from, Node<NodeData, void> to, _Edge<NodeData, void>) {
        ::inputGenerator::addEdge(from, to);
    }
}

// expandNode only works when the node has no incoming arcs
template<class NodeData, class EdgeData>
void Graph<NodeData, EdgeData>::expandNode(const Node& node, const Graph<NodeData, EdgeData>& graph, const std::vector<Node>& possibleNodes) {
    if (!hasNode(node))
        throw Exception("Node to expand must be in graph");

    if (graph.size() == 0)
        throw Exception("Graph to expand must contain at least one node");

    std::vector<Node> newNodes;
    newNodes.insert(newNodes.end(), graph.begin(), graph.end());

    for (auto &edge : node.edges()) {
        using namespace help;
        addEdge(randomElement(possibleNodes), edge.to(), edge);
        eraseEdge(edge);
    }

    newNodes[0].index() = node.index();
    nodes[node.index() - indexStart] = newNodes[0];

    for (auto it = newNodes.begin() + 1; it != newNodes.end(); ++it)
        addNodes({*it});
}

template<class NodeData, class EdgeData>
void Graph<NodeData, EdgeData>::expandEdge(const _Edge<NodeData, EdgeData>& edge, Graph<NodeData, EdgeData>& graph, const Node& from, const Node& to) {
    std::vector<Node> newNodes;

    eraseEdge(edge);

    addEdge(edge.from(), from);
    addEdge(to, edge.to());
    mergeGraph(graph);
}

template<class NodeData, class EdgeData>
void Graph<NodeData, EdgeData>::fuseGraph(Graph<NodeData, EdgeData>& graph,
                                          const std::vector< std::pair<Node, Node> > &mapping,
                                          Boolean::Object simpleGraph) {
#ifdef INPUT_GENERATOR_DEBUG
    for (auto &p : mapping) {
        if (!hasNode(p.first))
            throw Exception("Left side of a mapping must contain only nodes from the original graph");
        if (!graph.hasNode(p.second))
            throw Exception("Right side of a mapping must contain only nodes from the given graph");
    }
    // FIXME: add checks for uniqueness
#endif

    graph.Index(0);
    std::vector<bool> special(graph.size(), false);
    std::vector<Node> whom(graph.size());

    for (auto &p : mapping) {
        special[p.second.index()] = true;
        whom[p.second.index()] = p.first;
    }

    for (auto &p : mapping)
        for (auto &e : p.second.edges()) {
            if (!graph.hasNode(e.to()))
                continue;

            // special case, when we have to fuse the edge between two fused nodes
            if (special[e.to().index()]) {
                if (e.from().index() > e.to().index())
                    continue;
                using namespace help;
                if (!simpleGraph || !p.first.hasEdge(whom[e.to().index()]))
                    addEdge(p.first, whom[e.to().index()], e);
                eraseEdge(e);
                continue;
            }

            using namespace help;
            addEdge(p.first, e.to(), e);
            eraseEdge(e);
        }
    for (int i = 0; i < graph.size(); ++i)
        if (!special[i])
            addNodes({graph[i]});
}

template<class NodeData, class EdgeData>
void Graph<NodeData, EdgeData>::fuseGraph(Graph<NodeData, EdgeData>& graph,
                                          std::initializer_list< std::pair<Node, Node> > mapping,
                                          Boolean::Object simpleGraph) {
    return fuseGraph(graph, std::vector< std::pair<Node, Node> >(mapping.begin(), mapping.end()), simpleGraph);
}

template<class NodeData, class EdgeData>
void Graph<NodeData, EdgeData>::fuseGraph(Graph<NodeData, EdgeData>& graph, int atLeast, Boolean::Object simpleGraph) {
    int maximum = std::min(size(), graph.size());

    int pick = randomInt(atLeast, maximum);
    auto left = shuffle(randomSubsequence(*this), pick);
    auto right = shuffle(randomSubsequence(graph), pick);

    std::vector< std::pair<Node, Node> > mapping;
    mapping.reserve(pick);
    for (int i = 0; i < pick; ++i)
        mapping.emplace_back(left[i], right[i]);
    fuseGraph(graph, mapping, simpleGraph);
}

}  // namespace inputGenerator


#endif  // INPUT_GENERATOR_GRAPH_HPP_
