#include <iostream>

#include "../inputGenerator.hpp"

using namespace std;
using namespace inputGenerator;

ostream& operator<<(ostream& cout, const Graph<>& V) {
    cout << "Nodes: ";
    for (auto &node: V)
        cout << node.index << " ";
    cout << endl;
    cout << "Edges: " << endl;

    cout << "  ";
    for (int i = 0; i < V.size(); ++i) {
        cout.width(3);
        cout.fill(' ');

        cout << V[i].index;
    }

    cout << endl;

    for (int i = 0; i < V.size(); ++i) {
        cout.width(3);
        cout.fill(' ');
        cout << V[i].index;

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
    // Setting the seed before-hand
    // This should always generate the same thing
    Seed::create(1234567890);
    cout << "Random Chain of length 5 (undirected graph): " << endl;
    cout << chain(5) << endl;

    cout << "Random Path of length 5 (directed graph): " << endl;
    cout << path(5) << endl;
}
