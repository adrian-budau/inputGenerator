#if !defined(INPUT_GENERATOR_EDGE_HPP_)
#define INPUT_GENERATOR_EDGE_HPP_

namespace inputGenerator {

#if !defined(INPUT_GENERATOR_NODE_HPP_)
template<class NodeData, class EdgeData>
class Node;
#endif

template<class NodeData = int, class EdgeData = int>
class Edge {
  public:
    typedef Node<NodeData, EdgeData> NodeType;
    typedef Edge<NodeData, EdgeData> EdgeType;

    Edge(const Edge&);

    Edge(const NodeType&, const NodeType&);

    EdgeData data;

  private:
    friend class Node<NodeData, EdgeData>;

    NodeType *from, *to;
};

template<class NodeData, class EdgeData>
Edge<NodeData, EdgeData>::Edge(const Edge<NodeData, EdgeData>& that) {
  from = const_cast<NodeType*>(that.from);
  to = const_cast<NodeType*>(that.to);
}

template<class NodeData, class EdgeData>
Edge<NodeData, EdgeData>::Edge(const NodeType& _from, const NodeType& _to) {
  from = const_cast<NodeType*>(&_from);
  to = const_cast<NodeType*>(&_to);
}

}

#endif // INPUT_GENERATOR_EDGE_HPP_
