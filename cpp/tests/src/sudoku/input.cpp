#include <sstream>
#include <supl/test_results.hpp>

#include "sudoku.hpp"

auto main() -> int
{
  supl::test_results results;

  std::stringstream stream {R"(1 2 3 4 5 6 7 8 9
2 3 4 5 6 7 8 9 1
3 4 5 6 7 8 9 1 2
4 5 6 7 8 9 1 2 3
5 6 7 8 9 1 2 3 4
6 7 8 9 1 2 3 4 5
7 8 9 1 2 3 4 5 6
8 9 1 2 3 4 5 6 7
9 1 2 3 4 5 6 7 8
)"};

  Sudoku test;

  stream >> test;

  results.enforce_equal(
    test,
    Sudoku {
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
  });

  return results.print_and_return();
}
