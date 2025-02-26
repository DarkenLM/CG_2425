#pragma once

#include <string>
#include <vector>

#include "point.hpp"

typedef enum basegeometry_kind {
    _GEOMETRY_BASE,
    GEOMETRY_CONE,
    GEOMETRY_PLANE,
    GEOMETRY_BOX,
    GEOMETRY_SPHERE,
    GEOMETRY_CYLINDER,
    GEOMETRY_TORUS,
    GEOMETRY_FLATRING
} BaseGeometryKind;

class BaseGeometry {
   protected:
    BaseGeometryKind _kind = _GEOMETRY_BASE;
    std::vector<Point3D> vertices;

   public:
    virtual ~BaseGeometry() = default;
    std::vector<Point3D> getVertices() const { return this->vertices; };

    virtual std::vector<Point3D> serialize() = 0;
    static BaseGeometry* deserialize(std::string filePath) { return nullptr; };
};