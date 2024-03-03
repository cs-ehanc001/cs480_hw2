#include <cstddef>
#include <ranges>
#include <sstream>
#include <string_view>

#include <supl/utility.hpp>

#include <supl/test_results.hpp>
#include <supl/test_runner.hpp>
#include <supl/test_section.hpp>

#include "sudoku.hpp"

using namespace supl::literals::size_t_literal;

static auto test_checking_of_populated_board() -> supl::test_results
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

      results.enforce_false(illegal.is_solved());
    }
  }

  return results;
}

struct run_data {
  std::size_t row;
  std::size_t col;
  char cell_to;
  bool valid;
};

static void
run_check(const run_data& data, Sudoku board, supl::test_results& results)
{
  const auto& [row, col, cell_to, valid] {data};
  board.mdview()(row, col) = cell_to;
  results.enforce_equal(board.is_valid(),
                        valid,
                        supl::to_string(std::tuple {row, col, cell_to}));
}

static auto test_row_violation_in_partial_board() -> supl::test_results
{
  supl::test_results results;

  const Sudoku legal_partial {
    {
     // clang-format off
  '_', '9', '_', '_', '_', '6', '_', '4', '_',
  '_', '_', '5', '3', '_', '_', '_', '_', '8',
  '_', '_', '_', '_', '7', '_', '2', '_', '_',
  '_', '_', '1', '_', '5', '_', '_', '_', '3',
  '_', '6', '_', '_', '_', '9', '_', '7', '_',
  '2', '_', '_', '_', '8', '4', '1', '_', '_',
  '_', '_', '3', '_', '1', '_', '_', '_', '_',
  '8', '_', '_', '_', '_', '2', '5', '_', '_',
  '_', '5', '_', '4', '_', '_', '_', '8', '_'
     // clang-format on
    }
  };

  std::vector<run_data> runs {
    {0, 0, '1',  true},
    {0, 0, '3',  true},
    {0, 0, '6', false},
    {4, 3, '6', false},
    {4, 3, '1',  true},
    {4, 3, '2',  true},
    {4, 3, '7', false},
    {8, 6, '4', false},
    {8, 6, '9',  true}
  };

  std::ranges::for_each(runs, [&](const run_data& data) {
    run_check(data, legal_partial, results);
  });

  return results;
}

static auto test_col_violation_in_partial_board() -> supl::test_results
{
  supl::test_results results;

  const Sudoku legal_partial {
    {
     // clang-format off
  '_', '9', '_', '_', '_', '6', '_', '4', '_',
  '_', '_', '5', '3', '_', '_', '_', '_', '8',
  '_', '_', '_', '_', '7', '_', '2', '_', '_',
  '_', '_', '1', '_', '5', '_', '_', '_', '3',
  '_', '6', '_', '_', '_', '9', '_', '7', '_',
  '2', '_', '_', '_', '8', '4', '1', '_', '_',
  '_', '_', '3', '_', '1', '_', '_', '_', '_',
  '8', '_', '_', '_', '_', '2', '5', '_', '_',
  '_', '5', '_', '4', '_', '_', '_', '8', '_'
     // clang-format on
    }
  };

  std::vector<run_data> runs {
    {0, 0, '1',  true},
    {0, 0, '3',  true},
    {0, 0, '2', false},
    {4, 3, '3', false},
    {4, 3, '1',  true},
    {4, 3, '2',  true},
    {6, 6, '2', false},
    {8, 6, '4', false},
    {8, 6, '9',  true}
  };

  std::ranges::for_each(runs, [&](const run_data& data) {
    run_check(data, legal_partial, results);
  });

  return results;
}

static auto constraint_checking() -> supl::test_section
{
  supl::test_section section;

  section.add_test("Sudoku checking of populated board",
                   &test_checking_of_populated_board);
  section.add_test("Row checking partial board",
                   &test_row_violation_in_partial_board);
  section.add_test("Column checking partial board",
                   &test_col_violation_in_partial_board);

  return section;
}

auto main() -> int
{
  supl::test_runner runner;

  runner.add_section(constraint_checking());

  return runner.run();
}
