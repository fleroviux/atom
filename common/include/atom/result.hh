
#pragma once

#include <atom/panic.hh>
#include <algorithm>
#include <memory>
#include <type_traits>

namespace atom {

  #define ATOM_RESULT_EMPTY (-1)
  #define ATOM_RESULT_SUCCESS 0

  #define ATOM_FORWARD_ERROR(result) do { \
    const auto status_code = (result).Code(); \
    if(status_code != ATOM_RESULT_SUCCESS) { \
      return status_code;\
    } \
  } while(0)

  template<typename StatusCode, typename T>
    requires std::is_move_constructible_v<T>
  class Result {
    public:
      Result(StatusCode status_code) : m_status_code{status_code} {}

      Result(const T& value) requires std::copy_constructible<T> : m_status_code{(StatusCode)ATOM_RESULT_SUCCESS}, m_value{value} {}

      Result(T&& value) : m_status_code{(StatusCode)ATOM_RESULT_SUCCESS}, m_value{std::move(value)} {}

      Result(const Result&) = delete;

      Result(Result&& other_result) {
        operator=(other_result);
      }

     ~Result() { // needed if `T` has a non-trivial destructor since m_value is in a union
        if(Ok()) {
          m_value.~T();
        }
      }

      [[nodiscard]] bool Ok() const {
        return m_status_code == (StatusCode)ATOM_RESULT_SUCCESS;
      }

      [[nodiscard]] StatusCode Code() const {
        return m_status_code;
      }

      [[nodiscard]] T Unwrap() {
        if(!Ok()) {
          ATOM_PANIC("Attempted to unwrap an error result (status code: {})", (int)m_status_code);
        }
        m_status_code = (StatusCode)ATOM_RESULT_EMPTY;
        return std::move(m_value);
      }

      [[nodiscard]] T UnwrapOr(const T& fallback) requires std::is_copy_constructible_v<T> {
        if(!Ok()) {
          return fallback;
        }
        m_status_code = (StatusCode)ATOM_RESULT_EMPTY;
        return std::move(m_value);
      }

      Result& operator=(const Result&) = delete;

      Result& operator=(Result&& other_result) {
        std::swap(m_status_code, other_result.m_status_code);

        // As per C++11 standard a union member can only be initialized either during
        // initialization of the union or via placement-new. See: https://stackoverflow.com/a/33058919
        new (&m_value) T(std::move(other_result.m_value));
        return *this;
      }

    private:
      StatusCode m_status_code{};
      union { // Union used to avoid default initialization
        T m_value;
      };
  };

} // namespace atom
