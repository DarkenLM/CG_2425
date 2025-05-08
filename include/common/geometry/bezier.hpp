#pragma once

#include <functional>

#include "common/geometry/BaseGeometry.hpp"

/**
 * @brief Geometry class representing a tessellated Bezier surface.
 *
 * This class extends BaseGeometry to support loading and tessellating
 * Bezier patches, either from file or from precomputed data.
 */
class BezierGeometry : public BaseGeometry {
   public:
    /**
     * @brief Destructor.
     */
    virtual ~BezierGeometry();

    /**
     * @brief Constructs a Bezier geometry from a file and tessellation level.
     *
     * @param filename Path to the file containing Bezier patch data.
     * @param tessellationLevel Level of tessellation granularity.
     */
    BezierGeometry(const char* filename, int tessellationLevel);

    /**
     * @brief Constructs a Bezier geometry from precomputed data.
     *
     * @param vertices List of vertex positions.
     * @param normals List of normal vectors.
     * @param indices List of triangle indices.
     */
    BezierGeometry(std::vector<Point3D> vertices,
                   std::vector<Vector3<float>> normals,
                   std::vector<unsigned int> indices);

    /**
     * @brief Returns a copy of the geometry's vertices.
     * @return Vector of copied 3D points.
     */
    virtual std::vector<Point3D> copyVertices() override;

    /**
     * @brief Returns a copy of the geometry's normals.
     * @return Vector of copied normal vectors.
     */
    virtual std::vector<Vector3<float>> copyNormals() override;

    /**
     * @brief Returns a copy of the geometry's indices.
     * @return Vector of copied indices.
     */
    virtual std::vector<unsigned int> copyIndices() override;

   private:
    /**
     * @brief Tessellates a single Bezier patch.
     *
     * This function generates geometry from a 4x4 grid of control points.
     *
     * @param patchIndices Indices of control points forming the patch.
     * @param controlPoints List of all control points available.
     * @param tessellationLevel Number of subdivisions for tessellation.
     * @param addVertex Function used to add new vertices to the mesh.
     */
    void tessellateBezierPatch(
        const std::array<int, 16>& patchIndices,
        const std::vector<Point3D>& controlPoints,
        int tessellationLevel,
        const std::function<unsigned int(const Point3D&, const Vector3<float>&)>& addVertex);
};