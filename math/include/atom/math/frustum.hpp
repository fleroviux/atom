
#pragma once

#include <atom/math/box3.hpp>
#include <atom/math/plane.hpp>

namespace atom {

  /**
   * A frustum defined by six inward-facing parametric planes,
   * one {@link #Plane} for each {@link #Side} of the Frustum.
   */
  class Frustum {
    public:
      /**
       * Enumerate the six sides (or faces) of a Frustum.
       */
      enum class Side {
        NX = 0, /**< negative-X */
        PX = 1, /**< positive-X */
        NY = 2, /**< negative-Y */
        PY = 3, /**< positive-Y */
        NZ = 4, /**< negative-Z */
        PZ = 5  /**< positive-Z */
      };

      /**
       * Get the parametric {@link #Plane} for one {@link #Side} of this Frustum.
       *
       * @param side the side (or face)
       * @return the parametric {@link #Plane}
       */
      [[nodiscard]] auto GetPlane(Side side) const -> Plane const& {
        return planes[(int)side];
      }

      /**
       * Set the parametric {@link #Plane} for one {@link #Side} of this Frustum.
       *
       * @param side the side (or face)
       * @param the parametric {@link #Plane}
       */
      void SetPlane(Side side, Plane const& plane) {
        planes[(int)side] = plane;
      }

      /**
       * Calculate whether an axis-aligned bounding box ({@link #Box3}) is
       * at least partially contained within this Frustum.
       *
       * @param box the bounding box
       * @return true if the {@link #Box3} is partially or fully inside this Frustum
       */
      [[nodiscard]] bool ContainsBox(Box3 const& box) const {
        for (auto& plane : planes) { // NOLINT(readability-use-anyofallof)
          auto point = Vector3{
            plane.X() > 0 ? box.Max().X() : box.Min().X(),
            plane.Y() > 0 ? box.Max().Y() : box.Min().Y(),
            plane.Z() > 0 ? box.Max().Z() : box.Min().Z()
          };

          if (plane.GetDistanceToPoint(point) < 0) {
            return false;
          }
        }

        return true;
      }

    private:
      Plane planes[6];
  };

} // namespace atom
