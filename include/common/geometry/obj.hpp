#pragma once

#include "common/geometry/BaseGeometry.hpp"

class ObjGeometry : public BaseGeometry {
    public:
        virtual ~ObjGeometry();
        ObjGeometry(std::vector<Point3D> vertices);

        std::vector<Point3D> serialize() override;
};