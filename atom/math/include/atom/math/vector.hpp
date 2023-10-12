
#pragma once

#include <atom/integer.hpp>
#include <atom/math/traits.hpp>
#include <cmath>

namespace atom {

  namespace detail {

    /**
     * Generic vector template on type `T` with dimension `n`.
     *
     * @tparam Derived the final inheriting class
     * @tparam T       the underlying data type (i.e. float)
     * @tparam n       the number of components (or dimensions)
     */
    template<typename Derived, typename T, uint n>
    class Vector {
      public:
        /**
         * Default constructor. The vector will be zero-initialised.
         */
        Vector() = default;

        /**
         * Access a component of the vector via its index (between `0` and `n - 1`).
         * Out-of-bounds access is undefined behaviour.
         *
         * @param i the index
         * @return a reference to the component value
         */
        [[nodiscard]] auto operator[](int i) -> T& {
          return data[i];
        }

        /**
         * Read a component of the vector via its index (between `0` and `n - 1`).
         * Out-of-bounds access is undefined behaviour.
         *
         * @param i the index
         * @return the component value
         */
        [[nodiscard]] auto operator[](int i) const -> T {
          return data[i];
        }

        /**
         * Perform a component-wise summation of this vector with another vector.
         * Store the result in a new vector.
         *
         * @param other the other vector
         * @return the result vector
         */
        [[nodiscard]] auto operator+(Derived const& other) const -> Derived {
          Derived result{};
          for (uint i = 0; i < n; i++)
            result[i] = data[i] + other[i];
          return result;
        }

        /**
         * Perform a component-wise subtraction of another vector from this vector.
         * Store the result in a new vector.
         *
         * @param other the other vector
         * @return the result vector
         */
        [[nodiscard]] auto operator-(Derived const& other) const -> Derived {
          Derived result{};
          for (uint i = 0; i < n; i++)
            result[i] = data[i] - other[i];
          return result;
        }

        /**
         * Multiply each component of this vector with a scalar value.
         * Store the result in a new vector.
         *
         * @param value the scalar value
         * @return the result vector
         */
        [[nodiscard]] auto operator*(T value) const -> Derived {
          Derived result{};
          for (uint i = 0; i < n; i++)
            result[i] = data[i] * value;
          return result;
        }

        /**
         * Divide each component of this vector by a scalar value.
         * Store the result in a new vector.
         *
         * @param value the scalar value
         * @return the result vector
         */
        [[nodiscard]] auto operator/(T value) const -> Derived {
          Derived result{};
          for (uint i = 0; i < n; i++)
            result[i] = data[i] / value;
          return result;
        }

        /**
         * Perform a component-wise summation of this vector with another vector.
         * Store the result in this vector.
         *
         * @param other the other vector
         * @return a reference to this vector
         */
        auto operator+=(Derived const& other) -> Derived& {
          for (uint i = 0; i < n; i++)
            data[i] += other[i];
          return *static_cast<Derived*>(this);
        }

        /**
         * Perform a component-wise subtraction of another vector from this vector.
         * Store the result in this vector.
         *
         * @param other the other vector
         * @return a reference to this vector
         */
        auto operator-=(Derived const& other) -> Derived& {
          for (uint i = 0; i < n; i++)
            data[i] -= other[i];
          return *static_cast<Derived*>(this);
        }

        /**
         * Multiply each component of this vector with a scalar value.
         * Store the result in this vector.
         *
         * @param value the scalar value
         * @return a reference to this vector
         */
        auto operator*=(T value) -> Derived& {
          for (uint i = 0; i < n; i++)
            data[i] *= value;
          return *static_cast<Derived*>(this);
        }

        /**
         * Divide each component of this vector by a scalar value.
         * Store the result in this vector.
         *
         * @param value the scalar value
         * @return a reference to this vector
         */
        auto operator/=(T value) -> Derived& {
          for (uint i = 0; i < n; i++)
            data[i] /= value;
          return *static_cast<Derived*>(this);
        }

        /**
         * Negate each component in this vector. Store the result in a new vector.
         * @return the result vector
         */
        [[nodiscard]] auto operator-() const -> Derived {
          Derived result{};
          for (uint i = 0; i < n; i++)
            result[i] = -data[i];
          return result;
        }

        /**
         * Perform a component-wise equality-comparison of this vector with another vector.
         * @return true if all components are equal.
         */
        [[nodiscard]] bool operator==(Derived const& other) const {
          for (uint i = 0; i < n; i++)
            if (data[i] != other[i])
              return false;
          return true;
        }

        /**
         * Perform a component-wise inequality-comparison of this vector with another vector.
         * @return true if at least one component in unequal.
         */
        [[nodiscard]] bool operator!=(Derived const& other) const {
          return !(*this == other);
        }

        /**
         * Calculate the dot product between this vector and another vector.
         * See: https://tutorial.math.lamar.edu/classes/calcii/dotproduct.aspx
         *
         * @param other the other vector
         * @return the scalar result
         */
        [[nodiscard]] auto Dot(Derived const& other) const -> T {
          T result{};
          for (uint i = 0; i < n; i++)
            result += data[i] * other[i];
          return result;
        }

        /**
         * Perform linear interpolation between two vectors with a factor between `0` and `1`.
         *
         * @param a the vector at `factor = 0`
         * @param b the vector at `factor = 1`
         * @return the interpolated result vector
         */
        [[nodiscard]] static auto Lerp(Derived const& a, Derived const& b, T factor) -> Derived {
          Derived result{};
          T one_minus_factor = NumericConstants<T>::One() - factor;
          for (uint i = 0; i < n; i++)
            result[i] = a[i] * one_minus_factor + b[i] * factor;
          return result;
        }

