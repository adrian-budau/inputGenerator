#include <iostream>

#include "../src/tree.hpp"

using namespace std;
using namespace inputGenerator;

ostream& operator<<(ostream& cout, const Graph<>& V) {
    cout << "Nodes: ";
    for (auto &node: V)
        cout << node.index() << " ";
    cout << endl;
    cout << "Edges: " << endl;

    cout << "  ";
    for (auto &node: V) {
        cout.width(3);
        cout.fill(' ');

        cout << node.index();
    }

    cout << endl;

    for (auto &node: V) {
        cout.width(3);
        cout.fill(' ');
        cout << node.index();

        for (auto &secondNode: V)
            if (node.hasEdge(secondNode))
                cout << " * ";
            else
                cout << "   ";
        cout << endl;
    }

    return cout;
}

int main() {
    cout << "Random tree of size 5: " << tree(5) << endl;

    cout << endl;

    cout << "Random tree of size 8 but with diameter at least 4: " << wideTree(8, 4) << endl;

    cout << endl;

    auto graph = wideTree(5, 4, Boolean::False);
    graph.Index({{graph[0], 1}, {graph[1], 3}}, 1);
    cout << "Random tree of size 5, diameter at least 4 and the two nodes at least at that distance are 1 and 3 (indexed from 1): " << graph << endl;
}

