// M_PI does not work in visual studio without this BS, so use it
#define _USE_MATH_DEFINES

#include <cmath>
#include <vector>

#include "common/common.hpp"
#include "common/geometry/icosphere.hpp"
#include "common/geometry/point.hpp"

IcosphereGeometry::~IcosphereGeometry() = default;

void normalize(std::vector<Point3D>& vertices, float radius) {
    for (auto& v : vertices) {
        float length = sqrt(v.getX() * v.getX() + v.getY() * v.getY() + v.getZ() * v.getZ());
        v.setX(v.getX() / length * radius);
        v.setY(v.getY() / length * radius);
        v.setZ(v.getZ() / length * radius);
    }
}

void subdivide(std::vector<Point3D>& vertices, std::vector<unsigned int>& indices) {
    std::vector<unsigned int> newIndices;
    std::vector<Point3D> newVertices = vertices;

    for (size_t i = 0; i < indices.size(); i += 3) {
        int a = indices[i];
        int b = indices[i + 1];
        int c = indices[i + 2];

        Point3D v1 = vertices[a];
        Point3D v2 = vertices[b];
        Point3D v3 = vertices[c];

        Point3D v4 = {
            (v1.getX() + v2.getX()) / 2,
            (v1.getY() + v2.getY()) / 2,
            (v1.getZ() + v2.getZ()) / 2};
        Point3D v5 = {
            (v2.getX() + v3.getX()) / 2,
            (v2.getY() + v3.getY()) / 2,
            (v2.getZ() + v3.getZ()) / 2};
        Point3D v6 = {
            (v1.getX() + v3.getX()) / 2,
            (v1.getY() + v3.getY()) / 2,
            (v1.getZ() + v3.getZ()) / 2};

        // Normalize midpoints to ensure they lie on the sphere
        float length4 = sqrt(v4.getX() * v4.getX() + v4.getY() * v4.getY() + v4.getZ() * v4.getZ());
        v4.setX(v4.getX() / length4);
        v4.setY(v4.getY() / length4);
        v4.setZ(v4.getZ() / length4);

        float length5 = sqrt(v5.getX() * v5.getX() + v5.getY() * v5.getY() + v5.getZ() * v5.getZ());
        v5.setX(v5.getX() / length5);
        v5.setY(v5.getY() / length5);
        v5.setZ(v5.getZ() / length5);

        float length6 = sqrt(v6.getX() * v6.getX() + v6.getY() * v6.getY() + v6.getZ() * v6.getZ());
        v6.setX(v6.getX() / length6);
        v6.setY(v6.getY() / length6);
        v6.setZ(v6.getZ() / length6);

        int i4 = newVertices.size();
        newVertices.push_back(v4);
        int i5 = newVertices.size();
        newVertices.push_back(v5);
        int i6 = newVertices.size();
        newVertices.push_back(v6);

        newIndices.push_back(a);
        newIndices.push_back(i4);
        newIndices.push_back(i6);
        newIndices.push_back(i4);
        newIndices.push_back(b);
        newIndices.push_back(i5);
        newIndices.push_back(i6);
        newIndices.push_back(i5);
        newIndices.push_back(c);
        newIndices.push_back(i4);
        newIndices.push_back(i5);
        newIndices.push_back(i6);
    }

    vertices = newVertices;
    indices = newIndices;
}

IcosphereGeometry::IcosphereGeometry(int radius, int subdivisions) {
    this->_kind = GEOMETRY_ICOSPHERE;

    // Local vectors for vertices and indices
    std::vector<Point3D> vertices;
    std::vector<unsigned int> indices;

    // Creating standard icosahedron
    const float t = (1.0f + sqrt(5.0f)) / 2.0f;

    vertices.push_back(Point3D(-radius, t * radius, 0));
    vertices.push_back(Point3D(radius, t * radius, 0));
    vertices.push_back(Point3D(-radius, -t * radius, 0));
    vertices.push_back(Point3D(radius, -t * radius, 0));

    vertices.push_back(Point3D(0, -radius, t * radius));
    vertices.push_back(Point3D(0, radius, t * radius));
    vertices.push_back(Point3D(0, -radius, -t * radius));
    vertices.push_back(Point3D(0, radius, -t * radius));

    vertices.push_back(Point3D(t * radius, 0, -radius));
    vertices.push_back(Point3D(t * radius, 0, radius));
    vertices.push_back(Point3D(-t * radius, 0, -radius));
    vertices.push_back(Point3D(-t * radius, 0, radius));

    // Add the 20 faces of the icosahedron
    indices.insert(indices.end(), {0, 11, 5});
    indices.insert(indices.end(), {0, 5, 1});
    indices.insert(indices.end(), {0, 1, 7});
    indices.insert(indices.end(), {0, 7, 10});
    indices.insert(indices.end(), {0, 10, 11});

    indices.insert(indices.end(), {1, 5, 9});
    indices.insert(indices.end(), {5, 11, 4});
    indices.insert(indices.end(), {11, 10, 2});
    indices.insert(indices.end(), {10, 7, 6});
    indices.insert(indices.end(), {7, 1, 8});

    indices.insert(indices.end(), {3, 9, 4});
    indices.insert(indices.end(), {3, 4, 2});
    indices.insert(indices.end(), {3, 2, 6});
    indices.insert(indices.end(), {3, 6, 8});
    indices.insert(indices.end(), {3, 8, 9});

    indices.insert(indices.end(), {4, 9, 5});
    indices.insert(indices.end(), {2, 4, 11});
    indices.insert(indices.end(), {6, 2, 10});
    indices.insert(indices.end(), {8, 6, 7});
    indices.insert(indices.end(), {9, 8, 1});

    // Subdivide the icosahedron
    for (int i = 0; i < subdivisions; ++i) {
        subdivide(vertices, indices);
        normalize(vertices, radius);
    }

    // Copy vertices to this->vertices in the correct order
    for (size_t i = 0; i < indices.size(); i++) {
        this->vertices.push_back(vertices[indices[i]]);
    }
}
std::vector<Point3D> IcosphereGeometry::serielizeVertices() {
    std::vector<Point3D> ret;

    for (auto i : this->vertices) {
        ret.push_back(i.copy());
    }

    return ret;
}

std::vector<Vector3<float>> IcosphereGeometry::serielizeNormals() {
    std::vector<Vector3<float>> ret;

    for (auto i : this->normals) {
        ret.push_back(i.copy());
    }

    return ret;
}

std::vector<unsigned int> IcosphereGeometry::serielizeIndices() {
    std::vector<unsigned> ret;

    for (auto i : this->indices) {
        ret.push_back(i);
    }

    return ret;
}