// M_PI does not work in visual studio without this BS, so use it
#define _USE_MATH_DEFINES

#include <cmath>
#include <vector>

#include "common/common.hpp"
#include "common/geometry/icosphere.hpp"
#include "common/geometry/point.hpp"

IcosphereGeometry::~IcosphereGeometry() = default;

using EdgeKey = std::pair<unsigned int, unsigned int>;

struct EdgeKeyHasher {
    std::size_t operator()(const EdgeKey& k) const {
        return std::hash<unsigned int>()(k.first) ^ std::hash<unsigned int>()(k.second);
    }
};

std::pair<unsigned int, unsigned int> minmax(unsigned int i1, unsigned int i2) {
    if (i1 < i2)
        return {i1, i2};
    else
        return {i2, i1};
}

void subdivideIndexed(std::vector<Point3D>& vertices, std::vector<unsigned int>& indices,
                      std::unordered_map<EdgeKey, unsigned int, EdgeKeyHasher>& midpointCache,
                      float radius) {
    std::vector<unsigned int> newIndices;

    auto getMidpointIndex = [&](unsigned int i1, unsigned int i2) {
        EdgeKey key = minmax(i1, i2);
        auto it = midpointCache.find(key);
        if (it != midpointCache.end()) return it->second;

        Point3D v1 = vertices[i1];
        Point3D v2 = vertices[i2];

        Point3D mid = Point3D((v1.getX() + v2.getX()) / 2.0f,
                              (v1.getY() + v2.getY()) / 2.0f,
                              (v1.getZ() + v2.getZ()) / 2.0f);

        // Normalize
        float length = sqrt(mid.getX() * mid.getX() + mid.getY() * mid.getY() + mid.getZ() * mid.getZ());
        mid.setX(mid.getX() / length * radius);
        mid.setY(mid.getY() / length * radius);
        mid.setZ(mid.getZ() / length * radius);

        unsigned int index = vertices.size();
        vertices.push_back(mid);
        midpointCache[key] = index;
        return index;
    };

    for (size_t i = 0; i < indices.size(); i += 3) {
        unsigned int a = indices[i];
        unsigned int b = indices[i + 1];
        unsigned int c = indices[i + 2];

        unsigned int ab = getMidpointIndex(a, b);
        unsigned int bc = getMidpointIndex(b, c);
        unsigned int ca = getMidpointIndex(c, a);

        newIndices.insert(newIndices.end(), {a, ab, ca});
        newIndices.insert(newIndices.end(), {b, bc, ab});
        newIndices.insert(newIndices.end(), {c, ca, bc});
        newIndices.insert(newIndices.end(), {ab, bc, ca});
    }

    indices = newIndices;
}

IcosphereGeometry::IcosphereGeometry(int radius, int subdivisions) {
    this->_kind = GEOMETRY_ICOSPHERE;

    std::vector<Point3D> vertices;
    std::vector<unsigned int> indices;

    const float t = (1.0f + sqrt(5.0f)) / 2.0f;

    // Initial vertices
    vertices = {
        {-1, t, 0}, {1, t, 0}, {-1, -t, 0}, {1, -t, 0}, {0, -1, t}, {0, 1, t}, {0, -1, -t}, {0, 1, -t}, {t, 0, -1}, {t, 0, 1}, {-t, 0, -1}, {-t, 0, 1}};

    // Normalize all to radius
    for (auto& v : vertices) {
        float len = sqrt(v.getX() * v.getX() + v.getY() * v.getY() + v.getZ() * v.getZ());
        v.setX(v.getX() / len * radius);
        v.setY(v.getY() / len * radius);
        v.setZ(v.getZ() / len * radius);
    }

    // Initial 20 triangle indices
    indices = {
        0, 11, 5, 0, 5, 1, 0, 1, 7, 0, 7, 10, 0, 10, 11,
        1, 5, 9, 5, 11, 4, 11, 10, 2, 10, 7, 6, 7, 1, 8,
        3, 9, 4, 3, 4, 2, 3, 2, 6, 3, 6, 8, 3, 8, 9,
        4, 9, 5, 2, 4, 11, 6, 2, 10, 8, 6, 7, 9, 8, 1};

    std::unordered_map<EdgeKey, unsigned int, EdgeKeyHasher> midpointCache;

    // Apply subdivisions
    for (int i = 0; i < subdivisions; ++i) {
        subdivideIndexed(vertices, indices, midpointCache, radius);
    }

    // Final assignment
    this->vertices = vertices;
    this->indices = indices;

    for (const auto& v : vertices) {
        Vector3<float> normal(v.getX(), v.getY(), v.getZ());
        this->normals.push_back(normal.normalized());
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