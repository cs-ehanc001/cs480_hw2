#include <array>

#include "sudoku.hpp"

auto operator>>(std::istream& in, Sudoku& rhs) -> std::istream&
{
  std::copy_n(std::istream_iterator<char> {in}, 81, rhs.m_data.begin());
  return in;
}

auto operator<<(std::ostream& out, const Sudoku& rhs) -> std::ostream&
{

  std::string output_buffer {
    R"(
X X X | X X X | X X X
X X X | X X X | X X X
X X X | X X X | X X X
------+-------+------
X X X | X X X | X X X
X X X | X X X | X X X
X X X | X X X | X X X
------+-------+------
X X X | X X X | X X X
X X X | X X X | X X X
X X X | X X X | X X X
)"};

  for ( const char elem : rhs.m_data ) {
    output_buffer.at(output_buffer.find('X')) = elem;
  }

  return out << output_buffer;
}
