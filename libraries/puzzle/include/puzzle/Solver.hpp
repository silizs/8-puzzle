#ifndef PUZZLE_SOLVER_HPP
#define PUZZLE_SOLVER_HPP

#include <optional>

#include "puzzle/Board.hpp"

class Solver {
    class Solution {
    public:
        Solution() noexcept;
        Solution(const std::vector<Board>& elements) noexcept;

        [[nodiscard]] std::size_t moves() const noexcept;

        using const_iterator = std::vector<Board>::const_iterator;
        [[nodiscard]] const_iterator begin() const noexcept;
        [[nodiscard]] const_iterator end() const noexcept;

    private:
        std::vector<Board> m_moves;
    };

public:
    static Solution solve(const Board& board) noexcept;
};

std::optional<std::vector<std::vector<uint16_t>>> adjacent_state(int ic, int jc, int i, int j,
                                                                 std::vector<std::vector<uint16_t>> current) noexcept;

std::vector<std::vector<std::vector<uint16_t>>> adjacent_board_states(
    const std::vector<std::vector<uint16_t>>& current_board) noexcept;

std::vector<Board> algorithm(const Board& start, const Board& goal) noexcept;

#endif  // PUZZLE_SOLVER_HPP