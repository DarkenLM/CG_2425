#pragma once

#include "common/geometry/BaseGeometry.hpp"

class BoxGeometry : public BaseGeometry {
    public:
        virtual ~BoxGeometry();
        BoxGeometry(int length, int grid_size);
        BoxGeometry(std::vector<Point3D> vertices);
        
        std::vector<Point3D> serialize() override;
        static BoxGeometry* deserialize(std::string filePath);
};