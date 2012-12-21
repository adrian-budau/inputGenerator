#ifndef INPUT_GENERATOR_GRAPH_HPP_
#define INPUT_GENERATOR_GRAPH_HPP_

#include <vector>
#include <utility>
#include <unordered_set>
#include <algorithm>

#include "node.hpp"
#include "edge.hpp"
#include "vectors.hpp"

namespace inputGenerator {

template<class NodeData = int, class EdgeData = int>
class Graph {
  public:
    typedef Node<NodeData, EdgeData> NodeType;
    typedef Edge<NodeData, EdgeData> EdgeType;

    // basic iterators
    template<class VectorIteratorType>
    class Iterator {
      public:
        typedef int difference_type;
        typedef NodeType value_type;
        typedef typename VectorIteratorType::value_type pointer;
        typedef typename std::iterator_traits<pointer>::reference reference;
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
            return *(*alpha);
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
    typedef Iterator<typename std::vector<NodeType*>::iterator> iterator;
    typedef Iterator<typename std::vector<NodeType*>::const_iterator> const_iterator;

    Graph(const int& _size = 1, const bool& _fromZero = true);

    void Index(std::initializer_list< std::pair<NodeType&, int> > fixed, const int &from = 0);

    void Index(const int &from = 0);

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

    NodeType& operator[](const int &position) {
        return *(nodes[position]);
    }

    const NodeType& operator[](const int &position) const {
        return *(nodes[position]);
    }

    int size() const {
        return nodes.size();
    }

  private:
    std::vector<NodeType *> nodes;

    bool fromZero;
};

template<class NodeData, class EdgeData>
Graph<NodeData, EdgeData>::Graph(const int& _size, const bool& _fromZero) {
    // we create _size new nodes
    nodes.resize(_size);
    for (int i = 0; i < _size; ++i)
        nodes[i] = new NodeType(i);

    // make it so it supports both possibilities(nodes from 0 to size - 1 or from 1 to size)
    fromZero = _fromZero;
}

template<class NodeData, class EdgeData>
void Graph<NodeData, EdgeData>::Index(std::initializer_list< std::pair<NodeType&, int> > fixed, const int &from) {
    // we go through each fixed node and keep track of it
    std::unordered_set<unsigned> fixedNodes;
    // we also keep track of the values we have so we can assign demn easily
    std::unordered_set<int> usedValues;
    for (auto &node: fixed) {
        // stupid, very very stupid
        if (node.second < from || node.second >= from + int(nodes.size()))
            continue;

        // why would you give a node twice?
        // anyway we ignore following attempts, it's your duty to not have multiple such actions
        if (fixedNodes.find(node.first.getKey()) != fixedNodes.end())
            continue;

        // and why would you reuse values?
        // again we ignore such attempts
        if (usedValues.find(node.second) != usedValues.end())
            continue;

        node.first.index = node.second;
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
        if (fixedNodes.find(node->getKey()) != fixedNodes.end())
            continue;

        node->index = unusedValues.back();
        unusedValues.pop_back();
    }

    // let's reorder the nodes by this index
    sort(nodes.begin(), nodes.end(), [](const NodeType* first, const NodeType* second) {
        return first->index < second->index;
    });
}

template<class NodeData, class EdgeData>
void Graph<NodeData, EdgeData>::Index(const int &from) {
    return Index({}, from);
}

}
#endif // INPUT_GENERATOR_GRAPH_HPP_
