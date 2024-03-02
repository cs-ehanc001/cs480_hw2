cmake_minimum_required(VERSION 3.16.0)

project(
  sudoku_solver
  VERSION 0.0.1
  LANGUAGES CXX)

list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake")

include(compiler_flags)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(USE_CXX_STD 20)

if(NOT DEFINED CMAKE_BUILD_TYPE)
  message(FATAL_ERROR "Build type not specified!")
endif()

# Remove if not library
set(MAIN_PROJECT NO)
if(CMAKE_CURRENT_SOURCE_DIR STREQUAL CMAKE_SOURCE_DIR)
  set(MAIN_PROJECT YES)
endif()

set(TOP_DIR ${CMAKE_CURRENT_SOURCE_DIR})

option(SANITIZE_DEBUG "Sanitizers should be used in debug builds" NO)

option(SANITIZE_RELEASE "Sanitizers should be used in release builds" NO)

option(COMPILE_TESTS "Tests should be compiled" ${MAIN_PROJECT})

# Options specific to compiling with clang
option(DO_CLANG_TIDY "Run clang-tidy during build" NO)
option(DO_TIME_TRACE "Run time trace during build" NO)

message(STATUS "Build type: ${CMAKE_BUILD_TYPE}")

list(APPEND CMAKE_PREFIX_PATH ${CMAKE_CURRENT_LIST_DIR}/external)
find_package(supple REQUIRED)

add_library(common_properties INTERFACE)
target_include_directories(common_properties
                           INTERFACE ${CMAKE_CURRENT_SOURCE_DIR}/cpp/include)
target_link_libraries(common_properties INTERFACE compiler_flags supple::core)
target_compile_features(common_properties INTERFACE cxx_std_${USE_CXX_STD})

if(COMPILE_TESTS)
  include(testing)
  include(CTest)
  enable_testing()

  test_handling()
endif()

add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/cpp)

