#define _USE_MATH_DEFINES

#include <cmath>
#include <vector>

#include "common/common.hpp"
#include "common/geometry/flatRing.hpp"
#include "common/geometry/point.hpp"

FlatRingGeometry::~FlatRingGeometry() = default;

FlatRingGeometry::FlatRingGeometry(int majorRadius, int minorRadius, int slices) {
    this->_kind = GEOMETRY_FLATRING;

    float slicesJump = 2 * M_PI / slices;

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

    for (int i = 0; i < slices; i++) {
        float beta1 = i * slicesJump;
        float beta2 = (i + 1) * slicesJump;

        float outerX1 = (majorRadius + minorRadius) * cos(beta1);
        float outerZ1 = (majorRadius + minorRadius) * sin(beta1);
        float outerX2 = (majorRadius + minorRadius) * cos(beta2);
        float outerZ2 = (majorRadius + minorRadius) * sin(beta2);

        float innerX1 = (majorRadius - minorRadius) * cos(beta1);
        float innerZ1 = (majorRadius - minorRadius) * sin(beta1);
        float innerX2 = (majorRadius - minorRadius) * cos(beta2);
        float innerZ2 = (majorRadius - minorRadius) * sin(beta2);

        Point3D p1(innerX1, 0.0f, innerZ1);
        Point3D p2(outerX1, 0.0f, outerZ1);
        Point3D p3(outerX2, 0.0f, outerZ2);
        Point3D p4(innerX2, 0.0f, innerZ2);

        Vector3<float> up(0.0f, 1.0f, 0.0f);
        Vector3<float> down(0.0f, -1.0f, 0.0f);

        // --- Top face (CCW)
        unsigned int i1 = addVertex(p1, up);
        unsigned int i2 = addVertex(p2, up);
        unsigned int i3 = addVertex(p3, up);
        unsigned int i4 = addVertex(p4, up);

        this->indices.insert(this->indices.end(), {i1, i3, i2});
        this->indices.insert(this->indices.end(), {i1, i4, i3});

        // --- Bottom face (CW, so we flip the order for correct culling)
        unsigned int i5 = addVertex(p1, down);
        unsigned int i6 = addVertex(p4, down);
        unsigned int i7 = addVertex(p3, down);
        unsigned int i8 = addVertex(p2, down);

        this->indices.insert(this->indices.end(), {i5, i7, i6});
        this->indices.insert(this->indices.end(), {i5, i8, i7});
    }
}

std::vector<Point3D> FlatRingGeometry::serielizeVertices() {
    std::vector<Point3D> ret;

    for (auto i : this->vertices) {
        ret.push_back(i.copy());
    }

    return ret;
}

std::vector<Vector3<float>> FlatRingGeometry::serielizeNormals() {
    std::vector<Vector3<float>> ret;

    for (auto i : this->normals) {
        ret.push_back(i.copy());
    }

    return ret;
}

std::vector<unsigned int> FlatRingGeometry::serielizeIndices() {
    std::vector<unsigned> ret;

    for (auto i : this->indices) {
        ret.push_back(i);
    }

    return ret;
}