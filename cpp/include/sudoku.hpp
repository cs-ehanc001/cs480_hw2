#ifndef SUDOKU_HPP
#define SUDOKU_HPP

#include <algorithm>
#include <array>
#include <cstddef>
#include <iostream>
#include <iterator>
#include <string>

#include <supl/utility.hpp>

#include <mdspan/mdspan.hpp>

struct index_pair {
  std::size_t row;
  std::size_t col;

  friend inline auto operator<=>(const index_pair&,
                                 const index_pair&) noexcept = default;
};

class Sudoku
{
public:
private:

  std::array<char, 81> m_data {};

public:

  constexpr static inline std::array
    charset {'1', '2', '3', '4', '5', '6', '7', '8', '9', '_'};

  // default constructor leaves board in invalid state
  Sudoku() = default;

  explicit Sudoku(const std::array<char, 81>& arg)
      : m_data {arg}
  { }

  Sudoku(const Sudoku&) noexcept = default;
  Sudoku(Sudoku&&) noexcept = default;
  auto operator=(const Sudoku&) noexcept -> Sudoku& = default;
  auto operator=(Sudoku&&) noexcept -> Sudoku& = default;
  ~Sudoku() = default;

  [[nodiscard]] auto is_solved() const noexcept -> bool;

  [[nodiscard]] auto is_valid() const noexcept -> bool;

  [[nodiscard]] auto is_legal_assignment(index_pair idxs,
                                         char value) const noexcept
    -> bool;

  [[nodiscard]] auto data() const noexcept -> const std::array<char, 81>&
  {
    return m_data;
  }

  [[nodiscard]] auto data() noexcept -> std::array<char, 81>&
  {
    return m_data;
  }

  // Using reference implementation of C++23 std::mdspan (as Kokkos::mdspan)
  // backported for use in C++20
  // See https://www.wg21.link/P0009 for details
  //
  // In short, it is a non-owning view over contiguous data
  // and provides multidimensional indexing
  //
  // It is used here to wrap the linear contiguous std::array<char, 81> m_data
  // and index it similarly to as if it were a std::array<std::array, 9>, 9>

  [[nodiscard]] auto mdview() noexcept
  {
    return Kokkos::mdspan<char, Kokkos::extents<std::size_t, 9, 9>> {
      m_data.data()};
  }

  [[nodiscard]] auto mdview() const noexcept
  {
    return Kokkos::mdspan<const char, Kokkos::extents<std::size_t, 9, 9>> {
      m_data.data()};
  }

  // apply a single trivial move (cardinality of reduced domain == 1)
  // (i.e. 8/9 cells in a row are populated; populate last cell in the row
  // with the single possibility)
  //
  // returns true if move was applied, returns false if no trivial move exists
  [[nodiscard]] auto apply_trivial_move() noexcept -> bool;

  // apply only trivial move based on row constraints (mostly for testing purposes)
  [[nodiscard]] auto apply_trivial_row_move() noexcept -> bool;

  // apply only trivial move based on column constraints (mostly for testing purposes)
  [[nodiscard]] auto apply_trivial_column_move() noexcept -> bool;

  // apply only trivial move based on section constraints (mostly for testing purposes)
  [[nodiscard]] auto apply_trivial_section_move() noexcept -> bool;

  friend auto operator<=>(const Sudoku& lhs,
                          const Sudoku& rhs) noexcept = default;

  friend inline auto operator>>(std::istream& in, Sudoku& rhs) noexcept
    -> std::istream&
  {
    std::copy_n(std::istream_iterator<char> {in}, 81, rhs.m_data.begin());
    return in;
  }

  friend inline auto operator<<(std::ostream& out,
                                const Sudoku& rhs) noexcept
    -> std::ostream&
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
};

#endif
