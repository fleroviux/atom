
#pragma once

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
        *data = (*data & ~(1u << index)) | (value > 0 ? (1u << index) : 0);
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

    template<typename U>
    constexpr bool operator==(U const& rhs) const {
      return (U)this == rhs;
    }

    private:
      static constexpr T mask = bit::ones<T> >> (bit::number_of_bits<T>() - length) << bit;

      T data;
  };

} // namespace atom
