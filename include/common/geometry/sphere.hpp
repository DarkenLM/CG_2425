#pragma once

#include "common/geometry/BaseGeometry.hpp"

class SphereGeometry : public BaseGeometry {
    // protected:
    //     BaseGeometryKind _kind = GEOMETRY_SPHERE;

   public:
    virtual ~SphereGeometry();
    SphereGeometry(float radius, int slices, int stacks);
    SphereGeometry(std::vector<Point3D> vertices, std::vector<Vector3<float>> normals, std::vector<unsigned int> indices);

    virtual std::vector<Point3D> serielizeVertices() override;
    virtual std::vector<Vector3<float>> serielizeNormals() override;
    virtual std::vector<unsigned int> serielizeIndices() override;

    static SphereGeometry* deserialize(std::string filePath);
};