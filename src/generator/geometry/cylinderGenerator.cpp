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
    Point3D bottomCenter(0.0f, 0.0f, 0.0f);
    Point3D topCenter(0.0f, height, 0.0f);
    Vector3<float> bottomNormal(0.0f, -1.0f, 0.0f);
    Vector3<float> topNormal(0.0f, 1.0f, 0.0f);

    std::unordered_map<VertexKey, unsigned int> vertexMap;

    auto addVertex = [&](const Point3D& pos, const Vector3<float>& normal) {
        VertexKey key{pos, normal};
        auto it = vertexMap.find(key);
        if (it != vertexMap.end()) return it->second;

        unsigned int index = static_cast<unsigned int>(this->vertices.size());
        this->vertices.push_back(pos);
        this->normals.push_back(normal);
        vertexMap[key] = index;
        return index;
    };

    for (int i = 0; i < slices; ++i) {
        float angle1 = i * alpha;
        float angle2 = (i + 1) * alpha;

        float x1 = radius * cos(angle1);
        float z1 = radius * sin(angle1);
        float x2 = radius * cos(angle2);
        float z2 = radius * sin(angle2);

        Point3D p1(x1, 0.0f, z1);
        Point3D p2(x2, 0.0f, z2);
        Point3D p3 = p1.withY(height);
        Point3D p4 = p2.withY(height);

        // --- Bottom face ---
        unsigned int i1 = addVertex(p2, bottomNormal);
        unsigned int i2 = addVertex(bottomCenter, bottomNormal);
        unsigned int i3 = addVertex(p1, bottomNormal);
        this->indices.insert(this->indices.end(), {i1, i2, i3});

        // --- Top face ---
        unsigned int i4 = addVertex(p1.withY(height), topNormal);
        unsigned int i5 = addVertex(topCenter, topNormal);
        unsigned int i6 = addVertex(p2.withY(height), topNormal);
        this->indices.insert(this->indices.end(), {i4, i5, i6});

        // --- Side triangle 1 ---
        Vector3<float> n1(x1, 0.0f, z1);
        n1.normalize();
        Vector3<float> n2(x2, 0.0f, z2);
        n2.normalize();

        unsigned int i7 = addVertex(p1, n1);
        unsigned int i8 = addVertex(p3, n1);
        unsigned int i9 = addVertex(p2, n2);
        this->indices.insert(this->indices.end(), {i7, i8, i9});

        // --- Side triangle 2 ---
        unsigned int i10 = addVertex(p3, n1);
        unsigned int i11 = addVertex(p4, n2);
        unsigned int i12 = addVertex(p2, n2);
        this->indices.insert(this->indices.end(), {i10, i11, i12});
    }
}

std::vector<Point3D> CylinderGeometry::serielizeVertices() {
    std::vector<Point3D> ret;

    for (auto i : this->vertices) {
        ret.push_back(i.copy());
    }

    return ret;
}

std::vector<Vector3<float>> CylinderGeometry::serielizeNormals() {
    std::vector<Vector3<float>> ret;

    for (auto i : this->normals) {
        ret.push_back(i.copy());
    }

    return ret;
}

std::vector<unsigned int> CylinderGeometry::serielizeIndices() {
    std::vector<unsigned> ret;

    for (auto i : this->indices) {
        ret.push_back(i);
    }

    return ret;
}
