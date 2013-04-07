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

    bool eraseEdge(const EdgeType&);

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

    std::unordered_map<unsigned, EdgeType> _edges;

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
bool _Node<NodeData, EdgeData>::eraseEdge(const EdgeType& edge) {
    // first let's find it
    if (hasEdge(edge) == false)
        return false;
    auto realEdge = _edges.find(edge.getKey());
    auto where = _neighbours.equal_range(realEdge->second.to().getKey());

    for (auto it = where.first; it != where.second; ++it)
        if (it -> second == realEdge -> second) {
            _neighbours.erase(it);
            _edges.erase(realEdge);
            return true;
        }

    throw new Exception("Edge was in normal list but not in neighbour list");
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

    NodeWrapper() {
        if (is_const_object_())
            internalNode.reset(new NodeType(0));
    }

    NodeWrapper(const int &index):
        internalNode(new NodeType(index)) {
    }

    NodeWrapper(const NodeWrapper<NodeData, EdgeData> &otherNodeWrapper) {
        if (is_const_object_())
            otherNodeWrapper.lazyconstruct();
        internalNode = otherNodeWrapper.internalNode;
    }

    NodeWrapper(const std::shared_ptr<NodeType> &otherNode) {
        internalNode = otherNode;
    }

    NodeWrapper& operator=(const NodeWrapper& otherNodeWrapper) {
        otherNodeWrapper.lazyconstruct();
        internalNode = otherNodeWrapper.internalNode;
        return *this;
    }

    void lazyconstruct() const {
        if (!internalNode) // i'm cheating yes, but i know it's not a const object, i made sure
            const_cast<std::shared_ptr<NodeType>&>(internalNode).reset(new NodeType(0));
    }

    const unsigned& getKey() const {
        lazyconstruct();
        return internalNode -> getKey();
    }

    EdgeType addEdge(const NodeWrapper& otherNodeWrapper, const EdgeData& data = EdgeData()) const {
        lazyconstruct();
        otherNodeWrapper.lazyconstruct();
        auto edge = EdgeType(internalNode, otherNodeWrapper.internalNode);
        edge.data() = data;

        internalNode -> _neighbours.insert({otherNodeWrapper.getKey(), edge});
        internalNode -> _edges.insert({edge.getKey(), edge});

        return edge;
    }

    EdgeType addEdge(const NodeWrapper& otherNodeWrapper, const EdgeData& data, const unsigned &key) const {
        lazyconstruct();
        otherNodeWrapper.lazyconstruct();
        auto edge = EdgeType(internalNode, otherNodeWrapper.internalNode, key);
        edge.data() = data;

        internalNode -> _neighbours.insert({otherNodeWrapper.getKey(), edge});
        internalNode -> _edges.insert({edge.getKey(), edge});

        return edge;
    }

    EdgeType addEdge(const NodeWrapper& otherNodeWrapper, const std::shared_ptr<EdgeData>& dataPointer, const unsigned &key) const {
        lazyconstruct();
        otherNodeWrapper.lazyconstruct();
        auto edge = EdgeType(internalNode, otherNodeWrapper.internalNode, key, dataPointer);

        internalNode -> _neighbours.insert({otherNodeWrapper.getKey(), edge});
        internalNode -> _edges.insert({edge.getKey(), edge});

        return edge;
    }

    int& index() const {
        lazyconstruct();
        return internalNode -> index;
    }

    // you can force to find arcs that are not edges(in case you have both)
    std::vector<EdgeType> arcsTo(const NodeWrapper& otherNodeWrapper, const bool& forceSearch = false) const {
        lazyconstruct();
        otherNodeWrapper.lazyconstruct();
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
        lazyconstruct();
        otherNodeWrapper.lazyconstruct();

       if (forceSearch)
            return (internalNode -> edgesTo(*otherNodeWrapper.internalNode));

        auto interval = internalNode->_neighbours.equal_range(otherNodeWrapper.getKey());

        std::vector<EdgeType> result;
        for (auto &it = interval.first; it != interval.second; ++it)
            result.push_back(it->second);
        return result;
    }

    bool hasArc(const NodeWrapper& otherNodeWrapper, const bool& forceSearch = false) const {
        lazyconstruct();
        otherNodeWrapper.lazyconstruct();

        if (forceSearch)
            return internalNode -> hasArc(*otherNodeWrapper.internalNode);
        return internalNode->_neighbours.count(otherNodeWrapper.getKey()) > 0;
    }

    bool hasEdge(const NodeWrapper& otherNodeWrapper, const bool& forceSearch = false) const {
        lazyconstruct();
        otherNodeWrapper.lazyconstruct();

        if (forceSearch)
            return internalNode -> hasEdge(*otherNodeWrapper.internalNode);
        return internalNode->_neighbours.count(otherNodeWrapper.getKey()) > 0;
    }

    bool eraseEdge(const EdgeType& edge) const {
        lazyconstruct();

        return internalNode -> eraseEdge(edge);
    }

    std::vector<EdgeType> arcs(const bool& forceSearch = false) const {
        lazyconstruct();

        if (forceSearch)
            return internalNode -> arcs();

        std::vector<EdgeType> result;
        for (auto &arc: internalNode -> _neighbours)
            result.push_back(arc.second);
        return result;
    }

    std::vector<EdgeType> edges(const bool& forceSearch = false) const {
        lazyconstruct();

        if (forceSearch)
            return internalNode -> edges();

        std::vector<EdgeType> result;
        for (auto &edge: internalNode -> _neighbours)
            result.push_back(edge.second);
        return result;
    }


    NodeData& data() const {
        lazyconstruct();

        return internalNode -> data;
    }

    void clear() const {
        lazyconstruct();

        internalNode -> clear();
    }

    bool operator==(const NodeWrapper<NodeData, EdgeData> &that) const {
        return internalNode == that.internalNode;
    }

  private:
    bool is_const_object_() {
        return false;
    }

    bool is_const_object_() const {
        return true;
    }

    std::shared_ptr<NodeType> internalNode;
};

#define GCC_VERSION (__GNUC__ * 10000 + __GNUC_MINOR__ * 100 + __GNUC_PATCHLEVEL__)

#if GCC_VERSION >= 40700
template<class NodeData = int, class EdgeData = int>
using Node = NodeWrapper<NodeData, EdgeData>;
#endif
#undef GCC_VERSION

}

#endif // INPUT_GENERATOR_NODE_HPP_
