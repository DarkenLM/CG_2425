// M_PI does not work in visual studio without this BS, so use it
#define _USE_MATH_DEFINES

#include <cmath>
#include <vector>

#include "common/common.hpp"
#include "common/geometry/cone.hpp"
#include "common/geometry/point.hpp"

void coneTriangleGenerator(int radius, int height, int slices, int stacks, std::vector<Point3D> &points) {
    float alpha = (2 * M_PI) / slices;
    float stackHeight = static_cast<float>(height) / stacks;
    int nTriangles = 0;

    Point3D center(0.0f, 0.0f, 0.0f);

    for (int i = 0; i < slices; i++) {
        float x1 = radius * cos(i * alpha);
        float z1 = radius * sin(i * alpha);
        float x2 = radius * cos((i + 1) * alpha);
        float z2 = radius * sin((i + 1) * alpha);

        points.push_back(center);
        points.push_back(Point3D(x1, 0.0f, z1));
        points.push_back(Point3D(x2, 0.0f, z2));

        for (int j = 0; j < stacks; j++) {
            float y1 = j * stackHeight;
            float y2 = (j + 1) * stackHeight;

            x1 = radius * cos(i * alpha) * (1 - (float)(j + 1) / stacks);
            z1 = radius * sin(i * alpha) * (1 - (float)(j + 1) / stacks);
            x2 = radius * cos((i + 1) * alpha) * (1 - (float)(j + 1) / stacks);
            z2 = radius * sin((i + 1) * alpha) * (1 - (float)(j + 1) / stacks);

            float x3 = radius * cos(i * alpha) * (1 - (float)j / stacks);
            float z3 = radius * sin(i * alpha) * (1 - (float)j / stacks);
            float x4 = radius * cos((i + 1) * alpha) * (1 - (float)j / stacks);
            float z4 = radius * sin((i + 1) * alpha) * (1 - (float)j / stacks);

            points.push_back(Point3D(x2, y2, z2));
            points.push_back(Point3D(x3, y1, z3));
            points.push_back(Point3D(x1, y2, z1));

            points.push_back(Point3D(x2, y2, z2));
            points.push_back(Point3D(x4, y1, z4));
            points.push_back(Point3D(x3, y1, z3));
        }
    }
}

// std::vector<Point3D> coneGenerator(int radius, int height, int slices, int stacks) {
//     std::vector<Point3D> vertices;

//     coneTriangleGenerator(radius, height, slices, stacks, vertices);

//     for (size_t i = 0; i < vertices.size(); i++) {
//         std::cout << "Vertex " << i << ": " << vertices[i] << "\n";
//     }

//     std::cout << "Number of triangles in model: " << (vertices.size() / 3) << "\n";

//     return vertices;
// }

ConeGeometry::ConeGeometry(int radius, int height, int slices, int stacks) {
    this->_kind = GEOMETRY_CONE;
    coneTriangleGenerator(radius, height, slices, stacks, this->vertices);
}

std::vector<Point3D> ConeGeometry::serialize() {
    std::vector<Point3D> ret;

    for (auto i : this->vertices) {
        ret.push_back(i.copy());
    }

    return ret;
}