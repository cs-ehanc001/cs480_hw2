#include <algorithm>
#include <cassert>
#include <cstddef>
#include <ranges>

#include "section_table.hpp"
#include "sudoku.hpp"

// applies exactly one trivial move (only one possible value)
// returned bool indicates whether an assignment was made
//
// below snippet would apply trivial moves until none remained
// while(sudoku.apply_trivial_move());
auto Sudoku::apply_trivial_move() noexcept -> bool
{
  if ( this->apply_trivial_row_move() ) {
    return true;
  }

  if ( this->apply_trivial_column_move() ) {
    return true;
  }

  if ( this->apply_trivial_section_move() ) {
    return true;
  }

  return false;
}

// below code is less clean than I'd like, repeating myself too much
// but time constraints have forced my hand

// apply only trivial move based on row constraints (mostly for testing purposes)
auto Sudoku::apply_trivial_row_move() noexcept -> bool
{
  assert(this->is_valid());
  auto data_view = this->mdview();

  // table indicating which cells in the row are populated
  std::array<bool, 9> population_table {};

  for ( const std::size_t row : std::views::iota(0_z, 9_z) ) {

    // inner loop sets this value to location of empty cell
    std::size_t trivial_move_idx {};

    for ( const std::size_t col : std::views::iota(0_z, 9_z) ) {
      // lookup cell data
      const char cell {data_view(row, col)};

      // convert char to numeric value - 1
      // used to index population table
      //
      // '7' - '0' - 1 == 6, thus, if (population_table[6] == true),
      // a '7' has appeared in this row (outer loop)
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
    // and trivial_move_idx has the column number
    if ( std::ranges::count(population_table, false) == 1 ) {

      assert(std::ranges::find(population_table, false)
               - std::begin(population_table)
             >= 0);

      // location of false value in population table indicates which
      // value did not appear
      //
      // find(population_table, false) returns a pointer to the false value
      // (that pointer) - (pointer to first element) == index of the false
      //
      // Use that index to reverse the transformation
      // in inner loop for population_table_idx
      const char trivial_assignment_value {
        static_cast<char>((std::ranges::find(population_table, false)
                           - std::begin(population_table))
                          + '0' + 1)};

      assert(data_view(row, trivial_move_idx) == '_');
      data_view(row, trivial_move_idx) = trivial_assignment_value;
      assert(this->is_valid());
      return true;
    }

    assert(this->is_valid());

    // reset population table (fill with false)
    std::ranges::fill(population_table, false);
  }

  return false;
}

// apply only trivial move based on column constraints (mostly for testing purposes)
auto Sudoku::apply_trivial_column_move() noexcept -> bool
{
  assert(this->is_valid());
  auto data_view = this->mdview();

  // table indicating which cells in the column are populated
  std::array<bool, 9> population_table {};

  for ( const std::size_t col : std::views::iota(0_z, 9_z) ) {

    // inner loop sets this value to location of empty cell
    std::size_t trivial_move_idx {};

    for ( const std::size_t row : std::views::iota(0_z, 9_z) ) {
      // lookup cell data
      const char cell {data_view(row, col)};

      // convert char to numeric value - 1
      // used to index population table
      //
      // '7' - '0' - 1 == 6, thus, if (population_table[6] == true),
      // a '7' has appeared in this row (outer loop)
      const auto population_table_idx {(cell - '0') - 1};

      // skip if unpopulated
      if ( population_table_idx == ('_' - '0' - 1) ) {
        trivial_move_idx = row;
        continue;
      }

      assert(population_table_idx >= 0);
      assert(population_table_idx <= 8);

      population_table.at(static_cast<std::size_t>(population_table_idx)) =
        true;
    }

    // if only one unpopulated cell in the column,
    // a trivial move exists
    // and trivial_move_idx has the row number
    if ( std::ranges::count(population_table, false) == 1 ) {

      assert(std::ranges::find(population_table, false)
               - std::begin(population_table)
             >= 0);

      // location of false value in population table indicates which
      // value did not appear
      //
      // find(population_table, false) returns a pointer to the false value
      // (that pointer) - (pointer to first element) == index of the false
      //
      // Use that index to reverse the transformation
      // in inner loop for population_table_idx
      const char trivial_assignment_value {
        static_cast<char>((std::ranges::find(population_table, false)
                           - std::begin(population_table))
                          + '0' + 1)};

      assert(data_view(trivial_move_idx, col) == '_');
      data_view(trivial_move_idx, col) = trivial_assignment_value;
      assert(this->is_valid());
      return true;
    }

    assert(this->is_valid());

    // reset population table (fill with false)
    std::ranges::fill(population_table, false);
  }

  assert(this->is_valid());
  return false;
}

// apply only trivial move based on column constraints (mostly for testing purposes)
auto Sudoku::apply_trivial_section_move() noexcept -> bool
{
  assert(this->is_valid());
  auto data_view = this->mdview();

  // table indicating which cells in the section are populated
  std::array<bool, 9> population_table {};

  for ( const auto& section : section_table ) {

    // inner loop sets these values to location of empty cell
    std::size_t outer_row {};
    std::size_t outer_col {};

    for ( const auto& [row, col] : section ) {
      // lookup cell data
      const char cell {data_view(row, col)};

      // convert char to numeric value - 1
      // used to index population table
      //
      // '7' - '0' - 1 == 6, thus, if (population_table[6] == true),
      // a '7' has appeared in this row (outer loop)
      const auto population_table_idx {(cell - '0') - 1};

      // skip if unpopulated
      if ( population_table_idx == ('_' - '0' - 1) ) {
        outer_row = row;
        outer_col = col;
        continue;
      }

      assert(population_table_idx >= 0);
      assert(population_table_idx <= 8);

      population_table.at(static_cast<std::size_t>(population_table_idx)) =
        true;
    }

    // if only one unpopulated cell in the row,
    // a trivial move exists
    // and outer_row and outer_col are the indices
    if ( std::ranges::count(population_table, false) == 1 ) {

      assert(std::ranges::find(population_table, false)
               - std::begin(population_table)
             >= 0);

      // location of false value in population table indicates which
      // value did not appear
      //
      // find(population_table, false) returns a pointer to the false value
      // (that pointer) - (pointer to first element) == index of the false
      //
      // Use that index to reverse the transformation
      // in inner loop for population_table_idx
      const char trivial_assignment_value {
        static_cast<char>((std::ranges::find(population_table, false)
                           - std::begin(population_table))
                          + '0' + 1)};

      assert(data_view(outer_row, outer_col) == '_');
      data_view(outer_row, outer_col) = trivial_assignment_value;
      assert(this->is_valid());
      return true;
    }

    assert(this->is_valid());

    // reset population table (fill with false)
    std::ranges::fill(population_table, false);
  }

  assert(this->is_valid());
  return false;
}
