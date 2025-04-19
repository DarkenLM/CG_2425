#pragma once

#include "common/geometry/BaseGeometry.hpp"

class TorusGeometry : public BaseGeometry {
   public:
    virtual ~TorusGeometry();
    TorusGeometry(int majorRadius, int minorRadius, int slices, int sides);
    TorusGeometry(std::vector<Point3D> vertices, std::vector<Vector3<float>> normals, std::vector<unsigned int> indices);

    virtual std::vector<Point3D> serielizeVertices() override;
    virtual std::vector<Vector3<float>> serielizeNormals() override;
    virtual std::vector<unsigned int> serielizeIndices() override;

    static TorusGeometry* deserialize(std::string filePath);
};