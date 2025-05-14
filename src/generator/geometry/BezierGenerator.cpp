#define _USE_MATH_DEFINES

#include <array>
#include <cmath>
#include <filesystem>
#include <fstream>
#include <functional>
#include <iostream>
#include <unordered_map>
#include <vector>

#include "common/common.hpp"
#include "common/geometry/bezier.hpp"
#include "common/geometry/point.hpp"
#include "common/util/vectors.hpp"

BezierGeometry::~BezierGeometry() = default;

BezierGeometry::BezierGeometry(const char* filename, int tessellationLevel) {
    this->_kind = GEOMETRY_BEZIER;

    std::ifstream file(filename);
    if (!file.is_open()) {
        throw std::runtime_error("Failed to open Bezier patch file");
    }

    int patchCount;
    file >> patchCount;

    std::vector<std::array<int, 16>> patches(patchCount);
    for (int i = 0; i < patchCount; ++i) {
        for (int j = 0; j < 16; ++j) {
            file >> patches[i][j];
            if (file.peek() == ',') file.ignore();
        }
    }

    int vertexCount;
    file >> vertexCount;
    std::vector<Point3D> controlPoints(vertexCount);
    for (int i = 0; i < vertexCount; ++i) {
        float x, y, z;
        file >> x;
        if (file.peek() == ',') file.ignore();
        file >> y;
        if (file.peek() == ',') file.ignore();
        file >> z;

        controlPoints[i] = Point3D(x, y, z);
    }

    // Temporary vertex data
    struct TempVertexData {
        Point3D position;
        Vector3<float> normalSum;
        Vector2<float> uv;
    };

    std::vector<TempVertexData> tempVertices;
    std::unordered_map<VertexKey, unsigned int> vertexMap;

    auto addVertex = [&](const Point3D& pos, const Vector3<float>& normal, float u, float v) {
        VertexKey key{pos};
        auto it = vertexMap.find(key);
        if (it != vertexMap.end()) {
            tempVertices[it->second].normalSum += normal;
            return it->second;
        }

        unsigned int index = static_cast<unsigned int>(tempVertices.size());
        vertexMap[key] = index;
        tempVertices.push_back(TempVertexData{pos, normal, Vector2<float>(u, v)});
        return index;
    };

    for (const auto& patch : patches) {
        tessellateBezierPatch(patch, controlPoints, tessellationLevel, addVertex);
    }

    for (const auto& v : tempVertices) {
        this->vertices.push_back(v.position);
        this->normals.push_back(v.normalSum.normalized());
        this->uvs.push_back(v.uv);
    }
}

