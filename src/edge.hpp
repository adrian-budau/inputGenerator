#if !defined(INPUT_GENERATOR_EDGE_HPP_)
#define INPUT_GENERATOR_EDGE_HPP_

namespace inputGenerator {

#if !defined(INPUT_GENERATOR_NODE_HPP_)
template<class NodeData, class EdgeData>
class _Node;
#endif

template<class NodeData = int, class EdgeData = int>
class _Edge {
  public:
    typedef _Node<NodeData, EdgeData> NodeType;
    typedef _Edge<NodeData, EdgeData> EdgeType;

    _Edge(const _Edge&);

    _Edge(const NodeType&, const NodeType&);

    EdgeData data;

  private:
    friend class _Node<NodeData, EdgeData>;

    NodeType *from, *to;
};

template<class NodeData, class EdgeData>
_Edge<NodeData, EdgeData>::_Edge(const _Edge<NodeData, EdgeData>& that) {
  from = const_cast<NodeType*>(that.from);
  to = const_cast<NodeType*>(that.to);
}

template<class NodeData, class EdgeData>
_Edge<NodeData, EdgeData>::_Edge(const NodeType& _from, const NodeType& _to) {
  from = const_cast<NodeType*>(&_from);
  to = const_cast<NodeType*>(&_to);
}

}

#endif // INPUT_GENERATOR_EDGE_HPP_
