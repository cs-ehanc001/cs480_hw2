#ifndef SUDOKU_HPP
#define SUDOKU_HPP

#include <array>
#include <ranges>
#include <utility>

#include <supl/utility.hpp>

#include <mdspan/mdspan.hpp>

class Sudoku
{
public:

  // using reference implementation of C++23 std::mdspan
  // backported for use in C++20

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

  [[nodiscard]] auto data() const noexcept -> const std::array<char, 81>&
  {
    return m_data;
  }

  [[nodiscard]] auto data() noexcept -> std::array<char, 81>&
  {
    return m_data;
  }

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
  auto apply_trivial_move() noexcept -> bool;

  // apply only trivial move based on row constraints (mostly for testing purposes)
  auto apply_trivial_row_move() noexcept -> bool;

  // apply only trivial move based on column constraints (mostly for testing purposes)
  auto apply_trivial_column_move() noexcept -> bool;

  // apply only trivial move based on section constraints (mostly for testing purposes)
  auto apply_trivial_section_move() noexcept -> bool;

  /* template <typename Func> */
  /* void for_each_in_row(const std::size_t row, Func&& func) noexcept */
  /* { */
  /*   using namespace supl::literals::size_t_literal; */
  /*   const auto data_view {this->mdview()}; */
  /*   for ( const std::size_t col : std::views::iota(0_z, 9_z) ) { */
  /*     std::forward<Func>(func)(data_view(row, col)); */
  /*   } */
  /* } */

  /* template <typename Func> */
  /* void for_each_in_col(const std::size_t col, Func&& func) noexcept */
  /* { */
  /*   using namespace supl::literals::size_t_literal; */
  /*   const auto data_view {this->mdview()}; */
  /*   for ( const std::size_t row : std::views::iota(0_z, 9_z) ) { */
  /*     std::forward<Func>(func)(data_view(row, col)); */
  /*   } */
  /* } */

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
