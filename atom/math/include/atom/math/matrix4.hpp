
#pragma once

#include <array>
#include <cmath>
#include <atom/math/vector.hpp>

#ifndef M_PI
  #define M_PI 3.14159265358979323
#endif

namespace atom {

  namespace detail {

    /**
     * Generic 4x4 matrix template on type `T`.
     *
     * @tparam Derived the final inheriting class
     * @tparam Vec4    the four-dimensional vector type to be used
     * @tparam T       the underlying data type (i.e. float)
     */
    template<typename Derived, typename Vec4, typename T>
    class Matrix4 {
      public:
        /**
         * Default constructor.
         */
        Matrix4() = default;

        /**
         * Construct a Matrix4 from an array of scalars in row-major order.
         */
        explicit Matrix4(std::array<T, 16> const& elements) {
          for (uint i = 0; i < 16; i++)
            data[i & 3][i >> 2] = elements[i];
        }

        /**
         * Access a column vector of this matrix via its index (between `0` and `3`)
         *
         * @param i the index
         * @return a reference to the column vector
         */
        [[nodiscard]] auto operator[](int i) -> Vec4& {
          return data[i];
        }

        /**
         * Read a column vector of this matrix via its index (between `0` and `3`)
         *
         * @param i the index
         * @return a const-reference to the column vector
         */
        [[nodiscard]] auto operator[](int i) const -> Vec4 const& {
          return data[i];
        }

        [[nodiscard]] auto X() -> Vec4& { return data[0]; }
        [[nodiscard]] auto Y() -> Vec4& { return data[1]; }
        [[nodiscard]] auto Z() -> Vec4& { return data[2]; }
        [[nodiscard]] auto W() -> Vec4& { return data[3]; }

        [[nodiscard]] auto X() const -> Vec4 const& { return data[0]; }
        [[nodiscard]] auto Y() const -> Vec4 const& { return data[1]; }
        [[nodiscard]] auto Z() const -> Vec4 const& { return data[2]; }
        [[nodiscard]] auto W() const -> Vec4 const& { return data[3]; }

        /**
         * Apply this matrix on a four-dimensional vector.
         *
         * @param vec the vector
         * @return the result vector
         */
        [[nodiscard]] auto operator*(Vec4 const& vec) const -> Vec4 {
          Vec4 result{};
          for (uint i = 0; i < 4; i++)
            result += data[i] * vec[i];
          return result;
        }

        /**
         * Apply this matrix on each column vector of another matrix.
         * Store the result in a new matrix.
         *
         * @param other the other matrix
         * @return the result matrix
         */
        [[nodiscard]] auto operator*(Derived const& other) const -> Derived {
          Derived result{};
          for (uint i = 0; i < 4; i++)
            result[i] = *this * other[i];
          return result;
        }

