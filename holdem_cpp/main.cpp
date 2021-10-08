#include <iostream>
#include <string>
#include <vector>
#include <random>

#include "HashMap/HashMap.h"

template<> struct std::hash<std::vector<int>> {
    std::size_t operator()(std::vector<int> const& vector) const noexcept {
        std::size_t seed = vector.size();

        for (auto& i : vector) {
            seed ^= i + 0x9e3779b9 + (seed << 6) + (seed >> 2);
        }

        return seed;
    }
};

HashMap<std::vector<int>, int> cache;

void printVector(const std::vector<int>& vector) {
    for (int i : vector) {
        std::cout << i << ' ';
    }

    std::cout << std::endl;
}

std::vector<int> generateLine(int size) {
    std::vector<int> cards = {0, 0};
    std::vector<int> line;

    for (int i = 2; i <= 14; i++) {
        cards.push_back(4);
    }

    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> distr(2, 14);

    while (line.size() < size) {
        int card = distr(gen);

        if (cards[card] > 0) {
            cards[card]--;
            line.push_back(card);
        }
    }

    return line;
}

int greedy(std::vector<int>& line) {
    auto begin = line.begin();
    auto end = --line.end();
    auto select = *begin > *end ? begin : end;
    int value = *select;
    line.erase(select);
    return value;
}

int main()
{
    std::vector<int> line = generateLine(10);
    printVector(line);
    greedy(line);
    printVector(line);

    cache.put(line, 15);
    std::cout << cache[line] << std::endl;
}
