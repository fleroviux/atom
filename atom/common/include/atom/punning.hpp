
#pragma once

#include <atom/integer.hpp>
#include <string.h>

namespace atom {

  template<typename T>
  auto Read(void* data, uint offset) -> T {
    T value;
    memcpy(&value, (u8*)data + offset, sizeof(T));
    return value;
  }

  template<typename T>
  void Write(void* data, uint offset, T value) {
    memcpy((u8*)data + offset, &value, sizeof(T));
  }

} // namespace atom