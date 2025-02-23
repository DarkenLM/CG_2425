#pragma once

#include "common/geometry/BaseGeometry.hpp"

class PlaneGeometry : public BaseGeometry {
    public:
        PlaneGeometry(int length, int gridSize);
        PlaneGeometry(std::vector<Point3D>);
        
        std::vector<Point3D> serialize() override;
        static PlaneGeometry* deserialize(std::string filePath);
};