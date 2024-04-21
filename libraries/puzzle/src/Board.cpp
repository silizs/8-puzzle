#include "puzzle/Board.hpp"

#include <algorithm>
#include <iostream>
#include <random>

Board::Board() noexcept : data(std::vector<std::vector<uint16_t>>(0)) {}

Board::Board(const std::vector<std::vector<uint16_t>>& input) noexcept : data{input} {}

Board::Board(const std::vector<std::vector<unsigned>>& input) noexcept {
    data.reserve(input.size());

    for (const auto& vec : input) {
        std::vector<uint16_t> row;
        row.reserve(vec.size());

        for (auto value : vec) {
            row.push_back(value);
        }
        data.push_back(row);
    }
}

Board Board::create_goal(const unsigned size) noexcept {
    std::vector<std::vector<unsigned>> table(size, std::vector<unsigned>(size, 0));
    for (unsigned i = 0; i < size; i++) {
        for (unsigned j = 0; j < size; j++) {
            table[i][j] = i * size + j + 1;
        }
    }
    table[size - 1][size - 1] = 0;
    return Board(table);
}

Board Board::create_random(const unsigned size) noexcept {
    std::vector<unsigned> numbers(size * size, 0);
    for (unsigned i = 0; i < size * size; i++) {
        numbers[i] = i;
    }
    std::shuffle(numbers.begin(), numbers.end(), std::mt19937(std::random_device()()));

    std::vector<std::vector<unsigned>> table(size, std::vector<unsigned>(size, 0));
    for (unsigned i = 0; i < size; i++) {
        for (unsigned j = 0; j < size; j++) {
            table[i][j] = numbers[i * size + j];
        }
    }
    return Board(table);
}

std::size_t Board::size() const noexcept {
    return data.size();
}

bool Board::is_goal() const noexcept {
    return hamming() == 0;
}

bool Board::is_solvable() const noexcept {
    std::size_t data_size = data.size();
    if (data_size == 0 || data_size == 1) {
        return true;
    }

    std::size_t size_n = data_size * data_size;
    std::vector<unsigned> numbers(size_n, 0);
    for (unsigned i = 0; i < data_size; i++) {
        for (unsigned j = 0; j < data_size; j++) {
            numbers[i * data_size + j] = data[i][j];
        }
    }
    std::size_t permutations = 0;
    for (unsigned i = 0; i < size_n; i++) {
        for (unsigned j = i + 1; j < size_n; j++) {
            if (numbers[i] > numbers[j]) {
                permutations++;
            }
        }
    }
    unsigned distance_null = 0;
    for (unsigned i = 0; i < data_size; i++) {
        for (unsigned j = 0; j < data_size; j++) {
            if (data[i][j] == 0) {
                distance_null += (data_size - 1 - i) + (data_size - 1 - j);
            }
        }
    }

    return (permutations + distance_null + data_size) % 2 == 1;
}

unsigned Board::hamming() const noexcept {
    std::size_t data_size = data.size();
    if (data_size == 0) {
        return 0;
    }

    unsigned counter = 0;
    for (unsigned i = 0; i < data_size; i++) {
        for (unsigned j = 0; j < data_size; j++) {
            if (data[i][j] != i * data_size + j + 1) {
                counter++;
            }
        }
    }
    if (data[data_size - 1][data_size - 1] == 0) {
        counter--;
    }
    return counter;
}

unsigned Board::distance(unsigned i, unsigned j, unsigned value) const noexcept {
    std::size_t data_size = data.size();
    int64_t i_goal        = (value - 1) / data_size;
    int64_t j_goal        = (value - 1) % data_size;
    return std::abs(i_goal - i) + std::abs(j_goal - j);
}

unsigned Board::manhattan() const noexcept {
    std::size_t data_size = data.size();
    unsigned counter      = 0;
    for (unsigned i = 0; i < data_size; i++) {
        for (unsigned j = 0; j < data_size; j++) {
            if (data[i][j] != 0) {
                counter += distance(i, j, data[i][j]);
            }
        }
    }
    return counter;
}

std::string Board::to_string() const noexcept {
    std::size_t data_size = data.size();
    std::string str;
    for (unsigned i = 0; i < data_size; i++) {
        for (unsigned j = 0; j < data_size; j++) {
            str += std::to_string(data[i][j]) + " ";
            if (j == data_size - 1 && i != data_size - 1) {
                str += "\n";
            }
        }
    }
    return str;
}

std::size_t Board::hash() const noexcept {
    return std::hash<std::string>()(to_string());
}

std::span<const uint16_t> Board::operator[](unsigned int index) const noexcept {
    return data[index];
}

std::vector<std::vector<uint16_t>> Board::get_board() const noexcept {
    return data;
}

bool operator==(const Board& left, const Board& right) noexcept {
    return left.data == right.data;
}

bool operator!=(const Board& left, const Board& right) noexcept {
    return not(left == right);
}

std::ostream& operator<<(std::ostream& out, const Board& board) noexcept {
    return out << board.to_string();
}
