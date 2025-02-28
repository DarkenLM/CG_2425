#pragma once

#include "common/geometry/BaseGeometry.hpp"

class IcosphereGeometry : public BaseGeometry {
   public:
    virtual ~IcosphereGeometry();
    IcosphereGeometry(int radius, int subdivisions);
    IcosphereGeometry(std::vector<Point3D> vertices);

    std::vector<Point3D> serialize() override;
    static IcosphereGeometry* deserialize(std::string filePath);
};