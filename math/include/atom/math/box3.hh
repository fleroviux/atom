
#pragma once

#include <algorithm>
#include <atom/math/matrix4.hh>
#include <limits>

namespace atom {

  /**
   * A 3D axis-aligned bounding box (AABB).
   * The bounding box is defined through a minimum and a maximum vector.
   * Because the bounding box is axis-aligned, its eight vertices can be retrieved like this:
   *
   *   v0 = (min.x, min.y, min.z)
   *   v1 = (max.x, min.y, min.z)
   *   v2 = (min.x, max.y, min.z)
   *   v3 = (max.x, max.y, min.z)
   *   v4 = (min.x, min.y, max.z)
   *   v5 = (max.x, min.y, max.z)
   *   v6 = (min.x, max.y, max.z)
   *   v7 = (max.x, max.y, max.z)
   */
  class Box3 {
    public:
      [[nodiscard]] auto Min() -> Vector3& { return min; }
      [[nodiscard]] auto Max() -> Vector3& { return max; }

      [[nodiscard]] auto Min() const -> Vector3 const& { return min; }
      [[nodiscard]] auto Max() const -> Vector3 const& { return max; }

      /**
       * Apply a matrix transform on each vertex of this bounding box.
       * Because the new bounding box must be axis-aligned new mininum and maximum
       * vectors will be computed to fit the transformed bounding box.
       *
       * @param matrix the matrix transform
       * @return the transformed bounding box
       */
      [[nodiscard]] auto ApplyMatrix(Matrix4 const& matrix) const -> Box3 {
        Box3 box;

        auto min_x = matrix.X().XYZ() * min.X();
        auto max_x = matrix.X().XYZ() * max.X();
        auto min_y = matrix.Y().XYZ() * min.Y();
        auto max_y = matrix.Y().XYZ() * max.Y();
        auto min_z = matrix.Z().XYZ() * min.Z();
        auto max_z = matrix.Z().XYZ() * max.Z();
        auto translation = matrix.W().XYZ();

        Vector3 vertices[8];
        vertices[0] = min_x + min_y + min_z + translation;
        vertices[1] = max_x + min_y + min_z + translation;
        vertices[2] = min_x + max_y + min_z + translation;
        vertices[3] = max_x + max_y + min_z + translation;
        vertices[4] = min_x + min_y + max_z + translation;
        vertices[5] = max_x + min_y + max_z + translation;
        vertices[6] = min_x + max_y + max_z + translation;
        vertices[7] = max_x + max_y + max_z + translation;

        box.min = Vector3{
          +std::numeric_limits<float>::infinity(),
          +std::numeric_limits<float>::infinity(),
          +std::numeric_limits<float>::infinity()
        };

        box.max = Vector3{
          -std::numeric_limits<float>::infinity(),
          -std::numeric_limits<float>::infinity(),
          -std::numeric_limits<float>::infinity()
        };

        for (auto & vertex : vertices) {
          box.min.X() = std::min(box.min.X(), vertex.X());
          box.min.Y() = std::min(box.min.Y(), vertex.Y());
          box.min.Z() = std::min(box.min.Z(), vertex.Z());

          box.max.X() = std::max(box.max.X(), vertex.X());
          box.max.Y() = std::max(box.max.Y(), vertex.Y());
          box.max.Z() = std::max(box.max.Z(), vertex.Z());
        }

        return box;
      }

    private:
      Vector3 min; /**< the lower-left vertex */
      Vector3 max; /**< the upper-right vertex */
  };

} // namespace atom
