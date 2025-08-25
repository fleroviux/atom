load("@rules_cc//cc:defs.bzl", "cc_library")

cc_library(
  name = "atom-common",
  srcs = [ "common/src/panic.cpp" ],
  hdrs = [
    "common/include/atom/detail/parse_utils.hpp",
    "common/include/atom/arena.hpp",
    "common/include/atom/arguments.hpp",
    "common/include/atom/bit.hpp",
    "common/include/atom/const_char_array.hpp",
    "common/include/atom/float.hpp",
    "common/include/atom/hash.hpp",
    "common/include/atom/integer.hpp",
    "common/include/atom/literal.hpp",
    "common/include/atom/meta.hpp",
    "common/include/atom/non_copyable.hpp",
    "common/include/atom/panic.hpp",
    "common/include/atom/punning.hpp",
    "common/include/atom/result.hpp",
    "common/include/atom/vector_n.hpp"
  ],
  includes = [ "common/include" ],
  deps = [ "@fmt//:fmt" ]
)

cc_library(
  name = "atom-logger",
  srcs = [
    "logger/src/sink/console.cpp",
    "logger/src/sink/file.cpp",
    "logger/src/logger.cpp"
  ],
  hdrs = [
    "logger/include/atom/logger/sink/console.hpp",
    "logger/include/atom/logger/sink/file.hpp",
    "logger/include/atom/logger/logger.hpp"
  ],
  includes = [ "logger/include" ],
  deps = [ ":atom-common" ]
)

cc_library(
  name = "atom-math",
  hdrs = [
    "math/include/atom/math/box3.hpp",
    "math/include/atom/math/frustum.hpp",
    "math/include/atom/math/matrix4.hpp",
    "math/include/atom/math/plane.hpp",
    "math/include/atom/math/quaternion.hpp",
    "math/include/atom/math/traits.hpp",
    "math/include/atom/math/vector.hpp"
  ],
  includes = [ "math/include" ],
  deps = [ ":atom-common" ]
)

cc_library(
  name = "atom",
  deps = [ ":atom-common", ":atom-logger", ":atom-math" ]
)
