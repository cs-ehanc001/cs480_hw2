#include <iostream>
#include <vector>

#include <supl/utility.hpp>

auto main() -> int
{
  std::cout << supl::stream_adapter {
    std::vector {
                 1, 2,
                 3, 4,
                 42}
  } << '\n';
}
