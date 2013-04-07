#ifndef INPUT_GENERATOR_VECTORS_HPP_
#define INPUT_GENERATOR_VECTORS_HPP_

#include <set>
#include <random>
#include <utility>
#include <unordered_set>
#include <algorithm>
#include "boolean.hpp"
#include "exception.hpp"
#include "generator.hpp"
#include "numbers.hpp"

namespace inputGenerator {

template<class RandomAccessIterator>
void randomShuffle(RandomAccessIterator first, RandomAccessIterator last) {
    for (RandomAccessIterator it = first + 1; it != last; ++it)
        std::iter_swap(it, first + randomInt<std::ptrdiff_t>(0, last - first - 1));
}

template<class Container>
Container shuffle(const Container& data) {
    Container result(data);
    randomShuffle(result.begin(), result.end());
    return result;
}

template<class Container>
typename std::enable_if<
    !std::is_reference<Container>::value,
    Container>::type shuffle(Container&& data) {
    Container result(std::move(data));

    // we go through each position and chose a random character, using our generator so given seeds give the same shuffle
    randomShuffle(result.begin(), result.end());
    return result;
}

template<class Iterator>
typename std::iterator_traits<Iterator>::reference randomElement(Iterator first, Iterator last) {
#ifdef INPUT_GENERATOR_DEBUG
    if (first == last)
        throw Exception("randomElement expects `first` to be different than `last`");
#endif

    std::advance(first, randomInt<std::ptrdiff_t>(0, std::distance(first, last) - 1));
    return *first;
}

template<class Container>
auto randomElement(const Container &data) -> typename std::iterator_traits<decltype(data.begin())>::reference {
#ifdef INPUT_GENERATOR_DEBUG
    if (data.size() == 0)
        throw Exception("randomElement is expecting a non-empty container");
#endif
    return randomElement(data.begin(), data.end());
}

template<class DataType>
DataType randomElement(std::initializer_list<DataType> data) {
#ifdef INPUT_GENERATOR_DEBUG
    if (data.size() == 0)
        throw Exception("randomElement is expecting a non-empty container");
#endif
    return randomElement(data.begin(), data.end());
}

template<class IntType = int>
std::vector<IntType> randomSample(const size_t& numbers, const IntType& from = std::numeric_limits<IntType>::min(), const IntType& to = std::numeric_limits<IntType>::max()) {
#ifdef INPUT_GENERATOR_DEBUG
    if (from > to)
        throw Exception("randomSample expects `from` to be at most `to`");

    if (int64_t(numbers) > int64_t(to) - int64_t(from) + 1)
        throw Exception("randomSample expects `numbers` to be at most the number of elements in the interval");
#endif

    if (numbers == 0)
        return std::vector<IntType>();

    if (to - from + 1 == IntType(numbers)) {
        std::vector<IntType> sample(numbers);
        for (size_t i = 0; i < numbers; ++i)
            sample[i] = from + i;
        return sample;
    }

    std::unordered_set<IntType> takenNumbers;
    std::vector<IntType> sample;
    sample.reserve(numbers); // for some extra speed

    for (size_t taken = 0; taken < numbers; ++taken) {
        // long live "Programming Pearls"
        IntType toTake = randomInt<IntType>(from, to - numbers + taken);
        if (takenNumbers.find(toTake) == takenNumbers.end()) {
            sample.push_back(toTake);
            takenNumbers.insert(toTake);
        }
        else {
            sample.push_back(to - numbers + taken);
            takenNumbers.insert(to - numbers + taken);
        }
    }

    sort(sample.begin(), sample.end());
    return sample;
}

template<class Container>
Container randomSubsequence(const Container& data, const size_t &newSize) {
#ifdef INPUT_GENERATOR_DEBUG
    if (newSize > data.size())
        throw Exception("randomSubsequence expects the `newSize` to be at most the container's size");
#endif

    if (data.size() == 0 || newSize == 0)
        return Container();

    // fast for getting exactly one character
    if (newSize == 1) {
        Container result(1, randomElement(data));
        return result;
    }

    // we use the randomSample algorithm if the requested size is smaller than data.size() / log
    size_t limitHashes = data.size(), log;
    for (log = 1; (1u << log) < data.size(); ++log);
    limitHashes /= log;

    if (newSize <= limitHashes) {
        auto markedPositions = randomSample<size_t>(newSize, 0u, data.size() - 1);

        Container result;
        for (auto &position: markedPositions) {
            result.push_back(data[position]);
        }

        return result;
    } else {
        std::vector<size_t> positions(data.size());
        for (size_t i = 0; i < data.size(); ++i)
            positions[i] = i;

        positions = shuffle(positions);

        std::vector<bool> marked(data.size(), false);
        for (size_t i = 0; i < newSize; ++i)
            marked[positions[i]] = true;

        Container result;
        result.reserve(newSize);
        for (size_t i = 0; i < data.size(); ++i)
            if (marked[i])
                result.push_back(data[i]);
        return result;
    }
}

template<class Container>
Container randomSubsequence(const Container& data, Boolean::Object canNull = Boolean::True) {
#ifdef INPUT_GENERATOR_DEBUG
    if (data.size() == 0 && !canNull)
        throw Exception("randomSubsequence on an empty container must have `canNull` true");
#endif
    Container result;
    result.reserve(data.size());
    do {
        result.clear();
        for (size_t i = 0; i < data.size(); ++i)
            if (randomElement({true, false}))
                result.push_back(data[i]);
    } while (result.size() == 0 && !canNull);

    return result;
}

template<class Container>
Container randomSubstring(const Container& data, const size_t &newSize) {
#ifdef INPUT_GENERATOR_DEBUG
    if (newSize < 0 || newSize > data.size())
        throw Exception("randomSubstring expects the `newSize` to be at most the container's size");
#endif

    if (data.size() == 0 || newSize == 0)
        return Container();

    // fast for getting exactly one character
    if (newSize == 1) {
        Container result(1, randomElement(data));
        return result;
    }

    // This is easy because each start position has the same probability of appearing
    size_t position = randomUInt(0, data.size() - newSize);

    Container result;
    result.reserve(newSize);
    for (size_t i = position; i < position + newSize; ++i)
        result.push_back(data[i]);

    return result;
}

template<class Container>
Container randomSubstring(const Container& data, const size_t &least, const size_t &most) {
#ifdef INPUT_GENERATOR_DEBUG
    if (least > data.size())
        throw Exception("randomSubstring expects `least` to be at most the container's size");

    if (most > data.size())
        throw Exception("randomSubstring expects `most` to be at at most the container's size");

    if (least > most)
        throw Exception("randomSubstring expects `least` to be lower than or equal than `most`");
#endif

    // Very very hard, especially because we have to give different probabilities to each length
    std::vector<double> weight(data.size() + 1, 0.0);
    for (size_t i = least; i <= most; ++i)
        weight[i] = data.size() - i + 1;

    if (least == 0)
        weight[0] = 1;

    std::discrete_distribution<size_t>  distribution(weight.begin(), weight.end());
    size_t length = distribution(Generator::getGenerator());
    return randomSubstring(data, length);
}

template<class Container>
Container randomSubstring(const Container &data) {
    // it's obsolete to add exceptions here, they would just repeat with the ones above
    return randomSubstring(data, 0, data.size());
}

template<class IntType = int>
std::vector<IntType> randomPartition(const IntType &number, const size_t &parts) {
#ifdef INPUT_GENERATOR_DEBUG
    if (number < 1)
        throw Exception("randomPartition expects `number` to be strictly positive");

    if (parts < 1 || IntType(parts) > number)
        throw Exception("randomParition expects `parts` to be between 1 and `number`");
#endif
    auto splits = randomSample<IntType>(parts - 1, 2, number);

    std::vector<IntType> result;
    result.reserve(parts);

    IntType lastSplit = 1;
    for (auto &split: splits) {
        result.push_back(split - lastSplit);
        lastSplit = split;
    }

    result.push_back(number - lastSplit + 1);
    return result;
}

std::vector<int> randomPartition(const int &number) {
#ifdef INPUT_GENERATOR_DEBUG
    if (number < 1)
        throw Exception("randomPartition expects `number` to be strictly positive");
#endif
    std::vector<int> numbers;
    int lastSplit = 0;
    for (int i = 1; i < number; ++i) {
        if (randomElement({false, true}) == true) {
            numbers.push_back(i - lastSplit);
            lastSplit = i;
        }
    }

    numbers.push_back(number - lastSplit);
    return numbers;
}

}
#endif // INPUT_GENERATOR_VECTORS_HPP_
