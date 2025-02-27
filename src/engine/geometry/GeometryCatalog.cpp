#include "engine/geometry/GeometryCatalog.hpp"

BaseGeometry* createGeometryFromKind(BaseGeometryKind kind, std::vector<Point3D> vertices) {
    switch (kind) {
        case GEOMETRY_CONE: return new ConeGeometry(vertices); break;
        case GEOMETRY_PLANE: return new PlaneGeometry(vertices); break;
        case GEOMETRY_SPHERE: return new SphereGeometry(vertices); break;
        case GEOMETRY_CYLINDER: return new CylinderGeometry(vertices); break;
    }

    return nullptr;
}