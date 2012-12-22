#ifndef INPUT_GENERATOR_STRINGS_HPP_
#define INPUT_GENERATOR_STRINGS_HPP_

#include <string>
#include <vector>

#include "exception.hpp"
#include "numbers.hpp"
#include "vectors.hpp"

namespace inputGenerator {
std::string randomString(const int &size, const std::string &dictionary, const int &maxSame) {
    if (size == 0)
        return "";

    if (int(dictionary.size()) == 0)
        throw Exception("randomString expects the `dictionary` size to be bigger than 0 when asked to generate a non-null string");

    if (int64_t(maxSame) * int64_t(dictionary.size()) < size)
        throw Exception("randomString expects the `size` to be lower than the product of the `dictionary` size and the maximum number of identical characters");

    // the count
    std::vector<int> count(dictionary.size(), 0);

    std::string result;
    result.reserve(size);
    for (int i = 0; i < size; ++i) {
        int character;

        do {
            character = randomInt(0, dictionary.size() - 1);
        } while (maxSame > 0 && count[character] == maxSame);

        ++count[character];
        result.push_back(dictionary[character]);
    }

    return result;
}

std::string randomString(const int &size, const std::string &dictionary) {
    // no exceptions thrown here because the function with the same name will throw them
    return randomString(size, dictionary, dictionary.size());
}

const std::string lowerLetters = "abcdefghijklmnopqrstuvwxyz";
const std::string upperLetters = "ABCDEFGHIJKLMNOPQRSTUVWXYZ";
const std::string letters = lowerLetters + upperLetters;

const std::string digits = "0123456789";
const std::string special = "`~!@#$%^&*()_+-=[]{}\\|;:'\",<.>/?";

}

#endif // INPUT_GENERATOR_STRINGS_HPP_
