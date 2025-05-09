cmake_minimum_required(VERSION 3.24)
project(TPKit VERSION 0.0.1)
set(TPKit_DIR ${CMAKE_CURRENT_SOURCE_DIR}/Source)

include(CMake/SetupCXX.cmake)
include(CMake/SetupFmt.cmake)
include(CMake/SetupGTest.cmake)

include_directories(${TPKit_DIR})
add_subdirectory(Source/TX)
add_subdirectory(Source/Transport)
