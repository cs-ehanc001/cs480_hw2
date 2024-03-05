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
    supl::equals_any_of("--simple"sv, "--smart"sv)};
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

  // below is placeholder

  std::cout << sudoku << '\n';

  if ( be_smart ) {
    std::cout << "Solving smartly\n";
  } else {
    std::cout << "Solving simply\n";
  }

  return EXIT_SUCCESS;
}
