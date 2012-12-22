#include <vector>
#include <string>
#include <iostream>

#include "../inputGenerator.hpp"

using namespace std;
using namespace inputGenerator;

ostream& operator<<(ostream& stream, const vector<int> &data) {
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

    cout << "The array: " << array << std::endl;
    cout << "Random element of the array: " << element(array) << std::endl;
    cout << "Random shuffle of the array: " << shuffle(array) << std::endl;
    cout << "Random subsequence of the array: " << subsequence(array) << std::endl;
    cout << "Random substring of the array: " << substring(array) << std::endl;

    cout << std::endl;

    string S = "The quick brown fox jumps over the lazy dog";
    cout << "The string: " << S << std::endl;
    cout << "Random shuffle of the string: " << shuffle(S) << std::endl;
    cout << "Random subsequence of the string: " << subsequence(S) << std::endl;
    cout << "Random substring of the string: " << substring(S) << std::endl;
}
