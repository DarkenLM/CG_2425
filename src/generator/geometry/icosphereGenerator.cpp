#define _USE_MATH_DEFINES
#include <cmath>
#include <unordered_map>
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
    return (i1 < i2) ? std::make_pair(i1, i2) : std::make_pair(i2, i1);
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

        Point3D mid(
            (v1.getX() + v2.getX()) / 2.0f,
            (v1.getY() + v2.getY()) / 2.0f,
            (v1.getZ() + v2.getZ()) / 2.0f);

        // Normalize to radius
        float length = std::sqrt(mid.getX() * mid.getX() + mid.getY() * mid.getY() + mid.getZ() * mid.getZ());
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

    const float t = (1.0f + std::sqrt(5.0f)) / 2.0f;

    // Initial 12 vertices of an icosahedron
    vertices = {
        {-1, t, 0}, {1, t, 0}, {-1, -t, 0}, {1, -t, 0}, {0, -1, t}, {0, 1, t}, {0, -1, -t}, {0, 1, -t}, {t, 0, -1}, {t, 0, 1}, {-t, 0, -1}, {-t, 0, 1}};

    // Normalize all vertices to the specified radius
    for (auto& v : vertices) {
        float len = std::sqrt(v.getX() * v.getX() + v.getY() * v.getY() + v.getZ() * v.getZ());
        v.setX(v.getX() / len * radius);
        v.setY(v.getY() / len * radius);
        v.setZ(v.getZ() / len * radius);
    }

    // Initial 20 faces (indices)
    indices = {
        0, 11, 5, 0, 5, 1, 0, 1, 7, 0, 7, 10, 0, 10, 11,
        1, 5, 9, 5, 11, 4, 11, 10, 2, 10, 7, 6, 7, 1, 8,
        3, 9, 4, 3, 4, 2, 3, 2, 6, 3, 6, 8, 3, 8, 9,
        4, 9, 5, 2, 4, 11, 6, 2, 10, 8, 6, 7, 9, 8, 1};

    // Apply subdivisions
    std::unordered_map<EdgeKey, unsigned int, EdgeKeyHasher> midpointCache;
    for (int i = 0; i < subdivisions; ++i) {
        subdivideIndexed(vertices, indices, midpointCache, radius);
    }

    // Final assignment
    this->vertices = vertices;
    this->indices = indices;

    // --- Normals: averaged per vertex from face normals ---
    std::unordered_map<unsigned int, Vector3<float>> normalSums;
    std::unordered_map<unsigned int, int> normalCounts;

    for (size_t i = 0; i < indices.size(); i += 3) {
        unsigned int i0 = indices[i];
        unsigned int i1 = indices[i + 1];
        unsigned int i2 = indices[i + 2];

        Point3D p0 = vertices[i0];
        Point3D p1 = vertices[i1];
        Point3D p2 = vertices[i2];

        Vector3<float> v0(p0.getX(), p0.getY(), p0.getZ());
        Vector3<float> v1(p1.getX(), p1.getY(), p1.getZ());
        Vector3<float> v2(p2.getX(), p2.getY(), p2.getZ());

        Vector3<float> edge1 = v1 - v0;
        Vector3<float> edge2 = v2 - v0;
        Vector3<float> faceNormal = edge1.cross(edge2).normalized();

        for (unsigned int idx : {i0, i1, i2}) {
            normalSums[idx] += faceNormal;
            normalCounts[idx]++;
        }
    }

    this->normals.reserve(vertices.size());
    for (size_t i = 0; i < vertices.size(); ++i) {
        if (normalCounts.count(i)) {
            Vector3<float> avg = normalSums[i] * (1.0f / normalCounts[i]);
            this->normals.push_back(avg.normalized());
        } else {
            Vector3<float> fallback(vertices[i].getX(), vertices[i].getY(), vertices[i].getZ());
            this->normals.push_back(fallback.normalized());
        }
    }

    // --- UV Mapping: spherical projection ---
    this->uvs.reserve(vertices.size());
    for (const auto& v : vertices) {
        float x = v.getX();
        float y = v.getY();
        float z = v.getZ();

        float u = 0.5f + atan2(z, x) / (2.0f * static_cast<float>(M_PI));
        float vTex = 0.5f - asin(y / radius) / static_cast<float>(M_PI);

        this->uvs.emplace_back(u, vTex);
    }
}

std::vector<Point3D> IcosphereGeometry::copyVertices() {
    std::vector<Point3D> ret;
    for (auto& i : this->vertices) ret.push_back(i.copy());
    return ret;
}

std::vector<Vector3<float>> IcosphereGeometry::copyNormals() {
    std::vector<Vector3<float>> ret;
    for (auto& i : this->normals) ret.push_back(i.copy());
    return ret;
}

std::vector<unsigned int> IcosphereGeometry::copyIndices() {
    std::vector<unsigned int> ret;
    for (auto i : this->indices) ret.push_back(i);
    return ret;
}
