#pragma once

#include "common/geometry/BaseGeometry.hpp"

/**
 * @brief Geometry class representing a 3D cylinder.
 *
 * This class generates a cylinder mesh given its radius, height,
 * and resolution (number of slices around its circular base).
 */
class CylinderGeometry : public BaseGeometry {
   public:
    /**
     * @brief Destructor.
     */
    virtual ~CylinderGeometry();

    /**
     * @brief Constructs a cylinder geometry procedurally.
     *
     * @param radius Radius of the cylinder base.
     * @param height Height of the cylinder.
     * @param slices Number of radial subdivisions (circular segments).
     */
    CylinderGeometry(int radius, int height, int slices);

    /**
     * @brief Constructs a cylinder geometry from precomputed data.
     *
     * @param vertices List of vertex positions.
     * @param normals List of normal vectors.
     * @param indices List of triangle indices.
     */
    CylinderGeometry(std::vector<Point3D> vertices, std::vector<Vector3<float>> normals, std::vector<unsigned int> indices);

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