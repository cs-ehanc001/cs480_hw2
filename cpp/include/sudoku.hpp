#ifndef SUDOKU_HPP
#define SUDOKU_HPP

#include <array>

#include <supl/utility.hpp>

#include <mdspan/mdspan.hpp>

class Sudoku
{
public:

  using mdspan_t =
    Kokkos::mdspan<char, Kokkos::extents<std::size_t, 9, 9>>;

private:

  std::array<char, 81> m_data {};

  // using reference implementation of C++23 std::mdspan
  // backported for use in C++20
  mdspan_t m_data_view {m_data.data()};

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

  [[nodiscard]] auto data_view() -> mdspan_t
  {
    return m_data_view;
  }

  friend inline auto operator<(const Sudoku& lhs, const Sudoku& rhs)
    -> bool
  {
    return lhs.m_data < rhs.m_data;
  }

  friend inline auto operator==(const Sudoku& lhs, const Sudoku& rhs)
    -> bool
  {
    return lhs.m_data == rhs.m_data;
  }

  friend auto operator>>(std::istream& in, Sudoku& rhs) -> std::istream&;

  friend auto operator<<(std::ostream& out, const Sudoku& rhs)
    -> std::ostream&;
};

#endif
