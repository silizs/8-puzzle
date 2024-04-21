#ifndef PUZZLE_BOARD_HPP
#define PUZZLE_BOARD_HPP

#include <random>
#include <string>
#include <vector>
#include <span>

class Board {
public:
    Board() noexcept;
    explicit Board(const std::vector<std::vector<uint16_t>>& input) noexcept;
    explicit Board(const std::vector<std::vector<unsigned>>& input) noexcept;

    static Board create_goal(unsigned size) noexcept;
    static Board create_random(unsigned size) noexcept;

    [[nodiscard]] std::size_t size() const noexcept;
    [[nodiscard]] bool is_goal() const noexcept;
    [[nodiscard]] bool is_solvable() const noexcept;

    [[nodiscard]] unsigned hamming() const noexcept;
    [[nodiscard]] unsigned manhattan() const noexcept;
    [[nodiscard]] std::string to_string() const noexcept;
    [[nodiscard]] std::size_t hash() const noexcept;

    std::span<const uint16_t> operator[](unsigned index) const noexcept;
    [[nodiscard]] std::vector<std::vector<uint16_t>> get_board() const noexcept;

    friend bool operator==(const Board& left, const Board& right) noexcept;
    friend bool operator!=(const Board& left, const Board& right) noexcept;

    friend std::ostream& operator<<(std::ostream& out, const Board& board) noexcept;

private:
    [[nodiscard]] unsigned distance(unsigned i, unsigned j, unsigned value) const noexcept;

    std::vector<std::vector<uint16_t>> data;
};

#endif  // PUZZLE_BOARD_HPP
