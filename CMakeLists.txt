
cmake_minimum_required(VERSION 3.2...4.0 FATAL_ERROR)

project(atom CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

option(ATOM_USE_EXTERNAL_FMT "Use externally provided {fmt} library." OFF)

# Which atom modules are included from this CMakeLists:
option(ATOM_INCLUDE_LOGGER "Enable the atom-logger module" ON)
option(ATOM_INCLUDE_MATH "Enable the atom-math module" ON)

add_subdirectory(external)
add_subdirectory(atom/common)

if(ATOM_INCLUDE_LOGGER)
  add_subdirectory(atom/logger)
endif()

if(ATOM_INCLUDE_MATH)
  add_subdirectory(atom/math)
endif()