#pragma once

#include "common/geometry/BaseGeometry.hpp"

class FlatRingGeometry : public BaseGeometry {
   public:
    virtual ~FlatRingGeometry();
    FlatRingGeometry(int majorRadius, int minorRadius, int slices);
    FlatRingGeometry(std::vector<Point3D> vertices, std::vector<Vector3<float>> normals, std::vector<unsigned int> indices);

    virtual std::vector<Point3D> serielizeVertices() override;
    virtual std::vector<Vector3<float>> serielizeNormals() override;
    virtual std::vector<unsigned int> serielizeIndices() override;

    static FlatRingGeometry* deserialize(std::string filePath);
};