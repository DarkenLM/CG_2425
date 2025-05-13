#define _USE_MATH_DEFINES

#include <cmath>
#include <unordered_map>
#include <vector>

#include "common/common.hpp"
#include "common/geometry/box.hpp"
#include "common/geometry/point.hpp"
#include "common/util/vectors.hpp"

BoxGeometry::~BoxGeometry() = default;

BoxGeometry::BoxGeometry(int length, int gridSize) {
    this->_kind = GEOMETRY_BOX;

    float len = static_cast<float>(length);
    float half = len / 2.0f;
    float step = len / gridSize;

    std::unordered_map<VertexKey, unsigned int> vertexMap;
    std::unordered_map<unsigned int, Vector3<float>> normalSums;
    std::unordered_map<unsigned int, int> normalCounts;

    auto addFace = [&](Vector3<float> origin,
                       Vector3<float> uDir,
                       Vector3<float> vDir,
                       Vector3<float> faceNormal) {
        std::vector<std::vector<unsigned int>> gridIndices(gridSize + 1, std::vector<unsigned int>(gridSize + 1));

        for (int i = 0; i <= gridSize; ++i) {
            for (int j = 0; j <= gridSize; ++j) {
                Vector3<float> pos = origin + (uDir * (j * step)) + (vDir * (i * step));
                Point3D point = pos.toPoint3D();
                VertexKey key{point};

                unsigned int index;
                auto it = vertexMap.find(key);
                if (it != vertexMap.end()) {
                    index = it->second;
                } else {
                    index = static_cast<unsigned int>(vertices.size());
                    vertexMap[key] = index;
                    vertices.push_back(point);
                    normals.emplace_back(0.0f, 0.0f, 0.0f);  // placeholder

                    // UVs
                    float u = j / static_cast<float>(gridSize);
                    float v = i / static_cast<float>(gridSize);
                    uvs.emplace_back(u, v);
                }

                // Acumular a normal
                normalSums[index] += faceNormal;
                normalCounts[index] += 1;

                gridIndices[i][j] = index;
            }
        }

        // Generate face indices (2 triangles per quad)
        for (int i = 0; i < gridSize; ++i) {
            for (int j = 0; j < gridSize; ++j) {
                unsigned int i0 = gridIndices[i][j];
                unsigned int i1 = gridIndices[i + 1][j];
                unsigned int i2 = gridIndices[i + 1][j + 1];
                unsigned int i3 = gridIndices[i][j + 1];

                indices.push_back(i0);
                indices.push_back(i2);
                indices.push_back(i1);

                indices.push_back(i0);
                indices.push_back(i3);
                indices.push_back(i2);
            }
        }
    };

    // Add all 6 faces of the box
    addFace(Vector3<float>(-half, half, -half), Vector3<float>(0, 0, 1), Vector3<float>(1, 0, 0), Vector3<float>(0, 1, 0));    // +Y
    addFace(Vector3<float>(-half, -half, -half), Vector3<float>(1, 0, 0), Vector3<float>(0, 0, 1), Vector3<float>(0, -1, 0));  // -Y
    addFace(Vector3<float>(-half, -half, half), Vector3<float>(1, 0, 0), Vector3<float>(0, 1, 0), Vector3<float>(0, 0, 1));    // +Z
    addFace(Vector3<float>(half, -half, -half), Vector3<float>(-1, 0, 0), Vector3<float>(0, 1, 0), Vector3<float>(0, 0, -1));  // -Z
    addFace(Vector3<float>(half, -half, half), Vector3<float>(0, 0, -1), Vector3<float>(0, 1, 0), Vector3<float>(1, 0, 0));    // +X
    addFace(Vector3<float>(-half, -half, -half), Vector3<float>(0, 0, 1), Vector3<float>(0, 1, 0), Vector3<float>(-1, 0, 0));  // -X

    // Final pass: normalize the averaged normals (smooth normals for faces that share vertices)
    for (size_t i = 0; i < normals.size(); ++i) {
        if (normalCounts[i] > 0) {
            Vector3<float> avg = normalSums[i] * (1.0f / static_cast<float>(normalCounts[i]));
            avg.normalize();
            normals[i] = avg;
        }
    }
}

std::vector<Point3D> BoxGeometry::copyVertices() {
    std::vector<Point3D> ret;
    for (auto v : this->vertices) {
        ret.push_back(v.copy());
    }
    return ret;
}

std::vector<Vector3<float>> BoxGeometry::copyNormals() {
    std::vector<Vector3<float>> ret;
    for (auto n : this->normals) {
        ret.push_back(n.copy());
    }
    return ret;
}

std::vector<Vector2<float>> BoxGeometry::copyUVs() {
    std::vector<Vector2<float>> ret;
    for (auto uv : this->uvs) {
        ret.push_back(uv.copy());
    }
    return ret;
}

std::vector<unsigned int> BoxGeometry::copyIndices() {
    std::vector<unsigned int> ret;
    for (auto i : this->indices) {
        ret.push_back(i);
    }
    return ret;
}