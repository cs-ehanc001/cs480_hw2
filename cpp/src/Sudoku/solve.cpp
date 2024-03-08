#include <algorithm>
#include <array>
#include <cstddef>
#include <cstdlib>
#include <deque>
#include <iterator>
#include <ranges>
#include <stack>
#include <type_traits>
#include <vector>

#include <supl/predicates.hpp>
#include <supl/utility.hpp>

#include "sudoku.hpp"

auto null_optimization(Sudoku&) -> std::size_t
{
  return 0;
}

auto trivial_move_optimization(Sudoku& sudoku) -> std::size_t
{
  std::size_t trivial_assignment_count {};
  while ( sudoku.apply_trivial_move() ) {
    ++trivial_assignment_count;
  }

  return trivial_assignment_count;
}

auto Sudoku::solve(
  std::add_pointer_t<std::size_t(Sudoku&)> optimization_callback) noexcept
  -> std::size_t
{
  std::size_t assignment_count {};

  // apply any trivial moves available
  assignment_count += optimization_callback(*this);

  const std::array<variable_domain, 81> domains {this->query_domains()};

  if ( ! this->is_solved() ) {
    std::cerr << "No solution found!\n";
    std::exit(EXIT_FAILURE);
  }

  return assignment_count;
}
