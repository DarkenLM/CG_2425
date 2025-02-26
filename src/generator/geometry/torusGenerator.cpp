#define _USE_MATH_DEFINES

#include <cmath>
#include <vector>

#include "common/common.hpp"
#include "common/geometry/point.hpp"
#include "common/geometry/torus.hpp"

TorusGeometry::~TorusGeometry() = default;

TorusGeometry::TorusGeometry(int majorRadius, int minorRadius, int slices, int sides) {
    this->_kind = GEOMETRY_TORUS;

    float x1, y1, z1;
    float x2, y2, z2;
    float x3, y3, z3;
    float x4, y4, z4;
    float slicesJump = 2 * M_PI / slices;
    float sidesJump = 2 * M_PI / sides;
    float alpha, beta;

    for (int i = 0; i < slices; i++) {
        beta = slicesJump * i;

        for (int j = 0; j < sides; j++) {
            alpha = sidesJump * j;

            x1 = (majorRadius + minorRadius * cos(alpha)) * cos(beta);
            y1 = minorRadius * sin(alpha);
            z1 = (majorRadius + minorRadius * cos(alpha)) * sin(beta);

            x2 = (majorRadius + minorRadius * cos(alpha)) * cos(beta + slicesJump);
            y2 = y1;
            z2 = (majorRadius + minorRadius * cos(alpha)) * sin(beta + slicesJump);

            x3 = (majorRadius + minorRadius * cos(alpha + sidesJump)) * cos(beta);
            y3 = minorRadius * sin(alpha + sidesJump);
            z3 = (majorRadius + minorRadius * cos(alpha + sidesJump)) * sin(beta);

            x4 = (majorRadius + minorRadius * cos(alpha + sidesJump)) * cos(beta + slicesJump);
            y4 = y3;
            z4 = (majorRadius + minorRadius * cos(alpha + sidesJump)) * sin(beta + slicesJump);

            this->vertices.push_back(Point3D(x3, y3, z3));
            this->vertices.push_back(Point3D(x2, y2, z2));
            this->vertices.push_back(Point3D(x1, y1, z1));

            this->vertices.push_back(Point3D(x3, y3, z3));
            this->vertices.push_back(Point3D(x4, y4, z4));
            this->vertices.push_back(Point3D(x2, y2, z2));
        }
    }
}

std::vector<Point3D> TorusGeometry::serialize() {
    std::vector<Point3D> ret;

    for (auto i : this->vertices) {
        ret.push_back(i.copy());
    }

    return ret;
}