        /**
         * Calculate the inverse of this matrix.
         * If this matrix is not invertable (determinant = 0) then the operation is undefined.
         * @return the inverted matrix
         */
        [[nodiscard]] auto Inverse() const -> Derived {
          // Adapted from this code: https://stackoverflow.com/a/44446912
          auto a2323 = data[2][2] * data[3][3] - data[3][2] * data[2][3];
          auto a1323 = data[1][2] * data[3][3] - data[3][2] * data[1][3];
          auto a1223 = data[1][2] * data[2][3] - data[2][2] * data[1][3];
          auto a0323 = data[0][2] * data[3][3] - data[3][2] * data[0][3];
          auto a0223 = data[0][2] * data[2][3] - data[2][2] * data[0][3];
          auto a0123 = data[0][2] * data[1][3] - data[1][2] * data[0][3];
          auto a2313 = data[2][1] * data[3][3] - data[3][1] * data[2][3];
          auto a1313 = data[1][1] * data[3][3] - data[3][1] * data[1][3];
          auto a1213 = data[1][1] * data[2][3] - data[2][1] * data[1][3];
          auto a2312 = data[2][1] * data[3][2] - data[3][1] * data[2][2];
          auto a1312 = data[1][1] * data[3][2] - data[3][1] * data[1][2];
          auto a1212 = data[1][1] * data[2][2] - data[2][1] * data[1][2];
          auto a0313 = data[0][1] * data[3][3] - data[3][1] * data[0][3];
          auto a0213 = data[0][1] * data[2][3] - data[2][1] * data[0][3];
          auto a0312 = data[0][1] * data[3][2] - data[3][1] * data[0][2];
          auto a0212 = data[0][1] * data[2][2] - data[2][1] * data[0][2];
          auto a0113 = data[0][1] * data[1][3] - data[1][1] * data[0][3];
          auto a0112 = data[0][1] * data[1][2] - data[1][1] * data[0][2];

          auto det = data[0][0] * (data[1][1] * a2323 - data[2][1] * a1323 + data[3][1] * a1223)
                   - data[1][0] * (data[0][1] * a2323 - data[2][1] * a0323 + data[3][1] * a0223)
                   + data[2][0] * (data[0][1] * a1323 - data[1][1] * a0323 + data[3][1] * a0123)
                   - data[3][0] * (data[0][1] * a1223 - data[1][1] * a0223 + data[2][1] * a0123);

          auto recip_det = 1 / det;

          return Derived{{
            recip_det *  (data[1][1] * a2323 - data[2][1] * a1323 + data[3][1] * a1223),
            recip_det * -(data[1][0] * a2323 - data[2][0] * a1323 + data[3][0] * a1223),
            recip_det *  (data[1][0] * a2313 - data[2][0] * a1313 + data[3][0] * a1213),
            recip_det * -(data[1][0] * a2312 - data[2][0] * a1312 + data[3][0] * a1212),
            recip_det * -(data[0][1] * a2323 - data[2][1] * a0323 + data[3][1] * a0223),
            recip_det *  (data[0][0] * a2323 - data[2][0] * a0323 + data[3][0] * a0223),
            recip_det * -(data[0][0] * a2313 - data[2][0] * a0313 + data[3][0] * a0213),
            recip_det *  (data[0][0] * a2312 - data[2][0] * a0312 + data[3][0] * a0212),
            recip_det *  (data[0][1] * a1323 - data[1][1] * a0323 + data[3][1] * a0123),
            recip_det * -(data[0][0] * a1323 - data[1][0] * a0323 + data[3][0] * a0123),
            recip_det *  (data[0][0] * a1313 - data[1][0] * a0313 + data[3][0] * a0113),
            recip_det * -(data[0][0] * a1312 - data[1][0] * a0312 + data[3][0] * a0112),
            recip_det * -(data[0][1] * a1223 - data[1][1] * a0223 + data[2][1] * a0123),
            recip_det *  (data[0][0] * a1223 - data[1][0] * a0223 + data[2][0] * a0123),
            recip_det * -(data[0][0] * a1213 - data[1][0] * a0213 + data[2][0] * a0113),
            recip_det *  (data[0][0] * a1212 - data[1][0] * a0212 + data[2][0] * a0112)
          }};
        }

        /**
         * Get a new identity matrix.
         * @return the identity matrix
         */
        [[nodiscard]] static auto Identity() -> Derived {
          Derived result{};

          for (uint row = 0; row < 4; row++) {
            for (uint col = 0; col < 4; col++) {
              if (row == col) {
                result[col][row] = NumericConstants<T>::One();
              } else {
                result[col][row] = NumericConstants<T>::Zero();
              }
            }
          }

          return result;
        }

      private:
        Vec4 data[4] {}; /**< the four basis vectors of the matrix. */
    };

  } // namespace atom::detail

  /**
   * A 4x4 float matrix
   */
  class Matrix4 final : public detail::Matrix4<Matrix4, Vector4, float> {
    public:
      using detail::Matrix4<Matrix4, Vector4, float>::Matrix4;

      /**
       * Create a 3D scale matrix from three scalar values.
       *
       * @param x x-axis scale
       * @param y y-axis scale
       * @param z z-axis scale
       * @return the scale matrix
       */
      [[nodiscard]] static auto Scale(float x, float y, float z) -> Matrix4 {
        return Matrix4{{
          x, 0, 0, 0,
          0, y, 0, 0,
          0, 0, z, 0,
          0, 0, 0, 1
        }};
      }

      /**
       * Create a 3D scale matrix from a Vector3.
       *
       * @param vec the Vector3 that encodes the x-, y- and z-axis scale
       * @return the scale matrix
       */
      [[nodiscard]] static auto Scale(Vector3 const& vec) -> Matrix4 {
        return Scale(vec.X(), vec.Y(), vec.Z());
      }

      /**
       * Create a x-Axis rotation matrix from an angle.
       *
       * @param radians the angle in radians
       * @return the rotation matrix
       */
      [[nodiscard]] static auto RotationX(float radians) -> Matrix4 {
        auto cos = std::cos(radians);
        auto sin = std::sin(radians);

        return Matrix4{{
          1,   0,    0, 0,
          0, cos, -sin, 0,
          0, sin,  cos, 0,
          0,   0,    0, 1
        }};
      }

