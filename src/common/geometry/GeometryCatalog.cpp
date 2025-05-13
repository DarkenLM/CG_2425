#include "common/geometry/GeometryCatalog.hpp"

BaseGeometry* createGeometryFromKind(BaseGeometryKind kind, std::vector<Point3D> vertices, std::vector<Vector3<float>> normals, std::vector<Vector2<float>> uvs, std::vector<unsigned int> indices) {
    switch (kind) {
        case GEOMETRY_BOX:
            return new BoxGeometry(vertices, normals, uvs, indices);
            break;
        case GEOMETRY_CONE:
            return new ConeGeometry(vertices, normals, uvs, indices);
            break;
        case GEOMETRY_TORUS:
            return new TorusGeometry(vertices, normals, uvs, indices);
            break;
        case GEOMETRY_PLANE:
            return new PlaneGeometry(vertices, normals, uvs, indices);
            break;
        case GEOMETRY_SPHERE:
            return new SphereGeometry(vertices, normals, uvs, indices);
            break;
        case GEOMETRY_CYLINDER:
            return new CylinderGeometry(vertices, normals, uvs, indices);
            break;
        case GEOMETRY_FLATRING:
            return new FlatRingGeometry(vertices, normals, uvs, indices);
            break;
        case GEOMETRY_ICOSPHERE:
            return new IcosphereGeometry(vertices, normals, uvs, indices);
            break;
        case GEOMETRY_BEZIER:
            return new BezierGeometry(vertices, normals, uvs, indices);
            break;
        case GEOMETRY_OBJ:
            return new ObjGeometry(vertices, normals, uvs, indices);
            break;
    }
    return nullptr;
}