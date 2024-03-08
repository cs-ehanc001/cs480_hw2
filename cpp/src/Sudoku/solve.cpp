#include <algorithm>
#include <array>
#include <cstddef>
#include <cstdlib>
#include <deque>
#include <iterator>
#include <ranges>
#include <set>
#include <stack>
#include <type_traits>

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

// compile-time generation of every possible variable assignment
constexpr static std::array all_possible_assignments {[]() {
  std::array<Assignment,
             []() {
               constexpr auto square {[](const auto value) {
                 return value * value;
               }};
               return square(std::ranges::size(std::views::iota(0U, 9U)))
                    * std::ranges::size(std::views::iota('1', '9' + 1));
             }()>
    retval {};

  unsigned idx {};
  for ( const unsigned row : std::views::iota(0U, 9U) ) {
    for ( const unsigned col : std::views::iota(0U, 9U) ) {
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

static_assert(std::ranges::all_of(all_possible_assignments,
                                  [](const Assignment& assignment) {
                                    constexpr auto idx_pred {
                                      supl::less_eq(8)};

                                    constexpr auto value_pred {
                                      supl::between('1', '9')};

                                    return idx_pred(assignment.idxs.row)
                                        && idx_pred(assignment.idxs.col)
                                        && value_pred(assignment.value);
                                  }));

// implemented using dfs
// backtracking is baked in :)
auto Sudoku::solve(
  std::add_pointer_t<std::size_t(Sudoku&)> optimization_callback) noexcept
  -> std::size_t
{
  std::size_t assignment_count {};

  constexpr static std::size_t max_depth {50};

  std::set<Assignment> tried_assignments {};

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

  auto generate_states {[&assignment_count,
                         &optimization_callback,
                         &tried_assignments](
                          const Search_Node& search_node) {
    std::deque<Search_Node> retval;

    std::ranges::copy(
      all_possible_assignments
        // filter out illegal assignments
        | std::views::filter(
          [&search_node](const Assignment& assignment) -> bool {
            return search_node.sudoku.is_legal_assignment(assignment);
          })
        // filter out repeated assignments
        /* | std::views::filter( */
        /*   [&tried_assignments](const Assignment& assignment) -> bool { */
        /*     return ! tried_assignments.contains(assignment); */
        /*   }) */
        // perform the assignments
        | std::views::transform(
          [&search_node, &tried_assignments](
            const Assignment& assignment) -> Search_Node {
            tried_assignments.insert(assignment);
            return {search_node.sudoku.assign_copy(assignment),
                    search_node.depth + 1};
          })
        // filter out invalid states
        | std::views::filter([](const Search_Node& new_node) -> bool {
            return new_node.sudoku.is_valid();
          })
        // only accept boards that are either solved or
        // still have legal assignments to be made
        | std::views::filter([](const Search_Node& new_node) -> bool {
            const Sudoku& sudoku {new_node.sudoku};
            return sudoku.is_solved() || sudoku.has_legal_assignments();
          })
        | std::views::reverse | std::views::common,
      std::back_inserter(retval));

    // apply optimization if in use (application of forced moves)
    // no-op if not
    std::ranges::for_each(
      retval,
      [&assignment_count, &optimization_callback](Search_Node& new_node) {
        assignment_count += optimization_callback(new_node.sudoku);
      });

    assignment_count += retval.size();

    return retval;
  }};

  std::stack<Search_Node> frontier {generate_states({*this, 0})};

  const auto careful_pop {[&frontier]() {
    if ( frontier.empty() ) {
      std::cerr << "No solution found!\n";
      std::exit(EXIT_FAILURE);
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
      careful_pop();
    }

    std::cout << new_states.size()
              << " New States: " << supl::stream_adapter {new_states}
              << '\n';

    std::cout << "Expanded from: " << frontier.top().sudoku << '\n';
    std::cout << "Assignments: " << assignment_count << '\n';
    std::cout << std::flush;

    careful_pop();

    // copy new states to the frontier
    std::ranges::for_each(new_states,
                          [&frontier](const Search_Node& search_node) {
                            frontier.push(search_node);
                          });
  }

  if ( ! this->is_solved() ) {
    std::cerr << "No solution found!\n";
    std::exit(EXIT_FAILURE);
  }

  return assignment_count;
}
