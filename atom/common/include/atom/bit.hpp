
#pragma once

#include <atom/integer.hpp>
#include <climits>
#include <memory>
#include <type_traits>

namespace atom::bit {

  template<typename T>
  constexpr auto number_of_bits() -> uint {
    return CHAR_BIT * sizeof(T);
  }

  template<typename T, typename U = T>
  constexpr auto get_bit(T value, uint bit) -> U {
    return static_cast<U>((value >> bit) & 1);
  }

  template<typename T, typename U = T>
  constexpr auto get_field(T value, uint lowest_bit, uint count) -> U {
    return static_cast<U>((value >> lowest_bit) & ~(static_cast<T>(-1) << count));
  }

  template<typename T>
  constexpr auto rotate_right(T value, uint amount) -> T {
    auto bits = number_of_bits<T>();
    if (amount == 0)
      return value;
    return (value >> amount) | (value << (bits - amount));
  }

  namespace detail {
    template<typename T>
    constexpr auto build_pattern_mask(const char* pattern) -> T {
      auto result = T{};
      auto i = 0U;
      auto bits = number_of_bits<T>();
      while (i < bits && pattern[i] != 0) {
        if (pattern[i] == '0' || pattern[i] == '1')
          result |= 1ULL << (bits - i - 1);
        i++;
      }
      result >>= bits - i;
      return result;
    }

    template<typename T>
    constexpr auto build_pattern_value(const char* pattern) -> T {
      auto result = T{};
      auto i = 0U;
      auto bits = number_of_bits<T>();
      while (i < bits && pattern[i] != 0) {
        if (pattern[i] == '1')
          result |= 1ULL << (bits - i - 1);
        i++;
      }
      result >>= bits - i;
      return result;
    }
  } // namespace atom::bit::detail

  template<typename T>
  constexpr auto match_pattern(T value, const char* pattern) -> bool {
    return (value & detail::build_pattern_mask<T>(pattern)) == detail::build_pattern_value<T>(pattern);
  }

} // namespace atom::bit
