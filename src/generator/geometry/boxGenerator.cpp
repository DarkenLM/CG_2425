#define _USE_MATH_DEFINES

#include <cmath>
#include <vector>

#include "common/common.hpp"
#include "common/geometry/box.hpp"
#include "common/geometry/point.hpp"

BoxGeometry::~BoxGeometry() = default;

BoxGeometry::BoxGeometry(int length, int grid_size) {
    this->_kind = GEOMETRY_BOX;
    float len = (float)length;
    float size = (float)grid_size;
    float triangle_side_length = len / size;

    for (float i = 0; i < size; i++) {
        for (float j = 0; j < size; j++) {
            // Upper Face
            this->vertices.push_back(Point3D(-len / 2 + triangle_side_length * (j + 1), len / 2, -len / 2 + triangle_side_length * i));
            this->vertices.push_back(Point3D(-len / 2 + triangle_side_length * j, len / 2, -len / 2 + triangle_side_length * i));
            this->vertices.push_back(Point3D(-len / 2 + triangle_side_length * (j + 1), len / 2, -len / 2 + triangle_side_length * (i + 1)));

            this->vertices.push_back(Point3D(-len / 2 + triangle_side_length * j, len / 2, -len / 2 + triangle_side_length * i));
            this->vertices.push_back(Point3D(-len / 2 + triangle_side_length * j, len / 2, -len / 2 + triangle_side_length * (i + 1)));
            this->vertices.push_back(Point3D(-len / 2 + triangle_side_length * (j + 1), len / 2, -len / 2 + triangle_side_length * (i + 1)));

            // Down face
            this->vertices.push_back(Point3D(-len / 2 + triangle_side_length * (j + 1), -len / 2, -len / 2 + triangle_side_length * i));
            this->vertices.push_back(Point3D(-len / 2 + triangle_side_length * (j + 1), -len / 2, -len / 2 + triangle_side_length * (i + 1)));
            this->vertices.push_back(Point3D(-len / 2 + triangle_side_length * j, -len / 2, -len / 2 + triangle_side_length * i));

            this->vertices.push_back(Point3D(-len / 2 + triangle_side_length * j, -len / 2, -len / 2 + triangle_side_length * i));
            this->vertices.push_back(Point3D(-len / 2 + triangle_side_length * (j + 1), -len / 2, -len / 2 + triangle_side_length * (i + 1)));
            this->vertices.push_back(Point3D(-len / 2 + triangle_side_length * j, -len / 2, -len / 2 + triangle_side_length * (i + 1)));

            // Front face
            this->vertices.push_back(Point3D(len / 2, len / 2 - triangle_side_length * j, -len / 2 + triangle_side_length * i));
            this->vertices.push_back(Point3D(len / 2, len / 2 - triangle_side_length * j, -len / 2 + triangle_side_length * (i + 1)));
            this->vertices.push_back(Point3D(len / 2, len / 2 - triangle_side_length * (j + 1), -len / 2 + triangle_side_length * i));

            this->vertices.push_back(Point3D(len / 2, len / 2 - triangle_side_length * j, -len / 2 + triangle_side_length * (i + 1)));
            this->vertices.push_back(Point3D(len / 2, len / 2 - triangle_side_length * (j + 1), -len / 2 + triangle_side_length * (i + 1)));
            this->vertices.push_back(Point3D(len / 2, len / 2 - triangle_side_length * (j + 1), -len / 2 + triangle_side_length * i));

            // Left face
            this->vertices.push_back(Point3D(len / 2 - triangle_side_length * (i + 1), len / 2 - triangle_side_length * (j + 1), len / 2));
            this->vertices.push_back(Point3D(len / 2 - triangle_side_length * i, len / 2 - triangle_side_length * (j + 1), len / 2));
            this->vertices.push_back(Point3D(len / 2 - triangle_side_length * i, len / 2 - triangle_side_length * j, len / 2));

            this->vertices.push_back(Point3D(len / 2 - triangle_side_length * i, len / 2 - triangle_side_length * j, len / 2));
            this->vertices.push_back(Point3D(len / 2 - triangle_side_length * (i + 1), len / 2 - triangle_side_length * j, len / 2));
            this->vertices.push_back(Point3D(len / 2 - triangle_side_length * (i + 1), len / 2 - triangle_side_length * (j + 1), len / 2));

            // Right face
            this->vertices.push_back(Point3D(len / 2 - triangle_side_length * i, len / 2 - triangle_side_length * j, -len / 2));
            this->vertices.push_back(Point3D(len / 2 - triangle_side_length * i, len / 2 - triangle_side_length * (j + 1), -len / 2));
            this->vertices.push_back(Point3D(len / 2 - triangle_side_length * (i + 1), len / 2 - triangle_side_length * j, -len / 2));

            this->vertices.push_back(Point3D(len / 2 - triangle_side_length * i, len / 2 - triangle_side_length * (j + 1), -len / 2));
            this->vertices.push_back(Point3D(len / 2 - triangle_side_length * (i + 1), len / 2 - triangle_side_length * (j + 1), -len / 2));
            this->vertices.push_back(Point3D(len / 2 - triangle_side_length * (i + 1), len / 2 - triangle_side_length * j, -len / 2));

            // Back face
            this->vertices.push_back(Point3D(-len / 2, len / 2 - triangle_side_length * j, -len / 2 + triangle_side_length * i));
            this->vertices.push_back(Point3D(-len / 2, len / 2 - triangle_side_length * (j + 1), -len / 2 + triangle_side_length * i));
            this->vertices.push_back(Point3D(-len / 2, len / 2 - triangle_side_length * (j + 1), -len / 2 + triangle_side_length * (i + 1)));

            this->vertices.push_back(Point3D(-len / 2, len / 2 - triangle_side_length * j, -len / 2 + triangle_side_length * (i + 1)));
            this->vertices.push_back(Point3D(-len / 2, len / 2 - triangle_side_length * j, -len / 2 + triangle_side_length * i));
            this->vertices.push_back(Point3D(-len / 2, len / 2 - triangle_side_length * (j + 1), -len / 2 + triangle_side_length * (i + 1)));
        }
    }
}

std::vector<Point3D> BoxGeometry::serielizeVertices() {
    std::vector<Point3D> ret;

    for (auto i : this->vertices) {
        ret.push_back(i.copy());
    }

    return ret;
}

std::vector<Vector3<float>> BoxGeometry::serielizeNormals() {
    std::vector<Vector3<float>> ret;

    for (auto i : this->normals) {
        ret.push_back(i.copy());
    }

    return ret;
}

std::vector<unsigned int> BoxGeometry::serielizeIndices() {
    std::vector<unsigned> ret;

    for (auto i : this->indices) {
        ret.push_back(i);
    }

    return ret;
}