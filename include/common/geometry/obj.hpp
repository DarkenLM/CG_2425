#pragma once

#include "common/geometry/BaseGeometry.hpp"

class ObjGeometry : public BaseGeometry {
   public:
    virtual ~ObjGeometry();
    ObjGeometry(std::vector<Point3D> vertices, std::vector<Vector3<float>> normals, std::vector<unsigned int> indices);

    virtual std::vector<Point3D> serielizeVertices() override;
    virtual std::vector<Vector3<float>> serielizeNormals() override;
    virtual std::vector<unsigned int> serielizeIndices() override;
};