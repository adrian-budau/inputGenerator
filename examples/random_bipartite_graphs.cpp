#include <iostream>

#include "../inputGenerator.hpp"

using namespace std;
using namespace inputGenerator;

ostream& operator<<(ostream& cout, const pair<Graph<>, Graph<>> graph) {
    cout << endl;
    auto left = graph.first;
    auto right = graph.second;

    cout << "Nodes left: ";
    for (auto &node: left)
        cout << node.index() << "(" << node.getKey() << ") ";
    cout << endl;

    cout << "Nodes right: ";
    for (auto &node: right)
        cout << node.index() << "(" << node.getKey() << ") ";
    cout << endl;

    cout << endl;

    cout << "Edges: " << endl;

    cout << "  ";
    for (auto &node: right) {
        cout.width(3);
        cout.fill(' ');

        cout << node.index();
    }

    cout << endl;

    for (auto &node: left) {
        cout.width(3);
        cout.fill(' ');
        cout << node.index();

        for (auto &secondNode: right) {
            int many = node.edgesTo(secondNode).size();
            if (many > 1) {
                if (many > 9)
                    cout << " " << many;
                else
                    cout << " " << many << " ";
            }
            else if (many)
                cout << " * ";
            else
                cout << "   ";
        }
        cout << endl;
    }

    return cout;
}

int main() {
    cout << "Random bipartite graph with 5 nodes left, 6 nodes right, arbitrary number of edges and multiple edges through the same node " << bipartite(5, 6, true) << endl;

    cout << "Random bipartite graph with 10 nodes: " << bipartite(10) << endl;

    cout << "A 2-regular bipartite graph with 6 nodes: " << regularBipartite(6, 2) << endl;
}
