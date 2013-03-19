#ifndef INPUT_GENERATOR_NUMBERS_HPP_
#define INPUT_GENERATOR_NUMBERS_HPP_

#include <random>
#include <limits>

#include "exception.hpp"
#include "generator.hpp"

namespace inputGenerator {

// Integer Types
template<class IntType>
IntType randomInt(const IntType &from = std::numeric_limits<IntType>::min(), const IntType &to = std::numeric_limits<IntType>::max()) {
#ifdef INPUT_GENERATOR_DEBUG
    if (from > to)
        throw Exception("randomInt expects `from` to be lower than or equal than `to`"); 
#endif
    std::uniform_int_distribution<IntType> distribution(from, to);
    return distribution(Generator::getGenerator());
}

unsigned randomUInt(const unsigned &from = std::numeric_limits<unsigned>::min(), const unsigned &to = std::numeric_limits<unsigned>::max()) {
#ifdef INPUT_GENERATOR_DEBUG
    if (from > to)
        throw Exception("randomUInt expects `from` to be lower than or equal than `to`");
#endif
    return randomInt<unsigned>(from, to);
}

int64_t randomInt64(const int64_t &from = std::numeric_limits<int64_t>::min(), const int64_t &to = std::numeric_limits<int64_t>::max()) {
#ifdef INPUT_GENERATOR_DEBUG
    if (from > to)
        throw Exception("randomInt64 expects `from` to be lower than or equal than `to`");
#endif
    return randomInt<int64_t>(from, to);
}

uint64_t randomUInt64(const uint64_t &from = std::numeric_limits<uint64_t>::min(), const uint64_t &to = std::numeric_limits<uint64_t>::max()) {
#ifdef INPUT_GENERATOR_DEBUG
    if (from > to)
        throw Exception("randomUInt64 expects `from` to be lower than or equal than `to`");
#endif
    return randomInt<uint64_t>(from, to);
}

// Real Types
template<class RealType>
RealType randomReal(const RealType &from = std::numeric_limits<RealType>::min(), const RealType &to = std::numeric_limits<RealType>::max()) {
#ifdef INPUT_GENERATOR_DEBUG
    if (from >= to)
        throw Exception("randomReal expects `from` to be strictly lower than `to`");
#endif
    std::uniform_real_distribution<RealType> distribution(from, to);
    return distribution(Generator::getGenerator());
}

float randomFloat(const float &from = std::numeric_limits<float>::min(), const float &to = std::numeric_limits<float>::max()) {
#ifdef INPUT_GENERATOR_DEBUG
    if (from >= to)
        throw Exception("randomFloat expects `from` to be strictly lower than `to`");
#endif
    return randomReal<float>(from, to);
}

double randomDouble(const double &from = std::numeric_limits<double>::min(), const double &to = std::numeric_limits<double>::max()) {
#ifdef INPUT_GENERATOR_DEBUG
    if (from >= to)
        throw Exception("randomDouble expects `from` to be strictly lower than `to`");
#endif
    return randomReal<double>(from, to);
}

long double randomDouble64(const long double &from = std::numeric_limits<long double>::min(), const long double &to = std::numeric_limits<long double>::max()) {
#ifdef INPUT_GENERATOR_DEBUG
    if (from >= to)
        throw Exception("randomDouble64 expects `from` to be strictly lower than `to`");
#endif
    return randomReal<long double>(from, to);
}
}

#endif // INPUT_GENERATOR_NUMBERS_HPP_
