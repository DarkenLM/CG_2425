// M_PI does not work in visual studio without this BS, so use it
#define _USE_MATH_DEFINES

#include <cmath>
#include <vector>

#include "common/common.hpp"
#include "common/geometry/plane.hpp"
#include "common/geometry/point.hpp"

PlaneGeometry::~PlaneGeometry() = default;

PlaneGeometry::PlaneGeometry(int length, int gridSize) {
    this->_kind = GEOMETRY_PLANE;

    float flen = (float)length;
    float subDivUnit = length * 1.0f / gridSize;

    for (int i = 0; i < gridSize; i++) {
        for (int j = 0; j < gridSize; j++) {
            // Upper trigon
            this->vertices.push_back(Point3D(-flen / 2 + subDivUnit * (j + 1), 0, -flen / 2 + subDivUnit * i));
			this->vertices.push_back(Point3D(-flen / 2 + subDivUnit * j      , 0, -flen / 2 + subDivUnit * i));
			this->vertices.push_back(Point3D(-flen / 2 + subDivUnit * (j + 1), 0, -flen / 2 + subDivUnit * (i + 1)));

            // Lower trigon
			this->vertices.push_back(Point3D(-flen / 2 + subDivUnit * j      , 0, -flen / 2 + subDivUnit * i));
			this->vertices.push_back(Point3D(-flen / 2 + subDivUnit * j      , 0, -flen / 2 + subDivUnit * (i + 1)));
			this->vertices.push_back(Point3D(-flen / 2 + subDivUnit*  (j + 1), 0, -flen / 2 + subDivUnit * (i + 1)));
        }
    }
}

std::vector<Point3D> PlaneGeometry::serialize() {
    std::vector<Point3D> ret;

    for (auto i : this->vertices) {
        ret.push_back(i.copy());
    }

    return ret;
}