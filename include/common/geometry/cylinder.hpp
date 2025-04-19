#pragma once

#include "common/geometry/BaseGeometry.hpp"

class CylinderGeometry : public BaseGeometry {
    // protected:
    //     BaseGeometryKind _kind = GEOMETRY_CYLINDER;

   public:
    virtual ~CylinderGeometry();
    CylinderGeometry(int radius, int height, int slices);
    CylinderGeometry(std::vector<Point3D> vertices, std::vector<Vector3<float>> normals, std::vector<unsigned int> indices);

    virtual std::vector<Point3D> serielizeVertices() override;
    virtual std::vector<Vector3<float>> serielizeNormals() override;
    virtual std::vector<unsigned int> serielizeIndices() override;

    static CylinderGeometry* deserialize(std::string filePath);
};