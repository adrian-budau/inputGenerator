#ifndef INPUT_GENERATOR_STRINGS_HPP_
#define INPUT_GENERATOR_STRINGS_HPP_

#include <string>
#include <vector>
#include <utility>
#include <algorithm>

#include "exception.hpp"
#include "numbers.hpp"
#include "vectors.hpp"

namespace inputGenerator {
std::string randomString(const size_t &size,
                         const std::string &dictionary,
                         const size_t &maxSame) {
    if (size == 0)
        return "";

#ifdef INPUT_GENERATOR_DEBUG
    if (static_cast<int>(dictionary.size()) == 0)
        throw Exception("randomString expects the `dictionary` size to be"
                        "bigger than 0 when asked to generate a non-null "
                        "string");

    if (static_cast<int64_t>(maxSame) *
            static_cast<int64_t>(dictionary.size()) <
                static_cast<int64_t>(size))
        throw Exception("randomString expects the `size` to be lower than "
                        "the product of the `dictionary` size and the maxi"
                        "mum number of identical characters");
#endif
    if (maxSame >= size) {
        std::string result;
        for (size_t i = 0; i < size; ++i)
            result.push_back(randomElement(dictionary));
        return result;
    }

    // the count
    std::vector<size_t> count(dictionary.size(), 0);
    std::string temporary_dictionary(dictionary);
    size_t leftCharacters = temporary_dictionary.size();

    std::string result;
    result.reserve(size);
    for (size_t i = 0; i < size; ++i) {
        size_t character = randomUInt(0, leftCharacters - 1);
        result.push_back(temporary_dictionary[character]);

        if (++count[character] == maxSame) {
            --leftCharacters;
            std::swap(count[character], count[leftCharacters]);

            std::swap(temporary_dictionary[character],
                      temporary_dictionary[leftCharacters]);
        }
    }

    return result;
}

std::string randomString(const int &size,
                         const std::string &dictionary) {
    // no exceptions thrown here because the function with the same name
    // will throw them
    return randomString(size, dictionary, size);
}

const std::string lowerLetters = "abcdefghijklmnopqrstuvwxyz";
const std::string upperLetters = "ABCDEFGHIJKLMNOPQRSTUVWXYZ";
const std::string letters = lowerLetters + upperLetters;

const std::string digits = "0123456789";
const std::string special = "`~!@#$%^&*()_+-=[]{}\\|;:'\",<.>/?";

}  // namespace inputGenerator

#endif  // INPUT_GENERATOR_STRINGS_HPP_
