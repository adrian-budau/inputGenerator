#ifndef INPUT_GENERATOR_GENERATOR_HPP_
#define INPUT_GENERATOR_GENERATOR_HPP_

#include <random>

#include "seed.hpp"

namespace inputGenerator {

// Singleton
class Generator {
  public:
    static void create();

    static std::mt19937& getGenerator();

  private:
    // static constructor
    Generator();

    // static copy-constructor
    void operator=(const Generator&);

    // Whether or not the generator has been created
    static bool created;

    // the generator object itself
    static std::mt19937 generator;
};

bool Generator::created = false;

std::mt19937 Generator::generator;

void Generator::create() {
    created = true;
    generator = std::mt19937(Seed::getSeed());
}

std::mt19937& Generator::getGenerator() {
    if (!created) {
        create();
    }

    return generator;
}

void reSeed() {
    Seed::create();
    Generator::create();
}

void reSeed(Seed::data_type seed) {
    Seed::create(seed);
    Generator::create();
}

}  // namespace inputGenerator

#endif  // INPUT_GENERATOR_GENERATOR_HPP_
