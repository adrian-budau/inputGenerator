#ifndef INPUT_GENERATOR_VECTORS_HPP_
#define INPUT_GENERATOR_VECTORS_HPP_

#include <set>
#include <random>
#include <utility>
#include <unordered_set>

#include "exception.hpp"
#include "generator.hpp"
#include "numbers.hpp"

namespace inputGenerator {

template<class Container>
Container shuffle(const Container& data) {
    Container result = data;

    // we go through each position and chose a random character, using our generator so given seeds give the same shuffle
    for (int i = 0; i < int(result.size()); ++i) {
        int position = randomInt(i, result.size() - 1);
        swap(result[i], result[position]);
    }

    return result;
}

template<class IntType = int>
std::vector<IntType> randomSample(const int& numbers, const IntType& from = std::numeric_limits<IntType>::min(), const IntType& to = std::numeric_limits<IntType>::max()) {
    if (numbers < 0)
        throw Exception("randomSample expects `numbers` to be positive");

    if (from > to)
        throw Exception("randomSample expects `from` to be at most `to`");

    if (numbers == 0)
        return std::vector<IntType>();

    std::unordered_set<IntType> takenNumbers;
    std::vector<IntType> sample;
    sample.reserve(numbers); // for some extra speed

    for (int taken = 1; taken <= numbers; ++taken) {
        // long live "Programming Pearls"
        IntType toTake = randomIntType<IntType>(from, to - numbers + taken);
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
Container randomSubsequence(const Container& data, const int &newSize) {
    if (newSize < 0 || newSize > int(data.size()))
        throw Exception("randomSubsequence expects the `newSize` to be at least 0 and at most the container's size");

    if (data.size() == 0 || newSize == 0)
        return Container();

    // fast for getting exactly one character
    if (newSize == 1) {
        int position = randomInt(0, data.size() - 1);
        Container result(1, data[position]);
        return result;
    }

    // we use the randomSample algorithm if the requested size is smaller than data.size() / log
    int limitHashes = data.size(), log;
    for (log = 1; (1 << log) < int(data.size()); ++log);
    limitHashes /= log;

    if (newSize <= limitHashes) {
        auto markedPositions = randomSample(newSize, 0, int(data.size()) - 1);

        Container result;
        for (auto &position: markedPositions) {
            result.push_back(data[position]);
        }

        return result;
    } else {
        std::vector<int> positions(data.size());
        for (int i = 0; i < int(data.size()); ++i)
            positions[i] = i;

        positions = shuffle(positions);

        std::vector<bool> marked(data.size(), false);
        for (int i = 0; i < newSize; ++i)
            marked[positions[i]] = true;

        Container result;
        result.reserve(newSize);
        for (int i = 0; i < int(data.size()); ++i)
            if (marked[i])
                result.push_back(data[i]);
        return result;
    }
}

template<class Container>
Container randomSubsequence(const Container& data, const unsigned &newSize) {
    return randomSubsequence(data, int(newSize));
}

template<class Container>
Container randomSubsequence(const Container& data, const bool& canNull = true) {
    if (int(data.size()) == 0 && canNull == false)
        throw Exception("randomSubsequence on an empty container must have `canNull` true");

    Container result;
    result.reserve(data.size());
    do {
        result.clear();
        for (int i = 0; i < int(data.size()); ++i)
            if (randomInt(0, 1) == 1)
                result.push_back(data[i]);
    } while (result.size() == 0 && !canNull);

    return result;
}

template<class Container>
Container randomSubstring(const Container& data, const int &newSize) {
    if (newSize < 0 || newSize > int(data.size()))
        throw Exception("randomSubstring expects the `newSize` to be at least 0 and at most the container's size");


    if (data.size() == 0 || newSize == 0)
        return Container();

    // fast for getting exactly one character
    if (newSize == 1) {
        int position = randomInt(0, data.size() - 1);
        Container result(1, data[position]);
        return result;
    }

    // This is easy because each start position has the same probability of appearing
    int position = randomInt(0, data.size() - newSize);

    Container result;
    result.reserve(newSize);
    for (int i = position; i < position + newSize; ++i)
        result.push_back(data[i]);

    return result;
}

template<class Container>
Container randomSubstring(const Container& data, const int &least, const int &most) {
    if (least < 0 || least > int(data.size()))
        throw Exception("randomSubstring expects `least` to be at least 0 and at most the container's size");

    if (most < 0 || most > int(data.size()))
        throw Exception("randomSubstring expects `most` to be at least 0 and at most the container's size");

    if (least > most)
        throw Exception("randomSubstring expects `least` to be lower than or equal than `most`");


    // Very very hard, especially because we have to give different probabilities to each length
    std::vector<double> weight(data.size() + 1, 0.0);
    for (int i = least; i <= most; ++i)
        weight[i] = data.size() - i + 1;

    if (least == 0)
        weight[0] = 1;

    std::discrete_distribution<int>  distribution(weight.begin(), weight.end());
    int length = distribution(Generator::getGenerator());
    return randomSubstring(data, length);
}

template<class Container>
Container randomSubstring(const Container &data) {
    // it's obsolete to add exceptions here, they would just repeat with the ones above
    return randomSubstring(data, 0, data.size());
}

template<class Container>
auto randomElement(Container &data) -> typename std::iterator_traits<decltype(data.begin())>::reference {
    if (data.size() == 0)
        throw Exception("randomElement is expecting a non-empty container");
    return data[randomInt(0, data.size() - 1)];
}

template<class Container>
auto randomElement(const Container &data) -> typename std::iterator_traits<decltype(data.begin())>::reference {
    if (data.size() == 0)
        throw Exception("randomElement is expecting a non-empty container");
    return data[randomInt(0, data.size() - 1)];
}

template<class DataType>
DataType randomElement(std::initializer_list<DataType> data) {
    return randomElement(std::vector<DataType>(data));
}

template<class IntType = int>
std::vector<IntType> randomPartition(const IntType &number, const int &parts) {
    if (number < 1)
        throw Exception("randomPartition expects `number` to be strictly positive");

    if (parts < 1 || parts > number)
        throw Exception("randomParition expects `parts` to be between 1 and `number`");

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
    if (number < 1)
        throw Exception("randomPartition expects `number` to be strictly positive");

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
