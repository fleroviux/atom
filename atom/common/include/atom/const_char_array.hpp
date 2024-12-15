
#pragma once

#include <atom/integer.hpp>
#include <algorithm>

namespace atom {

template<size_t length>
class ConstCharArray {
  public:
    constexpr ConstCharArray(const char(&src_array)[length]) {
      std::copy_n(src_array, length, m_data);
    }

    constexpr char operator[](size_t i) const {
      return m_data[i];
    }

  private:
    char m_data[length];
};

} // namespace atom
