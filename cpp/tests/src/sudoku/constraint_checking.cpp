#include <cstddef>
#include <ranges>
#include <string_view>

#include <supl/algorithm.hpp>
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
  index_pair idxs;
  char cell_to;
  bool valid;
};

static void run_is_valid_check(const run_data& data,
                               Sudoku board,
                               supl::test_results& results)
{
  const auto& [idxs, cell_to, valid] {data};
  const auto& [row, col] {idxs};
  board.mdview()(row, col) = cell_to;
  results.enforce_equal(
    board.is_valid(), valid, supl::to_string(std::pair {idxs, cell_to}));
}

static auto get_legal_partial() -> const Sudoku&
{
  static const Sudoku legal_partial {
    {
     // clang-format off
//       0    1    2            3    4    5            6    7    8
/* 0 */ '_', '9', '_', /* 0 */ '_', '_', '6', /* 0 */ '_', '4', '_', /* 0 */
/* 1 */ '_', '_', '5', /* 1 */ '3', '_', '_', /* 1 */ '_', '_', '8', /* 1 */
/* 2 */ '_', '_', '_', /* 2 */ '_', '7', '_', /* 2 */ '2', '_', '_', /* 2 */
//       0    1    2            3    4    5            6    7    8
/* 3 */ '_', '_', '1', /* 3 */ '_', '5', '_', /* 3 */ '_', '_', '3', /* 3 */
/* 4 */ '_', '6', '_', /* 4 */ '_', '_', '9', /* 4 */ '_', '7', '_', /* 4 */
/* 5 */ '2', '_', '_', /* 5 */ '_', '8', '4', /* 5 */ '1', '_', '_', /* 5 */
//       0    1    2            3    4    5            6    7    8
/* 6 */ '_', '_', '3', /* 6 */ '_', '1', '_', /* 6 */ '_', '_', '_', /* 6 */
/* 7 */ '8', '_', '_', /* 7 */ '_', '_', '2', /* 7 */ '5', '_', '_', /* 7 */
/* 8 */ '_', '5', '_', /* 8 */ '4', '_', '_', /* 8 */ '_', '8', '_'  /* 8 */
//       0    1    2            3    4    5            6    7    8
     // clang-format on
    }
  };

  return legal_partial;
}

static auto test_row_violation_in_partial_board() -> supl::test_results
{
  supl::test_results results;

  std::vector<run_data> runs {
    {{0, 0}, '1',  true},
    {{0, 0}, '3',  true},
    {{0, 0}, '6', false},
    {{4, 3}, '6', false},
    {{4, 3}, '1',  true},
    {{4, 3}, '2',  true},
    {{4, 3}, '7', false},
    {{8, 6}, '4', false},
    {{8, 6}, '9',  true},
  };

  std::ranges::for_each(runs, [&](const run_data& data) {
    run_is_valid_check(data, get_legal_partial(), results);
  });

  return results;
}

static auto test_column_violation_in_partial_board() -> supl::test_results
{
  supl::test_results results;

  std::vector<run_data> runs {
    {{0, 0}, '1',  true},
    {{0, 0}, '3',  true},
    {{0, 0}, '2', false},
    {{4, 3}, '3', false},
    {{4, 3}, '1',  true},
    {{4, 3}, '2',  true},
    {{6, 6}, '2', false},
    {{8, 6}, '4', false},
    {{8, 6}, '9',  true}
  };

  std::ranges::for_each(runs, [&](const run_data& data) {
    run_is_valid_check(data, get_legal_partial(), results);
  });

  return results;
}

static auto test_section_violation_in_partial_board() -> supl::test_results
{
  supl::test_results results;

  std::vector<run_data> runs {
    {{0, 0}, '1',  true},
    {{0, 0}, '3',  true},
    {{4, 3}, '5', false},
    {{4, 3}, '1',  true},
    {{4, 3}, '2',  true},
    {{8, 6}, '9',  true},
    {{0, 0}, '5', false},
    {{0, 3}, '7', false},
    {{0, 6}, '8', false},
    {{3, 0}, '6', false},
    {{3, 3}, '8', false},
    {{3, 6}, '7', false},
    {{6, 0}, '8', false},
    {{6, 3}, '2', false},
    {{6, 6}, '8', false},
  };

  std::ranges::for_each(runs, [&](const run_data& data) {
    run_is_valid_check(data, get_legal_partial(), results);
  });

  return results;
}

