#pragma once

#include "common/geometry/BaseGeometry.hpp"

/**
 * @class SphereGeometry
 * @brief Represents a sphere geometry, inheriting from the BaseGeometry class.
 *
 * This class models a 3D sphere geometry and provides methods to copy its vertices, normals, and indices.
 */
class SphereGeometry : public BaseGeometry {
   public:
    /**
     * @brief Destructor for SphereGeometry.
     */
    virtual ~SphereGeometry();

    /**
     * @brief Main constructor for SphereGeometry.
     *
     * This constructor initializes the sphere with a given radius, number of slices, and stacks.
     * The sphere is constructed by tessellating it based on these parameters.
     *
     * @param radius The radius of the sphere.
     * @param slices The number of slices (longitude divisions) for the sphere.
     * @param stacks The number of stacks (latitude divisions) for the sphere.
     */
    SphereGeometry(float radius, int slices, int stacks);

    /**
     * @brief Alternative constructor for SphereGeometry when vertices, normals, and indices are pre-computed.
     *
     * @param vertices A vector containing the vertices of the sphere.
     * @param normals A vector containing the normals of the sphere.
     * @param indices A vector containing the indices that define the geometry.
     */
    SphereGeometry(std::vector<Point3D> vertices, std::vector<Vector3<float>> normals, std::vector<unsigned int> indices);

    /**
     * @brief Copies the vertices of the sphere.
     *
     * @return A vector of Point3D representing the vertices of the sphere.
     */
    virtual std::vector<Point3D> copyVertices() override;

    /**
     * @brief Copies the normals of the sphere.
     *
     * @return A vector of Vector3<float> representing the normals of the sphere.
     */
    virtual std::vector<Vector3<float>> copyNormals() override;

    /**
     * @brief Copies the indices of the sphere.
     *
     * @return A vector of unsigned integers representing the indices of the sphere.
     */
    virtual std::vector<unsigned int> copyIndices() override;
};