#pragma once

#include "common/geometry/BaseGeometry.hpp"

/**
 * @brief Geometry class representing an icosphere.
 *
 * This class generates a spherical geometry based on recursive
 * subdivision of an icosahedron. It's ideal for evenly distributed
 * vertices on a sphere.
 */
class IcosphereGeometry : public BaseGeometry {
   public:
    /**
     * @brief Destructor.
     */
    virtual ~IcosphereGeometry();

    /**
     * @brief Constructs an icosphere geometry procedurally.
     *
     * @param radius Radius of the sphere.
     * @param subdivisions Number of recursive subdivision steps.
     *        Higher values create smoother spheres.
     */
    IcosphereGeometry(int radius, int subdivisions);

    /**
     * @brief Constructs an icosphere geometry from precomputed data.
     *
     * @param vertices List of vertex positions.
     * @param normals List of normal vectors.
     * @param uvs List of UV coordinates.
     * @param indices List of triangle indices.
     */
    IcosphereGeometry(std::vector<Point3D> vertices,
                      std::vector<Vector3<float>> normals,
                      std::vector<Vector2<float>> uvs,  // Add UVs here
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