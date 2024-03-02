add_library(Game_and_Logic STATIC sudoku.cpp)
target_link_libraries(Game_and_Logic common_properties)

add_executable(${PROJECT_NAME} main.cpp)
target_link_libraries(${PROJECT_NAME} common_properties Game_and_Logic)
set_target_properties(${PROJECT_NAME} PROPERTIES RUNTIME_OUTPUT_DIRECTORY
                                                 ${CMAKE_BINARY_DIR})