static auto test_is_legal_assignment() -> supl::test_results
{
  supl::test_results results;

  std::vector<run_data> runs {
    {{0, 0}, '1',  true},
    {{0, 0}, '2', false},
    {{0, 0}, '3',  true},
    {{0, 0}, '5', false},
    {{0, 0}, '6', false},
    {{0, 3}, '7', false},
    {{0, 6}, '8', false},
    {{3, 0}, '6', false},
    {{3, 3}, '8', false},
    {{3, 6}, '7', false},
    {{4, 3}, '1',  true},
    {{4, 3}, '2',  true},
    {{4, 3}, '3', false},
    {{4, 3}, '5', false},
    {{4, 3}, '6', false},
    {{4, 3}, '7', false},
    {{6, 0}, '8', false},
    {{6, 3}, '2', false},
    {{6, 6}, '2', false},
    {{6, 6}, '8', false},
    {{8, 6}, '4', false},
    {{8, 6}, '9',  true},
  };

  for ( const auto& [idxs, cell_to, valid] : runs ) {
    const auto& [row, col] {idxs};
    Sudoku test_value {get_legal_partial()};
    results.enforce_equal(
      test_value.is_legal_assignment({row, col}, cell_to),
      valid,
      supl::to_string(std::pair {idxs, cell_to}));
  }

  return results;
}

