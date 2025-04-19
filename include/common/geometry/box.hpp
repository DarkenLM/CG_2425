#pragma once

#include "common/geometry/BaseGeometry.hpp"

class BoxGeometry : public BaseGeometry {
   public:
    virtual ~BoxGeometry();
    BoxGeometry(int length, int grid_size);
    BoxGeometry(std::vector<Point3D> vertices, std::vector<Vector3<float>> normals, std::vector<unsigned int> indices);

    virtual std::vector<Point3D> serielizeVertices() override;
    virtual std::vector<Vector3<float>> serielizeNormals() override;
    virtual std::vector<unsigned int> serielizeIndices() override;

    static BoxGeometry* deserialize(std::string filePath);
};