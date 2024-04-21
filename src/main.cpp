#include <iostream>

#include "puzzle/Solver.hpp"

int main() {
    std::cout << sizeof(unsigned) << " " << sizeof(uint16_t) << '\n';
    const auto board    = Board::create_random(4);
    const auto solution = Solver::solve(board);
    std::cout << solution.moves() << std::endl;
    for (const auto& move : solution) {
        std::cout << move << std::endl;
    }
}