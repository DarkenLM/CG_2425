#include "engine/geometry/GeometryCatalog.hpp"

BaseGeometry* createGeometryFromKind(BaseGeometryKind kind, std::vector<Point3D> vertices) {
    switch (kind) {
        case GEOMETRY_BOX: return new BoxGeometry(vertices); break;
        case GEOMETRY_CONE: return new ConeGeometry(vertices); break;
        case GEOMETRY_TORUS: return new TorusGeometry(vertices); break;
        case GEOMETRY_PLANE: return new PlaneGeometry(vertices); break;
        case GEOMETRY_SPHERE: return new SphereGeometry(vertices); break;
        case GEOMETRY_CYLINDER: return new CylinderGeometry(vertices); break;
        case GEOMETRY_FLATRING: return new FlatRingGeometry(vertices); break;
        case GEOMETRY_ICOSPHERE: return new IcosphereGeometry(vertices); break;
    }

    return nullptr;
}