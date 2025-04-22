// M_PI does not work in Visual Studio without this BS, so use it
#define _USE_MATH_DEFINES

#include <cmath>
#include <vector>

#include "common/common.hpp"
#include "common/geometry/point.hpp"
#include "common/geometry/sphere.hpp"

SphereGeometry::~SphereGeometry() = default;

SphereGeometry::SphereGeometry(float radius, int slices, int stacks) {
    this->_kind = GEOMETRY_SPHERE;

    std::unordered_map<VertexKey, unsigned int> vertexMap;

    auto addVertex = [&](const Point3D& pos) {
        Vector3<float> normal = Vector3<float>(pos.getX(), pos.getY(), pos.getZ()).normalized();
        VertexKey key{pos, normal};
        auto it = vertexMap.find(key);
        if (it != vertexMap.end()) return it->second;

        unsigned int index = static_cast<unsigned int>(this->vertices.size());
        vertexMap[key] = index;
        this->vertices.push_back(pos);
        this->normals.push_back(normal);
        return index;
    };

    float sliceStep = 2.0f * M_PI / slices;
    float stackStep = M_PI / stacks;

    std::vector<std::vector<Point3D>> points;

    // Build points on a Y-up sphere
    for (int i = 0; i <= stacks; ++i) {
        float stackAngle = M_PI / 2 - i * stackStep;  // From π/2 (top) to -π/2 (bottom)
        float xy = radius * cosf(stackAngle);         // r * cos(φ)
        float y = radius * sinf(stackAngle);          // r * sin(φ)

        std::vector<Point3D> row;
        for (int j = 0; j <= slices; ++j) {
            float sectorAngle = j * sliceStep;  // θ angle

            float x = xy * cosf(sectorAngle);
            float z = xy * sinf(sectorAngle);
            row.push_back(Point3D(x, y, z));
        }
        points.push_back(row);
    }

    // Build triangles
    for (int i = 0; i < stacks; ++i) {
        for (int j = 0; j < slices; ++j) {
            Point3D p1 = points[i][j];
            Point3D p2 = points[i + 1][j];
            Point3D p3 = points[i][j + 1];
            Point3D p4 = points[i + 1][j + 1];

            // Triangle 1 (top-left triangle)
            unsigned int i1 = addVertex(p1);
            unsigned int i2 = addVertex(p3);
            unsigned int i3 = addVertex(p2);
            this->indices.insert(this->indices.end(), {i1, i2, i3});

            // Triangle 2 (bottom-right triangle)
            unsigned int i4 = addVertex(p3);
            unsigned int i5 = addVertex(p4);
            unsigned int i6 = addVertex(p2);
            this->indices.insert(this->indices.end(), {i4, i5, i6});
        }
    }
}

std::vector<Point3D> SphereGeometry::serielizeVertices() {
    std::vector<Point3D> ret;

    for (auto i : this->vertices) {
        ret.push_back(i.copy());
    }

    return ret;
}

std::vector<Vector3<float>> SphereGeometry::serielizeNormals() {
    std::vector<Vector3<float>> ret;

    for (auto i : this->normals) {
        ret.push_back(i.copy());
    }

    return ret;
}

std::vector<unsigned int> SphereGeometry::serielizeIndices() {
    std::vector<unsigned> ret;

    for (auto i : this->indices) {
        ret.push_back(i);
    }

    return ret;
}
