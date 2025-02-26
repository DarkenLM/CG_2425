#define _USE_MATH_DEFINES

#include <cmath>
#include <vector>

#include "common/common.hpp"
#include "common/geometry/flatRing.hpp"
#include "common/geometry/point.hpp"

FlatRingGeometry::~FlatRingGeometry() = default;

FlatRingGeometry::FlatRingGeometry(int majorRadius, int minorRadius, int slices) {
    this->_kind = GEOMETRY_FLATRING;

    float x1, y1, z1;
    float x2, y2, z2;
    float x3, y3, z3;
    float x4, y4, z4;
    float slicesJump = 2 * M_PI / slices;
    float beta;

    for (int i = 0; i < slices; i++) {
        beta = slicesJump * i;

        x1 = (majorRadius + minorRadius) * cos(beta);
        y1 = 0.0f;
        z1 = (majorRadius + minorRadius) * sin(beta);

        x2 = (majorRadius + minorRadius) * cos(beta + slicesJump);
        y2 = 0.0f;
        z2 = (majorRadius + minorRadius) * sin(beta + slicesJump);

        x3 = (majorRadius - minorRadius) * cos(beta);
        y3 = 0.0f;
        z3 = (majorRadius - minorRadius) * sin(beta);

        x4 = (majorRadius - minorRadius) * cos(beta + slicesJump);
        y4 = 0.0f;
        z4 = (majorRadius - minorRadius) * sin(beta + slicesJump);

        // Faces
        // UP
        this->vertices.push_back(Point3D(x3, y3, z3));
        this->vertices.push_back(Point3D(x2, y2, z2));
        this->vertices.push_back(Point3D(x1, y1, z1));
        // DOWN
        this->vertices.push_back(Point3D(x2, y2, z2));
        this->vertices.push_back(Point3D(x4, y4, z4));
        this->vertices.push_back(Point3D(x3, y3, z3));
        // UP
        this->vertices.push_back(Point3D(x3, y3, z3));
        this->vertices.push_back(Point3D(x4, y4, z4));
        this->vertices.push_back(Point3D(x2, y2, z2));
        // DOWN
        this->vertices.push_back(Point3D(x1, y1, z2));
        this->vertices.push_back(Point3D(x2, y2, z2));
        this->vertices.push_back(Point3D(x3, y3, z3));
    }
}

std::vector<Point3D> FlatRingGeometry::serialize() {
    std::vector<Point3D> ret;

    for (auto i : this->vertices) {
        ret.push_back(i.copy());
    }

    return ret;
}
