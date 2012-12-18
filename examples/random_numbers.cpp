#include <iostream>

#include "../inputGenerator.hpp"

using namespace std;
using namespace inputGenerator;

int main() {
    cout << "Random number between 1 and 100: " << randomInt(1, 100) << std::endl;
    cout << "Random real number between 0 and 1: " << randomDouble(0, 1) << std::endl;
}
