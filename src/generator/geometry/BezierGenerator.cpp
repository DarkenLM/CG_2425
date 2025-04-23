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
#include "engine/glContext.hpp"

BezierGeometry::~BezierGeometry() = default;

BezierGeometry::BezierGeometry(const char* filename, int tessellationLevel) {
    this->_kind = GEOMETRY_BEZIER;

    // Read the Bezier patch file
    std::ifstream file(filename);
    if (!file.is_open()) {
        throw std::runtime_error("Failed to open Bezier patch file");
    }

    // Read number of patches
    int patchCount;
    file >> patchCount;

    // Read patches (16 control points each)
    std::vector<std::array<int, 16>>
        patches(patchCount);
    for (int i = 0; i < patchCount; ++i) {
        std::cout << "Patch: " << i << ": ";
        for (int j = 0; j < 16; ++j) {
            file >> patches[i][j];
            if (file.peek() == ',') file.ignore();
            std::cout << patches[i][j] << " ";
        }
        std::cout << "\n";
    }

    // Read number of vertices
    int vertexCount;
    file >> vertexCount;
    std::vector<Point3D> controlPoints(vertexCount);

    // Read vertices
    for (int i = 0; i < vertexCount; ++i) {
        float x, y, z;
        file >> x;
        if (file.peek() == ',') file.ignore();
        file >> y;
        if (file.peek() == ',') file.ignore();
        file >> z;

        controlPoints[i] = Point3D(x, y, z);
    }

    // Vertex deduplication map
    std::unordered_map<VertexKey, unsigned int> vertexMap;

    auto addVertex = [&](const Point3D& pos, const Vector3<float>& normal) {
        VertexKey key{pos, normal};
        auto it = vertexMap.find(key);
        if (it != vertexMap.end()) return it->second;

        unsigned int index = static_cast<unsigned int>(this->vertices.size());
        this->vertices.push_back(pos);
        this->normals.push_back(normal);
        vertexMap[key] = index;
        return index;
    };

    // Tessellate each patch
    for (const auto& patch : patches) {
        tessellateBezierPatch(patch, controlPoints, tessellationLevel, addVertex);
    }
}

void BezierGeometry::tessellateBezierPatch(
    const std::array<int, 16>& patchIndices,
    const std::vector<Point3D>& controlPoints,
    int tessellationLevel,
    const std::function<unsigned int(const Point3D&, const Vector3<float>&)>& addVertex) {
    // Precompute Bernstein basis functions
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

    // Generate grid of points and normals
    std::vector<std::vector<unsigned int>> gridIndices(tessellationLevel + 1);
    for (int i = 0; i <= tessellationLevel; ++i) {
        float u = float(i) / tessellationLevel;
        gridIndices[i].resize(tessellationLevel + 1);

        for (int j = 0; j <= tessellationLevel; ++j) {
            float v = float(j) / tessellationLevel;

            // Evaluate position
            Point3D vertex(0, 0, 0);
            for (int k = 0; k < 4; ++k) {
                for (int l = 0; l < 4; ++l) {
                    float bu = bernstein(k, u);
                    float bv = bernstein(l, v);
                    int idx = patchIndices[k * 4 + l];
                    if (idx >= controlPoints.size()) {
                        std::cerr << "Invalid control point index: " << idx
                                  << " (max: " << controlPoints.size() << ")" << std::endl;
                        continue;
                    }
                    vertex = vertex + controlPoints[idx] * (bu * bv);
                }
            }

            // Evaluate partial derivatives for normal calculation
            Point3D du(0, 0, 0);
            Point3D dv(0, 0, 0);
            for (int k = 0; k < 4; ++k) {
                for (int l = 0; l < 4; ++l) {
                    float bu = bernstein(k, u);
                    float bv = bernstein(l, v);
                    float dbu = 0;
                    float dbv = 0;

                    // Derivative calculations
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

            // Calculate normal from partial derivatives
            Vector3<float> tangentU(du.getX(), du.getY(), du.getZ());
            Vector3<float> tangentV(dv.getX(), dv.getY(), dv.getZ());
            Vector3<float> normal = tangentU.cross(tangentV);
            normal.normalize();

            unsigned int index = addVertex(vertex, normal);
            gridIndices[i][j] = index;
        }
    }

    // Generate triangles - ensure proper winding order
    for (int i = 0; i < tessellationLevel; ++i) {
        for (int j = 0; j < tessellationLevel; ++j) {
            // First triangle (counter-clockwise)
            this->indices.push_back(gridIndices[i][j]);
            this->indices.push_back(gridIndices[i][j + 1]);
            this->indices.push_back(gridIndices[i + 1][j]);

            // Second triangle (counter-clockwise)
            this->indices.push_back(gridIndices[i + 1][j]);
            this->indices.push_back(gridIndices[i][j + 1]);
            this->indices.push_back(gridIndices[i + 1][j + 1]);
        }
    }
}

std::vector<Point3D> BezierGeometry::serielizeVertices() {
    std::vector<Point3D> ret;

    for (auto i : this->vertices) {
        ret.push_back(i.copy());
    }

    return ret;
}

std::vector<Vector3<float>> BezierGeometry::serielizeNormals() {
    std::vector<Vector3<float>> ret;

    for (auto i : this->normals) {
        ret.push_back(i.copy());
    }

    return ret;
}

std::vector<unsigned int> BezierGeometry::serielizeIndices() {
    std::vector<unsigned> ret;

    for (auto i : this->indices) {
        ret.push_back(i);
    }

    return ret;
}

BezierGeometry::BezierGeometry(std::vector<Point3D> vertices,
                               std::vector<Vector3<float>> normals,
                               std::vector<unsigned int> indices) {
    this->_kind = GEOMETRY_BEZIER;
    this->vertices = std::move(vertices);
    this->normals = std::move(normals);
    this->indices = std::move(indices);
}

BezierGeometry* BezierGeometry::deserialize(std::string filePath) {
    // Implementation would depend on your serialization format
    throw std::runtime_error("Deserialization not yet implemented");
}