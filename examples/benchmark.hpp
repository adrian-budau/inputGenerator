#include <chrono>

using namespace std::chrono;

template<typename duration_type>
double time_elapsed(const duration_type & d) {
    return duration_cast<duration<double>>(d).count();
}

template<typename function>
double time_taken(function f) {
    auto start = system_clock::now();

    f();
    auto end = system_clock::now();

    return time_elapsed(end - start);
}
