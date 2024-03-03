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

  // check rows
  for ( const std::size_t row : std::views::iota(0_z, 9_z) ) {
    for ( const std::size_t col : std::views::iota(0_z, 9_z) ) {
      const auto idx {(data_view(row, col) - '0') - 1};

      if ( idx == ('_' - '0' - 1) ) {
        continue;
      }

      std::cout << supl::stream_adapter{
        std::tuple {row, col, idx, data_view(row, col)}}
                << std::endl;

      assert(supl::to_string(idx + 1)
             == supl::to_string(data_view(row, col)));

      std::cout << supl::stream_adapter {check_table} << std::endl;

      assert(idx >= 0);
      assert(idx <= 8);

      if ( bool& check_table_entry =
             check_table.at(static_cast<std::size_t>(idx));
           check_table_entry ) {
        std::cout << "BAD" << std::endl;
        return false;
      } else {
        check_table_entry = true;
      }
    }

    std::ranges::fill(check_table, false);
  }

  // check columns

  // factor out logic to be less repetitive??
  // Literally just swapping outer and inner in the data_view access

  // check 3x3 sections

  return true;
}

auto Sudoku::is_solved() const noexcept -> bool
{
  return is_populated(*this) && is_legal_state(*this);
}

