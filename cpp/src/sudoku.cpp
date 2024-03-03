#include <array>
#include <cassert>
#include <ranges>

#include <supl/utility.hpp>

#include "sudoku.hpp"

using supl::size_t_literal::operator""_z;

static auto is_populated(const Sudoku& sudoku) noexcept -> bool
{
  return std::ranges::find(sudoku.data(), '_') == end(sudoku.data());
}

template <typename Iter, typename T>
static auto is_uniformly_filled(Iter begin, Iter end, const T& value)
  -> bool
{
  for ( ; begin != end; ++begin ) {
    if ( *begin != value ) {
      return false;
    }
  }

  return true;
}

// Only determines if constraints are intact
// A partially-filled board which does not violate constraints will return true
static auto is_legal_state(const Sudoku& sudoku) noexcept -> bool
{
  const auto data_view = sudoku.mdview();

  std::array<bool, 9> check_table {};

  // returns true if iteration is ok (indeterminate for complete board)
  // returns false if iteration is bad (definitively board is in illegal state)
  auto row_col_check_iteration {[&](std::size_t row, std::size_t col) {
    const auto idx {(data_view(row, col) - '0') - 1};

    if ( idx == ('_' - '0' - 1) ) {
      return true;
    }

    assert(supl::to_string(idx + 1)
           == supl::to_string(data_view(row, col)));
    assert(idx >= 0);
    assert(idx <= 8);

    /* std::cout << sudoku << '\n'; */

    /* std::cout << supl::stream_adapter{std::tuple{row, col, check_table, idx, check_table.at(idx)}} << '\n'; */

    if ( bool& check_table_entry =
           check_table.at(static_cast<std::size_t>(idx));
         check_table_entry ) {
      return false;
    } else {
      check_table_entry = true;
    }

    return true;
  }};

  // check row-wise
  for ( const std::size_t row : std::views::iota(0_z, 9_z) ) {
    for ( const std::size_t col : std::views::iota(0_z, 9_z) ) {
      if ( ! row_col_check_iteration(row, col) ) {
        return false;
      }
    }

    std::ranges::fill(check_table, false);
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
      if ( ! row_col_check_iteration(row, col) ) {
        return false;
      }
    }

    std::ranges::fill(check_table, false);
  }

  // columns are good

  // check sections

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
