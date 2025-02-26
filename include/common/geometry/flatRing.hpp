#pragma once

#include "common/geometry/BaseGeometry.hpp"

class FlatRingGeometry : public BaseGeometry {
   public:
    virtual ~FlatRingGeometry();
    FlatRingGeometry(int majorRadius, int minorRadius, int slices);
    FlatRingGeometry(std::vector<Point3D> vertices);

    std::vector<Point3D> serialize() override;
    static FlatRingGeometry* deserialize(std::string filePath);
};