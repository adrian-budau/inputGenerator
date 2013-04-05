#include <iostream>

// this should work only if the library was installed right
#define INPUT_GENERATOR_DEBUG
#include <inputGenerator.hpp>

using namespace std;
using namespace inputGenerator;

int main() {
    cout << "Random number between between -10 and 10: " << randomInt(-10, 10) << endl;
}