      protected:
        T data[n] { NumericConstants<T>::Zero() }; /**< the `n` components of the vector. */
    };

    /**
     * A two-dimensional vector template on type `T`
     *
     * @tparam Derived the final inheriting class
     * @tparam T       the underlying data type (i.e. float)
     */
    template<typename Derived, typename T>
    class Vector2 : public Vector<Derived, T, 2> {
      public:
        using Vector<Derived, T, 2>::Vector;

        /**
         * Construct a Vector2 from two scalar values.
         */
        Vector2(T x, T y) {
          this->data[0] = x;
          this->data[1] = y;
        }

        [[nodiscard]] auto X() -> T& { return this->data[0]; }
        [[nodiscard]] auto Y() -> T& { return this->data[1]; }

        [[nodiscard]] auto X() const -> T { return this->data[0]; }
        [[nodiscard]] auto Y() const -> T { return this->data[1]; }
    };

    /**
     * A three-dimensional vector template on type `T`
     *
     * @tparam Derived the final inheriting class
     * @tparam T       the underlying data type (i.e. float)
     */
    template<typename Derived, typename T>
    class Vector3 : public Vector<Derived, T, 3> {
      public:
        using Vector<Derived, T, 3>::Vector;

        /**
         * Construct a Vector3 from three scalar values.
         */
        Vector3(T x, T y, T z) {
          this->data[0] = x;
          this->data[1] = y;
          this->data[2] = z;
        }

        [[nodiscard]] auto X() -> T& { return this->data[0]; }
        [[nodiscard]] auto Y() -> T& { return this->data[1]; }
        [[nodiscard]] auto Z() -> T& { return this->data[2]; }

        [[nodiscard]] auto X() const -> T { return this->data[0]; }
        [[nodiscard]] auto Y() const -> T { return this->data[1]; }
        [[nodiscard]] auto Z() const -> T { return this->data[2]; }

        /**
         * Calculate the cross product of this vector with another vector.
         * See: https://tutorial.math.lamar.edu/classes/calcii/crossproduct.aspx
         *
         * @param other the other vector
         * @returm the result vector
         */
        [[nodiscard]] auto Cross(Vector3 const& other) const -> Vector3 {
          return {
            this->data[1] * other[2] - this->data[2] * other[1],
            this->data[2] * other[0] - this->data[0] * other[2],
            this->data[0] * other[1] - this->data[1] * other[0]
          };
        }
    };

    /**
     * A four-dimensional vector template on type `T`
     *
     * @tparam Derived the final inheriting class
     * @tparam Vec3    the three-dimensional vector type to be used
     * @tparam T       the underlying data type (i.e. float)
     */
    template<typename Derived, typename Vec3, typename T>
    class Vector4 : public Vector<Derived, T, 4> {
      public:
        using Vector<Derived, T, 4>::Vector;

        /**
         * Construct a Vector4 from four scalar values.
         */
        Vector4(T x, T y, T z, T w) {
          this->data[0] = x;
          this->data[1] = y;
          this->data[2] = z;
          this->data[3] = w;
        }

        /**
         * Construct a Vector4 from a Vector3 and a scalar w-component.
         */
        explicit Vector4(Vec3 const& xyz, T w = NumericConstants<T>::One()) {
          this->data[0] = xyz.X();
          this->data[1] = xyz.Y();
          this->data[2] = xyz.Z();
          this->data[3] = w;
        }

        [[nodiscard]] auto X() -> T& { return this->data[0]; }
        [[nodiscard]] auto Y() -> T& { return this->data[1]; }
        [[nodiscard]] auto Z() -> T& { return this->data[2]; }
        [[nodiscard]] auto W() -> T& { return this->data[3]; }

        [[nodiscard]] auto X() const -> T { return this->data[0]; }
        [[nodiscard]] auto Y() const -> T { return this->data[1]; }
        [[nodiscard]] auto Z() const -> T { return this->data[2]; }
        [[nodiscard]] auto W() const -> T { return this->data[3]; }

        [[nodiscard]] auto XYZ() const -> Vec3 { return Vec3{X(), Y(), Z()}; }
    };

  } // namespace atom::detail

  /**
   * A two-dimensional float vector
   */
  class Vector2 final : public detail::Vector2<Vector2, float> {
    public:
      using detail::Vector2<Vector2, float>::Vector2;
  };

  /**
   * A three-dimensional float vector
   */
  class Vector3 final : public detail::Vector3<Vector3, float> {
    public:
      using detail::Vector3<Vector3, float>::Vector3;

      /**
       * Calculate the euclidean length of this vector.
       * @return the calculated length
       */
      [[nodiscard]] auto Length() const -> float {
        return std::sqrt(Dot(*this));
      }

      /**
       * Set the euclidean length of this vector to one while preserving its direction.
       * If this vector is the zero vector then this operation is undefined.
       * @return a reference to this vector
       */
      auto Normalize() -> Vector3& {
        *this *= 1.0f / Length();
        return *this;
      }
  };

  /**
   * A four-dimensional float vector
   */
  class Vector4 final : public detail::Vector4<Vector4, Vector3, float> {
    public:
      using detail::Vector4<Vector4, Vector3, float>::Vector4;
  };

} // namespace atom
