
#pragma once

#include <atom/const_char_array.hpp>
#include <atom/integer.hpp>
#include <concepts>
#include <memory>
#include <type_traits>
#include <limits>

namespace atom::bit {

  template<typename T>
  constexpr auto number_of_bits() -> uint {
    return 8 * sizeof(T);
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

  namespace detail {

    template<typename T, ConstCharArray pattern, typename Functor, size_t base_i, size_t current_i, typename... Args>
    constexpr auto pattern_extract_impl(T value, Functor&& functor, Args&&... args) {
      if constexpr(base_i == number_of_bits<T>()) {
        return functor(std::forward<Args>(args)...);
      } else {
        constexpr char current_char = pattern[base_i];

        if constexpr(current_i == number_of_bits<T>() - 1u || pattern[current_i + 1u] != current_char) {
          // The current range ends here, either because the next character is different or because we hit the end of the pattern.
          if constexpr(current_char != '0' && current_char != '1' && current_char != '?') {
            // The character in this range isn't 0 or 1. This indicates that this is a bit field we're interested in.
            // Extract the bit field, append it to the parameter pack, then continue with the next range.
            constexpr size_t lsb = number_of_bits<T>() - current_i - 1u;
            constexpr size_t count = current_i - base_i + 1u;
            return pattern_extract_impl<T, pattern, Functor, current_i + 1u, current_i + 1u, Args..., T>(value, std::forward<Functor>(functor), std::forward<Args>(args)..., get_field(value, lsb, count));
          } else {
            // The current range contains a sequence of zeroes or ones, so we just ignore it and continue with the next range.
            return pattern_extract_impl<T, pattern, Functor, current_i + 1u, current_i + 1u, Args...>(value, std::forward<Functor>(functor), std::forward<Args>(args)...);
          }
        } else {
          // Next pattern character is same as current so just extent the range to the next character.
          return pattern_extract_impl<T, pattern, Functor, base_i, current_i + 1u, Args...>(value, std::forward<Functor>(functor), std::forward<Args>(args)...);
        }
      }
    }

  } // namespace atom::bit::detail

  template<typename T, ConstCharArray pattern, typename Functor>
  constexpr auto pattern_extract(T value, Functor&& functor) {
    return detail::pattern_extract_impl<T, pattern, Functor, 0u, 0u>(value, std::forward<Functor>(functor));
  }

  template<typename T>
  constexpr T ones = (T)std::numeric_limits<std::make_unsigned_t<T>>::max();

} // namespace atom::bit

namespace atom {

  template<uint bit, uint length, typename T>
  requires std::unsigned_integral<T>
  struct Bits {
    struct Bit {
      constexpr Bit(uint index, T& data) : index{index}, data{&data} {}

      constexpr operator unsigned() const {
        return (*data >> index) & 1u;
      }

      constexpr Bit& operator=(unsigned value) {
        *data = (*data & ~((T)1 << index)) | (value > 0 ? ((T)1 << index) : 0);
        return *this;
      }

      uint index;
      T* data;
    };

    template<typename U>
    constexpr explicit operator U() const {
      return (U)((data & mask) >> bit);
    }

    constexpr operator unsigned() const {
      return (data & mask) >> bit;
    }

    template<typename U>
    constexpr Bits& operator=(U value) {
      data = (data & ~mask) | (((T)value << bit) & mask);
      return *this;
    }

    constexpr Bit operator[](uint index) {
      return {bit + index, data};
    }

    constexpr bool operator[](uint index) const {
      return data & ((T)1 << (bit + index));
    }

    template<typename U>
    constexpr bool operator==(U const& rhs) const {
      return (this->operator U()) == rhs;
    }

    private:
      static constexpr T mask = bit::ones<T> >> (bit::number_of_bits<T>() - length) << bit;

      T data;
  };

} // namespace atom