static auto test_query_filled_domain() -> supl::test_results
{
  supl::test_results results;

  const Sudoku test {
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

  const auto test_view {test.mdview()};

  const auto domains {test.query_domains()};

  for ( const auto& domain : domains ) {
    results.enforce_true(domain.legal_assignments.none(),
                         supl::to_string(domain));

    results.enforce_equal(test_view(domain.idxs.row, domain.idxs.col),
                          test_view(domain.idxs.row, domain.idxs.col),
                          supl::to_string(domain));
  }

  return results;
}

static auto test_query_legal_partial_domain() -> supl::test_results
{
  supl::test_results results;

  const Sudoku test {get_legal_partial()};

  const auto domains {test.query_domains()};

  decltype(domains) expected_domains {
    variable_domain {{0, 0}, std::bitset<9> {"001000101"}, '_'},
    variable_domain {{0, 1}, std::bitset<9> {"000000000"}, '9'},
    variable_domain {{0, 2}, std::bitset<9> {"011000010"}, '_'},
    variable_domain {{0, 3}, std::bitset<9> {"010010011"}, '_'},
    variable_domain {{0, 4}, std::bitset<9> {"000000010"}, '_'},
    variable_domain {{0, 5}, std::bitset<9> {"000000000"}, '6'},
    variable_domain {{0, 6}, std::bitset<9> {"001000100"}, '_'},
    variable_domain {{0, 7}, std::bitset<9> {"000000000"}, '4'},
    variable_domain {{0, 8}, std::bitset<9> {"001010001"}, '_'},
 /* variable_domain {{1, 0}, std::bitset<9> {"111111111"}, '_'}, */
  /* variable_domain {{1, 1}, std::bitset<9> {"111111111"}, '_'}, */
  /* variable_domain {{1, 2}, std::bitset<9> {"000000000"}, '5'}, */
  /* variable_domain {{1, 3}, std::bitset<9> {"000000000"}, '3'}, */
  /* variable_domain {{1, 4}, std::bitset<9> {"111111111"}, '_'}, */
  /* variable_domain {{1, 5}, std::bitset<9> {"111111111"}, '_'}, */
  /* variable_domain {{1, 6}, std::bitset<9> {"111111111"}, '_'}, */
  /* variable_domain {{1, 7}, std::bitset<9> {"111111111"}, '_'}, */
  /* variable_domain {{1, 8}, std::bitset<9> {"000000000"}, '8'}, */
  /* variable_domain {{2, 0}, std::bitset<9> {"111111111"}, '_'}, */
  /* variable_domain {{2, 1}, std::bitset<9> {"111111111"}, '_'}, */
  /* variable_domain {{2, 2}, std::bitset<9> {"111111111"}, '_'}, */
  /* variable_domain {{2, 3}, std::bitset<9> {"111111111"}, '_'}, */
  /* variable_domain {{2, 4}, std::bitset<9> {"000000000"}, '7'}, */
  /* variable_domain {{2, 5}, std::bitset<9> {"111111111"}, '_'}, */
  /* variable_domain {{2, 6}, std::bitset<9> {"000000000"}, '2'}, */
  /* variable_domain {{2, 7}, std::bitset<9> {"111111111"}, '_'}, */
  /* variable_domain {{2, 8}, std::bitset<9> {"111111111"}, '_'}, */
  /* variable_domain {{3, 0}, std::bitset<9> {"111111111"}, '_'}, */
  /* variable_domain {{3, 1}, std::bitset<9> {"111111111"}, '_'}, */
  /* variable_domain {{3, 2}, std::bitset<9> {"000000000"}, '1'}, */
  /* variable_domain {{3, 3}, std::bitset<9> {"111111111"}, '_'}, */
  /* variable_domain {{3, 4}, std::bitset<9> {"000000000"}, '5'}, */
  /* variable_domain {{3, 5}, std::bitset<9> {"111111111"}, '_'}, */
  /* variable_domain {{3, 6}, std::bitset<9> {"111111111"}, '_'}, */
  /* variable_domain {{3, 7}, std::bitset<9> {"111111111"}, '_'}, */
  /* variable_domain {{3, 8}, std::bitset<9> {"000000000"}, '3'}, */
  /* variable_domain {{4, 0}, std::bitset<9> {"111111111"}, '_'}, */
  /* variable_domain {{4, 1}, std::bitset<9> {"000000000"}, '6'}, */
  /* variable_domain {{4, 2}, std::bitset<9> {"111111111"}, '_'}, */
  /* variable_domain {{4, 3}, std::bitset<9> {"111111111"}, '_'}, */
  /* variable_domain {{4, 4}, std::bitset<9> {"111111111"}, '_'}, */
  /* variable_domain {{4, 5}, std::bitset<9> {"000000000"}, '9'}, */
  /* variable_domain {{4, 6}, std::bitset<9> {"111111111"}, '_'}, */
  /* variable_domain {{4, 7}, std::bitset<9> {"000000000"}, '7'}, */
  /* variable_domain {{4, 8}, std::bitset<9> {"111111111"}, '_'}, */
  /* variable_domain {{5, 0}, std::bitset<9> {"000000000"}, '2'}, */
  /* variable_domain {{5, 1}, std::bitset<9> {"111111111"}, '_'}, */
  /* variable_domain {{5, 2}, std::bitset<9> {"111111111"}, '_'}, */
  /* variable_domain {{5, 3}, std::bitset<9> {"111111111"}, '_'}, */
  /* variable_domain {{5, 4}, std::bitset<9> {"000000000"}, '8'}, */
  /* variable_domain {{5, 5}, std::bitset<9> {"000000000"}, '4'}, */
  /* variable_domain {{5, 6}, std::bitset<9> {"000000000"}, '1'}, */
  /* variable_domain {{5, 7}, std::bitset<9> {"111111111"}, '_'}, */
  /* variable_domain {{5, 8}, std::bitset<9> {"111111111"}, '_'}, */
  /* variable_domain {{6, 0}, std::bitset<9> {"111111111"}, '_'}, */
  /* variable_domain {{6, 1}, std::bitset<9> {"111111111"}, '_'}, */
  /* variable_domain {{6, 2}, std::bitset<9> {"000000000"}, '3'}, */
  /* variable_domain {{6, 3}, std::bitset<9> {"111111111"}, '_'}, */
  /* variable_domain {{6, 4}, std::bitset<9> {"000000000"}, '1'}, */
  /* variable_domain {{6, 5}, std::bitset<9> {"111111111"}, '_'}, */
  /* variable_domain {{6, 6}, std::bitset<9> {"111111111"}, '_'}, */
  /* variable_domain {{6, 7}, std::bitset<9> {"111111111"}, '_'}, */
  /* variable_domain {{6, 8}, std::bitset<9> {"111111111"}, '_'}, */
  /* variable_domain {{7, 0}, std::bitset<9> {"000000000"}, '8'}, */
  /* variable_domain {{7, 1}, std::bitset<9> {"111111111"}, '_'}, */
  /* variable_domain {{7, 2}, std::bitset<9> {"111111111"}, '_'}, */
  /* variable_domain {{7, 3}, std::bitset<9> {"111111111"}, '_'}, */
  /* variable_domain {{7, 4}, std::bitset<9> {"111111111"}, '_'}, */
  /* variable_domain {{7, 5}, std::bitset<9> {"000000000"}, '2'}, */
  /* variable_domain {{7, 6}, std::bitset<9> {"000000000"}, '5'}, */
  /* variable_domain {{7, 7}, std::bitset<9> {"111111111"}, '_'}, */
  /* variable_domain {{7, 8}, std::bitset<9> {"111111111"}, '_'}, */
  /* variable_domain {{8, 0}, std::bitset<9> {"111111111"}, '_'}, */
  /* variable_domain {{8, 1}, std::bitset<9> {"000000000"}, '5'}, */
  /* variable_domain {{8, 2}, std::bitset<9> {"111111111"}, '_'}, */
  /* variable_domain {{8, 3}, std::bitset<9> {"000000000"}, '4'}, */
  /* variable_domain {{8, 4}, std::bitset<9> {"111111111"}, '_'}, */
  /* variable_domain {{8, 5}, std::bitset<9> {"111111111"}, '_'}, */
  /* variable_domain {{8, 6}, std::bitset<9> {"111111111"}, '_'}, */
  /* variable_domain {{8, 7}, std::bitset<9> {"000000000"}, '8'}, */
  /* variable_domain {{8, 8}, std::bitset<9> {"111111111"}, '_'}, */
  };

  supl::for_each_both(
    domains.begin(),
    domains.begin() + 8,
    expected_domains.begin(),
    expected_domains.end(),
    [&results](const auto& result, const auto& expected) {
      results.enforce_equal(result, expected);
    });

  return results;
}

static auto test_has_legal_assignments() -> supl::test_results
{
  supl::test_results results;

  const Sudoku hard {
    {
     // clang-format off
'7', '_', '_', '_', '_', '_', '_', '_', '_',
'6', '_', '_', '4', '1', '_', '2', '5', '_',
'_', '1', '3', '_', '9', '5', '_', '_', '_',
'8', '6', '_', '_', '_', '_', '_', '_', '_',
'3', '_', '1', '_', '_', '_', '4', '_', '5',
'_', '_', '_', '_', '_', '_', '_', '8', '6',
'_', '_', '_', '8', '4', '_', '5', '3', '_',
'_', '4', '2', '_', '3', '6', '_', '_', '7',
'_', '_', '_', '_', '_', '_', '_', '_', '9',
     // clang-format on
    }
  };

  const Sudoku impossible {
    {
     // clang-format off
'7', '3', '2', '1', '8', '_', '4', '9', '6',
'5', '6', '_', '2', '9', '4', '7', '1', '3',
'8', '1', '4', '3', '6', '_', '5', '2', '_',
'3', '7', '5', '9', '1', '2', '8', '_', '4',
'4', '2', '6', '8', '7', '5', '1', '3', '9',
'1', '9', '8', '4', '3', '_', '6', '5', '7',
'6', '5', '3', '_', '2', '7', '9', '4', '1',
'9', '4', '1', '6', '5', '3', '_', '7', '2',
'2', '8', '_', '_', '4', '_', '3', '6', '5',
     // clang-format on
    }
  };

  results.enforce_true(hard.has_legal_assignments());
  results.enforce_false(impossible.has_legal_assignments());

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
                   &test_column_violation_in_partial_board);
  section.add_test("Section checking partial board",
                   &test_section_violation_in_partial_board);
  section.add_test("is_legal_assignment", &test_is_legal_assignment);
  section.add_test("domain query - filled", &test_query_filled_domain);
  section.add_test("domain query - legal partial",
                   &test_query_legal_partial_domain);
  section.add_test("has_legal_assignments", &test_has_legal_assignments);

  return section;
}

auto main() -> int
{
  supl::test_runner runner;

  runner.add_section(constraint_checking());

  return runner.run();
}
