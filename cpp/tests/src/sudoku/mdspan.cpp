#include <supl/test_results.hpp>

#include "sudoku.hpp"

auto main() -> int
{
  supl::test_results results;

  Sudoku test {
  // clang-format off
    {'1', '2', '3', '4', '5', '6', '7', '8', '9',
     '2', '3', '4', '5', '6', '7', '8', '9', '1',
     '3', '4', '5', '6', '7', '8', '9', '1', '2',
     '4', '5', '6', '7', '8', '9', '1', '2', '3',
     '5', '6', '7', '8', '9', '1', '2', '3', '4',
     '6', '7', '8', '9', '1', '2', '3', '4', '5',
     '7', '8', '9', '1', '2', '3', '4', '5', '6',
     '8', '9', '1', '2', '3', '4', '5', '6', '7',
     '9', '1', '2', '3', '4', '5', '6', '7', '8'}
  // clang-format on
  };

  Sudoku::mdspan_t data_view = test.data_view();

  results.enforce_equal(data_view(0, 0), '1');
  results.enforce_equal(data_view(0, 1), '2');
  results.enforce_equal(data_view(0, 2), '3');

  results.enforce_equal(data_view(1, 0), '2');
  results.enforce_equal(data_view(2, 0), '3');

  results.enforce_equal(data_view(1, 1), '3');

  results.enforce_equal(data_view(8, 8), '8');

  data_view(1, 1) = '4';

  results.enforce_equal(data_view(1, 1), '4');

  return results.print_and_return();
}
