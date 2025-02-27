#pragma once

#include "common/geometry/BaseGeometry.hpp"

class CylinderGeometry : public BaseGeometry {
    // protected:
    //     BaseGeometryKind _kind = GEOMETRY_CYLINDER;

    public:
        virtual ~CylinderGeometry();
        CylinderGeometry(int radius, int height, int slices);
        CylinderGeometry(std::vector<Point3D> vertices);

        std::vector<Point3D> serialize() override;
        static CylinderGeometry* deserialize(std::string filePath);
};