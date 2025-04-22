#define _USE_MATH_DEFINES

#include <cmath>
#include <vector>

#include "common/common.hpp"
#include "common/geometry/box.hpp"
#include "common/geometry/point.hpp"
#include "engine/glContext.hpp"

BoxGeometry::~BoxGeometry() = default;

BoxGeometry::BoxGeometry(int length, int gridSize) {
    this->_kind = GEOMETRY_BOX;

    float len = static_cast<float>(length);
    float half = len / 2.0f;
    int vertexOffset = 0;

    std::unordered_map<VertexKey, unsigned int> vertexMap;

    auto addFace = [&](Vector3<float> origin,
                       Vector3<float> uDir,
                       Vector3<float> vDir,
                       Vector3<float> normal) {
        float step = len / gridSize;

        for (int i = 0; i <= gridSize; ++i) {
            for (int j = 0; j <= gridSize; ++j) {
                Vector3<float> pos = origin + (uDir * (j * step)) + (vDir * (i * step));
                Point3D point = pos.toPoint3D();

                VertexKey key{point, normal};
                auto it = vertexMap.find(key);

                unsigned int index;
                if (it != vertexMap.end()) {
                    index = it->second;
                } else {
                    index = static_cast<unsigned int>(vertices.size());
                    vertices.push_back(point);
                    normals.push_back(normal);
                    vertexMap[key] = index;
                }
            }
        }

        for (int i = 0; i < gridSize; ++i) {
            for (int j = 0; j < gridSize; ++j) {
                int row1 = vertexOffset + i * (gridSize + 1);
                int row2 = vertexOffset + (i + 1) * (gridSize + 1);

                unsigned int i0 = row1 + j;
                unsigned int i1 = row2 + j;
                unsigned int i2 = row2 + j + 1;
                unsigned int i3 = row1 + j + 1;

                indices.push_back(i0);
                indices.push_back(i1);
                indices.push_back(i2);

                indices.push_back(i0);
                indices.push_back(i2);
                indices.push_back(i3);
            }
        }

        vertexOffset += (gridSize + 1) * (gridSize + 1);
    };

    // +Y
    addFace(Vector3<float>(-half, half, -half),
            Vector3<float>(1, 0, 0),
            Vector3<float>(0, 0, 1),
            Vector3<float>(0, 1, 0));

    // -Y
    addFace(Vector3<float>(-half, -half, half),
            Vector3<float>(1, 0, 0),
            Vector3<float>(0, 0, -1),
            Vector3<float>(0, -1, 0));

    // +Z
    addFace(Vector3<float>(-half, -half, half),
            Vector3<float>(0, 1, 0),
            Vector3<float>(1, 0, 0),
            Vector3<float>(0, 0, 1));

    // -Z
    addFace(Vector3<float>(half, -half, -half),
            Vector3<float>(0, 1, 0),
            Vector3<float>(-1, 0, 0),
            Vector3<float>(0, 0, -1));

    // +X
    addFace(Vector3<float>(half, -half, half),
            Vector3<float>(0, 1, 0),
            Vector3<float>(0, 0, -1),
            Vector3<float>(1, 0, 0));

    // -X
    addFace(Vector3<float>(-half, -half, -half),
            Vector3<float>(0, 1, 0),
            Vector3<float>(0, 0, 1),
            Vector3<float>(-1, 0, 0));
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