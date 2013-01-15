#include <iostream>

// this should work only if the library was installed right
#include <inputGenerator.hpp>

using namespace std;
using namespace inputGenerator;

int main() {
    cout << "Random number between between -10 and 10: " << randomInt(-10, 10) << endl;
}
