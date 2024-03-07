#include <chrono>
#include <cstdlib>
#include <fstream>
#include <iostream>
#include <ranges>
#include <string_view>

#include <supl/predicates.hpp>

#include "sudoku.hpp"

void print_help_message([[maybe_unused]] const int argc,
                        const char* const* const argv)
{
  std::cerr << "Usage:\n"
            << argv[0] << " --simple [input_file.dat]\n"
            << argv[0] << " --smart [input_file.dat]\n";
}

auto main(const int argc, const char* const* const argv) -> int
{
  using namespace std::literals;  // for operator""sv string_view literal

  for ( const int i : std::views::iota(1, argc) ) {
    constexpr static auto help_pred {
      supl::equals_any_of("--help"sv, "-h"sv)};
    if ( help_pred(argv[i]) ) {
      print_help_message(argc, argv);
      return EXIT_SUCCESS;
    }
  }

  if ( argc != 3 ) {
    print_help_message(argc, argv);
    return EXIT_FAILURE;
  }

  constexpr static auto strategy_pred {
    supl::equals_any_of("--simple"sv, "--smart"sv, "--just-print"sv)};
  if ( ! strategy_pred(argv[1]) ) {
    std::cerr << "Bad search strategy: \"" << argv[1]
              << "\". Must be [--simple] or [--smart].\n";
    print_help_message(argc, argv);
    return EXIT_FAILURE;
  }

  const bool be_smart {"--smart"sv == argv[1]};

  Sudoku sudoku {[argc, argv]() {
    std::ifstream infile {argv[2]};

    if ( ! infile.is_open() ) {
      std::cerr << "Error opening file: \"" << argv[2] << "\"\n";
      print_help_message(argc, argv);
      std::exit(EXIT_FAILURE);
    }
    Sudoku retval;
    infile >> retval;
    return retval;
  }()};

  std::cout << "Beginning state:\n" << sudoku << '\n';

  // undocumented feature to just print an input file
  if ( "--just-print"sv == argv[1] ) {
    return EXIT_SUCCESS;
  }

  const auto optimization_callback {be_smart ? &trivial_move_optimization
                                             : &null_optimization};

  const auto start_time {std::chrono::steady_clock::now()};

  const std::size_t assignment_count {sudoku.solve(optimization_callback)};

  const auto end_time {std::chrono::steady_clock::now()};

  std::cout << "Solution state:\n" << sudoku << "\n\n";
  std::cout << "Solution found with: " << assignment_count
            << " variable assignments\n";

  std::cout << "Solution took: "
            << std::chrono::duration_cast<std::chrono::microseconds>(
                 end_time - start_time)
                 .count()
            << "us\n"
            << "Equal to: "
            << std::chrono::duration_cast<std::chrono::milliseconds>(
                 end_time - start_time)
                 .count()
            << "ms\n"
            << "Equal to: "
            << std::chrono::duration_cast<std::chrono::seconds>(
                 end_time - start_time)
                 .count()
            << "s\n";

  return EXIT_SUCCESS;
}
