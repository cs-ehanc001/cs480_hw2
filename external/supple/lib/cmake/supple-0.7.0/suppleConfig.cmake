
####### Expanded from @PACKAGE_INIT@ by configure_package_config_file() #######
####### Any changes to this file will be overwritten by the next CMake run ####
####### The input file was suppleConfig.cmake.in                            ########

get_filename_component(PACKAGE_PREFIX_DIR "${CMAKE_CURRENT_LIST_DIR}/../../../" ABSOLUTE)

macro(set_and_check _var _file)
  set(${_var} "${_file}")
  if(NOT EXISTS "${_file}")
    message(FATAL_ERROR "File or directory ${_file} referenced by variable ${_var} does not exist !")
  endif()
endmacro()

macro(check_required_components _NAME)
  foreach(comp ${${_NAME}_FIND_COMPONENTS})
    if(NOT ${_NAME}_${comp}_FOUND)
      if(${_NAME}_FIND_REQUIRED_${comp})
        set(${_NAME}_FOUND FALSE)
      endif()
    endif()
  endforeach()
endmacro()

####################################################################################

cmake_minimum_required(VERSION 3.16.0)

if(TARGET supple::core)
  return()
endif()

string(
  REGEX
  REPLACE "lib/cmake/supple-0.7.0$"
          "include" supple_include_path
          ${CMAKE_CURRENT_LIST_DIR})

add_library(supple::core INTERFACE IMPORTED)
target_include_directories(supple::core
                           INTERFACE ${supple_include_path}/supple/core)
target_compile_features(supple::core INTERFACE cxx_std_17)

add_library(supple::testing INTERFACE IMPORTED)
target_include_directories(supple::testing
                           INTERFACE ${supple_include_path}/supple/testing)
target_link_libraries(supple::testing INTERFACE supple::core)
target_compile_features(supple::testing INTERFACE cxx_std_17)

add_library(supple::full INTERFACE IMPORTED)
target_link_libraries(supple::full INTERFACE supple::core)
target_link_libraries(supple::full INTERFACE supple::testing)
target_compile_features(supple::full INTERFACE cxx_std_17)

if(NOT DEFINED supple_FIND_QUIETLY)
  message("-- Found supple @ ${CMAKE_CURRENT_LIST_DIR}")
endif()
