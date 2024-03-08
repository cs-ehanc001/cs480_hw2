#include <string_view>

#include <supl/utility.hpp>

#include <supl/test_results.hpp>
#include <supl/test_runner.hpp>
#include <supl/test_section.hpp>

#include "sudoku.hpp"

using namespace supl::literals::size_t_literal;

static auto test_trivially_solvable() -> supl::test_results
{
  supl::test_results results;

  Sudoku trivially_solvable {
    {
     // clang-format off
  '1', '9', '_', '5', '2', '6', '_', '_', '_',
  '7', '_', '5', '3', '_', '1', '6', '9', '8',
  '3', '_', '6', '_', '7', '_', '2', '1', '5',
  '9', '8', '_', '2', '5', '7', '_', '6', '3',
  '5', '_', '4', '1', '_', '9', '8', '_', '2',
  '2', '3', '7', '_', '8', '4', '1', '5', '9',
  '4', '7', '_', '8', '1', '_', '9', '_', '6',
  '_', '1', '9', '7', '6', '2', '_', '3', '4',
  '6', '5', '2', '4', '_', '3', '7', '8', '1'
     // clang-format on
    }
  };

  results.enforce_false(trivially_solvable.is_solved());
  results.enforce_true(trivially_solvable.is_valid());

  // NOLINTNEXTLINE(*braces*)
  while ( trivially_solvable.apply_trivial_move() )
    ;

  results.enforce_true(trivially_solvable.is_solved());
  results.enforce_true(trivially_solvable.is_valid());

  const Sudoku expected_result {
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

  const Sudoku& result = trivially_solvable;
  results.enforce_equal(result, expected_result);

  return results;
}

static auto trivial_moves() -> supl::test_section
{
  supl::test_section section;

  section.add_test("Trivially solvable sudoku", &test_trivially_solvable);

  return section;
}

auto main() -> int
{
  supl::test_runner runner;

  runner.add_section(trivial_moves());

  return runner.run();
}
