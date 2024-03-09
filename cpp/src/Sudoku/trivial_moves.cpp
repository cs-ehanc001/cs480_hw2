#include <bit>
#include <cassert>
#include <cmath>

#include "sudoku.hpp"

// applies exactly one trivial move (only one possible value)
// returned bool indicates whether an assignment was made
auto Sudoku::apply_trivial_move() noexcept -> bool
{
  const auto domains {this->query_domains()};

  for ( const auto& domain : domains ) {
    // skip populated cells
    if ( domain.value != '_' ) {
      continue;
    }

    // cell value == '_' therefore is unpopulated

    // if variable domain has not been reduced to a single possibility,
    // skip it
    if ( domain.legal_assignments.count() != 1 ) {
      continue;
    }

    // should be equivalent to above if
    assert(std::popcount(domain.legal_assignments.to_ulong()) == 1);
    assert(std::has_single_bit(domain.legal_assignments.to_ulong()));

    // cell value == '_' AND has single element domain
    // assignment is forced

    // extract assignment value from compacted domain

    const char assignment_value {[&]() -> char {
      const auto domain_ulong {domain.legal_assignments.to_ulong()};
      const auto log2 {std::log2(domain_ulong)};
      const auto rounded {std::lround(log2)};
      const auto assignment_long {rounded + '0' + 1};

      assert(assignment_long >= '1');
      assert(assignment_long <= '9');

      return static_cast<char>(assignment_long);
    }()};  // Immediately Invoked Lambda Expression

    [[maybe_unused]] const bool assignment_good {this->try_assign(
      {domain.idxs.row, domain.idxs.col}, assignment_value)};
    assert(assignment_good);
    return true;
  }

  return false;
}

