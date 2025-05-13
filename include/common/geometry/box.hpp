#pragma once

#include "common/geometry/BaseGeometry.hpp"

/**
 * @brief Geometry class representing a 3D box.
 *
 * This class provides functionality to generate a box shape with configurable
 * dimensions and tessellation granularity.
 */
class BoxGeometry : public BaseGeometry {
   public:
    /**
     * @brief Destructor.
     */
    virtual ~BoxGeometry();

    /**
     * @brief Constructs a box with the given size and grid resolution.
     *
     * @param length Length of the box along each axis.
     * @param grid_size Number of subdivisions along each face.
     */
    BoxGeometry(int length, int grid_size);

    /**
     * @brief Constructs a box geometry from precomputed data.
     *
     * @param vertices List of vertex positions.
     * @param normals List of normal vectors.
     * @param uvs List of UV coordinates.
     * @param indices List of triangle indices.
     */
    BoxGeometry(std::vector<Point3D> vertices,
                std::vector<Vector3<float>> normals,
                std::vector<Vector2<float>> uvs,  // Added UV support here
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
     * @brief Returns a copy of the geometry's UVs.
     * @return Vector of copied UV coordinates.
     */
    virtual std::vector<Vector2<float>> copyUVs() override;

    /**
     * @brief Returns a copy of the geometry's indices.
     * @return Vector of copied indices.
     */
    virtual std::vector<unsigned int> copyIndices() override;
};