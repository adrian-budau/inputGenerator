#include <iostream>
#include <iomanip>

#include "../inputGenerator.hpp"

using namespace std;
using namespace inputGenerator;

int main() {
    cout << "Random number between 1 and 100: " << randomInt(1, 100) << endl;
    cout << "Random 64 bit number between -10^18 and 10^18: " << randomInt64(-1e+18, 1e+18) << endl;

    cout << endl;

    cout << "Random real number between 0 and 1: " << randomDouble(0, 1) << endl;
    cout << "Random high precision real number between 0 and 10^10: " << fixed << setprecision(10) << randomDouble64(0, 1e10) << endl;
}
