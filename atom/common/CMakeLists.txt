
cmake_minimum_required(VERSION 3.2)
project(atom-common CXX)

set(SOURCES
  src/panic.cpp
)

set(HEADERS
)

set(HEADERS_PUBLIC
  include/atom/detail/parse_utils.hpp
  include/atom/arena.hpp
  include/atom/arguments.hpp
  include/atom/bit.hpp
  include/atom/const_char_array.hpp
  include/atom/float.hpp
  include/atom/hash.hpp
  include/atom/integer.hpp
  include/atom/literal.hpp
  include/atom/meta.hpp
  include/atom/non_copyable.hpp
  include/atom/non_copyable.hpp
  include/atom/panic.hpp
  include/atom/punning.hpp
  include/atom/result.hpp
  include/atom/vector_n.hpp
)

add_library(atom-common ${SOURCES} ${HEADERS} ${HEADERS_PUBLIC})
target_include_directories(atom-common PUBLIC include)
target_link_libraries(atom-common PUBLIC fmt)