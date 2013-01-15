#include <vector>
#include <string>
#include <iostream>

#include "../inputGenerator.hpp"

using namespace std;
using namespace inputGenerator;

template<class IntType>
ostream& operator<<(ostream& stream, const vector<IntType> &data) {
    stream << "{";
    for (int i = 0; i < int(data.size()) - 1; ++i)
        stream << data[i] << ", ";
    if (data.size() > 0)
        stream << data.back();
    stream << "}";
    return stream;
}

int main() {
    vector<int> array = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};

    cout << "The array: " << array << endl;
    cout << "Random element of the array: " << randomElement(array) << endl;
    cout << "Random shuffle of the array: " << shuffle(array) << endl;
    cout << "Random subsequence of the array: " << randomSubsequence(array) << endl;
    cout << "Random substring of the array: " << randomSubstring(array) << endl;

    cout << endl;

    string S = "The quick brown fox jumps over the lazy dog";
    cout << "The string: " << S << endl;
    cout << "Random shuffle of the string: " << shuffle(S) << endl;
    cout << "Random subsequence of the string: " << randomSubsequence(S) << endl;
    cout << "Random substring of the string: " << randomSubstring(S) << endl;

    cout << endl;

    cout << "5 Random 64 bits numbers: " << randomSample<int64_t>(5, -1e18, 1e18) << endl;
    cout << "Random partition of the number 10: " << randomPartition(10) << endl;
    cout << "Random partition of the number 10 in 3 parts: " << randomPartition(10, 3) << endl;
    cout << "Random partition of the number 10^18 in 4 parts (complexity is linear on the number of parts): " << randomPartition(int64_t(1e18), 4) << endl;
}
