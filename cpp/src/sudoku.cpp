#include <array>

#include "sudoku.hpp"

static auto is_populated(const Sudoku& sudoku) -> bool
{
  return std::ranges::find(sudoku.data(), '_') == end(sudoku.data());
}

static auto is_legal_solution(const Sudoku& sudoku) -> bool
{

  // check rows
}

auto Sudoku::is_solved() const -> bool
{
  /* return is_populated(*this) && is_legal_solution(*this); */
}

