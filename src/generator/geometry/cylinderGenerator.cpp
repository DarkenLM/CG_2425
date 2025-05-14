// M_PI does not work in visual studio without this BS, so use it
#define _USE_MATH_DEFINES

#include <cmath>
#include <vector>

#include "common/common.hpp"
#include "common/geometry/cylinder.hpp"
#include "common/geometry/point.hpp"

CylinderGeometry::~CylinderGeometry() = default;

CylinderGeometry::CylinderGeometry(int radius, int height, int slices) {
    this->_kind = GEOMETRY_CYLINDER;
    float alpha = (2 * M_PI) / slices;

    auto addVertex = [&](const Point3D& pos, const Vector3<float>& normal, float u, float v) {
        unsigned int index = static_cast<unsigned int>(this->vertices.size());
        this->vertices.push_back(pos);
        this->normals.push_back(normal);
        this->uvs.push_back(Vector2<float>(u, v));
        return index;
    };

    // Constants from your specifications
    const float SIDE_V_MIN = 0.375f;
    const float SIDE_V_MAX = 1.0f;
    const float TOP_CENTER_U = 0.4375f;
    const float TOP_CENTER_V = 0.1875f;
    const float BOTTOM_CENTER_U = 0.8125f;
    const float BOTTOM_CENTER_V = 0.1875f;
    const float CIRCLE_RADIUS_UV = 0.1875f;

    // --- Bottom Face ---
    Point3D bottomCenter(0.0f, 0.0f, 0.0f);
    Vector3<float> down(0.0f, -1.0f, 0.0f);
    for (int i = 0; i < slices; ++i) {
        float angle1 = i * alpha;
        float angle2 = (i + 1) * alpha;

        Point3D p1(radius * cos(angle1), 0.0f, radius * sin(angle1));
        Point3D p2(radius * cos(angle2), 0.0f, radius * sin(angle2));

        // UVs for bottom circle (radius 0.1875 around center)
        float u1 = BOTTOM_CENTER_U + CIRCLE_RADIUS_UV * cos(angle1);
        float v1 = BOTTOM_CENTER_V + CIRCLE_RADIUS_UV * sin(angle1);
        float u2 = BOTTOM_CENTER_U + CIRCLE_RADIUS_UV * cos(angle2);
        float v2 = BOTTOM_CENTER_V + CIRCLE_RADIUS_UV * sin(angle2);

        unsigned int i1 = addVertex(p2, down, u2, v2);
        unsigned int i2 = addVertex(bottomCenter, down, BOTTOM_CENTER_U, BOTTOM_CENTER_V);
        unsigned int i3 = addVertex(p1, down, u1, v1);
        this->indices.insert(this->indices.end(), {i1, i2, i3});
    }

    // --- Top Face ---
    Point3D topCenter(0.0f, height, 0.0f);
    Vector3<float> up(0.0f, 1.0f, 0.0f);
    for (int i = 0; i < slices; ++i) {
        float angle1 = i * alpha;
        float angle2 = (i + 1) * alpha;

        Point3D p1(radius * cos(angle1), height, radius * sin(angle1));
        Point3D p2(radius * cos(angle2), height, radius * sin(angle2));

        // UVs for top circle (radius 0.1875 around center)
        float u1 = TOP_CENTER_U + CIRCLE_RADIUS_UV * cos(angle1);
        float v1 = TOP_CENTER_V + CIRCLE_RADIUS_UV * sin(angle1);
        float u2 = TOP_CENTER_U + CIRCLE_RADIUS_UV * cos(angle2);
        float v2 = TOP_CENTER_V + CIRCLE_RADIUS_UV * sin(angle2);

        unsigned int i1 = addVertex(p1, up, u1, v1);
        unsigned int i2 = addVertex(topCenter, up, TOP_CENTER_U, TOP_CENTER_V);
        unsigned int i3 = addVertex(p2, up, u2, v2);
        this->indices.insert(this->indices.end(), {i1, i2, i3});
    }

    // --- Side Faces ---
    for (int i = 0; i < slices; ++i) {
        float angle1 = i * alpha;
        float angle2 = (i + 1) * alpha;

        Point3D bottom1(radius * cos(angle1), 0.0f, radius * sin(angle1));
        Point3D bottom2(radius * cos(angle2), 0.0f, radius * sin(angle2));
        Point3D top1 = bottom1.withY(height);
        Point3D top2 = bottom2.withY(height);

        // Convert to Vector3 for cross product
        Vector3<float> vBottom1 = bottom1.toVector3();
        Vector3<float> vBottom2 = bottom2.toVector3();
        Vector3<float> vTop1 = top1.toVector3();
        Vector3<float> vTop2 = top2.toVector3();

        // Calculate face normals (flat shading)
        Vector3<float> faceNormal1 = (vTop1 - vBottom1).cross(vBottom2 - vBottom1).normalized();
        Vector3<float> faceNormal2 = (vTop2 - vTop1).cross(vBottom2 - vTop1).normalized();

        // UV mapping for side (full width, 0.375-1.0 height)
        float u1 = static_cast<float>(i) / slices;      // 0 to 1
        float u2 = static_cast<float>(i + 1) / slices;  // 0 to 1
        float vBottom = SIDE_V_MIN;                     // 0.375
        float vTop = SIDE_V_MAX;                        // 1.0

        // Create two triangles per slice
        unsigned int i1 = addVertex(bottom1, faceNormal1, u1, vBottom);
        unsigned int i2 = addVertex(top1, faceNormal1, u1, vTop);
        unsigned int i3 = addVertex(bottom2, faceNormal1, u2, vBottom);

        unsigned int i4 = addVertex(top1, faceNormal2, u1, vTop);
        unsigned int i5 = addVertex(top2, faceNormal2, u2, vTop);
        unsigned int i6 = addVertex(bottom2, faceNormal2, u2, vBottom);

        this->indices.insert(this->indices.end(), {i1, i2, i3});
        this->indices.insert(this->indices.end(), {i4, i5, i6});
    }
}

std::vector<Point3D> CylinderGeometry::copyVertices() {
    std::vector<Point3D> ret;

    for (auto i : this->vertices) {
        ret.push_back(i.copy());
    }

    return ret;
}

std::vector<Vector3<float>> CylinderGeometry::copyNormals() {
    std::vector<Vector3<float>> ret;

    for (auto i : this->normals) {
        ret.push_back(i.copy());
    }

    return ret;
}

std::vector<unsigned int> CylinderGeometry::copyIndices() {
    std::vector<unsigned> ret;

    for (auto i : this->indices) {
        ret.push_back(i);
    }

    return ret;
}
