#pragma once

#include "common/geometry/BaseGeometry.hpp"

/**
 * @brief Geometry class representing a flat ring or annulus.
 *
 * This class generates a flat ring (disk with a hole) geometry based
 * on the given radii and number of radial subdivisions.
 */
class FlatRingGeometry : public BaseGeometry {
   public:
    /**
     * @brief Destructor.
     */
    virtual ~FlatRingGeometry();

    /**
     * @brief Constructs a flat ring geometry procedurally.
     *
     * @param majorRadius Outer radius of the ring.
     * @param minorRadius Inner radius (hole) of the ring.
     * @param slices Number of radial subdivisions around the ring.
     */
    FlatRingGeometry(int majorRadius, int minorRadius, int slices);

    /**
     * @brief Constructs a flat ring geometry from precomputed data.
     *
     * @param vertices List of vertex positions.
     * @param normals List of normal vectors.
     * @param indices List of triangle indices.
     */
    FlatRingGeometry(std::vector<Point3D> vertices, std::vector<Vector3<float>> normals, std::vector<unsigned int> indices);

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