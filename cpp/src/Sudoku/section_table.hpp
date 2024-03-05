#ifndef SECTION_TABLE_HPP
#define SECTION_TABLE_HPP

#include <array>
#include <utility>

#include <supl/utility.hpp>

using supl::size_t_literal::operator""_z;

// dirty indexing hack
//       v each section
// array<array<pair<index,index>>>
//             ^ indices for each cell in the section
// ^ all sections
constexpr static std::array section_table {
  // {{{
  // section 0,0
  std::array {std::pair {0_z, 0_z},
              std::pair {0_z, 1_z},
              std::pair {0_z, 2_z},
              std::pair {1_z, 0_z},
              std::pair {1_z, 1_z},
              std::pair {1_z, 2_z},
              std::pair {2_z, 0_z},
              std::pair {2_z, 1_z},
              std::pair {2_z, 2_z}},
 // section 0,1
  std::array {std::pair {0_z, 3_z},
              std::pair {0_z, 4_z},
              std::pair {0_z, 5_z},
              std::pair {1_z, 3_z},
              std::pair {1_z, 4_z},
              std::pair {1_z, 5_z},
              std::pair {2_z, 3_z},
              std::pair {2_z, 4_z},
              std::pair {2_z, 5_z}},
 // section 0,2
  std::array {std::pair {0_z, 6_z},
              std::pair {0_z, 7_z},
              std::pair {0_z, 8_z},
              std::pair {1_z, 6_z},
              std::pair {1_z, 7_z},
              std::pair {1_z, 8_z},
              std::pair {2_z, 6_z},
              std::pair {2_z, 7_z},
              std::pair {2_z, 8_z}},
 // section 1,0
  std::array {std::pair {3_z, 0_z},
              std::pair {3_z, 1_z},
              std::pair {3_z, 2_z},
              std::pair {4_z, 0_z},
              std::pair {4_z, 1_z},
              std::pair {4_z, 2_z},
              std::pair {5_z, 0_z},
              std::pair {5_z, 1_z},
              std::pair {5_z, 2_z}},
 // section 1,1
  std::array {std::pair {3_z, 3_z},
              std::pair {3_z, 4_z},
              std::pair {3_z, 5_z},
              std::pair {4_z, 3_z},
              std::pair {4_z, 4_z},
              std::pair {4_z, 5_z},
              std::pair {5_z, 3_z},
              std::pair {5_z, 4_z},
              std::pair {5_z, 5_z}},
 // section 1,2
  std::array {std::pair {3_z, 6_z},
              std::pair {3_z, 7_z},
              std::pair {3_z, 8_z},
              std::pair {4_z, 6_z},
              std::pair {4_z, 7_z},
              std::pair {4_z, 8_z},
              std::pair {5_z, 6_z},
              std::pair {5_z, 7_z},
              std::pair {5_z, 8_z}},
 // section 2,0
  std::array {std::pair {6_z, 0_z},
              std::pair {6_z, 1_z},
              std::pair {6_z, 2_z},
              std::pair {7_z, 0_z},
              std::pair {7_z, 1_z},
              std::pair {7_z, 2_z},
              std::pair {8_z, 0_z},
              std::pair {8_z, 1_z},
              std::pair {8_z, 2_z}},
 // section 2,1
  std::array {std::pair {6_z, 3_z},
              std::pair {6_z, 4_z},
              std::pair {6_z, 5_z},
              std::pair {7_z, 3_z},
              std::pair {7_z, 4_z},
              std::pair {7_z, 5_z},
              std::pair {8_z, 3_z},
              std::pair {8_z, 4_z},
              std::pair {8_z, 5_z}},
 // section 2,2
  std::array {std::pair {6_z, 6_z},
              std::pair {6_z, 7_z},
              std::pair {6_z, 8_z},
              std::pair {7_z, 6_z},
              std::pair {7_z, 7_z},
              std::pair {7_z, 8_z},
              std::pair {8_z, 6_z},
              std::pair {8_z, 7_z},
              std::pair {8_z, 8_z}},
 // }}}
};

#endif
