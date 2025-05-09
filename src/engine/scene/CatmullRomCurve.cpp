#include "engine/scene/CatmullRomCurve.hpp"

#include <GL/glut.h>

#include <cmath>
#include <vector>

#include "common/geometry/point.hpp"

// Constructor using Point3D objects
CatmullRom::CatmullRom(const std::vector<Point3D>& controlPoints)
    : points(controlPoints), POINT_COUNT(controlPoints.size()) {}

// Function to get the global Catmull-Rom point
void CatmullRom::getGlobalCatmullRomPoint(float gt, float* pos, float* deriv) const {
    float t = gt * POINT_COUNT;
    int index = static_cast<int>(floor(t));
    t = t - index;

    // Indices for the control points
    int indices[4];
    indices[0] = (index + POINT_COUNT - 1) % POINT_COUNT;
    indices[1] = (indices[0] + 1) % POINT_COUNT;
    indices[2] = (indices[1] + 1) % POINT_COUNT;
    indices[3] = (indices[2] + 1) % POINT_COUNT;

    // Call to calculate the Catmull-Rom point and derivative
    getCatmullRomPoint(t,
                       points[indices[0]],  // Access the raw data of Point3D
                       points[indices[1]],
                       points[indices[2]],
                       points[indices[3]],
                       pos, deriv);
}

void CatmullRom::renderCatmullRomCurve() {
    float pos[3];
    float deriv[3];

    glBegin(GL_LINE_LOOP);
    for (float i = 0.01f; i < 1.0f; i += 0.01f) {
        getGlobalCatmullRomPoint(i, pos, deriv);
        glVertex3f(pos[0], pos[1], pos[2]);
    }
    glEnd();
}

void CatmullRom::buildRotMatrix(float* x, float* y, float* z, float* m) {
    m[0] = x[0];
    m[1] = x[1];
    m[2] = x[2];
    m[3] = 0;
    m[4] = y[0];
    m[5] = y[1];
    m[6] = y[2];
    m[7] = 0;
    m[8] = z[0];
    m[9] = z[1];
    m[10] = z[2];
    m[11] = 0;
    m[12] = 0;
    m[13] = 0;
    m[14] = 0;
    m[15] = 1;
}

void CatmullRom::multMatrixVector(const float* m, const float* v, float* res) const {
    for (int j = 0; j < 4; ++j) {
        res[j] = 0;
        for (int k = 0; k < 4; ++k) {
            res[j] += v[k] * m[j * 4 + k];
        }
    }
}

void CatmullRom::getCatmullRomPoint(float t, Point3D p0, Point3D p1, Point3D p2, Point3D p3,
                                    float* pos, float* deriv) const {
    std::vector<float> p0Vector = {p0.getX(), p0.getY(), p0.getZ()};
    std::vector<float> p1Vector = {p1.getX(), p1.getY(), p1.getZ()};
    std::vector<float> p2Vector = {p2.getX(), p2.getY(), p2.getZ()};
    std::vector<float> p3Vector = {p3.getX(), p3.getY(), p3.getZ()};

    float m[4][4] = {
        {-0.5f, 1.5f, -1.5f, 0.5f},
        {1.0f, -2.5f, 2.0f, -0.5f},
        {-0.5f, 0.0f, 0.5f, 0.0f},
        {0.0f, 1.0f, 0.0f, 0.0f}};

    for (int i = 0; i < 3; i++) {
        // Using Point3D data for control points
        float v[4] = {p0Vector[i], p1Vector[i], p2Vector[i], p3Vector[i]};
        float A[4];
        multMatrixVector(&m[0][0], v, A);

        pos[i] = A[0] * t * t * t + A[1] * t * t + A[2] * t + A[3];
        deriv[i] = 3 * A[0] * t * t + 2 * A[1] * t + A[2];
    }
}
