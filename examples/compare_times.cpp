#include <iostream>
#include <ctime>
#include <cstdlib>
#include <limits>
#include <climits>
#include "../src/numbers.hpp"


int main() {
    std::cout << "Generating 100.000.000 numbers in range 0 MAX_INT: " << std::endl; 

    int N = 100000000;

    int *A = new int[N];
    clock_t start = clock();
    for (int i = 0; i < N; ++i)
        A[i] = inputGenerator::randomInt(0, std::numeric_limits<int>::max());
    double first = double(clock() - start) / CLOCKS_PER_SEC;

    start = clock();
    srand(time(NULL));
    for (int i = 0; i < N; ++i)
        A[i] = rand() % INT_MAX;
    double second = double(clock() - start) / CLOCKS_PER_SEC;

    std::cout.setf(std::ios::fixed, std::ios::floatfield);
    std::cout.precision(6);
    std::cout << "Time for inputGenerator library: " << first << std::endl;
    std::cout << "Time for classic rand() modulo: " << second << std::endl;
}
