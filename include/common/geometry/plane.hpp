#pragma once

#include "common/geometry/BaseGeometry.hpp"

class PlaneGeometry : public BaseGeometry {
    // protected:
    //     BaseGeometryKind _kind = GEOMETRY_PLANE;

    public:
        virtual ~PlaneGeometry();
        PlaneGeometry(int length, int gridSize);
        PlaneGeometry(std::vector<Point3D> vertices);
        
        std::vector<Point3D> serialize() override;
        static PlaneGeometry* deserialize(std::string filePath);
};