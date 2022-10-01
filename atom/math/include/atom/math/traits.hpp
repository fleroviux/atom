
#pragma once

namespace atom {

  template<typename T>
  struct NumericConstants {
    // static constexpr auto Zero() -> T;
    // static constexpr auto One()  -> T;
  };

  template<>
  struct NumericConstants<float> {
    static constexpr auto Zero() -> float {
      return 0;
    }

    static constexpr auto One() -> float {
      return 1;
    }
  };

  template<>
  struct NumericConstants<double> {
    static constexpr auto Zero() -> double {
      return 0;
    }

    static constexpr auto One() -> double {
      return 1;
    }
  };

} // namespace atom
