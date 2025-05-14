// M_PI does not work in visual studio without this BS, so use it
#define _USE_MATH_DEFINES

#include <cmath>
#include <vector>

#include "common/common.hpp"
#include "common/geometry/cone.hpp"
#include "common/geometry/point.hpp"
#include "common/util/vectors.hpp"

ConeGeometry::~ConeGeometry() = default;

ConeGeometry::ConeGeometry(int radius, int height, int slices, int stacks) {
    this->_kind = GEOMETRY_CONE;

    float alpha = (2 * M_PI) / slices;
    Point3D baseCenter(0.0f, 0.0f, 0.0f);
    Point3D apex(0.0f, static_cast<float>(height), 0.0f);

    std::unordered_map<VertexKey, unsigned int> vertexMap;
    std::unordered_map<unsigned int, Vector3<float>> normalSums;
    std::unordered_map<unsigned int, int> normalCounts;

    auto addVertex = [&](const Point3D& pos, const Vector3<float>& faceNormal, float u, float v, bool forceNew = false) {
        unsigned int index;
        VertexKey key{pos};

        if (!forceNew) {
            auto it = vertexMap.find(key);
            if (it != vertexMap.end()) {
                index = it->second;
            } else {
                index = static_cast<unsigned int>(this->vertices.size());
                vertexMap[key] = index;
                this->vertices.push_back(pos);
                this->normals.emplace_back(0.0f, 0.0f, 0.0f);
                this->uvs.emplace_back(u, v);
            }
        } else {
            // Always create a new vertex (used for duplicated base circle vertices)
            index = static_cast<unsigned int>(this->vertices.size());
            this->vertices.push_back(pos);
            this->normals.emplace_back(0.0f, 0.0f, 0.0f);
            this->uvs.emplace_back(u, v);
        }

        normalSums[index] += faceNormal;
        normalCounts[index]++;
        return index;
    };

    // --- Base ---
    Vector3<float> down(0.0f, -1.0f, 0.0f);  // Normal for the base (flat shading)
    for (int i = 0; i < slices; i++) {
        float angle1 = i * alpha;
        float angle2 = (i + 1) * alpha;

        float x1 = radius * cos(angle1);
        float z1 = radius * sin(angle1);
        float x2 = radius * cos(angle2);
        float z2 = radius * sin(angle2);

        Point3D p1(x1, 0.0f, z1);
        Point3D p2(x2, 0.0f, z2);

        // UVs for the base using polar coordinates
        auto uv = [](float angle, float radius) {
            return Vector2<float>(0.5f + cos(angle) * 0.5f, 0.5f + sin(angle) * 0.5f);
        };

        unsigned int i1_base = addVertex(p2, down, uv(angle2, radius).first, uv(angle2, radius).second, true);
        unsigned int i2_base = addVertex(baseCenter, down, 0.5f, 0.5f, true);
        unsigned int i3_base = addVertex(p1, down, uv(angle1, radius).first, uv(angle1, radius).second, true);

        // Use these indices to create the base face
        this->indices.insert(this->indices.end(), {i1_base, i2_base, i3_base});
    }

    // --- Side ---
    for (int i = 0; i < slices; i++) {
        for (int j = 0; j < stacks; j++) {
            float t1 = static_cast<float>(j) / stacks;
            float t2 = static_cast<float>(j + 1) / stacks;

            float y1 = t1 * height;
            float y2 = t2 * height;

            float scale1 = 1.0f - t1;
            float scale2 = 1.0f - t2;

            float angle1 = i * alpha;
            float angle2 = (i + 1) * alpha;

            float x1 = radius * cos(angle1);
            float z1 = radius * sin(angle1);
            float x2 = radius * cos(angle2);
            float z2 = radius * sin(angle2);

            Point3D p1(x1 * scale2, y2, z1 * scale2);
            Point3D p2(x1 * scale1, y1, z1 * scale1);
            Point3D p3(x2 * scale2, y2, z2 * scale2);
            Point3D p4(x2 * scale1, y1, z2 * scale1);

            // Normals for two triangles forming a quad
            Vector3<float> n1 = (p3.toVector3() - p1.toVector3()).cross(p2.toVector3() - p1.toVector3()).normalized();
            Vector3<float> n2 = (p3.toVector3() - p2.toVector3()).cross(p4.toVector3() - p2.toVector3()).normalized();

            // Corrected side UVs:
            float u1_side = static_cast<float>(i) / slices;
            float u2_side = static_cast<float>(i + 1) / slices;
            float v1_side = static_cast<float>(j) / stacks;
            float v2_side = static_cast<float>(j + 1) / stacks;

            // For the last slice, we want to connect back to the beginning
            if (i == slices - 1) {
                u2_side = 1.0f;
            }

            // Duplicating base vertices for side faces:
            unsigned int i1_side = addVertex(p1, n1, u1_side, v2_side);
            unsigned int i2_side = addVertex(p2, n1, u1_side, v1_side);
            unsigned int i3_side = addVertex(p3, n1, u2_side, v2_side);

            unsigned int i4_side = addVertex(p3, n2, u2_side, v2_side);
            unsigned int i5_side = addVertex(p2, n2, u1_side, v1_side);
            unsigned int i6_side = addVertex(p4, n2, u2_side, v1_side);

            // Create the indices for the side faces
            this->indices.insert(this->indices.end(), {i1_side, i3_side, i2_side});
            this->indices.insert(this->indices.end(), {i4_side, i6_side, i5_side});
        }
    }

    // --- Normalize normals ---
    for (size_t i = 0; i < this->normals.size(); ++i) {
        if (normalCounts[i] > 0) {
            Vector3<float> avg = normalSums[i] * (1.0f / static_cast<float>(normalCounts[i]));
            this->normals[i] = avg.normalized();
        }
    }
}

std::vector<Point3D> ConeGeometry::copyVertices() {
    std::vector<Point3D> ret;

    for (auto i : this->vertices) {
        ret.push_back(i.copy());
    }

    return ret;
}

std::vector<Vector3<float>> ConeGeometry::copyNormals() {
    std::vector<Vector3<float>> ret;

    for (auto i : this->normals) {
        ret.push_back(i.copy());
    }

    return ret;
}

std::vector<unsigned int> ConeGeometry::copyIndices() {
    std::vector<unsigned> ret;

    for (auto i : this->indices) {
        ret.push_back(i);
    }

    return ret;
}