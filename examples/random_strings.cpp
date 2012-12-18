#include <string>
#include <iostream>

#include "../inputGenerator.hpp"

using namespace std;
using namespace inputGenerator;

int main() {
    cout << "Very good password: " << randomString(8, letters + digits + special) << std::endl;
    cout << "A string with 0/1 containg half zeroes and half ones: " << randomString(10, "01", 5) << std::endl;
}
