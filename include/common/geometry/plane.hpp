#pragma once

#include "common/geometry/BaseGeometry.hpp"

class PlaneGeometry : public BaseGeometry {
    // protected:
    //     BaseGeometryKind _kind = GEOMETRY_PLANE;

   public:
    virtual ~PlaneGeometry();
    PlaneGeometry(int length, int gridSize);
    PlaneGeometry(std::vector<Point3D> vertices, std::vector<Vector3<float>> normals, std::vector<unsigned int> indices);

    virtual std::vector<Point3D> serielizeVertices() override;
    virtual std::vector<Vector3<float>> serielizeNormals() override;
    virtual std::vector<unsigned int> serielizeIndices() override;

    static PlaneGeometry* deserialize(std::string filePath);
};