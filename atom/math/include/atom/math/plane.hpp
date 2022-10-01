
#pragma once

#include <atom/math/vector.hpp>

namespace atom {

  namespace detail {

    /**
     * A parametric plane defined by a normal vector and a signed distance from the origin along the normal.
     *
     * @tparam T the underlying data type (i.e. float)
     * @tparam Vec3 the three-dimensional vector type to be used
     */
    template<typename T, typename Vec3>
    class Plane {
      public:
        /**
         * Default constructor. The plane is initialized to lie at (0, 0, 0) and point upwards (on the y-Axis).
         */
        Plane() = default;

        /**
         * Construct a Plane from a normal vector and a signed distance.
         *
         * @param normal the normal vector
         * @param distance the signed distance from the origin
         */
        explicit Plane(Vec3 const& normal, T distance = NumericConstants<T>::zero())
          : normal(normal), distance(distance) {
        }

        [[nodiscard]] auto X() -> T& { return normal.X(); }
        [[nodiscard]] auto Y() -> T& { return normal.Y(); }
        [[nodiscard]] auto Z() -> T& { return normal.Z(); }
        [[nodiscard]] auto W() -> T& { return normal.W(); }

        [[nodiscard]] auto X() const -> T { return normal.X(); }
        [[nodiscard]] auto Y() const -> T { return normal.Y(); }
        [[nodiscard]] auto Z() const -> T { return normal.Z(); }
        [[nodiscard]] auto W() const -> T { return normal.W(); }

        /**
         * Get the normal vector of this plane.
         * @return the normal vector
         */
        [[nodiscard]] auto GetNormal() const -> Vec3 {
          return normal;
        }

        /**
         * Set the new_normal vector for this plane.
         *
         * @param new_normal the normal vector
         */
        void SetNormal(Vec3 const& new_normal) {
          normal = new_normal;
        }

        /**
         * Get the signed distance of this plane from the origin.
         */
        [[nodiscard]] auto GetDistance() const -> T {
          return distance;
        }

        /**
         * Set the signed distance from the origin for this plane.
         */
        void SetDistance(T new_distance) {
          distance = new_distance;
        }

        /**
         * Compute the signed distance of a point from this plane.
         *
         * @param point the point
         * @return the signed distance
         */
        [[nodiscard]] auto GetDistanceToPoint(Vec3 const& point) const -> T {
          return point.X() * normal.X() +
                 point.Y() * normal.Y() +
                 point.Z() * normal.Z() - distance;
        }

      private:
        Vec3 normal{
          NumericConstants<T>::Zero(),
          NumericConstants<T>::One(),
          NumericConstants<T>::Zero()
        };
        T distance = NumericConstants<T>::Zero();
    };

  } // namespace atom::detail

  using Plane = detail::Plane<float, Vector3>;

} // namespace atom
