#include <algorithm>
#include <array>
#include <cstddef>
#include <cstdlib>
#include <deque>
#include <iterator>
#include <ranges>
#include <stack>
#include <type_traits>
#include <vector>

#include <supl/predicates.hpp>
#include <supl/utility.hpp>

#include "sudoku.hpp"

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

  const Assignment first_assignment {
    [&first_unassigned_variable]() -> Assignment {
      std::cout << "Domain: " << first_unassigned_variable << '\n';

      for ( const unsigned long bit : std::views::iota(0UL, 9UL) ) {

        if ( first_unassigned_variable.legal_assignments.test(bit) ) {

          std::cout << "Bit " << bit << " is first set" << '\n';

          const char value {static_cast<char>(bit + '0' + 1)};

          std::cout << "Value: " << value << '\n';

          assert(value >= '1');
          assert(value <= '9');

          return {first_unassigned_variable.idxs, value};
        }
      }
    }()};  // Immediately Invoked Lambda Expression

  std::cout << first_assignment << '\n';

  Sudoku next {this->assign_copy(first_assignment)};
  ++assignment_count;

  std::cout << "Next: " << next << std::boolalpha << next.is_solved()
            << '\n';

  if ( next.is_solved() ) {  // yay!
    *this = next;
    return {assignment_count, true};
  }

  const auto [more_assignments,
              keep_going] {next.solve(optimization_callback)};
}
