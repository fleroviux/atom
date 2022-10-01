
cmake_minimum_required(VERSION 3.2)
project(atom-math CXX)

set(HEADERS_PUBLIC
  include/atom/math/box3.hpp
  include/atom/math/frustum.hpp
  include/atom/math/matrix4.hpp
  include/atom/math/plane.hpp
  include/atom/math/quaternion.hpp
  include/atom/math/traits.hpp
  include/atom/math/vector.hpp
)

add_library(atom-math INTERFACE ${HEADERS_PUBLIC})
target_include_directories(atom-math INTERFACE include)
target_link_libraries(atom-math INTERFACE atom-common)