void BezierGeometry::tessellateBezierPatch(
    const std::array<int, 16>& patchIndices,
    const std::vector<Point3D>& controlPoints,
    int tessellationLevel,
    const std::function<unsigned int(const Point3D&, const Vector3<float>&, float, float)>& addVertex) {
    auto bernstein = [](int i, float t) {
        switch (i) {
            case 0:
                return (1 - t) * (1 - t) * (1 - t);
            case 1:
                return 3 * t * (1 - t) * (1 - t);
            case 2:
                return 3 * t * t * (1 - t);
            case 3:
                return t * t * t;
            default:
                return 0.0f;
        }
    };

    std::vector<std::vector<unsigned int>> gridIndices(tessellationLevel + 1);
    for (int i = 0; i <= tessellationLevel; ++i) {
        float u = float(i) / tessellationLevel;
        gridIndices[i].resize(tessellationLevel + 1);

        for (int j = 0; j <= tessellationLevel; ++j) {
            float v = float(j) / tessellationLevel;

            // Compute the position of the vertex on the surface using Bezier patch formula
            Point3D vertex(0, 0, 0);
            for (int k = 0; k < 4; ++k) {
                for (int l = 0; l < 4; ++l) {
                    float bu = bernstein(k, u);
                    float bv = bernstein(l, v);
                    int idx = patchIndices[k * 4 + l];
                    if (idx >= controlPoints.size()) continue;
                    vertex = vertex + controlPoints[idx] * (bu * bv);
                }
            }

            // Compute the tangents for normal calculation
            Point3D du(0, 0, 0), dv(0, 0, 0);
            for (int k = 0; k < 4; ++k) {
                for (int l = 0; l < 4; ++l) {
                    float bu = bernstein(k, u);
                    float bv = bernstein(l, v);
                    float dbu = 0, dbv = 0;

                    if (k == 0)
                        dbu = -3 * (1 - u) * (1 - u);
                    else if (k == 1)
                        dbu = 3 * (1 - u) * (1 - u) - 6 * u * (1 - u);
                    else if (k == 2)
                        dbu = 6 * u * (1 - u) - 3 * u * u;
                    else if (k == 3)
                        dbu = 3 * u * u;

                    if (l == 0)
                        dbv = -3 * (1 - v) * (1 - v);
                    else if (l == 1)
                        dbv = 3 * (1 - v) * (1 - v) - 6 * v * (1 - v);
                    else if (l == 2)
                        dbv = 6 * v * (1 - v) - 3 * v * v;
                    else if (l == 3)
                        dbv = 3 * v * v;

                    int idx = patchIndices[k * 4 + l];
                    if (idx >= controlPoints.size()) continue;
                    du = du + controlPoints[idx] * (dbu * bv);
                    dv = dv + controlPoints[idx] * (bu * dbv);
                }
            }

            Vector3<float> tangentU(du.getX(), du.getY(), du.getZ());
            Vector3<float> tangentV(dv.getX(), dv.getY(), dv.getZ());
            Vector3<float> normal = tangentV.cross(tangentU);
            normal.normalize();

            // Improved UV calculation with boundary handling
            float texU = u;
            float texV = 1.0f - v;  // Flip V coordinate to match standard texture orientation

            // For the last vertex in each row/column, ensure exact 1.0 or 0.0 to prevent seams
            if (i == tessellationLevel) texU = 1.0f;  // Rightmost column
            if (j == tessellationLevel) texV = 0.0f;  // Bottom row

            // Boundary cases for adjacent patches
            if (i == 0) texU = 0.0f;  // Leftmost column
            if (j == 0) texV = 1.0f;  // Top row

            // Add the vertex (possibly duplicate for edges)
            unsigned int index = addVertex(vertex, normal, texU, texV);
            gridIndices[i][j] = index;
        }
    }

    // Create triangles for the patch
    for (int i = 0; i < tessellationLevel; ++i) {
        for (int j = 0; j < tessellationLevel; ++j) {
            this->indices.push_back(gridIndices[i][j]);
            this->indices.push_back(gridIndices[i][j + 1]);
            this->indices.push_back(gridIndices[i + 1][j]);

            this->indices.push_back(gridIndices[i + 1][j]);
            this->indices.push_back(gridIndices[i][j + 1]);
            this->indices.push_back(gridIndices[i + 1][j + 1]);
        }
    }
}

std::vector<Point3D> BezierGeometry::copyVertices() {
    std::vector<Point3D> ret;
    for (auto i : this->vertices) {
        ret.push_back(i.copy());
    }
    return ret;
}

std::vector<Vector3<float>> BezierGeometry::copyNormals() {
    std::vector<Vector3<float>> ret;
    for (auto i : this->normals) {
        ret.push_back(i.copy());
    }
    return ret;
}

std::vector<Vector2<float>> BezierGeometry::copyUVs() {
    std::vector<Vector2<float>> ret;
    for (auto i : this->uvs) {
        ret.push_back(i.copy());
    }
    return ret;
}

std::vector<unsigned int> BezierGeometry::copyIndices() {
    std::vector<unsigned> ret;
    for (auto i : this->indices) {
        ret.push_back(i);
    }
    return ret;
}

BezierGeometry::BezierGeometry(std::vector<Point3D> vertices,
                               std::vector<Vector3<float>> normals,
                               std::vector<Vector2<float>> uvs,
                               std::vector<unsigned int> indices) {
    this->_kind = GEOMETRY_BEZIER;
    this->vertices = std::move(vertices);
    this->normals = std::move(normals);
    this->uvs = std::move(uvs);
    this->indices = std::move(indices);
}
