#include <supl/test_runner.hpp>
#include <supl/test_section.hpp>

static auto test_column_checking() -> supl::test_results
{
  supl::test_results results;

  return results;
}

static auto constraint_checking() -> supl::test_section
{
  supl::test_section section;

  section.add_test("Sudoku column checking", &test_column_checking);

  return section;
}

auto main() -> int
{
  supl::test_runner runner;

  runner.add_section(constraint_checking());

  return runner.run();
}
