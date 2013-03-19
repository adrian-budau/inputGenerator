#include <iostream>
#include <iomanip>

#include "../src/numbers.hpp"

using namespace std;
using namespace inputGenerator;

int main() {
    cout << "Random integer between -100 and 100: " << randomInt(-100, 100) << endl;
    cout << "Random unsigned integer, we can omit parameters: " << randomUInt() << endl;
    cout << "Random 64-bit integer: " << randomInt64() << endl;
    cout << "Random 64-bit unsigned integer bigger than 2^63: " << randomUInt64(1ULL << 63) << endl;

    cout << endl;

    cout << "Random real number between 0 and 1: " << randomReal(0.0, 1.0) << endl;
    cout << "Random high precision real number: between -10^10 and 10^10" << fixed << setprecision(10) << randomDouble64(-1e10, 1e10) << endl;
}
