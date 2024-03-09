#include <algorithm>
#include <array>
#include <cassert>
#include <cstddef>
#include <cstdlib>
#include <ranges>
#include <type_traits>
#include <utility>
#include <vector>

#include "sudoku.hpp"

template std::string supl::to_string<Sudoku>(const Sudoku&);

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
  -> std::pair<std::size_t, bool>
{
  std::size_t assignment_count {};

  // gotta be valid
  if ( ! this->is_valid() ) {
    return {0, false};
  }

  // gotta have legal assignments
  if ( ! this->has_legal_assignments() ) {
    return {0, false};
  }

  // apply any trivial moves available
  assignment_count += optimization_callback(*this);

  if ( this->is_solved() ) {
    return {assignment_count, true};
  }

  const std::array<variable_domain, 81> all_domains {
    this->query_domains()};

  const variable_domain first_unassigned_variable {*std::ranges::find_if(
    all_domains, [](const variable_domain& domain) -> bool {
      return domain.value == '_';
    })};

  // better be a variable with legal assignments!
  // (should be checked above)
  assert(first_unassigned_variable.legal_assignments.any());

  const std::vector<Assignment> possible_assignments {
    [&first_unassigned_variable]() -> std::vector<Assignment> {
      std::vector<Assignment> retval {};

      for ( const unsigned long bit : std::views::iota(0UL, 9UL) ) {

        if ( first_unassigned_variable.legal_assignments.test(bit) ) {

          const char value {static_cast<char>(bit + '0' + 1)};

          assert(value >= '1');
          assert(value <= '9');

          retval.emplace_back(first_unassigned_variable.idxs, value);
        }
      }

      return retval;
    }()};  // Immediately Invoked Lambda Expression

  for ( const Assignment& assignment : possible_assignments ) {
    Sudoku next {this->assign_copy(assignment)};
    ++assignment_count;

    if ( next.is_solved() ) {  // yay!
      *this = next;
      return {assignment_count, true};
    }

    const auto [increased_count,
                is_solved] {next.solve(optimization_callback)};
    assignment_count += increased_count;

    if ( is_solved ) {
      assert(next.is_solved());
      *this = next;
      return {assignment_count, true};
    }
  }

  return {assignment_count, false};
}
