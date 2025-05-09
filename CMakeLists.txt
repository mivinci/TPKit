cmake_minimum_required(VERSION 3.24)
file(READ version TPKIT_VERSION)
project(TPKit VERSION ${TPKIT_VERSION})

# Set up CMake options
option(TPKIT_STATIC "Build static library" OFF)
option(TPKIT_USE_FMTLIB "Use fmt lib" OFF)

# Set up compile definitions
if (TPKIT_USE_FMTLIB)
  add_definitions(-DUSE_FMTLIB)
endif (TPKIT_USE_FMTLIB)

# Set up source directory
set(TPKIT_SOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/Source)
include_directories(${TPKIT_SOURCE_DIR})
add_definitions(-DTPKIT_SOURCE_DIR="${TPKIT_SOURCE_DIR}")

# Set up CMake modules
include(CMake/SetupCXX.cmake)
include(CMake/SetupFmt.cmake)
include(CMake/SetupGTest.cmake)

# Set up TPKit modules
add_subdirectory(${TPKIT_SOURCE_DIR}/Transport)
add_subdirectory(${TPKIT_SOURCE_DIR}/TX)
set(TPKIT_MODULES Transport TX)

# Set up TPKit files
file(GLOB TPKIT_HEADERS ${TPKIT_SOURCE_DIR}/*.h)
file(GLOB ${TPKIT_SOURCE_DIR}/*.cc)

# Set up the TPKit shared library
add_library(TPKit SHARED ${TPKIT_HEADERS} ${TPKIT_SOURCES})
target_compile_definitions(TPKit PRIVATE BUILD_TPKIT -Dfvisibility=hidden)
target_link_libraries(TPKit PRIVATE ${TPKIT_MODULES})

# Set up TPKit static library
if (TPKIT_STATIC)
  add_library(TPKit_Static STATIC ${TPKIT_HEADERS} ${TPKIT_SOURCES})
  set_target_properties(TPKit_Static PROPERTIES OUTPUT_NAME "TPKit")
  target_link_libraries(TPKit PRIVATE ${TPKIT_MODULES})
endif (TPKIT_STATIC)
