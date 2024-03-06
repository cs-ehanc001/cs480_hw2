#include <algorithm>
#include <array>
#include <cstddef>
#include <deque>
#include <iterator>
#include <ranges>
#include <stack>
#include <type_traits>

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

// compile-time generation of every possible variable assignment
constexpr static std::array all_possible_assignments {[]() {
  using namespace supl::literals::size_t_literal;

  std::array<Assignment,
             []() {
               constexpr auto square {[](const auto value) {
                 return value * value;
               }};
               return square(std::ranges::size(std::views::iota(0_z, 9_z)))
                    * std::ranges::size(std::views::iota('1', '9' + 1));
             }()>
    retval {};

  std::size_t idx {};
  for ( const std::size_t row : std::views::iota(0_z, 9_z) ) {
    for ( const std::size_t col : std::views::iota(0_z, 9_z) ) {
      for ( const char value : std::views::iota('1', '9' + 1) ) {
        retval.at(idx) = {
          {row, col},
          value
        };
        ++idx;
      }
    }
  }

  return retval;
}()};

// implemented using dfs
// no depth limiting required, as infinite loops are impossible
// backtracking is baked in :)
auto Sudoku::solve(
  std::add_pointer_t<std::size_t(Sudoku&)> optimization_callback) noexcept
  -> std::size_t
{
  using namespace supl::literals::size_t_literal;
  std::size_t assignment_count {};

  // apply any trivial moves available
  assignment_count += optimization_callback(*this);

  auto generate_states {[&assignment_count](const Sudoku& sudoku) {
    std::deque<Sudoku> retval;

    std::ranges::copy(
      all_possible_assignments
        | std::views::filter(
          [&sudoku](const Assignment& assignment) -> bool {
            return sudoku.is_legal_assignment(assignment);
          })
        | std::views::transform([&sudoku](const Assignment& assignment) {
            return sudoku.assign_copy(assignment);
          }),
      std::back_inserter(retval));

    assignment_count += retval.size();

    return retval;
  }};

  std::stack<Sudoku> frontier {generate_states(*this)};

  while ( ! frontier.empty() ) {
    // goal check top before expansion
    if ( frontier.top().is_solved() ) {
      *this = frontier.top();  // goal achieved! yay!
      break;
    }

    // generate child nodes
    std::deque<Sudoku> new_states {generate_states(frontier.top())};

    // apply optimization if in use
    // no-op if not
    std::ranges::for_each(
      new_states,
      [&assignment_count, &optimization_callback](Sudoku& sudoku) {
        assignment_count += optimization_callback(sudoku);
      });

    // copy new states to the frontier
    std::ranges::for_each(new_states, [&frontier](const Sudoku& sudoku) {
      frontier.push(sudoku);
    });
  }

  assert(this->is_solved());
  return assignment_count;
}
