
cmake_minimum_required(VERSION 3.2)

project(atom-logger CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(SOURCES
  src/sink/console.cpp
  src/sink/file.cpp
  src/logger.cpp
)

set(HEADERS
)

set(HEADERS_PUBLIC
  include/atom/logger/sink/console.hpp
  include/atom/logger/sink/file.hpp
  include/atom/logger/logger.hpp
)

add_library(atom-logger ${SOURCES} ${HEADERS} ${HEADERS_PUBLIC})
target_link_libraries(atom-logger PUBLIC fmt atom-common)
target_include_directories(atom-logger PRIVATE src)
target_include_directories(atom-logger PUBLIC include)