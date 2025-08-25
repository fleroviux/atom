
#pragma once

namespace atom {

  class NonMoveable {
    public:
      NonMoveable() = default;
      NonMoveable(const NonMoveable&) = delete;

      NonMoveable& operator=(const NonMoveable&) = delete;
  };

} // namespace atom
