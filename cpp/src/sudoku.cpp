#include <array>
#include <cassert>
#include <cstddef>
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

// dirty indexing hack
//       v each section
// array<array<pair<index,index>>>
//             ^ indices for each cell in the section
// ^ all sections
constexpr static std::array section_table {
  // {{{
  // section 0,0
  std::array {std::pair {0_z, 0_z},
              std::pair {0_z, 1_z},
              std::pair {0_z, 2_z},
              std::pair {1_z, 0_z},
              std::pair {1_z, 1_z},
              std::pair {1_z, 2_z},
              std::pair {2_z, 0_z},
              std::pair {2_z, 1_z},
              std::pair {2_z, 2_z}},
 // section 0,1
  std::array {std::pair {0_z, 3_z},
              std::pair {0_z, 4_z},
              std::pair {0_z, 5_z},
              std::pair {1_z, 3_z},
              std::pair {1_z, 4_z},
              std::pair {1_z, 5_z},
              std::pair {2_z, 3_z},
              std::pair {2_z, 4_z},
              std::pair {2_z, 5_z}},
 // section 0,2
  std::array {std::pair {0_z, 6_z},
              std::pair {0_z, 7_z},
              std::pair {0_z, 8_z},
              std::pair {1_z, 6_z},
              std::pair {1_z, 7_z},
              std::pair {1_z, 8_z},
              std::pair {2_z, 6_z},
              std::pair {2_z, 7_z},
              std::pair {2_z, 8_z}},
 // section 1,0
  std::array {std::pair {3_z, 0_z},
              std::pair {3_z, 1_z},
              std::pair {3_z, 2_z},
              std::pair {4_z, 0_z},
              std::pair {4_z, 1_z},
              std::pair {4_z, 2_z},
              std::pair {5_z, 0_z},
              std::pair {5_z, 1_z},
              std::pair {5_z, 2_z}},
 // section 1,1
  std::array {std::pair {3_z, 3_z},
              std::pair {3_z, 4_z},
              std::pair {3_z, 5_z},
              std::pair {4_z, 3_z},
              std::pair {4_z, 4_z},
              std::pair {4_z, 5_z},
              std::pair {5_z, 3_z},
              std::pair {5_z, 4_z},
              std::pair {5_z, 5_z}},
 // section 1,2
  std::array {std::pair {3_z, 6_z},
              std::pair {3_z, 7_z},
              std::pair {3_z, 8_z},
              std::pair {4_z, 6_z},
              std::pair {4_z, 7_z},
              std::pair {4_z, 8_z},
              std::pair {5_z, 6_z},
              std::pair {5_z, 7_z},
              std::pair {5_z, 8_z}},
 // section 2,0
  std::array {std::pair {6_z, 0_z},
              std::pair {6_z, 1_z},
              std::pair {6_z, 2_z},
              std::pair {7_z, 0_z},
              std::pair {7_z, 1_z},
              std::pair {7_z, 2_z},
              std::pair {8_z, 0_z},
              std::pair {8_z, 1_z},
              std::pair {8_z, 2_z}},
 // section 2,1
  std::array {std::pair {6_z, 3_z},
              std::pair {6_z, 4_z},
              std::pair {6_z, 5_z},
              std::pair {7_z, 3_z},
              std::pair {7_z, 4_z},
              std::pair {7_z, 5_z},
              std::pair {8_z, 3_z},
              std::pair {8_z, 4_z},
              std::pair {8_z, 5_z}},
 // section 2,2
  std::array {std::pair {6_z, 6_z},
              std::pair {6_z, 7_z},
              std::pair {6_z, 8_z},
              std::pair {7_z, 6_z},
              std::pair {7_z, 7_z},
              std::pair {7_z, 8_z},
              std::pair {8_z, 6_z},
              std::pair {8_z, 7_z},
              std::pair {8_z, 8_z}},
 // }}}
};

// Only determines if constraints are intact
// A partially-filled board which does not violate constraints will return true
static auto is_legal_state(const Sudoku& sudoku) noexcept -> bool
{
  const auto data_view = sudoku.mdview();

  std::array<bool, 9> check_table {};

  // returns true if iteration is ok (indeterminate for complete board)
  // returns false if iteration is bad (definitively board is in illegal state)
  auto check_iteration {[&](const char cell) {
    const auto idx {(cell - '0') - 1};

    if ( idx == ('_' - '0' - 1) ) {
      return true;
    }

    assert(supl::to_string(idx + 1) == supl::to_string(cell));
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
      if ( ! check_iteration(data_view(row, col)) ) {
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
      if ( ! check_iteration(data_view(row, col)) ) {
        return false;
      }
    }

    std::ranges::fill(check_table, false);
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

    std::ranges::fill(check_table, false);
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

// applies exactly one trivial move (only one possible value)
// returned bool indicates whether an assignment was made
//
// below snippet would apply trivial moves until none remained
// while(sudoku.apply_trivial_move());
auto Sudoku::apply_trivial_move() noexcept -> bool
{
  assert(this->is_valid());
  auto data_view = this->mdview();

  std::array<bool, 9> population_table {};

  // perform trivial move based on row constraints
  // (if one exists)
  for ( const std::size_t row : std::views::iota(0_z, 9_z) ) {
    std::size_t trivial_move_idx {};
    for ( const std::size_t col : std::views::iota(0_z, 9_z) ) {
      const char cell {data_view(row, col)};
      const auto population_table_idx {(cell - '0') - 1};

      // skip if unpopulated
      if ( population_table_idx == ('_' - '0' - 1) ) {
        trivial_move_idx = col;
        continue;
      }

      assert(population_table_idx >= 0);
      assert(population_table_idx <= 8);

      population_table.at(static_cast<std::size_t>(population_table_idx)) =
        true;
    }

    // if only one unpopulated cell in the row,
    // a trivial move exists
    if ( std::ranges::count(population_table, false) == 1 ) {

      assert(std::ranges::find(population_table, false)
               - std::begin(population_table)
             >= 0);

      const char trivial_assignment_value {
        static_cast<char>((std::ranges::find(population_table, false)
                           - std::begin(population_table))
                          + '0' + 1)};

      data_view(row, trivial_move_idx) = trivial_assignment_value;
      assert(this->is_valid());
      return true;
    }

    assert(this->is_valid());
    std::ranges::fill(population_table, false);
  }

  assert(this->is_valid());
  return false;
}
