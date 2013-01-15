#if !defined(INPUT_GENERATOR_NODE_HPP_)
#define INPUT_GENERATOR_NODE_HPP_

#include <vector>
#include <unordered_map>
#include <memory>

namespace inputGenerator {

#if !defined(INPUT_GENERATOR_EDGE_HPP_)
template<class NodeData, class EdgeData>
class _Edge;
#endif

/**
 * Just some forward declaration
 */
template<class NodeData, class EdgeData>
class NodeWrapper;

/**
 * Memory used is 16 * 2 ( from unordered multimaps) + 8 (the key and index) + (the data which by default is 4)
 */
template<class NodeData = int, class EdgeData = int>
class _Node {
  public:
    typedef _Node<NodeData, EdgeData> NodeType;
    typedef _Edge<NodeData, EdgeData> EdgeType;

    _Node(const int &_index = 0);

    const unsigned& getKey() const;

    EdgeType addEdge(NodeType&);

    EdgeType addEdge(NodeType&, const unsigned &key);

    EdgeType addEdge(const EdgeType&);

    bool hasEdge(const EdgeType&) const;

    std::vector<EdgeType> arcsTo(const NodeType&) const;

    std::vector<EdgeType> edgesTo(const NodeType&) const;

    bool hasArc(const NodeType&) const;

    bool hasEdge(const NodeType&) const;

    std::vector<EdgeType> arcs() const;

    std::vector<EdgeType> edges() const;

    void clear();

    NodeData data;

    // the index of the node
    int index;

  private:
    friend class _Edge<NodeData, EdgeData>;
    friend class NodeWrapper<NodeData, EdgeData>;

    // damn that's a lot, lot of memory, we're lucky we don't copy the data
    // well it's only 4 bytes anyway so that's not much of an improvement
    // now for a graph of N nodes and E edges we have at least N * 44 + 2 * E * 20 bytes occupied
    std::unordered_multimap<unsigned, EdgeType> _neighbours;

    std::unordered_multimap<unsigned, EdgeType> _edges;

    // auto-increment value to assign each _Node object a key
    static unsigned keyCount;

    // the key itself
    unsigned key;
};

template<class NodeData, class EdgeData>
unsigned _Node<NodeData, EdgeData>::keyCount = 0;

template<class NodeData, class EdgeData>
_Node<NodeData, EdgeData>::_Node(const int &_index) {
    index = _index;
    key = keyCount++;
}

template<class NodeData, class EdgeData>
const unsigned& _Node<NodeData, EdgeData>::getKey() const {
    return key;
}

template<class NodeData, class EdgeData>
bool _Node<NodeData, EdgeData>::hasEdge(const EdgeType& edge) const {
    return _edges.count(edge.getKey()) > 0;
}

template<class NodeData, class EdgeData>
std::vector<_Edge<NodeData, EdgeData>> _Node<NodeData, EdgeData>::arcsTo(const NodeType& otherNode) const {
    std::vector<EdgeType> result;

    auto range = _neighbours.equal_range(otherNode.getKey());
    for (auto it = range.first; it != range.second; ++it)
        if (!otherNode.hasEdge(it->second))
            result.push_back(it->second);
    return result;
}

template<class NodeData, class EdgeData>
std::vector<_Edge<NodeData, EdgeData>> _Node<NodeData, EdgeData>::edgesTo(const NodeType& otherNode) const {
    std::vector<EdgeType> result;

    auto range = _neighbours.equal_range(otherNode.getKey());
    for (auto it = range.first; it != range.second; ++it)
        if (otherNode.hasEdge(it->second))
            result.push_back(it->second);
    return result;
}

template<class NodeData, class EdgeData>
bool _Node<NodeData, EdgeData>::hasArc(const NodeType& otherNode) const {
    auto range = _neighbours.equal_range(otherNode.getKey());
    for (auto it = range.first; it != range.second; ++it)
        if (!otherNode.hasEdge(it->second.getKey()))
            return true;
    return false;
}

template<class NodeData, class EdgeData>
bool _Node<NodeData, EdgeData>::hasEdge(const NodeType& otherNode) const {
    auto range = _neighbours.equal_range(otherNode.getKey());
    for (auto it = range.first; it != range.second; ++it)
        if (otherNode.hasEdge(it->second.getKey()))
            return true;
    return false;
}

template<class NodeData, class EdgeData>
std::vector<_Edge<NodeData, EdgeData>> _Node<NodeData, EdgeData>::arcs() const {
    std::vector<EdgeType> result;
    for (auto &arc: _edges)
        if (!arc.second._to.lock()->hasEdge(arc.second))
            result.push_back(arc.second);
    return result;
}

template<class NodeData, class EdgeData>
std::vector<_Edge<NodeData, EdgeData>> _Node<NodeData, EdgeData>::edges() const {
    std::vector<EdgeType> result;
    for (auto &edge: _edges)
        if (edge.second._to.lock()->hasEdge(edge.second))
            result.push_back(edge.second);
    return result;
}

template<class NodeData, class EdgeData>
void _Node<NodeData, EdgeData>::clear() {
    _neighbours.clear();
    _edges.clear();
}

// the problem with nodes is the fact they can't be copied
/// this is a wrapper to allow such copying without having to deal with pointers
template<class NodeData, class EdgeData>
class NodeWrapper {
  public:
    typedef _Node<NodeData, EdgeData> NodeType;
    typedef _Edge<NodeData, EdgeData> EdgeType;

