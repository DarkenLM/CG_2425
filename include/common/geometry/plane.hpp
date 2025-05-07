#pragma once

#include "common/geometry/BaseGeometry.hpp"

/**
 * @brief Geometry class representing a 3D plane.
 *
 * This class generates a plane mesh with specified dimensions and
 * grid resolution, useful for representing flat surfaces.
 */
class PlaneGeometry : public BaseGeometry {
   public:
    /**
     * @brief Destructor.
     */
    virtual ~PlaneGeometry();

    /**
     * @brief Constructs a plane geometry procedurally.
     *
     * @param length Length of the plane (both width and height).
     * @param gridSize Number of subdivisions along each axis.
     */
    PlaneGeometry(int length, int gridSize);

    /**
     * @brief Constructs a plane geometry from precomputed data.
     *
     * @param vertices List of vertex positions.
     * @param normals List of normal vectors.
     * @param indices List of triangle indices.
     */
    PlaneGeometry(std::vector<Point3D> vertices, std::vector<Vector3<float>> normals, std::vector<unsigned int> indices);

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
};