#ifndef INPUT_GENERATOR_ADVANCED_TREE_HPP_
#define INPUT_GENERATOR_ADVANCED_TREE_HPP_

#include <vector>
#include <cassert>
#include <functional>
#include <random>
#include <sstream>

#include "tree.hpp"
#include "generator.hpp"

namespace inputGenerator {

template<typename int_t>
static int random_int(int_t left, int_t right)
{
    std::uniform_int_distribution<int_t> distribution(left, right);
    return distribution(Generator::getGenerator());
}

template<typename T>
typename std::vector<T>::const_iterator
random_choice(const std::vector<T>& v)
{
    assert (!v.empty());
    return v.begin() + random_int(0, static_cast<int>(v.size() - 1));
}


template <typename node_value_t>
struct AdvancedTreeGenerator {

    using graph_t = Graph<node_value_t>;
    using node_t = typename graph_t::Node;

    static std::vector<node_t> leaves(const graph_t& V)
    {
        std::vector<node_t> ret;
        for (auto &node : V) {
            if (node.edges().size() == 1) {
                ret.push_back(node);
            }
        }

        if (V.size() == 1) {
            ret = {V[0]};
        }

        assert (!ret.empty());
        return ret;
    }

    struct Tree {
        graph_t m_graph;
        std::vector<node_t> m_leaves;

        void init(graph_t graph)
        {
            m_graph = graph;
            m_leaves = leaves(m_graph);
        }

        // For two graphs of the same shape, this should return isomorphic nodes.
        const std::vector<node_t>& nodes_of_interest() const
        {
            return m_leaves;
        }

        typename std::vector<node_t>::const_iterator
            random_node_of_interest() const
        {
            return random_choice(nodes_of_interest());
        }

        operator graph_t&()
        {
            return m_graph;
        }

        node_t& operator[] (int idx)
        {
            return m_graph[idx];
        }

        graph_t* operator-> ()
        {
            return &m_graph;
        }

        Tree& combine(Tree& v)
        {
            if (v->size() > m_graph.size()) {
                return v.combine(*this);
            }
            auto it_exile = random_node_of_interest();
            node_t exile = *it_exile;
            m_leaves.erase(it_exile);
            m_graph.expandNode(exile, graph_t(v), {*v.random_node_of_interest()});

            m_leaves.insert(m_leaves.end(), v.m_leaves.begin(), v.m_leaves.end());
            return *this;
        }
    };

    struct Chain : public Tree {
        int m_nnodes;

        static Chain make(int nnodes)
        {
            Chain c;
            c.init(chain<node_value_t>(nnodes, Boolean::False));
            c.m_nnodes = nnodes;
            return c;
        }
    };

    struct HTree : public Tree {
        using Tree::m_graph;
        Chain m_left;
        Chain m_right;

        static HTree make(int lchain, int lleft, int lright)
        {
            HTree h;

            graph_t lant = Chain::make(lchain);
            {
                Chain add = Chain::make(lleft);
                lant.expandNode(lant[0], add, {add[lleft / 2]});
                h.m_left = add;
            }

            {
                Chain add = Chain::make(lright);
                lant.expandNode(lant[lchain - 1], add, {add[lright / 2]});
                h.m_right = add;
            }
            h.init(lant);

            return h;
        }

        /// Merge the left side of *this* with the right side of oth.
        HTree& merge_left(HTree oth)
        {
            assert (m_left->size() == oth.m_right->size()); // Impossible to merge otherwise.

            std::vector<std::pair<node_t, node_t>> mapping;

            {
                for (auto it1 = m_left->begin(), it2 = oth.m_right->begin();
                        it1 != m_left->end() && it2 != oth.m_right->end();
                        ++it1, ++it2) {
                    mapping.push_back({*it1, *it2});
                }
            }

            m_graph.fuseGraph(oth.m_graph, mapping, Boolean::True);
            m_left = oth.m_left;
            return *this;
        }

        /// Merge the right side of *this* with the left side of oth.
        HTree& merge_right(HTree oth)
        {
            assert (m_right->size() == oth.m_left->size()); // Impossible to merge otherwise.

            std::vector<std::pair<node_t, node_t>> mapping;

            {
                for (auto it1 = m_right->begin(), it2 = oth.m_left->begin();
                        it1 != m_right->end() && it2 != oth.m_left->end();
                        ++it1, ++it2) {
                    mapping.push_back({*it1, *it2});
                }
            }

            m_graph.fuseGraph(oth.m_graph, mapping, Boolean::True);
            m_right = oth.m_right;
            return *this;
        }
        // Chain `nchains` H graphs together.
        static HTree make_nchains(int nchains, int lchain, int lwings)
        {
            HTree ret = HTree::make(lchain, lwings, lwings);
            for (int i = 0; i < nchains; ++i) {
                ret.merge_left(HTree::make(lchain, lwings, lwings));
            }
            return ret;
        }
    };

    struct RandomTree : public Tree {
        static RandomTree make(int nnodes)
        {
            RandomTree ret;
            ret.init(tree<node_value_t>(nnodes));
            return ret;
        }
    };

    struct WideRandomTree : public Tree {
        static WideRandomTree make(int nnodes, int mindiam)
        {
            WideRandomTree ret;
            ret.init(wideTree<node_value_t>(nnodes, mindiam));
            return ret;
        }
    };

    struct Star : public Tree {
        int m_mid;

        static Star make(int nnodes)
        {
            graph_t g(nnodes);

            for (int i = 1; i < nnodes; ++i) {
                addEdge(g[0], g[i]);
            }

            Star bc;
            bc.m_mid = 0;
            bc.init(g);

            return bc;
        }
    };
};

} // namespace inputGenerator
#endif // INPUT_GENERATOR_ADVANCED_TREE_HPP_
