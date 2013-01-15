#include <iostream>

#include "../inputGenerator.hpp"

using namespace std;
using namespace inputGenerator;

ostream& operator<<(ostream& cout, const Graph<>& V) {
    cout << "Nodes: ";
    for (auto &node: V)
        cout << node.index() << "(" << node.getKey() << ") ";
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
    cout << "Random tree of size 5: " << tree(5) << "\n";
}

