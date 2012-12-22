#if !defined(INPUT_GENERATOR_NODE_HPP_)
#define INPUT_GENERATOR_NODE_HPP_

#include <vector>
#include <unordered_map>

namespace inputGenerator {

#if !defined(INPUT_GENERATOR_EDGE_HPP_)
template<class NodeData, class EdgeData>
class _Edge;
#endif

template<class NodeData = int, class EdgeData = int>
class _Node {
  public:
    typedef _Node<NodeData, EdgeData> NodeType;
    typedef _Edge<NodeData, EdgeData> EdgeType;

    _Node(const int &_index = 0);

    const unsigned& getKey() const;

    void addEdge(const NodeType&);

    void addEdge(const EdgeType&);

    int edgesTo(const NodeType&) const;

    NodeData data;

    // the index of the node
    int index;

  private:
    friend class _Edge<NodeData, EdgeData>;

    std::unordered_multimap<unsigned, EdgeType> neighbours;

    // auto-increment value to asign each _Node object a key
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
void _Node<NodeData, EdgeData>::addEdge(const EdgeType& edge) {
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
void _Node<NodeData, EdgeData>::addEdge(const NodeType& otherNode) {
    EdgeType edge(*this, otherNode);
    addEdge(edge);
}

template<class NodeData, class EdgeData>
int _Node<NodeData, EdgeData>::edgesTo(const NodeType& otherNode) const {
    return neighbours.count(otherNode.getKey());
}

// the problem with nodes is the fact they can't be copied
// this is a wrapper to allow such copying without having to deal with pointers
template<class NodeData, class EdgeData>
class NodeWrapper {
  public:
    typedef _Node<NodeData, EdgeData> NodeType;

    NodeWrapper(const int &index = 0) {
        internalNode = new NodeType(index);
    }

    NodeWrapper(const NodeWrapper<NodeData, EdgeData> &otherNodeWrapper) {
        internalNode = otherNodeWrapper.internalNode;
    }

    NodeWrapper(const NodeType* otherNode) {
        internalNode = otherNode;
    }

    NodeWrapper(const NodeType& otherNode) {
        internalNode = &otherNode;
    }

    NodeWrapper& operator=(const NodeWrapper& otherNodeWrapper) {
        internalNode = otherNodeWrapper.internalNode;
        return *this;
    }

    const unsigned& getKey() const {
        return internalNode -> getKey();
    }

    void addEdge(const NodeWrapper& otherNodeWrapper) {
        internalNode -> addEdge(*otherNodeWrapper.internalNode);
    }

    int& index() {
        return internalNode -> index;
    }

    int& index() const {
        return internalNode -> index;
    }

    int edgesTo(const NodeWrapper& otherNodeWrapper) const {
        return (internalNode -> edgesTo(*otherNodeWrapper.internalNode));
    }

  private:
    NodeType *internalNode;
};

}

#endif // INPUT_GENERATOR_NODE_HPP_
