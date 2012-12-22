#include <iostream>

#include "../inputGenerator.hpp"

using namespace std;
using namespace inputGenerator;

ostream& operator<<(ostream& cout, const Graph<>& V) {
    cout << "Nodes: ";
    for (auto &node: V)
        cout << node.index() << " ";
    cout << endl;
    cout << "Edges: " << endl;

    cout << "  ";
    for (int i = 0; i < V.size(); ++i) {
        cout.width(3);
        cout.fill(' ');

        cout << V[i].index();
    }

    cout << endl;

    for (int i = 0; i < V.size(); ++i) {
        cout.width(3);
        cout.fill(' ');
        cout << V[i].index();

        for (int j = 0; j < V.size(); ++j)
            if (V[i].edgesTo(V[j]))
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

