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

  std::array<bool, 9> column_check_table {};

  // check columns
  for ( const std::size_t outer : std::views::iota(1_z, 10_z) ) {
    for ( const std::size_t inner : std::views::iota(1_z, 10_z) ) {
      const auto idx {data_view(outer, inner) - '0'};
      assert(idx > 0);
      assert(idx <= 9);

      if ( bool& check_table_entry =
             column_check_table.at(static_cast<std::size_t>(idx));
           check_table_entry ) {
        return false;
      } else {
        check_table_entry = true;
      }
    }
  }

  // check rows

  // factor out logic to be less repetitive??
  // Literally just swapping outer and inner in the data_view access

  // check 3x3 sections
}

auto Sudoku::is_solved() const noexcept -> bool
{
  return is_populated(*this) && is_legal_state(*this);
}

