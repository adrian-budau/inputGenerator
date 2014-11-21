#if !defined(INPUT_GENERATOR_EDGE_HPP_)
#define INPUT_GENERATOR_EDGE_HPP_

#include <memory>

namespace inputGenerator {

#if !defined(INPUT_GENERATOR_NODE_HPP_)
template<class NodeData, class EdgeData>
class _NodeBase;

template<class NodeData, class EdgeData>
class NodeWrapper;

template<class NodeData, class EdgeData>
class _Node;
#endif

template<class NodeData, class EdgeData>
class _EdgeBase {
  public:
    typedef _Node<NodeData, EdgeData> NodeType;
    typedef _EdgeBase<NodeData, EdgeData> EdgeType;

    _EdgeBase(const _EdgeBase&);

    _EdgeBase(const std::shared_ptr<NodeType>&,
              const std::shared_ptr<NodeType>&);

    _EdgeBase(const std::shared_ptr<NodeType>&,
              const std::shared_ptr<NodeType>&,
              const int&);

    const unsigned& getKey() const;

    NodeWrapper<NodeData, EdgeData> from() const;

    NodeWrapper<NodeData, EdgeData> to() const;

    bool operator==(const EdgeType&) const;

  protected:
    std::weak_ptr<NodeType> _from, _to;

    friend class _NodeBase<NodeData, EdgeData>;

    // auto-increment value to assign each each a key
    // so edge from x to y should match edge to y to x
    static unsigned keyCount;

    // the key itself
    unsigned key;
};

template<class NodeData, class EdgeData>
unsigned _EdgeBase<NodeData, EdgeData>::keyCount = 0;

template<class NodeData, class EdgeData>
_EdgeBase<NodeData, EdgeData>::_EdgeBase(
        const _EdgeBase<NodeData, EdgeData>& that) {
    _from = that._from;
    _to = that._to;
    key = that.key;
}

template<class NodeData, class EdgeData>
_EdgeBase<NodeData, EdgeData>::_EdgeBase(const std::shared_ptr<NodeType>& from,
                                 const std::shared_ptr<NodeType>& to) {
    _from = from;
    _to = to;
    key = keyCount++;
}

template<class NodeData, class EdgeData>
_EdgeBase<NodeData, EdgeData>::_EdgeBase(const std::shared_ptr<NodeType>& from,
                                 const std::shared_ptr<NodeType>& to,
                                 const int &_key) {
    _from = from;
    _to = to;
    key = _key;
}

template<class NodeData, class EdgeData>
const unsigned& _EdgeBase<NodeData, EdgeData>::getKey() const {
    return key;
}

template<class NodeData, class EdgeData>
NodeWrapper<NodeData, EdgeData> _EdgeBase<NodeData, EdgeData>::from() const {
    return NodeWrapper<NodeData, EdgeData>(_from.lock());
}

template<class NodeData, class EdgeData>
NodeWrapper<NodeData, EdgeData> _EdgeBase<NodeData, EdgeData>::to() const {
    return NodeWrapper<NodeData, EdgeData>(_to.lock());
}

template<class NodeData, class EdgeData>
bool _EdgeBase<NodeData, EdgeData>::operator==(
        const _EdgeBase<NodeData, EdgeData> &edge) const {
    return getKey() == edge.getKey();
}

/**
 * Memory used is 20 bytes + (the data which by default is 0, hopefully)
 */
template<class NodeData, class EdgeData>
class _Edge : public _EdgeBase<NodeData, EdgeData> {
  public:
    typedef _Node<NodeData, EdgeData> NodeType;
    typedef _Edge<NodeData, EdgeData> EdgeType;

    _Edge(const _Edge& that):
            _EdgeBase<NodeData, EdgeData>(that) {
        _data = that._data;
    }

    _Edge(const std::shared_ptr<NodeType>& from,
          const std::shared_ptr<NodeType>& to):
            _EdgeBase<NodeData, EdgeData>(from, to) {
        _data.reset(new EdgeData());
    }

    _Edge(const std::shared_ptr<NodeType>& from,
          const std::shared_ptr<NodeType>& to,
          const int& key,
          const std::shared_ptr<EdgeData>& data = new EdgeData):
            _EdgeBase<NodeData, EdgeData>(from, to, key) {
        _data = data;
    }


    EdgeData& data() const {
        return *_data;
    }


    std::shared_ptr<EdgeData> dataPointer() const {
        return _data;
    }

  private:
    friend class _Node<NodeData, EdgeData>;
    friend class _NodeBase<NodeData, EdgeData>;

    std::shared_ptr<EdgeData> _data;
};

template<class NodeData>
class _Edge<NodeData, void> : public _EdgeBase<NodeData, void> {
  public:
    typedef _Node<NodeData, void> NodeType;
    typedef _Edge<NodeData, void> EdgeType;

    _Edge(const _Edge& that):
        _EdgeBase<NodeData, void>(that) {
    }

    _Edge(const std::shared_ptr<NodeType>& from,
          const std::shared_ptr<NodeType>& to):
        _EdgeBase<NodeData, void>(from, to) {
    }

    _Edge(const std::shared_ptr<NodeType>& from,
          const std::shared_ptr<NodeType>& to,
          const int& key):
            _EdgeBase<NodeData, void>(from, to, key) {
    }

    friend class _Node<NodeData, void>;
    friend class _NodeBase<NodeData, void>;
};

template<class NodeData, class EdgeData>
_Edge<NodeData, EdgeData> addEdge(const NodeWrapper<NodeData, EdgeData> &from,
                                  const NodeWrapper<NodeData, EdgeData> &to,
                                  const EdgeData& data = EdgeData()) {
    auto edge = from.addEdge(to, data);
    to.addEdge(from, edge.dataPointer(), edge.getKey());
    return edge;
}

template<class NodeData>
_Edge<NodeData, void> addEdge(const NodeWrapper<NodeData, void> &from,
                              const NodeWrapper<NodeData, void> &to) {
    auto edge = from.addEdge(to);
    if (from != to)
        to.addEdge(from, edge.getKey());
    return edge;
}

template<class NodeData, class EdgeData>
bool eraseEdge(const _Edge<NodeData, EdgeData> &edge) {
    // edge lives in `from` so let's delete from the other end first
    edge.to().eraseEdge(edge);
    return edge.from().eraseEdge(edge);
}

}  // namespace inputGenerator

#endif  // INPUT_GENERATOR_EDGE_HPP_
