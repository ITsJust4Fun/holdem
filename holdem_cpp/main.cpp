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

int dynamic(const std::vector<int>& line, bool isTurn) {
    if (cache.contains(line)) {
        return cache[line];
    }

    if (line.size() <= 0) {
        return 0;
    }

    auto begin = line.begin();
    auto end = --line.end();

    if (!isTurn) {
        if (*begin > *end) {
            return dynamic(std::vector<int>(++line.begin(), line.end()), !isTurn);
        } else {
            return dynamic(std::vector<int>(line.begin(), --line.end()), !isTurn);
        }
    }

    if (line.size() < 3) {
        auto select = *begin > *end ? begin : end;
        int value = *select;
        return value;
    }

    int leftSum = dynamic(std::vector<int>(++line.begin(), line.end()), !isTurn) + *begin;
    int rightSum = dynamic(std::vector<int>(line.begin(), --line.end()), !isTurn) + *end;

    cache.put(line, std::max(leftSum, rightSum));
    return std::max(leftSum, rightSum);
}

int dynamicSelection(std::vector<int>& line) {
    if (line.size() <= 1) {
        auto begin = line.begin();
        int value = *begin;
        line.erase(begin);
        return value;
    }

    auto begin = line.begin();
    auto end = --line.end();

    int leftSum = dynamic(std::vector<int>(++line.begin(), line.end()), false) + *begin;
    int rightSum = dynamic(std::vector<int>(line.begin(), --line.end()), false) + *end;

    auto select = leftSum > rightSum ? begin : end;
    int value = *select;
    line.erase(select);
    return value;
}

int main()
{
    int playerOneScore = 0;
    int playerTwoScore = 0;

    int cardsQuantity = 0;

    std::cout << "Enter cards quantity" << std::endl;
    std::cin >> cardsQuantity;

    std::vector<int> line = generateLine(cardsQuantity);
    printVector(line);

    while (line.size()) {
        int selection = greedy(line);
        playerOneScore += selection;
        std::cout << "player one selected: " << selection << std::endl;
        printVector(line);

        if (line.size()) {
            int selection = dynamicSelection(line);
            playerTwoScore += selection;
            std::cout << "player two selected: " << selection << std::endl;
            printVector(line);
        }
    }

    if (playerOneScore > playerTwoScore) {
        std::cout << "player one win with score: " << playerOneScore << std::endl;
        std::cout << "player two score: " << playerTwoScore << std::endl;
    } else if (playerOneScore == playerTwoScore) {
        std::cout << "player one score: " << playerOneScore << std::endl;
        std::cout << "player two score: " << playerTwoScore << std::endl;
    } else {
        std::cout << "player two win with score: " << playerTwoScore << std::endl;
        std::cout << "player one score: " << playerOneScore << std::endl;
    }
}
