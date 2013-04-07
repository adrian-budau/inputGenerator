#include <iostream>
#include <chrono>

#include <ctime>
#include <cstdlib>
#include <limits>
#include <climits>

#include "../inputGenerator.hpp"

using namespace std::chrono;

template<class duration_type>
double time_elapsed(const duration_type & d) {
    return duration_cast<duration<double>>(d).count();
}

template<class function>
double time_taken(function f) {
    auto start = system_clock::now();

    f();
    auto end = system_clock::now();

    return time_elapsed(end - start);
}

void testRandomInt() {
    std::cout << "Generating 100.000.000 numbers in range 0 MAX_INT: " << std::endl;

    int N = 100000000;

    int *A = new int[N];
    auto start = system_clock::now();

    for (int i = 0; i < N; ++i)
        A[i] = inputGenerator::randomInt(0, std::numeric_limits<int>::max());
    auto mid = system_clock::now();

    double first = time_elapsed(mid - start);

    srand(time(NULL));
    for (int i = 0; i < N; ++i)
        A[i] = rand() % INT_MAX;

    auto end = system_clock::now();

    double second = time_elapsed(end - mid);

    std::cout.setf(std::ios::fixed, std::ios::floatfield);
    std::cout.precision(6);
    std::cout << "Time for inputGenerator library: " << first << std::endl;
    std::cout << "Time for classic rand() modulo: " << second << std::endl;

    std::cout << std::endl;
}

void testGeneratingGraphs() {
    auto pause = []() {
        auto graph = inputGenerator::Graph<>(100000);
    };

    auto chainGraphTest = []() {
        auto graph = inputGenerator::chain(1000000);
    };

    auto pathGraphTest = []() {
        auto graph = inputGenerator::path(1000000);
    };

    auto treeGraphTest = []() {
        auto graph = inputGenerator::tree(1000000);
    };

    auto wideTreeGraphTest = []() {
        auto graph = inputGenerator::wideTree(1000000, 30000);
    };

    auto undirectedGraphTest = []() {
            auto graph = inputGenerator::undirectedGraph(100000, 500000);
    };

    auto undirectedConnectedGraphTest = []() {
        auto graph = inputGenerator::undirectedGraph(100000, 500000, inputGenerator::Boolean::True);
    };


    auto bipartiteGraphTest = []() {
        auto graph = inputGenerator::bipartite(500000, 500000, 1000000);
    };

    auto regularBipartiteGraphTest = [](const int &nodes, const int &degree) {
        auto graph = inputGenerator::regularBipartite(nodes, degree);
    };

    std::cout << "For graph generation there will be some time between tests to have consistency (FIXME: find out why I need to do this)!" << std::endl;

    pause();
    std::cout << "Generating a 1 million node chain takes " << time_taken(chainGraphTest) << std::endl;

    pause();
    std::cout << "Generating a 1 million node path takes " << time_taken(pathGraphTest) << std::endl;

    pause();
    std::cout << "Generating a 1 million node tree takes " << time_taken(treeGraphTest) << std::endl;

    pause();
    std::cout << "Generating a 1 million node wide tree (diameter at least 30.000) takes " << time_taken(wideTreeGraphTest) << std::endl;

    pause();
    std::cout << "Generating a 100.000 node graph with 500.000 edges takes " << time_taken(undirectedGraphTest) << std::endl;

    pause();
    std::cout << "Generating a 100.000 node connected graph with 500.000 edges takes " << time_taken(undirectedConnectedGraphTest) << std::endl;

    pause();
    std::cout << "Generating a 1.000.000 node (500.000 left and 500.000 right) bipartite graph with 1.000.000 edges takes " << time_taken(bipartiteGraphTest) << std::endl;

    pause();
    std::cout << "Generating a 400.000 node (200.000 left and 200.000 right) regular bipartite graph with degree 4 on each node takes " << time_taken(std::bind(regularBipartiteGraphTest, 200000, 4)) << std::endl;

    pause();
    std::cout << "Generating a 10.000 node (5.000 left and 5.000 right) regular bipartite graph with degree 100 on each node takes " << time_taken(std::bind(regularBipartiteGraphTest, 5000, 100)) << std::endl;

    std::cout << std::endl;
}

int main() {
    inputGenerator::reSeed();

    testRandomInt();

    testGeneratingGraphs();
}
