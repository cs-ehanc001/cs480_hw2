#include <array>
#include <bitset>
#include <cassert>
#include <cstddef>
#include <ranges>

#include <supl/utility.hpp>

#include "section_table.hpp"
#include "sudoku.hpp"

using supl::size_t_literal::operator""_z;

static auto is_populated(const Sudoku& sudoku) noexcept -> bool
{
  return std::ranges::find(sudoku.data(), '_') == end(sudoku.data());
}

// anonymous namespace to enforce internal linkage
namespace {
// function object implementing a single cell check
// to be used in a loop over a single entire constraint region
// (row, column, section)
//
// check_table must be reset externally between region checks
//
// refactored out of a lambda with by-reference captures
// which was refactored out of the body of an inner loop
struct check_iteration_handler {

  // NOLINTNEXTLINE(*member*)
  std::bitset<9>& check_table;
  decltype(std::declval<const Sudoku&>().mdview()) data_view;

  auto operator()(const char cell) -> bool
  {
    if ( cell == '_' ) {
      return true;
    }

    // a char which is a digit has numerical value:
    // (stoi(to_string(the_char)) + '0')
    // thus, (cell - '0') will map '2' to 2
    // this is off-by-one of the desired index into the check_table,
    // so, this line maps '2' to 1
    const auto raw_idx {(cell - '0') - 1};

    // sanity checks, guaranteed to hold if value of cell
    // is numeric or '_' ('_' handled by early exit)
    assert(raw_idx >= 0);
    assert(raw_idx <= 8);
    assert(supl::to_string(raw_idx + 1) == supl::to_string(cell));

    const std::size_t idx {static_cast<std::size_t>(raw_idx)};

    // duplicate checking: if '2' appears twice,
    // check_table[1] will already be set
    if ( check_table.test(idx) ) {
      return false;
    } else {
      check_table.set(idx);
    }

    return true;
  }
};
}  // namespace

// Only determines if constraints are intact
// A partially-filled board which does not violate constraints will return true
static auto is_legal_state(const Sudoku& sudoku) noexcept -> bool
{
  const auto data_view = sudoku.mdview();

  std::bitset<9> check_table {};

  // returns true if iteration is ok (indeterminate for complete board)
  // returns false if iteration is bad (definitively board is in illegal state)
  check_iteration_handler check_iteration {check_table, data_view};

  // check row-wise

  for ( const std::size_t row : std::views::iota(0_z, 9_z) ) {
    for ( const std::size_t col : std::views::iota(0_z, 9_z) ) {
      if ( ! check_iteration(data_view(row, col)) ) {
        return false;
      }
    }

    check_table.reset();
  }

  // rows are good

  // row check alone is adequate for a populated board
  // early exit
  if ( is_populated(sudoku) ) {
    return true;
  }

  // check column-wise
  for ( const std::size_t col : std::views::iota(0_z, 9_z) ) {
    for ( const std::size_t row : std::views::iota(0_z, 9_z) ) {
      if ( ! check_iteration(data_view(row, col)) ) {
        return false;
      }
    }

    check_table.reset();
  }

  // columns are good

  // check sections
  for ( const auto& section : section_table ) {
    for ( const auto& [row, col] : section ) {
      if ( const bool ok_so_far {check_iteration(data_view(row, col))};
           ! ok_so_far ) {
        return false;
      }
    }

    check_table.reset();
  }

  return true;
}

auto Sudoku::is_solved() const noexcept -> bool
{
  return is_populated(*this) && is_legal_state(*this);
}

auto Sudoku::is_valid() const noexcept -> bool
{
  return is_legal_state(*this);
}

auto Sudoku::is_legal_assignment(index_pair idxs,
                                 char value) const noexcept -> bool
{
  /* std::bitset<9> check_table {}; */

  /* for ( const auto& row : std::views::iota(0_z, 9_z) ) { */

  /* } */
}
