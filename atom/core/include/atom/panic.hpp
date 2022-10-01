
#pragma once

#include <fmt/format.h>
#include <string>
#include <utility>

namespace atom {

  namespace detail {

    [[noreturn]] void CallPanicHandler(const char* file, int line, const char* message);

    template<typename... Args>
    [[noreturn]] void Panic(const char* file, int line, const char* format, Args&&... args) {
      std::string message = fmt::format(fmt::runtime(format), std::forward<Args>(args)...);

      CallPanicHandler(file, line, message.c_str());
    }

  } // namespace atom::detail

  typedef void (*PanicHandlerFn)(const char* file, int line, const char* message);

  void SetPanicHandler(PanicHandlerFn handler);

} // namespace atom

#define ATOM_PANIC(format, ...) atom::detail::Panic(__FILE__, __LINE__, format, ## __VA_ARGS__);
