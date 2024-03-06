#include <cstddef>
#include <ranges>
#include <type_traits>

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
  using namespace supl::literals::size_t_literal;
  std::size_t assignment_count {};

  assignment_count += optimization_callback(*this);

  // intentionally after first optimization_callback call
  Sudoku backtrack_backup = *this;

  for ( const std::size_t row : std::views::iota(0_z, 9_z) ) {
    for ( const std::size_t col : std::views::iota(0_z, 9_z) ) { }
  }

  assert(this->is_solved());
  return assignment_count;
}
