#ifndef INPUT_GENERATOR_VECTORS_HPP_
#define INPUT_GENERATOR_VECTORS_HPP_

#include <set>
#include <random>

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

template<class Container>
Container subsequence(const Container& data, const int &newSize) {
    if (data.size() == 0 || newSize == 0)
        return Container();

    // fast for getting exactly one character
    if (newSize == 1) {
        int position = randomInt(0, data.size() - 1);
        Container result(1, data[position]);
        return result;
    }

    // we use hashes if newSize <= data.size() / (log * 2)
    int limitHashes = data.size(), log;
    for (log = 1; (1 << log) < int(data.size()); ++log);
    limitHashes /= log;

    if (newSize <= limitHashes) {
        // we randomly pick characters, probabilisticly the expected number of moves to gain different positions
        // we need 2 * newSize operations, by the time we reach 3 * newSize or 4 * newSize the probability will grow very much
        std::set<int> markedPositions;
        int readyCharacters = 0;

        while (readyCharacters < newSize) {
            int position = randomInt(0, data.size() - 1);
            if (markedPositions.find(position) == markedPositions.end()) {
                markedPositions.insert(position);
                ++readyCharacters;
            }
        }

        Container result;
        result.reserve(newSize);
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
Container subsequence(const Container& data, const bool& canNull = true) {
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
Container substring(const Container& data, const int &newSize) {
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
Container substring(const Container& data, const bool& canNull = true) {
    // Very very hard, especially because we have to give different probabilities to each length
    std::vector<double> weight(data.size() + 1, 0.0);
    for (int i = 1; i <= int(data.size()); ++i)
        weight[i] = data.size() - i + 1;

    if (canNull)
        weight[0] = 1.0;

    std::discrete_distribution<int>  distribution(weight.begin(), weight.end());
    int length = distribution(Generator::getGenerator());
    return substring(data, length);
}

}

#endif // INPUT_GENERATOR_VECTORS_HPP_
