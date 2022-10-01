
cmake_minimum_required(VERSION 3.2)
project(atom-core CXX)

set(SOURCES
  src/panic.cpp
)

set(HEADERS
)

set(HEADERS_PUBLIC
  include/atom/integer.hpp
  include/atom/panic.hpp
)

add_library(atom-core ${SOURCES} ${HEADERS} ${HEADERS_PUBLIC})
target_include_directories(atom-core PUBLIC include)
target_link_libraries(atom-core PUBLIC fmt)