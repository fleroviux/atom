
#include <atom/panic.hpp>
#include <cstdlib>
#include <fmt/color.h>

namespace atom {

  void DefaultPanicHandler(const char* file, int line, const char* message) {
    fmt::print(fmt::fg(fmt::color::red), "panic: {}:{}: {}", file, line, message);
  }

  static PanicHandlerFn g_panic_handler_fn = &DefaultPanicHandler;

  namespace detail {

    [[noreturn]] void CallPanicHandler(const char* file, int line, const char* message) {
      g_panic_handler_fn(file, line, message);
      std::exit(-1);
    }

  } // namespace atom::detail

  void SetPanicHandler(PanicHandlerFn handler) {
    g_panic_handler_fn = handler;
  }

} // namespace atom