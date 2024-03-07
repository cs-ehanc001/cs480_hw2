#ifndef SECTION_TABLE_HPP
#define SECTION_TABLE_HPP

#include <array>

#include "sudoku.hpp"

// dirty indexing hack
//       v each section
// array<array<pair<index,index>>>
//             ^ indices for each cell in the section
// ^ all sections
constexpr static std::array section_table {
  // {{{
  // section 0,0
  std::array {index_pair {0, 0},
              index_pair {0, 1},
              index_pair {0, 2},
              index_pair {1, 0},
              index_pair {1, 1},
              index_pair {1, 2},
              index_pair {2, 0},
              index_pair {2, 1},
              index_pair {2, 2}},
 // section 0,1
  std::array {index_pair {0, 3},
              index_pair {0, 4},
              index_pair {0, 5},
              index_pair {1, 3},
              index_pair {1, 4},
              index_pair {1, 5},
              index_pair {2, 3},
              index_pair {2, 4},
              index_pair {2, 5}},
 // section 0,2
  std::array {index_pair {0, 6},
              index_pair {0, 7},
              index_pair {0, 8},
              index_pair {1, 6},
              index_pair {1, 7},
              index_pair {1, 8},
              index_pair {2, 6},
              index_pair {2, 7},
              index_pair {2, 8}},
 // section 1,0
  std::array {index_pair {3, 0},
              index_pair {3, 1},
              index_pair {3, 2},
              index_pair {4, 0},
              index_pair {4, 1},
              index_pair {4, 2},
              index_pair {5, 0},
              index_pair {5, 1},
              index_pair {5, 2}},
 // section 1,1
  std::array {index_pair {3, 3},
              index_pair {3, 4},
              index_pair {3, 5},
              index_pair {4, 3},
              index_pair {4, 4},
              index_pair {4, 5},
              index_pair {5, 3},
              index_pair {5, 4},
              index_pair {5, 5}},
 // section 1,2
  std::array {index_pair {3, 6},
              index_pair {3, 7},
              index_pair {3, 8},
              index_pair {4, 6},
              index_pair {4, 7},
              index_pair {4, 8},
              index_pair {5, 6},
              index_pair {5, 7},
              index_pair {5, 8}},
 // section 2,0
  std::array {index_pair {6, 0},
              index_pair {6, 1},
              index_pair {6, 2},
              index_pair {7, 0},
              index_pair {7, 1},
              index_pair {7, 2},
              index_pair {8, 0},
              index_pair {8, 1},
              index_pair {8, 2}},
 // section 2,1
  std::array {index_pair {6, 3},
              index_pair {6, 4},
              index_pair {6, 5},
              index_pair {7, 3},
              index_pair {7, 4},
              index_pair {7, 5},
              index_pair {8, 3},
              index_pair {8, 4},
              index_pair {8, 5}},
 // section 2,2
  std::array {index_pair {6, 6},
              index_pair {6, 7},
              index_pair {6, 8},
              index_pair {7, 6},
              index_pair {7, 7},
              index_pair {7, 8},
              index_pair {8, 6},
              index_pair {8, 7},
              index_pair {8, 8}},
 // }}}
};

#endif
