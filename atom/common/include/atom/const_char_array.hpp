
#pragma once

#include <atom/integer.hpp>
#include <algorithm>

namespace atom {

/**
 * A constant array of char which can be constructed for example from a string literal without decaying to const char*.
 * This is useful for passing string literals as template arguments.
 */
template<size_t length>
struct ConstCharArray {
  constexpr ConstCharArray(const char(&src_array)[length]) { // NOLINT(*-explicit-constructor)
    std::copy_n(src_array, length, m_data);
  }

  constexpr char operator[](size_t i) const {
    return m_data[i];
  }

  char m_data[length]{}; // @todo: This should be private, but can't be. Why?
};

} // namespace atom
