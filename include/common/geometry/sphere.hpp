#pragma once

#include "common/geometry/BaseGeometry.hpp"

class SphereGeometry : public BaseGeometry {
    public:
        virtual ~SphereGeometry();
        SphereGeometry(int radius, int slices, int stacks);
        SphereGeometry(std::vector<Point3D> vertices);
        
        std::vector<Point3D> serialize() override;
        static SphereGeometry* deserialize(std::string filePath);
};