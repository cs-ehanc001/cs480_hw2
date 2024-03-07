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
// backtracking is baked in :)
auto Sudoku::solve(
  std::add_pointer_t<std::size_t(Sudoku&)> optimization_callback) noexcept
  -> std::size_t
{
  using namespace supl::literals::size_t_literal;
  std::size_t assignment_count {};

  constexpr static std::size_t max_depth {50};

  struct Search_Node {
    Sudoku sudoku;
    std::size_t depth;

    void to_stream(std::ostream& out) const noexcept
    {
      out << '{' << sudoku << depth << '}';
    }
  };

  // apply any trivial moves available
  assignment_count += optimization_callback(*this);

  auto generate_states {
    [&assignment_count](const Search_Node& search_node) {
      std::deque<Search_Node> retval;

      std::ranges::copy(
        all_possible_assignments
          | std::views::filter(
            [&search_node](const Assignment& assignment) -> bool {
              return search_node.sudoku.is_legal_assignment(assignment);
            })
          | std::views::transform(
            [&search_node](const Assignment& assignment) -> Search_Node {
              return {search_node.sudoku.assign_copy(assignment),
                      search_node.depth + 1};
            }),
        std::back_inserter(retval));

      assignment_count += retval.size();

      return retval;
    }};

  std::stack<Search_Node> frontier {generate_states({*this, 0})};

  const auto careful_pop {[&frontier]() {
    if ( frontier.empty() ) {
      std::cerr << "No solution found!\n";
      exit(EXIT_FAILURE);
    }

    std::cout << "POP!\n";
    frontier.pop();
  }};

  while ( ! frontier.empty() ) {
    // goal check top before expansion
    if ( frontier.top().sudoku.is_solved() ) {
      *this = frontier.top().sudoku;  // goal achieved! yay!
      break;
    }

    // do not exceed maximum depth
    while ( frontier.top().depth >= max_depth ) {
      std::cout << "DEEP POP!\n";
      careful_pop();
    }

    // generate child nodes
    std::deque<Search_Node> new_states {generate_states(frontier.top())};

    if ( new_states.empty() ) {
      careful_pop();
    }

    // apply optimization if in use
    // no-op if not
    std::ranges::for_each(new_states,
                          [&assignment_count, &optimization_callback](
                            Search_Node& search_node) {
                            assignment_count +=
                              optimization_callback(search_node.sudoku);
                          });

    std::cout << new_states.size()
              << " New States: " << supl::stream_adapter {new_states}
              << '\n';

    std::cout << "Expanded from: " << frontier.top().sudoku << '\n';
    std::cout << "Assignments: " << assignment_count << '\n';

    careful_pop();

    // copy new states to the frontier
    std::ranges::for_each(new_states,
                          [&frontier](const Search_Node& search_node) {
                            frontier.push(search_node);
                          });
  }

  assert(this->is_solved());
  return assignment_count;
}
