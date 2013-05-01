#include <iostream>

#define INPUT_GENERATOR_DEBUG
#include "../src/undirected_graph.hpp"

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
    cout << "Random undirected graph with 5 nodes: " << undirectedGraph(5) << endl;

    cout << endl;

    cout << "Random connected undirected graph with 6 nodes and 10 edges: " << undirectedGraph(6, 10, Boolean::True) << endl;

    cout << endl;

    auto graph = undirectedGraph(4, Boolean::True);
    graph.mergeGraph(undirectedGraph(3, Boolean::True));
    graph.Index();
    cout << "Random undirected graph with 2 connected components, one with 4 nodes and the other with 3: " << graph << endl;
}