      /**
       * Create a y-Axis rotation matrix from an angle.
       *
       * @param radians the angle in radians
       * @return the rotation matrix
       */
      [[nodiscard]] static auto RotationY(float radians) -> Matrix4 {
        auto cos = std::cos(radians);
        auto sin = std::sin(radians);

        return Matrix4{{
          cos, 0,  sin, 0,
          0,   1,    0, 0,
         -sin, 0,  cos, 0,
          0,   0,    0, 1
        }};
      }

      /**
       * Create a z-Axis rotation matrix from an angle.
       *
       * @param radians the angle in radians
       * @return the rotation matrix
       */
      [[nodiscard]] static auto RotationZ(float radians) -> Matrix4 {
        auto cos = std::cos(radians);
        auto sin = std::sin(radians);

        return Matrix4{{
          cos, -sin, 0, 0,
          sin,  cos, 0, 0,
            0,    0, 1, 0,
            0,    0, 0, 1
        }};
      }

      /**
       * Create a 3D translation matrix from three scalar values.
       *
       * @param x x-axis translation
       * @param y y-axis translation
       * @param z z-axis translation
       * @return the translation matrix
       */
      [[nodiscard]] static auto Translation(float x, float y, float z) -> Matrix4 {
        return Matrix4{{
          1, 0, 0, x,
          0, 1, 0, y,
          0, 0, 1, z,
          0, 0, 0, 1
        }};
      }

      /**
       * Create a 3D translation matrix from a Vector3.
       *
       * @param vec the Vector3 that encodes the x-, y- and z-axis translation
       * @return the translation matrix
       */
      [[nodiscard]] static auto Translation(Vector3 const& vec) -> Matrix4 {
        return Matrix4::Translation(vec.X(), vec.Y(), vec.Z());
      }

      /**
       * Create a perspective projection for OpenGL (negative z-Axis is forward; `-1` to `+1` depth range)
       *
       * @param fov_y        vertical field-of-view (radians)
       * @param aspect_ratio ratio of width to height
       * @param near         near clipping plane distance (from origin)
       * @param far          far clipping plane distance (from origin)
       * @return the projection matrix
       */
      [[nodiscard]] static auto PerspectiveGL(
        float fov_y,
        float aspect_ratio,
        float near,
        float far
      ) -> Matrix4 {
        // cot(fov_y/2) = tan((pi - fov_y)/2)
        auto y = std::tan(((float)M_PI - fov_y) * 0.5f);
        auto x = y / aspect_ratio;

        auto a = 1 / (near - far);
        auto b = (far + near) * a;
        auto c = 2 * far * near * a;

        return Matrix4{{
          x, 0,  0, 0,
          0, y,  0, 0,
          0, 0,  b, c,
          0, 0, -1, 0
        }};
      }

      /**
       * Create a perspective projection for Vulkan (negative z-Axis is forward; `0` to `1` depth range)
       *
       * @param fov_y        vertical field-of-view (radians)
       * @param aspect_ratio ratio of width to height
       * @param near         near clipping plane distance (from origin)
       * @param far          far clipping plane distance (from origin)
       * @return the projection matrix
       */
      [[nodiscard]] static auto PerspectiveVK(
        float fov_y,
        float aspect_ratio,
        float near,
        float far
      ) -> Matrix4 {
        // cot(fov_y/2) = tan((pi - fov_y)/2)
        auto y = std::tan(((float)M_PI - fov_y) * 0.5f);
        auto x = y / aspect_ratio;

        auto a = 1 / (near - far);
        auto b = (far + near) * a * 0.5f - 0.5f;
        auto c = far * near * a;

        return Matrix4{{
          x, 0,  0, 0,
          0, y,  0, 0,
          0, 0,  b, c,
          0, 0, -1, 0
        }};
      }

      /**
       * Create an orthographic projection for OpenGL (negative z-Axis is forward; `-1` to `+1` depth range)
       *
       * @param left   left clipping plane distance
       * @param right  right clipping plane distance
       * @param bottom bottom clipping plane distance
       * @param top    top clipping plane distance
       * @param near   near clipping plane distance
       * @param far    far clipping plane distance
       * @return the projection matrix
       */
      [[nodiscard]] static auto OrthographicGL(
        float left,
        float right,
        float bottom,
        float top,
        float near,
        float far
      ) -> Matrix4 {
        auto w = right - left;
        auto h = top - bottom;
        auto d = far - near;

        auto tx = -(right + left) / w;
        auto ty = -(top + bottom) / h;
        auto tz = -(far + near) / d;

        auto x =  2.0f / w;
        auto y =  2.0f / h;
        auto z = -2.0f / d;

        return Matrix4{{
          x, 0, 0, tx,
          0, y, 0, ty,
          0, 0, z, tz,
          0, 0, 0, 1
        }};
      }
  };

} // namespace atom
