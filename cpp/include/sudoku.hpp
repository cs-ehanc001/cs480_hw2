#ifndef SUDOKU_HPP
#define SUDOKU_HPP

#include <array>

#include <supl/utility.hpp>

class Sudoku
{
private:

  std::array<char, 81> m_data;

public:

  constexpr static inline std::array
    charset {'1', '2', '3', '4', '5', '6', '7', '8', '9', '_'};

  Sudoku() = delete;
  Sudoku(const Sudoku&) noexcept = default;
  Sudoku(Sudoku&&) noexcept = default;
  auto operator=(const Sudoku&) noexcept -> Sudoku& = default;
  auto operator=(Sudoku&&) noexcept -> Sudoku& = default;
  ~Sudoku() = default;

  friend inline auto operator<=>(const Sudoku&, const Sudoku&) = default;

  friend auto operator>>(std::istream& in, Sudoku& rhs) -> std::istream&;

  friend auto operator<<(std::ostream& out, const Sudoku& rhs)
    -> std::ostream&;
};

#endif
