#pragma once

// #include <vector>
// #include "common/geometry/point.hpp"
// std::vector<Point3D> coneGenerator(int radius, int height, int slices, int stacks);

#include "common/geometry/BaseGeometry.hpp"

class ConeGeometry : public BaseGeometry {
    // protected:
    //     BaseGeometryKind _kind = GEOMETRY_CONE;

    public:
        virtual ~ConeGeometry();
        ConeGeometry(int radius, int height, int slices, int stacks);
        ConeGeometry(std::vector<Point3D> vertices);

        std::vector<Point3D> serialize() override;
        static ConeGeometry* deserialize(std::string filePath);
};