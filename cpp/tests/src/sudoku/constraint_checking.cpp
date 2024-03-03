#include <ranges>

#include <supl/utility.hpp>

#include <supl/test_runner.hpp>
#include <supl/test_section.hpp>

#include "sudoku.hpp"

using namespace supl::literals::size_t_literal;

static auto test_checking() -> supl::test_results
{
  supl::test_results results;

  const Sudoku legal {
    {
     // clang-format off
  '1', '9', '8', '5', '2', '6', '3', '4', '7',
  '7', '2', '5', '3', '4', '1', '6', '9', '8',
  '3', '4', '6', '9', '7', '8', '2', '1', '5',
  '9', '8', '1', '2', '5', '7', '4', '6', '3',
  '5', '6', '4', '1', '3', '9', '8', '7', '2',
  '2', '3', '7', '6', '8', '4', '1', '5', '9',
  '4', '7', '3', '8', '1', '5', '9', '2', '6',
  '8', '1', '9', '7', '6', '2', '5', '3', '4',
  '6', '5', '2', '4', '9', '3', '7', '8', '1'
     // clang-format on
    }
  };

  results.enforce_true(legal.is_solved());

  // create a failure in each cell
  for ( const std::size_t row : std::views::iota(0_z, 9_z) ) {
    for ( const std::size_t col : std::views::iota(0_z, 9_z) ) {
      Sudoku illegal {legal};
      char& fail_point = illegal.mdview()(row, col);

      ++fail_point;
      if ( fail_point == '9' + 1 ) {
        fail_point = '1';
      }

      std::cout << "Checking board with cell changed at: " << supl::stream_adapter{std::pair{row, col}}
      << '\n' << legal << illegal << std::endl;

      results.enforce_false(illegal.is_solved());
    }
  }

  return results;
}

static auto constraint_checking() -> supl::test_section
{
  supl::test_section section;

  section.add_test("Sudoku column checking", &test_checking);

  return section;
}

auto main() -> int
{
  supl::test_runner runner;

  runner.add_section(constraint_checking());

  return runner.run();
}