    NodeWrapper(const int &index = 0):
      internalNode(new NodeType(index)) {
    }

    NodeWrapper(const NodeWrapper<NodeData, EdgeData> &otherNodeWrapper) {
        internalNode = otherNodeWrapper.internalNode;
    }

    NodeWrapper(const std::shared_ptr<NodeType> &otherNode) {
        internalNode = otherNode;
    }

    NodeWrapper& operator=(const NodeWrapper& otherNodeWrapper) {
        internalNode = otherNodeWrapper.internalNode;
        return *this;
    }

    const unsigned& getKey() const {
        return internalNode -> getKey();
    }

    EdgeType addEdge(const NodeWrapper& otherNodeWrapper, const EdgeData& data = EdgeData()) const {
        auto edge = EdgeType(internalNode, otherNodeWrapper.internalNode);
        edge.data() = data;

        internalNode -> _neighbours.insert({otherNodeWrapper.getKey(), edge});
        internalNode -> _edges.insert({edge.getKey(), edge});

        return edge;
    }

    EdgeType addEdge(const NodeWrapper& otherNodeWrapper, const EdgeData& data, const unsigned &key) const {
        auto edge = EdgeType(internalNode, otherNodeWrapper.internalNode, key);
        edge.data() = data;

        internalNode -> _neighbours.insert({otherNodeWrapper.getKey(), edge});
        internalNode -> _edges.insert({edge.getKey(), edge});

        return edge;
    }

    EdgeType addEdge(const NodeWrapper& otherNodeWrapper, const std::shared_ptr<EdgeData>& dataPointer, const unsigned &key) const {
        auto edge = EdgeType(internalNode, otherNodeWrapper.internalNode, key, dataPointer);

        internalNode -> _neighbours.insert({otherNodeWrapper.getKey(), edge});
        internalNode -> _edges.insert({edge.getKey(), edge});

        return edge;
    }

    int& index() const {
        return internalNode -> index;
    }

    // you can force to find arcs that are not edges(in case you have both)
    std::vector<EdgeType> arcsTo(const NodeWrapper& otherNodeWrapper, const bool& forceSearch = false) const {
        if (forceSearch)
            return (internalNode -> arcsTo(*otherNodeWrapper.internalNode));

        auto interval = internalNode->_neighbours.equal_range(otherNodeWrapper.getKey());

        std::vector<EdgeType> result;
        for (auto &it = interval.first; it != interval.second; ++it)
            result.push_back(it->second);
        return result;
    }

    // same here
    std::vector<EdgeType> edgesTo(const NodeWrapper& otherNodeWrapper, const bool& forceSearch = false) const {
        if (forceSearch)
            return (internalNode -> edgesTo(*otherNodeWrapper.internalNode));

        auto interval = internalNode->_neighbours.equal_range(otherNodeWrapper.getKey());

        std::vector<EdgeType> result;
        for (auto &it = interval.first; it != interval.second; ++it)
            result.push_back(it->second);
        return result;
    }

    bool hasArc(const NodeWrapper& otherNodeWrapper, const bool& forceSearch = false) const {
        if (forceSearch)
            return internalNode -> hasArc(*otherNodeWrapper.internalNode);
        return internalNode->_neighbours.count(otherNodeWrapper.getKey()) > 0;
    }

    bool hasEdge(const NodeWrapper& otherNodeWrapper, const bool& forceSearch = false) const {
        if (forceSearch)
            return internalNode -> hasEdge(*otherNodeWrapper.internalNode);
        return internalNode->_neighbours.count(otherNodeWrapper.getKey()) > 0;
    }

    std::vector<EdgeType> arcs(const bool& forceSearch = false) const {
        if (forceSearch)
            return internalNode -> arcs();

        std::vector<EdgeType> result;
        for (auto &arc: internalNode -> _neighbours)
            result.push_back(arc.second);
        return result;
    }

    std::vector<EdgeType> edges(const bool& forceSearch = false) const {
        if (forceSearch)
            return internalNode -> edges();

        std::vector<EdgeType> result;
        for (auto &edge: internalNode -> _neighbours)
            result.push_back(edge.second);
        return result;
    }


    NodeData& data() const {
        return internalNode -> data;
    }

    void clear() const {
        internalNode -> clear();
    }

    bool operator==(const NodeWrapper<NodeData, EdgeData> &that) const {
        return internalNode == that.internalNode;
    }

  private:
    std::shared_ptr<NodeType> internalNode;
};

}

#endif // INPUT_GENERATOR_NODE_HPP_
