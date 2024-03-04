#include <cstddef>
#include <ranges>
#include <string_view>

#include <supl/utility.hpp>

#include <supl/test_results.hpp>
#include <supl/test_runner.hpp>
#include <supl/test_section.hpp>

#include "sudoku.hpp"

using namespace supl::literals::size_t_literal;

static auto test_trivial_row_moves() -> supl::test_results
{
  supl::test_results results;

  Sudoku trivial_row_moves {
    {
     // clang-format off
  '_', '9', '8', '5', '_', '6', '3', '4', '7',
  '7', '_', '5', '3', '4', '1', '6', '9', '8',
  '3', '4', '_', '9', '7', '8', '2', '1', '5',
  '9', '8', '1', '2', '5', '7', '4', '6', '3',
  '5', '6', '4', '1', '_', '9', '8', '7', '2',
  '2', '3', '7', '6', '8', '_', '1', '5', '9',
  '4', '7', '3', '8', '1', '5', '_', '2', '6',
  '8', '1', '9', '7', '6', '2', '5', '_', '4',
  '6', '5', '2', '4', '9', '3', '7', '8', '_'
     // clang-format on
    }
  };

  for ( [[maybe_unused]] const int _ : std::views::iota(0, 7) ) {
    results.enforce_true(trivial_row_moves.apply_trivial_row_move());
  }

  results.enforce_false(trivial_row_moves.apply_trivial_row_move());
  results.enforce_false(trivial_row_moves.apply_trivial_row_move());
  results.enforce_true(trivial_row_moves.is_valid());
  results.enforce_false(trivial_row_moves.is_solved());

  const Sudoku expected_result {
    {
     // clang-format off
  '_', '9', '8', '5', '_', '6', '3', '4', '7',
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

  const Sudoku& result = trivial_row_moves;
  results.enforce_equal(result, expected_result);

  return results;
}

static auto test_trivial_column_moves() -> supl::test_results
{
  supl::test_results results;

  Sudoku trivial_column_moves {
    {
     // clang-format off
  '_', '9', '8', '5', '2', '6', '3', '4', '7',
  '7', '2', '5', '3', '_', '1', '6', '9', '8',
  '3', '4', '_', '9', '7', '8', '_', '_', '5',
  '9', '8', '1', '2', '5', '7', '4', '6', '_',
  '5', '6', '4', '1', '3', '9', '8', '7', '2',
  '2', '_', '7', '6', '8', '_', '1', '_', '9',
  '4', '7', '3', '8', '1', '5', '9', '2', '6',
  '8', '1', '9', '7', '6', '2', '5', '3', '4',
  '6', '5', '2', '4', '9', '3', '7', '8', '1'
     // clang-format on
    }
  };

  for ( [[maybe_unused]] const int _ : std::views::iota(0, 7) ) {
    results.enforce_true(trivial_column_moves.apply_trivial_column_move());
  }

  results.enforce_false(trivial_column_moves.apply_trivial_column_move());
  results.enforce_false(trivial_column_moves.apply_trivial_column_move());
  results.enforce_true(trivial_column_moves.is_valid());
  results.enforce_false(trivial_column_moves.is_solved());

  const Sudoku expected_result {
    {
     // clang-format off
  '1', '9', '8', '5', '2', '6', '3', '4', '7',
  '7', '2', '5', '3', '4', '1', '6', '9', '8',
  '3', '4', '6', '9', '7', '8', '2', '_', '5',
  '9', '8', '1', '2', '5', '7', '4', '6', '3',
  '5', '6', '4', '1', '3', '9', '8', '7', '2',
  '2', '3', '7', '6', '8', '4', '1', '_', '9',
  '4', '7', '3', '8', '1', '5', '9', '2', '6',
  '8', '1', '9', '7', '6', '2', '5', '3', '4',
  '6', '5', '2', '4', '9', '3', '7', '8', '1'
     // clang-format on
    }
  };

  const Sudoku& result = trivial_column_moves;
  results.enforce_equal(result, expected_result);

  return results;
}

static auto test_trivial_section_moves() -> supl::test_results
{
  supl::test_results results;

  Sudoku trivial_section_moves {
    {
     // clang-format off
  '1', '9', '8', '5', '_', '6', '_', '4', '7',
  '7', '_', '5', '3', '4', '1', '6', '9', '8',
  '3', '4', '6', '9', '7', '8', '2', '1', '5',
  '9', '8', '1', '2', '_', '7', '4', '6', '3',
  '5', '6', '4', '1', '3', '9', '8', '7', '2',
  '_', '3', '7', '6', '8', '4', '1', '5', '9',
  '4', '_', '3', '8', '_', '5', '9', '2', '6',
  '8', '1', '9', '7', '6', '2', '_', '_', '4',
  '6', '5', '2', '4', '9', '3', '7', '8', '1'
     // clang-format on
    }
  };

  for ( [[maybe_unused]] const int _ : std::views::iota(0, 7) ) {
    results.enforce_true(
      trivial_section_moves.apply_trivial_section_move());
  }

  results.enforce_false(
    trivial_section_moves.apply_trivial_section_move());
  results.enforce_false(
    trivial_section_moves.apply_trivial_section_move());
  results.enforce_true(trivial_section_moves.is_valid());
  results.enforce_false(trivial_section_moves.is_solved());

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
  '8', '1', '9', '7', '6', '2', '_', '_', '4',
  '6', '5', '2', '4', '9', '3', '7', '8', '1'
     // clang-format on
    }
  };

  const Sudoku& result = trivial_section_moves;
  results.enforce_equal(result, expected_result);

  return results;
}

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

  section.add_test("Trivial row moves", &test_trivial_row_moves);
  section.add_test("Trivial column moves", &test_trivial_column_moves);
  section.add_test("Trivial section moves", &test_trivial_section_moves);
  section.add_test("Trivially solvable sudoku", &test_trivially_solvable);

  return section;
}

auto main() -> int
{
  supl::test_runner runner;

  runner.add_section(trivial_moves());

  return runner.run();
}
