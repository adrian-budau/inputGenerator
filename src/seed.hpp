#ifndef INPUT_GENERATOR_SEED_HPP_
#define INPUT_GENERATOR_SEED_HPP_

#include <random>
#include <iostream>  // NOLINT(readability/streams)

namespace inputGenerator {

// Singleton
class Seed {
  public:
    typedef std::random_device::result_type data_type;

    static void create();

    static void create(const data_type &);

    static data_type getSeed();

    static void log();

    // logging, useful to recreate tests
    // set it to true and we can see the seed generated
    static bool logging;

  private:
    // Private Constructor
    Seed();

    // Private Copy-Constructor
    void operator=(const Seed&);

    // the seed, kept in case of something
    static data_type seed;

    // wheather we created the seed or not
    // Useful for writing less code
    // if we need the seed and did not create it we create it now
    static bool created;
};

// set it to false if you really need it shut off
bool Seed::logging = true;

Seed::data_type Seed::seed;

bool Seed::created = false;

void Seed::create() {
    created = true;

    std::random_device generator;
    seed = generator();

    log();
}

void Seed::create(const data_type &_seed) {
    created = true;

    seed = _seed;

    log();
}

Seed::data_type Seed::getSeed() {
    if (!created) {
        create();
    }

    return seed;
}

void Seed::log() {
    if (logging) {
        std::clog << "Seed generated: " << seed << std::endl;
    }
}

}  // namespace inputGenerator

#endif  // INPUT_GENERATOR_SEED_HPP_
