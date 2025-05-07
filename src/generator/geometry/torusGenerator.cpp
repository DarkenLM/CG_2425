#define _USE_MATH_DEFINES

#include <cmath>
#include <vector>

#include "common/common.hpp"
#include "common/geometry/point.hpp"
#include "common/geometry/torus.hpp"

// Helper to compute torus points
Point3D torusPoint(float R, float r, float beta, float alpha) {
    float x = (R + r * cos(alpha)) * cos(beta);
    float y = r * sin(alpha);
    float z = (R + r * cos(alpha)) * sin(beta);
    return Point3D(x, y, z);
}

TorusGeometry::~TorusGeometry() = default;

TorusGeometry::TorusGeometry(int majorRadius, int minorRadius, int slices, int sides) {
    this->_kind = GEOMETRY_TORUS;

    float sliceStep = 2 * M_PI / slices;
    float sideStep = 2 * M_PI / sides;

    std::unordered_map<VertexKey, unsigned int> vertexMap;

    auto addVertex = [&](const Point3D& pos, const Vector3<float>& normal) {
        VertexKey key{pos, normal};
        auto it = vertexMap.find(key);
        if (it != vertexMap.end()) return it->second;

        unsigned int index = static_cast<unsigned int>(this->vertices.size());
        vertexMap[key] = index;
        this->vertices.push_back(pos);
        this->normals.push_back(normal);
        return index;
    };

    for (int i = 0; i < slices; ++i) {
        float beta = i * sliceStep;
        float nextBeta = (i + 1) * sliceStep;

        for (int j = 0; j < sides; ++j) {
            float alpha = j * sideStep;
            float nextAlpha = (j + 1) * sideStep;

            // Four corners of the quad
            Point3D p1 = torusPoint(majorRadius, minorRadius, beta, alpha);
            Point3D p2 = torusPoint(majorRadius, minorRadius, nextBeta, alpha);
            Point3D p3 = torusPoint(majorRadius, minorRadius, beta, nextAlpha);
            Point3D p4 = torusPoint(majorRadius, minorRadius, nextBeta, nextAlpha);

            // Normals (from center of torus tube to point)
            Point3D center1(majorRadius * cos(beta), 0, majorRadius * sin(beta));
            Vector3<float> n1 = Vector3<float>(p1.getX() - center1.getX(),
                                               p1.getY() - center1.getY(),
                                               p1.getZ() - center1.getZ())
                                    .normalized();
            Vector3<float> n2 = Vector3<float>(p2.getX() - center1.getX(),
                                               p2.getY() - center1.getY(),
                                               p2.getZ() - center1.getZ())
                                    .normalized();
            Vector3<float> n3 = Vector3<float>(p3.getX() - center1.getX(),
                                               p3.getY() - center1.getY(),
                                               p3.getZ() - center1.getZ())
                                    .normalized();
            Vector3<float> n4 = Vector3<float>(p4.getX() - center1.getX(),
                                               p4.getY() - center1.getY(),
                                               p4.getZ() - center1.getZ())
                                    .normalized();

            // Add vertices with normals
            unsigned int i1 = addVertex(p1, n1);
            unsigned int i2 = addVertex(p2, n2);
            unsigned int i3 = addVertex(p3, n3);
            unsigned int i4 = addVertex(p4, n4);

            // Add triangles (CCW order)
            this->indices.insert(this->indices.end(), {i1, i3, i2});
            this->indices.insert(this->indices.end(), {i3, i4, i2});
        }
    }
}

std::vector<Point3D> TorusGeometry::copyVertices() {
    std::vector<Point3D> ret;

    for (auto i : this->vertices) {
        ret.push_back(i.copy());
    }

    return ret;
}

std::vector<Vector3<float>> TorusGeometry::copyNormals() {
    std::vector<Vector3<float>> ret;

    for (auto i : this->normals) {
        ret.push_back(i.copy());
    }

    return ret;
}

std::vector<unsigned int> TorusGeometry::copyIndices() {
    std::vector<unsigned> ret;

    for (auto i : this->indices) {
        ret.push_back(i);
    }

    return ret;
}
