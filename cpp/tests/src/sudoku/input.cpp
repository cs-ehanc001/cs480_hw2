#include <sstream>
#include <supl/test_results.hpp>

#include "sudoku.hpp"

auto main() -> int
{
  supl::test_results results;

  std::stringstream stream {R"(1 9 8 5 2 6 3 4 7
7 2 5 3 4 1 6 9 8
3 4 6 9 7 8 2 1 5
9 8 1 2 5 7 4 6 3
5 6 1 1 3 9 8 7 2
2 3 7 6 8 4 1 5 9
4 7 3 8 1 5 9 2 6
8 1 9 7 6 2 5 3 4
6 5 2 4 9 3 7 8 1
)"};

  Sudoku test;

  stream >> test;

  results.enforce_equal(
    test,
    Sudoku {
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
  });

  return results.print_and_return();
}
