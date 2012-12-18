#ifndef INPUT_GENERATOR_STRINGS_HPP_
#define INPUT_GENERATOR_STRINGS_HPP_

#include <string>
#include <vector>

#include "numbers.hpp"
#include "vectors.hpp"

namespace inputGenerator {
    std::string randomString(const int &size, const std::string &dictionary, const int &maxSame = 0) {
        if (maxSame > 0 && int64_t(maxSame) * int64_t(dictionary.size()) < size)
            return "";

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

    const std::string lowerLetters = "abcdefghijklmnopqrstuvwxyz";
    const std::string upperLetters = "ABCDEFGHIJKLMNOPQRSTUVWXYZ";
    const std::string letters = lowerLetters + upperLetters;

    const std::string digits = "0123456789";
    const std::string special = "`~!@#$%^&*()_+-=[]{}\\|;:'\",<.>/?";
}

#endif // INPUT_GENERATOR_STRINGS_HPP_
