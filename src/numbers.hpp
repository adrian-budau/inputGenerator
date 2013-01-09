#ifndef INPUT_GENERATOR_NUMBERS_HPP_
#define INPUT_GENERATOR_NUMBERS_HPP_

#include <random>

#include "exception.hpp"
#include "generator.hpp"

namespace inputGenerator {

// Integer Types
template<class IntType>
IntType randomIntType(const IntType &from, const IntType &to) {
    if (from > to)
        throw Exception("randomIntType expects `from` to be lower than or equal than `to`"); 

    std::uniform_int_distribution<IntType> distribution(from, to);
    return distribution(Generator::getGenerator());
}

int randomInt(const int &from, const int &to) {
    if (from > to)
        throw Exception("randomInt expects `from` to be lower than or equal than `to`");

    return randomIntType<int>(from, to);
}

unsigned randomUInt(const unsigned &from, const unsigned &to) {
    if (from > to)
        throw Exception("randomUInt expects `from` to be lower than or equal than `to`");

    return randomIntType<unsigned>(from, to);
}

int64_t randomInt64(const int64_t &from, const int64_t &to) {
    if (from > to)
        throw Exception("randomInt64 expects `from` to be lower than or equal than `to`");

    return randomIntType<int64_t>(from, to);
}

uint64_t randomUInt64(const uint64_t &from, const uint64_t &to) {
    if (from > to)
        throw Exception("randomUInt64 expects `from` to be lower than or equal than `to`");

    return randomIntType<uint64_t>(from, to);
}

// Real Types
template<class RealType>
RealType randomRealType(const RealType &from, const RealType &to) {
    if (from >= to)
        throw Exception("randomRealType expects `from` to be strictly lower than `to`");

    std::uniform_real_distribution<RealType> distribution(from, to);
    return distribution(Generator::getGenerator());
}

float randomFloat(const float &from, const float &to) {
    if (from >= to)
        throw Exception("randomFloat expects `from` to be strictly lower than `to`");

    return randomRealType<float>(from, to);
}

double randomDouble(const double &from, const double &to) {
    if (from >= to)
        throw Exception("randomDouble expects `from` to be strictly lower than `to`");

    return randomRealType<double>(from, to);
}

long double randomDouble64(const long double &from, const long double &to) {
    if (from >= to)
        throw Exception("randomDouble64 expects `from` to be strictly lower than `to`");

    return randomRealType<long double>(from, to);
}



}

#endif // INPUT_GENERATOR_NUMBERS_HPP_
