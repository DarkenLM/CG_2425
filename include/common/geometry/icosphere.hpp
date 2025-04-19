#pragma once

#include "common/geometry/BaseGeometry.hpp"

class IcosphereGeometry : public BaseGeometry {
   public:
    virtual ~IcosphereGeometry();
    IcosphereGeometry(int radius, int subdivisions);
    IcosphereGeometry(std::vector<Point3D> vertices, std::vector<Vector3<float>> normals, std::vector<unsigned int> indices);

    virtual std::vector<Point3D> serielizeVertices() override;
    virtual std::vector<Vector3<float>> serielizeNormals() override;
    virtual std::vector<unsigned int> serielizeIndices() override;

    static IcosphereGeometry* deserialize(std::string filePath);
};