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
// check_table must be reset between region checks
struct check_iteration_handler {

  // NOLINTNEXTLINE(*member*)
  std::bitset<9> check_table;

  auto operator()(const char cell) noexcept -> bool
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

  void reset() noexcept
  {
    check_table.reset();
  }

  void set(std::size_t idx) noexcept
  {
    check_table.set(idx);
  }
};
}  // namespace

// Only determines if constraints are intact
// A partially-filled board which does not violate constraints will return true
static auto is_legal_state(const Sudoku& sudoku) noexcept -> bool
{
  const auto data_view = sudoku.mdview();

  // returns true if iteration is ok (indeterminate for complete board)
  // returns false if iteration is bad (definitively board is in illegal state)
  check_iteration_handler check_iteration {};

  // check row-wise

  for ( const std::size_t row : std::views::iota(0_z, 9_z) ) {
    for ( const std::size_t col : std::views::iota(0_z, 9_z) ) {
      if ( ! check_iteration(data_view(row, col)) ) {
        return false;
      }
    }

    check_iteration.reset();
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

    check_iteration.reset();
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

    check_iteration.reset();
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

auto Sudoku::is_legal_assignment(const index_pair idxs,
                                 const char value) const noexcept -> bool
{
  // value must be in the widest domain
  assert(value >= '1');
  assert(value <= '9');

  const auto data_view = this->mdview();

  // only unpopulated cells may be assigned to
  if ( data_view(idxs.row, idxs.col) != '_' ) {
    return false;
  }

  check_iteration_handler check_iteration {};

  const auto raw_idx {(value - '0') - 1};

  // sanity checks, guaranteed to hold if value of cell is valid
  // (checked by previous assert)
  assert(raw_idx >= 0);
  assert(raw_idx <= 8);
  assert(supl::to_string(raw_idx + 1) == supl::to_string(value));

  // index for
  const std::size_t idx {static_cast<std::size_t>(raw_idx)};

  // check across column
  check_iteration.set(idx);
  for ( const std::size_t row : std::views::iota(0_z, 9_z) ) {
    if ( ! check_iteration(data_view(row, idxs.col)) ) {
      return false;
    }
  }

  check_iteration.reset();

  check_iteration.set(idx);
  for ( const std::size_t col : std::views::iota(0_z, 9_z) ) {
    if ( ! check_iteration(data_view(idxs.row, col)) ) {
      return false;
    }
  }

  check_iteration.reset();

  check_iteration.set(idx);

  // get subtable in section_table which contains indices in question
  const auto& section_subtable {[&]() {
    for ( const auto& subtable : section_table ) {
      if ( std::ranges::find(subtable, idxs) != subtable.end() ) {
        return subtable;
      }
    }
    assert(false);
  }()};  // Immediately Invoked Lambda Expression

  for ( const auto& [row, col] : section_subtable ) {
    if ( ! check_iteration(data_view(row, col)) ) {
      return false;
    }
  }

  return true;
}

///////////////////////////////////////////// DOMAINS

// get remaining domain of each unassigned variable
// and check for any variable with a null domain
//
// find '_' and save positions,
// trim down domain by walking the enclosing row, col, and section

auto Sudoku::query_domains() const noexcept
  -> std::array<variable_domain, 81>
{
  std::array<variable_domain, 81> domains;
  Kokkos::mdspan<variable_domain, Kokkos::extents<std::size_t, 9, 9>>
    domain_view {domains.data()};

  const auto board_view {this->mdview()};

  // initialize
  for ( const std::size_t row : std::views::iota(0_z, 9_z) ) {
    for ( const std::size_t col : std::views::iota(0_z, 9_z) ) {
      domain_view(row, col) = {
        {row, col},
        {}, // default-constructed bitset is all 0s
        board_view(row, col)
      };
    }
  }

  // set domain for unassigned cells (will be reduced)
  for ( auto& domain : domains ) {
    if ( domain.value == '_' ) {
      domain.legal_assignments.reset();
      domain.legal_assignments.flip();
    }
  }

  // reduce by row
  for ( auto& domain : domains ) {
    // skip assigned cells
    if ( domain.value != '_' ) {
      continue;
    }

    const std::size_t row {domain.idxs.row};
    for ( const std::size_t col : std::views::iota(0_z, 9_z) ) {
      // assigned cell means domain may be able to be reduced
      const char cell_value {board_view(row, col)};
      if ( cell_value != '_' ) {
        domain.legal_assignments.reset(
          static_cast<std::size_t>(cell_value - '0' - 1));
      }
    }
  }

  // reduce by column
  for ( auto& domain : domains ) {
    // skip assigned cells
    if ( domain.value != '_' ) {
      continue;
    }

    const std::size_t col {domain.idxs.col};
    for ( const std::size_t row : std::views::iota(0_z, 9_z) ) {
      // assigned cell means domain may be able to be reduced
      const char cell_value {board_view(row, col)};
      if ( cell_value != '_' ) {
        domain.legal_assignments.reset(
          static_cast<std::size_t>(cell_value - '0' - 1));
      }
    }
  }

  // reduce by section
  for ( auto& domain : domains ) {
    // get subtable in section_table which contains indices in question
    const auto& section_subtable {[&]() {
      for ( const auto& subtable : section_table ) {
        if ( std::ranges::find(subtable, domain.idxs) != subtable.end() ) {
          return subtable;
        }
      }
      assert(false);
    }()};  // Immediately Invoked Lambda Expression

    // walk the subtable of section indices
    for ( const auto& [row, col] : section_subtable ) {
      // assigned cell means domain may be able to be reduced
      const char cell_value {board_view(row, col)};
      if ( cell_value != '_' ) {
        domain.legal_assignments.reset(
          static_cast<std::size_t>(cell_value - '0' - 1));
      }
    }
  }

  return domains;
}

auto Sudoku::is_possible() const noexcept -> bool
{

  const auto domains {this->query_domains()};

  for ( const auto& domain : domains ) {
    // skip populated cells
    if ( domain.value != '_' ) {
      continue;
    }

    // cell value == '_' therefore is unpopulated

    if ( domain.legal_assignments.none() ) {
      return false;
    }
  }

  return true;
}
