#pragma once

#include "common/geometry/BaseGeometry.hpp"

class TorusGeometry : public BaseGeometry {
   public:
    virtual ~TorusGeometry();
    TorusGeometry(int majorRadius, int minorRadius, int slices, int sides);
    TorusGeometry(std::vector<Point3D> vertices);

    std::vector<Point3D> serialize() override;
    static TorusGeometry* deserialize(std::string filePath);
};