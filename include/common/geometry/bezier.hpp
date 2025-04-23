#pragma once

#include <array>
#include <functional>
#include <string>
#include <vector>

#include "common/geometry/BaseGeometry.hpp"
#include "common/geometry/point.hpp"

class BezierGeometry : public BaseGeometry {
   public:
    virtual ~BezierGeometry();

    // Main constructor with filename and tessellation level
    BezierGeometry(const char* filename, int tessellationLevel);

    // Alternative constructor for pre-computed geometry
    BezierGeometry(std::vector<Point3D> vertices,
                   std::vector<Vector3<float>> normals,
                   std::vector<unsigned int> indices);

    // Serialization methods
    virtual std::vector<Point3D> serielizeVertices() override;
    virtual std::vector<Vector3<float>> serielizeNormals() override;
    virtual std::vector<unsigned int> serielizeIndices() override;

    // Static deserialization method
    static BezierGeometry* deserialize(std::string filePath);

   private:
    // Helper method for tessellating Bezier patches
    void tessellateBezierPatch(
        const std::array<int, 16>& patchIndices,
        const std::vector<Point3D>& controlPoints,
        int tessellationLevel,
        const std::function<unsigned int(const Point3D&, const Vector3<float>&)>& addVertex);
};