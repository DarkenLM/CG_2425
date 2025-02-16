#pragma once

// #include <vector>
// #include "common/geometry/point.hpp"
// void coneTriangleGenerator(int radius, int height, int slices, int stacks, std::vector<Point3D>& points);
// std::vector<Point3D> coneGenerator(int radius, int height, int slices, int stacks);

#include "common/geometry/BaseGeometry.hpp"

class ConeGeometry : public BaseGeometry {
    public:
        ConeGeometry(int radius, int height, int slices, int stacks);
        
        std::vector<Point3D> serialize() override;
        static ConeGeometry* deserialize(std::string filePath);
};