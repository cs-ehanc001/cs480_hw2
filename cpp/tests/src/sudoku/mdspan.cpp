#include <supl/test_results.hpp>

#include "sudoku.hpp"

auto main() -> int
{
  supl::test_results results;

  Sudoku test {
    {
     // clang-format off
  '1', '9', '8', '5', '2', '6', '3', '4', '7',
  '7', '2', '5', '3', '4', '1', '6', '9', '8',
  '3', '4', '6', '9', '7', '8', '2', '1', '5',
  '9', '8', '1', '2', '5', '7', '4', '6', '3',
  '5', '6', '1', '1', '3', '9', '8', '7', '2',
  '2', '3', '7', '6', '8', '4', '1', '5', '9',
  '4', '7', '3', '8', '1', '5', '9', '2', '6',
  '8', '1', '9', '7', '6', '2', '5', '3', '4',
  '6', '5', '2', '4', '9', '3', '7', '8', '1'
     // clang-format on
    }
  };

  auto data_view = test.mdview();

  results.enforce_equal(data_view(0, 0), '1');
  results.enforce_equal(data_view(0, 1), '9');
  results.enforce_equal(data_view(0, 2), '8');

  results.enforce_equal(data_view(1, 0), '7');
  results.enforce_equal(data_view(2, 0), '3');

  results.enforce_equal(data_view(1, 1), '2');

  results.enforce_equal(data_view(8, 8), '1');

  data_view(1, 1) = '4';

  results.enforce_equal(data_view(1, 1), '4');

  // better not compile
  /* auto const_view = std::as_const(test).mdview(); */
  /* const_view(3, 5) = '8'; */
  // and does not! :)

  return results.print_and_return();
}
