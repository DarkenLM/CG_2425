#define _USE_MATH_DEFINES

#include <cmath>
#include <unordered_map>
#include <vector>

#include "common/common.hpp"
#include "common/geometry/point.hpp"
#include "common/geometry/sphere.hpp"

SphereGeometry::~SphereGeometry() = default;

SphereGeometry::SphereGeometry(float radius, int slices, int stacks) {
    this->_kind = GEOMETRY_SPHERE;

    struct VertexData {
        Point3D position;
        Vector3<float> normal;
        Vector2<float> uv;
    };

    std::vector<VertexData> verticesData;
    std::vector<std::vector<unsigned int>> indexGrid;

    float sliceStep = 2.0f * M_PI / slices;
    float stackStep = M_PI / stacks;

    // First pass: create all vertices with their geometric normals
    for (int i = 0; i <= stacks; ++i) {
        float stackAngle = M_PI / 2 - i * stackStep;
        float xy = radius * cosf(stackAngle);
        float y = radius * sinf(stackAngle);

        std::vector<unsigned int> row;

        for (int j = 0; j <= slices; ++j) {
            float sectorAngle = j * sliceStep;
            float x = xy * cosf(sectorAngle);
            float z = xy * sinf(sectorAngle);

            Point3D pos(x, y, z);
            Vector3<float> normal = Vector3<float>(x, y, z).normalized();

            // Keep your original u calculation but ensure proper wrapping
            float u = 1.0f - (static_cast<float>(j) / slices);
            float v = 1.0f - (static_cast<float>(i) / stacks);

            // For the last vertex in each ring, we want u=0.0 to match the first vertex's u=1.0
            // (because of the 1.0 - operation)
            if (j == slices) u = 0.0f;

            verticesData.push_back({pos, normal, Vector2<float>(u, v)});
            row.push_back(static_cast<unsigned int>(verticesData.size() - 1));
        }

        indexGrid.push_back(row);
    }

    // Second pass: create smooth normals by averaging adjacent faces
    for (int i = 0; i < stacks; ++i) {
        for (int j = 0; j < slices; ++j) {
            unsigned int i1 = indexGrid[i][j];
            unsigned int i2 = indexGrid[i][j + 1];
            unsigned int i3 = indexGrid[i + 1][j];
            unsigned int i4 = indexGrid[i + 1][j + 1];

            const Point3D& p1 = verticesData[i1].position;
            const Point3D& p2 = verticesData[i2].position;
            const Point3D& p3 = verticesData[i3].position;
            const Point3D& p4 = verticesData[i4].position;

            Vector3<float> v1(p1.getX(), p1.getY(), p1.getZ());
            Vector3<float> v2(p2.getX(), p2.getY(), p2.getZ());
            Vector3<float> v3(p3.getX(), p3.getY(), p3.getZ());
            Vector3<float> v4(p4.getX(), p4.getY(), p4.getZ());

            // Triangle 1
            Vector3<float> n1 = (v2 - v1).cross(v3 - v1).normalized();
            // Triangle 2
            Vector3<float> n2 = (v4 - v2).cross(v3 - v2).normalized();

            // Add face normals to all participating vertices
            verticesData[i1].normal += n1;
            verticesData[i2].normal += n1 + n2;
            verticesData[i3].normal += n1 + n2;
            verticesData[i4].normal += n2;

            this->indices.insert(this->indices.end(), {i1, i2, i3});
            this->indices.insert(this->indices.end(), {i2, i4, i3});
        }
    }

    // Handle seam vertices by copying normals and UVs from first to last vertex in each ring
    for (int i = 0; i <= stacks; ++i) {
        unsigned int first = indexGrid[i][0];
        unsigned int last = indexGrid[i][slices];
        verticesData[last].normal = verticesData[first].normal;
        // For the flipped UVs, we need to set u=0.0 at the seam to match u=1.0 at the start
        verticesData[last].uv = Vector2<float>(0.0f, verticesData[first].uv.second);
    }

    // Final normalization and assignment
    for (auto& vd : verticesData) {
        vd.normal.normalize();
        this->vertices.push_back(vd.position);
        this->normals.push_back(vd.normal);
        this->uvs.push_back(vd.uv);
    }
}

std::vector<Point3D> SphereGeometry::copyVertices() {
    std::vector<Point3D> ret;

    for (size_t i = 0; i < this->vertices.size(); ++i) {
        ret.push_back(this->vertices[i].copy());
    }

    return ret;
}

std::vector<Vector3<float>> SphereGeometry::copyNormals() {
    std::vector<Vector3<float>> ret;

    for (size_t i = 0; i < this->normals.size(); ++i) {
        ret.push_back(this->normals[i].copy());
    }

    return ret;
}

std::vector<unsigned int> SphereGeometry::copyIndices() {
    std::vector<unsigned int> ret;

    for (size_t i = 0; i < this->indices.size(); ++i) {
        ret.push_back(this->indices[i]);
    }

    return ret;
}
