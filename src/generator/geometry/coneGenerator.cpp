// M_PI does not work in visual studio without this BS, so use it
#define _USE_MATH_DEFINES

#include <cmath>
#include <vector>

#include "common/common.hpp"
#include "common/geometry/cone.hpp"
#include "common/geometry/point.hpp"
#include "engine/glContext.hpp"

ConeGeometry::~ConeGeometry() = default;

ConeGeometry::ConeGeometry(int radius, int height, int slices, int stacks) {
    this->_kind = GEOMETRY_CONE;

    float alpha = (2 * M_PI) / slices;
    float stackHeight = static_cast<float>(height) / stacks;
    Point3D baseCenter(0.0f, 0.0f, 0.0f);
    Point3D apex(0.0f, static_cast<float>(height), 0.0f);

    std::unordered_map<VertexKey, unsigned int> vertexMap;

    auto addVertex = [&](const Point3D& pos, const Vector3<float>& normal) {
        VertexKey key{pos, normal};
        auto it = vertexMap.find(key);
        if (it != vertexMap.end()) return it->second;

        unsigned int index = static_cast<unsigned int>(this->vertices.size());
        vertexMap[key] = index;
        this->vertices.push_back(pos);
        this->normals.push_back(normal);
        return index;
    };

    // --- Base ---
    for (int i = 0; i < slices; i++) {
        float x1 = radius * cos(i * alpha);
        float z1 = radius * sin(i * alpha);
        float x2 = radius * cos((i + 1) * alpha);
        float z2 = radius * sin((i + 1) * alpha);

        Point3D p1(x1, 0.0f, z1);
        Point3D p2(x2, 0.0f, z2);
        Vector3<float> down(0.0f, -1.0f, 0.0f);

        unsigned int i1 = addVertex(p2, down);
        unsigned int i2 = addVertex(baseCenter, down);
        unsigned int i3 = addVertex(p1, down);

        this->indices.insert(this->indices.end(), {i1, i2, i3});
    }

    // --- Side ---
    for (int i = 0; i < slices; i++) {
        for (int j = 0; j < stacks; j++) {
            float t1 = static_cast<float>(j) / stacks;
            float t2 = static_cast<float>(j + 1) / stacks;

            float y1 = t1 * height;
            float y2 = t2 * height;

            float scale1 = 1.0f - t1;
            float scale2 = 1.0f - t2;

            float x1 = radius * cos(i * alpha);
            float z1 = radius * sin(i * alpha);
            float x2 = radius * cos((i + 1) * alpha);
            float z2 = radius * sin((i + 1) * alpha);

            Point3D p1(x1 * scale2, y2, z1 * scale2);
            Point3D p2(x1 * scale1, y1, z1 * scale1);
            Point3D p3(x2 * scale2, y2, z2 * scale2);
            Point3D p4(x2 * scale1, y1, z2 * scale1);

            // Manual conversion to Vector3 for math
            Vector3<float> v1 = Vector3<float>(p3.getX() - p1.getX(), p3.getY() - p1.getY(), p3.getZ() - p1.getZ());
            Vector3<float> v2 = Vector3<float>(p2.getX() - p1.getX(), p2.getY() - p1.getY(), p2.getZ() - p1.getZ());
            Vector3<float> normal1 = v1.cross(v2).normalized();

            Vector3<float> v3 = Vector3<float>(p3.getX() - p2.getX(), p3.getY() - p2.getY(), p3.getZ() - p2.getZ());
            Vector3<float> v4 = Vector3<float>(p4.getX() - p2.getX(), p4.getY() - p2.getY(), p4.getZ() - p2.getZ());
            Vector3<float> normal2 = v3.cross(v4).normalized();

            unsigned int i1 = addVertex(p1, normal1);
            unsigned int i2 = addVertex(p2, normal1);
            unsigned int i3 = addVertex(p3, normal1);

            unsigned int i4 = addVertex(p3, normal2);
            unsigned int i5 = addVertex(p2, normal2);
            unsigned int i6 = addVertex(p4, normal2);

            this->indices.insert(this->indices.end(), {i1, i3, i2});
            this->indices.insert(this->indices.end(), {i4, i6, i5});
        }
    }
}

std::vector<Point3D> ConeGeometry::serielizeVertices() {
    std::vector<Point3D> ret;

    for (auto i : this->vertices) {
        ret.push_back(i.copy());
    }

    return ret;
}

std::vector<Vector3<float>> ConeGeometry::serielizeNormals() {
    std::vector<Vector3<float>> ret;

    for (auto i : this->normals) {
        ret.push_back(i.copy());
    }

    return ret;
}

std::vector<unsigned int> ConeGeometry::serielizeIndices() {
    std::vector<unsigned> ret;

    for (auto i : this->indices) {
        ret.push_back(i);
    }

    return ret;
}