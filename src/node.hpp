#if !defined(INPUT_GENERATOR_NODE_HPP_)
#define INPUT_GENERATOR_NODE_HPP_

#include <vector>
#include <unordered_map>

namespace inputGenerator {

#if !defined(INPUT_GENERATOR_EDGE_HPP_)
template<class NodeData, class EdgeData>
class Edge;
#endif

template<class NodeData = int, class EdgeData = int>
class Node {
  public:
    typedef Node<NodeData, EdgeData> NodeType;
    typedef Edge<NodeData, EdgeData> EdgeType;

    Node(const int &_index = 0);

    const unsigned& getKey() const;

    void addEdge(const NodeType&);

    void addEdge(const EdgeType&);

    int edgesTo(const NodeType&) const;

    NodeData data;

    // the index of the node
    int index;

  private:
    friend class Edge<NodeData, EdgeData>;

    std::unordered_multimap<unsigned, EdgeType> neighbours;

    // auto-increment value to asign each Node object a key
    static unsigned keyCount;

    // the key itself
    unsigned key;
};

template<class NodeData, class EdgeData>
unsigned Node<NodeData, EdgeData>::keyCount = 0;

template<class NodeData, class EdgeData>
Node<NodeData, EdgeData>::Node(const int &_index) {
    index = _index;
    key = keyCount++;
}

template<class NodeData, class EdgeData>
const unsigned& Node<NodeData, EdgeData>::getKey() const {
    return key;
}

template<class NodeData, class EdgeData>
void Node<NodeData, EdgeData>::addEdge(const EdgeType& edge) {
    // just so you dont add stupid edges
    if (edge.from != this) {
        EdgeType newEdge = edge;
        newEdge.from = this;
        neighbours.insert({edge.to->getKey(), newEdge});
    } else {
        neighbours.insert({edge.to->getKey(), edge});
    }
}

template<class NodeData, class EdgeData>
void Node<NodeData, EdgeData>::addEdge(const NodeType& otherNode) {
    EdgeType edge(*this, otherNode);
    addEdge(edge);
}

template<class NodeData, class EdgeData>
int Node<NodeData, EdgeData>::edgesTo(const NodeType& otherNode) const {
    return neighbours.count(otherNode.getKey());
}

}

#endif // INPUT_GENERATOR_NODE_HPP_
