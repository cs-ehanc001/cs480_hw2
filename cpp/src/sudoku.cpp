#include <array>

#include "sudoku.hpp"

auto operator>>(std::istream& in, Sudoku& rhs) -> std::istream&
{
  std::copy_n(std::istream_iterator<char> {in}, 81, rhs.m_data.begin());
  return in;
}

auto operator<<(std::ostream& out, const Sudoku& rhs) -> std::ostream&
{
  static std::array<std::pair<char, std::string_view>, 10> mapping {
    std::pair<char, std::string_view> {'1', " 1 "},
    std::pair<char, std::string_view> {'2', " 2 "},
    std::pair<char, std::string_view> {'3', " 3 "},
    std::pair<char, std::string_view> {'4', " 4 "},
    std::pair<char, std::string_view> {'5', " 5 "},
    std::pair<char, std::string_view> {'6', " 6 "},
    std::pair<char, std::string_view> {'7', " 7 "},
    std::pair<char, std::string_view> {'8', " 8 "},
    std::pair<char, std::string_view> {'9', " 9 "},
    std::pair<char, std::string_view> {'_', "   "}
  };

  std::string output_buffer {
    R"(
XXX XXX XXX | XXX XXX XXX | XXX XXX XXX
XXX XXX XXX | XXX XXX XXX | XXX XXX XXX
XXX XXX XXX | XXX XXX XXX | XXX XXX XXX
------------+-------------+------------
XXX XXX XXX | XXX XXX XXX | XXX XXX XXX
XXX XXX XXX | XXX XXX XXX | XXX XXX XXX
XXX XXX XXX | XXX XXX XXX | XXX XXX XXX
------------+-------------+------------
XXX XXX XXX | XXX XXX XXX | XXX XXX XXX
XXX XXX XXX | XXX XXX XXX | XXX XXX XXX
XXX XXX XXX | XXX XXX XXX | XXX XXX XXX
)"};

  for ( const char elem : rhs.m_data ) {
    output_buffer.replace(output_buffer.find("XXX"),
                          3,
                          (*std::ranges::find_if(mapping,
                                                 [&](const auto& kv) {
                                                   return kv.first == elem;
                                                 }))
                            .second);
  }

  return out << output_buffer;
}
