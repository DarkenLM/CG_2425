#pragma once

#include "common/geometry/BaseGeometry.hpp"

/**
 * @brief Geometry class representing a 3D cone.
 *
 * This class provides functionality to generate a cone mesh using
 * configurable parameters such as radius, height, and resolution.
 */
class ConeGeometry : public BaseGeometry {
   public:
    /**
     * @brief Destructor.
     */
    virtual ~ConeGeometry();

    /**
     * @brief Constructs a cone geometry procedurally.
     *
     * @param radius Radius of the cone base.
     * @param height Height of the cone.
     * @param slices Number of radial subdivisions (like pie slices).
     * @param stacks Number of vertical subdivisions along the height.
     */
    ConeGeometry(int radius, int height, int slices, int stacks);

    /**
     * @brief Constructs a cone geometry from precomputed data.
     *
     * @param vertices List of vertex positions.
     * @param normals List of normal vectors.
     * @param uvs List of UV coordinates.
     * @param indices List of triangle indices.
     */
    ConeGeometry(std::vector<Point3D> vertices,
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
    virtual std::vector<Vector2<float>> copyUVs() override;  // Method to copy UVs

    /**
     * @brief Returns a copy of the geometry's indices.
     * @return Vector of copied indices.
     */
    virtual std::vector<unsigned int> copyIndices() override;
};