#include <iostream>
#include <cassert>
#include <functional>
#include <random>
#include <sstream>

#include "../inputGenerator.hpp"

#include "benchmark.hpp"

using namespace std;
using namespace inputGenerator;

using node_value_t = int;

using gen = AdvancedTreeGenerator<node_value_t>;
using graph_t = gen::graph_t;
using Tree = gen::Tree;
using HTree = gen::HTree;
using Chain = gen::Chain;
using Star = gen::Star;
using RandomTree = gen::RandomTree;
using WideRandomTree = gen::WideRandomTree;

void init(int seed = 1337)
{
    Seed::logging = false;
    Seed::create(seed);
}

bool random_flip(double p)
{
    uniform_real_distribution<double>d (0, 1);
    return d(Generator::getGenerator()) < p;
}



string format(graph_t V)
{
    ostringstream cout;
    V.Index(1);
    cout << V.size() << "\n";
    int req = V.size() - 1;


    for (const auto& n : V) {
        cout << n.data() << " ";
    }
    cout.seekp(-1, ios_base::end);
    cout << "\n";

    for (const auto& e : V.edges())
    {
        cout << e.from().index() << " " << e.to().index() << "\n";
        req--;
    }

    assert (req == 0);

    return cout.str();
}

struct countfefete
{
    int n;

    countfefete(int n):
        n(n)
    { }

    Tree build(Tree acum)
    {
        while (true) {
            assert (acum->size() <= n); // Too many nodes already. Abort!

            if (acum->size() == n) {
                return acum;
            }

            int rem = n - acum->size();

            assert (rem > 0);

            // Base cases, because it's hard to calculate the exact node count
            // for the fancier types of trees.
            //
            // +1 for rem because when two trees are merged, one of the nodes is dropped.
            if (rem <= 5) {
                acum = RandomTree::make(rem + 1)
                    .combine(acum);
                continue;
            }
            if (rem <= 15) {
                acum = WideRandomTree::make(rem + 1, rem / 2 + 1)
                    .combine(acum);
                continue;
            }

            auto comb = [&] (Tree t)
            {
                if (t->size() <= rem + 1) {
                    acum = t.combine(acum);
                }
                assert (acum->size() <= n);
            };

            vector<function<void()>> transitions = {
                [&] () {
                    int lstar = random_int(2, 10);
                    comb(Star::make(lstar));
                },
                [&]() {
                    int lchain = random_int(2, 10);
                    comb(Chain::make(lchain));
                },
                [&]() {
                    comb(HTree::make_nchains(10, 3, 3));
                },
                [&]() {
                    comb(HTree::make_nchains(10, 5, 5));
                },
                [&]() {
                    comb(WideRandomTree::make(30, 10));
                },
                [&]() {
                    comb(RandomTree::make(30));
                },
                [&]() {
                    comb(RandomTree::make(10));
                },
                [&]() {
                    comb(WideRandomTree::make(50, 30));
                },
                [&]() {
                    comb(countfefete(sqrt(n)).build(Chain::make(2)));
                }
            };

            // Randomly choose a transition and then continue.
            (*random_choice(transitions))();
        }
    }

    static Tree generate(int seed, int N, int maxval, double p)
    {
        init(seed);
        int x = 1;
        auto next_value = [&] () -> int
        {
            // Give it an old value.
            if (random_flip(p)) {
                return random_int(1, x);
            }

            // Otherwise give it a new, unique id.
            // Note that this might be reused at a later point.
            return x++;
        };

        auto tree = countfefete(N).build(Chain::make(2));

        for (auto &node : tree.m_graph) {
            node.data() = next_value();
        }

        assert (tree->size() == N);

        set<int> values_that_appear;

        for (const auto & node : tree.m_graph) {
            values_that_appear.insert(node.data());
        }

        assert (maxval >= static_cast<int>(values_that_appear.size())); // Impossible to assign values otherwise.

        // We want to keep the relative ordering of the node values.
        // e.g. val_for_perm[x] <= val_for_perm[y] <-> x <= y.
        // This is because of the way the trees are structured, built on layers upon layers.
        // So the outermost leaves will have the lowest values, then their descendants and so on and so forth.
        map<int, int> val_for_perm;
        {

            set<int> incl;
            for (size_t i = 0; i < values_that_appear.size(); ++i) {
                int val;
                do{
                    val = random_int(1, maxval);
                } while (incl.count(val));
                incl.insert(val);
            }

            assert (incl.size() == values_that_appear.size());

            auto src = values_that_appear.begin();
            auto dst = incl.begin();

            while (src != values_that_appear.end() &&
                    dst != incl.end()) {
                val_for_perm[*src] = *dst;
                ++src;
                ++dst;
            }

            assert (src == values_that_appear.end());
            assert (dst == incl.end());
        }

        for (auto & node : tree.m_graph) {
            node.data() = val_for_perm[node.data()];
            assert (1 <= node.data() && node.data() <= maxval);
        }

        return tree;
    }
};

int main(int argc, char* argv[]) {
    if (argc != 5) {
        cerr << "Usage: " << argv[0] << " seed N maxval prob\n";
        cerr << "Generate a tree with N nodes, with values from 0..valmax in nodes.\n";
        cerr << "Each node has a probability of `prob` to be assigned an old value and (1 - prob) to be given a new one\n";
        exit(-1);
    }
    vector<string> arg(argv, argv + argc);
    int seed = stoi(arg[1]);
    int N = stoi(arg[2]);
    int maxval = stoi(arg[3]);
    double p = stod(arg[4]);

    std::cout
        << "Generating a countfefete input with " << N << " nodes takes " <<
        time_taken([&]()
        {
            countfefete::generate(seed, N, maxval, p);
        }) << std::endl;

    return 0;
}
