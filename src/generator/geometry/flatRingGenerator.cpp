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

    auto addVertex = [&](const Point3D& pos, const Vector3<float>& normal, float u, float v) {
        unsigned int index = static_cast<unsigned int>(this->vertices.size());
        this->vertices.push_back(pos);
        this->normals.push_back(normal.normalized());
        this->uvs.push_back(Vector2<float>(u, v));
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

        // Top face UVs (0,0)-(1,1)
        unsigned int i1 = addVertex(p1, up, 0.0f, 0.0f);
        unsigned int i2 = addVertex(p2, up, 1.0f, 0.0f);
        unsigned int i3 = addVertex(p3, up, 1.0f, 1.0f);
        unsigned int i4 = addVertex(p4, up, 0.0f, 1.0f);

        this->indices.insert(this->indices.end(), {i1, i3, i2});
        this->indices.insert(this->indices.end(), {i1, i4, i3});

        // Bottom face UVs same as top face but winding order flipped for correct face orientation
        unsigned int i5 = addVertex(p1, down, 0.0f, 0.0f);
        unsigned int i6 = addVertex(p2, down, 1.0f, 0.0f);
        unsigned int i7 = addVertex(p3, down, 1.0f, 1.0f);
        unsigned int i8 = addVertex(p4, down, 0.0f, 1.0f);

        // Notice the order here is flipped compared to top face to maintain correct winding (CW)
        this->indices.insert(this->indices.end(), {i5, i6, i7});
        this->indices.insert(this->indices.end(), {i5, i7, i8});
    }
}

std::vector<Point3D> FlatRingGeometry::copyVertices() {
    std::vector<Point3D> ret;

    for (auto i : this->vertices) {
        ret.push_back(i.copy());
    }

    return ret;
}

std::vector<Vector3<float>> FlatRingGeometry::copyNormals() {
    std::vector<Vector3<float>> ret;

    for (auto i : this->normals) {
        ret.push_back(i.copy());
    }

    return ret;
}

std::vector<unsigned int> FlatRingGeometry::copyIndices() {
    std::vector<unsigned> ret;

    for (auto i : this->indices) {
        ret.push_back(i);
    }

    return ret;
}