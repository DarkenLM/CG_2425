// M_PI does not work in visual studio without this BS, so use it
#define _USE_MATH_DEFINES

#include <cmath>
#include <vector>

#include "common/common.hpp"
#include "common/geometry/sphere.hpp"
#include "common/geometry/point.hpp"


SphereGeometry::~SphereGeometry() = default;

SphereGeometry::SphereGeometry(int radius, int slices, int stacks) {
    this->_kind = GEOMETRY_SPHERE;
    
}

std::vector<Point3D> SphereGeometry::serialize() {
    std::vector<Point3D> ret;

    for (auto i : this->vertices) {
        ret.push_back(i.copy());
    }

    return ret;
}