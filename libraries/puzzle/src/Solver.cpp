#include "puzzle/Solver.hpp"

#include <iostream>
#include <map>
#include <memory>
#include <queue>
#include <set>
#include <unordered_map>

Solver::Solution::Solution() noexcept {
    m_moves.resize(0);
}

Solver::Solution::Solution(const std::vector<Board>& elements) noexcept : m_moves(elements) {}

std::size_t Solver::Solution::moves() const noexcept {
    if (m_moves.empty()) {
        return 0;
    } else {
        return m_moves.size() - 1;
    }
}

Solver::Solution::const_iterator Solver::Solution::begin() const noexcept {
    return m_moves.begin();
}

Solver::Solution::const_iterator Solver::Solution::end() const noexcept {
    return m_moves.end();
}

std::optional<std::vector<std::vector<uint16_t>>> adjacent_state(int ic, int jc, int i, int j,
                                                                 std::vector<std::vector<uint16_t>> current) noexcept {
    int size = static_cast<int>(current.size());
    if (i < 0 || i >= size || j < 0 || j >= size) {
        return {};
    } else {
        current[ic][jc] = current[i][j];
        current[i][j]   = 0;
        return {current};
    }
}

std::vector<std::vector<std::vector<uint16_t>>> adjacent_board_states(
    const std::vector<std::vector<uint16_t>>& current) noexcept {
    using board = std::vector<std::vector<uint16_t>>;
    std::vector<board> result;
    std::size_t size = current.size();

    uint16_t ic = 0;
    uint16_t jc = 0;
    for (uint16_t i = 0; i < size; i++) {
        for (uint16_t j = 0; j < size; j++) {
            if (current[i][j] == 0) {
                ic = i;
                jc = j;
            }
        }
    }
    int pairs[4][2] = {{-1, 0}, {1, 0}, {0, -1}, {0, 1}};
    for (auto& pair : pairs) {
        int i                    = static_cast<int>(ic) + pair[0];
        int j                    = static_cast<int>(jc) + pair[1];
        std::optional<board> one = adjacent_state(ic, jc, i, j, current);
        if (one) {
            result.emplace_back(*one);
        }
    }

    return result;
}

struct solution_step {
    solution_step(const Board& other, std::size_t cost, std::size_t depth, const std::shared_ptr<solution_step>& prev)
        : state(other), cost(cost), depth(depth), prev(prev) {}

    Board state;
    std::size_t cost;
    std::size_t depth;
    std::shared_ptr<solution_step> prev;
};

using solution_ptr = std::shared_ptr<solution_step>;

std::vector<Board> algorithm(const Board& start, const Board& goal) noexcept {
    auto cmp = [](const solution_ptr& left, const solution_ptr& right) {
        return (left->cost + left->depth) > (right->cost + right->depth);
    };

    std::priority_queue<solution_ptr, std::vector<solution_ptr>, decltype(cmp)> queue{cmp};
    std::unordered_map<std::size_t, solution_ptr> checked;

    auto initial_state    = std::make_shared<solution_step>(start, start.manhattan(), 0, nullptr);
    checked[start.hash()] = initial_state;
    queue.push(initial_state);

    while (not queue.empty()) {
        auto current = queue.top();
        if (current->state == goal) {
            break;
        }

        queue.pop();
        const auto one_step   = adjacent_board_states(current->state.get_board());
        const auto next_depth = current->depth + 1;

        for (auto& next : one_step) {
            auto next_board      = Board(next);
            const auto next_hash = next_board.hash();
            const auto next_cost = next_board.manhattan();

            if (not checked.contains(next_hash) || next_depth < checked[next_hash]->depth) {
                auto next_step     = std::make_shared<solution_step>(next_board, next_cost, next_depth, current);
                checked[next_hash] = next_step;
                queue.push(next_step);
            }
        }

        constexpr std::size_t max_queue_size = 50'000;
        if (queue.size() > max_queue_size * 2) {
            std::priority_queue<solution_ptr, std::vector<solution_ptr>, decltype(cmp)> new_queue{cmp};
            for (std::size_t i = 0; i < max_queue_size; i++) {
                new_queue.push(queue.top());
                queue.pop();
            }

            std::swap(queue, new_queue);
        }
    }

    std::vector<Board> result;
    auto current = queue.top();
    while (current) {
        result.insert(result.begin(), current->state);
        current = current->prev;
    }

    return result;
}

Solver::Solution Solver::solve(const Board& board) noexcept {
    if (board.size() == 0 || board.size() == 1) {
        std::vector<Board> result(1, board);
        return {result};
    }
    if (not board.is_solvable()) {
        std::vector<Board> result(0);
        return {result};
    }

    Board goal                = Board::create_goal(board.size());
    std::vector<Board> result = algorithm(board, goal);
    return {result};
}
