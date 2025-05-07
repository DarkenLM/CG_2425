#pragma once
#include <cmath>
#include <vector>

#include "common/geometry/point.hpp"  // Assuming Point3D is defined here.

/**
 * @class CatmullRom
 * @brief Handles Catmull-Rom spline interpolation, rendering, and transformation matrix calculations.
 */
class CatmullRom {
   public:
    /**
     * @brief Constructs the CatmullRom curve from a list of control points.
     * @param controlPoints A vector of 3D control points.
     */
    CatmullRom(const std::vector<Point3D>& controlPoints);

    /**
     * @brief Computes a position and derivative on the global Catmull-Rom curve.
     * @param gt Global t parameter (0 to 1).
     * @param pos Output position on the curve.
     * @param deriv Output derivative at the point on the curve.
     */
    void getGlobalCatmullRomPoint(float gt, float* pos, float* deriv) const;

    /**
     * @brief Renders the Catmull-Rom curve using OpenGL.
     */
    void renderCatmullRomCurve();

    /**
     * @brief Builds a rotation matrix from the X, Y, and Z axis vectors.
     * @param x X-axis vector.
     * @param y Y-axis vector.
     * @param z Z-axis vector.
     * @param m Output 4x4 matrix.
     */
    void buildRotMatrix(float* x, float* y, float* z, float* m);

   private:
    std::vector<Point3D> points;  ///< Control points for the Catmull-Rom curve.
    int POINT_COUNT;              ///< Number of control points.

    /**
     * @brief Multiplies a 4x4 matrix with a 4D vector.
     * @param m 4x4 matrix.
     * @param v Input vector.
     * @param res Output result vector.
     */
    void multMatrixVector(const float* m, const float* v, float* res) const;

    /**
     * @brief Computes a point and derivative on a local Catmull-Rom segment.
     * @param t Local t parameter (0 to 1).
     * @param p0 First control point.
     * @param p1 Second control point.
     * @param p2 Third control point.
     * @param p3 Fourth control point.
     * @param pos Output interpolated position.
     * @param deriv Output derivative.
     */
    void getCatmullRomPoint(float t, Point3D p0, Point3D p1, Point3D p2, Point3D p3,
                            float* pos, float* deriv) const;
};