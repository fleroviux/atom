
#pragma once

namespace atom {

  class NonCopyable {
    public:
      NonCopyable() = default;
      NonCopyable(const NonCopyable&) = delete;

      NonCopyable& operator=(const NonCopyable&) = delete;
  };

} // namespace atom
