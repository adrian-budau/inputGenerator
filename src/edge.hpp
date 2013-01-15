#if !defined(INPUT_GENERATOR_EDGE_HPP_)
#define INPUT_GENERATOR_EDGE_HPP_

#include <memory>

namespace inputGenerator {

#if !defined(INPUT_GENERATOR_NODE_HPP_)
template<class NodeData, class EdgeData>
class _Node;

template<class NodeData, class EdgeData>
class NodeWrapper;

#endif

/**
 * Memory used is 20 bytes + (the data which by default is 4)
 */
template<class NodeData = int, class EdgeData = int>
class _Edge {
  public:
    typedef _Node<NodeData, EdgeData> NodeType;
    typedef _Edge<NodeData, EdgeData> EdgeType;

    _Edge(const _Edge&);

    _Edge(const std::shared_ptr<NodeType>&, const std::shared_ptr<NodeType>&);

    _Edge(const std::shared_ptr<NodeType>&, const std::shared_ptr<NodeType>&, const int&, const std::shared_ptr<EdgeData>& data = new EdgeData);

    const unsigned& getKey() const;

    NodeWrapper<NodeData, EdgeData> from();

    NodeWrapper<NodeData, EdgeData> to();

    EdgeData& data() const;

    std::shared_ptr<EdgeData> dataPointer() const;

  private:
    friend class _Node<NodeData, EdgeData>;

    std::weak_ptr<NodeType> _from, _to;

    // auto-increment value to assign each each a key so edge from x to y should match edge to y to x
    static unsigned keyCount;

    // the key itself
    unsigned key;

    std::shared_ptr<EdgeData> _data;
};

template<class NodeData, class EdgeData>
unsigned _Edge<NodeData, EdgeData>::keyCount = 0;

template<class NodeData, class EdgeData>
_Edge<NodeData, EdgeData>::_Edge(const _Edge<NodeData, EdgeData>& that) {
    _from = that._from;
    _to = that._to;
    key = that.key;

    _data = that._data;
}

template<class NodeData, class EdgeData>
_Edge<NodeData, EdgeData>::_Edge(const std::shared_ptr<NodeType>& from, const std::shared_ptr<NodeType>& to) {
    _from = from;
    _to = to;
    key = keyCount++;

    _data.reset(new EdgeData());
}

template<class NodeData, class EdgeData>
_Edge<NodeData, EdgeData>::_Edge(const std::shared_ptr<NodeType>& from, const std::shared_ptr<NodeType>& to, const int &_key, const std::shared_ptr<EdgeData>& data) {
    _from = from;
    _to = to;
    key = _key;

    _data = data;
}

template<class NodeData, class EdgeData>
const unsigned& _Edge<NodeData, EdgeData>::getKey() const {
    return key;
}

template<class NodeData, class EdgeData>
NodeWrapper<NodeData, EdgeData> _Edge<NodeData, EdgeData>::from() {
    return _from.lock();
}

template<class NodeData, class EdgeData>
NodeWrapper<NodeData, EdgeData> _Edge<NodeData, EdgeData>::to() {
    return _to.lock();
}

template<class NodeData, class EdgeData>
EdgeData& _Edge<NodeData, EdgeData>::data() const {
    return *_data;
}

template<class NodeData, class EdgeData>
std::shared_ptr<EdgeData> _Edge<NodeData, EdgeData>::dataPointer() const {
    return _data;
}

template<class NodeData, class EdgeData>
_Edge<NodeData, EdgeData> addEdge(const NodeWrapper<NodeData, EdgeData> &from, const NodeWrapper<NodeData, EdgeData> &to, const EdgeData& data = EdgeData()) {
    auto edge = from.addEdge(to, data);
    to.addEdge(from, edge.dataPointer(), edge.getKey());
    return edge;
}

/*
template<class NodeData, class EdgeData>
class EdgeWrapper {
  public:
    typedef _Edge<NodeData, EdgeData> EdgeType;

    EdgeWrapper(const EdgeWrapper<NodeData, EdgeData> &otherEdgeWrapper) {
        internalEdge = otherEdgeWrapper.internalEdge;
    }

    EdgeWrapper(EdgeType* otherEdge): internalEdge(otherEdge) {
    }

    EdgeWrapper(EdgeTyp
    NodeWrapper<NodeData, EdgeData> from() {
        return NodeWrapper<NodeData, EdgeData>(internalEdge -> from);
    }

    NodeWrapper<NodeData, EdgeData> to() {
        return NodeWrapper<NodeData, EdgeData>(internalEdge -> to);
    }

  private:
    std::shared_ptr<EdgeType> internalEdge;
};
*/

}

#endif // INPUT_GENERATOR_EDGE_HPP_
