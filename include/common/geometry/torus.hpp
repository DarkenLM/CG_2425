#pragma once

#include "common/geometry/BaseGeometry.hpp"

class TorusGeometry : public BaseGeometry {
   public:
    /**
     * @brief Destructor for TorusGeometry.
     */
    virtual ~TorusGeometry();

    /**
     * @brief Main constructor for TorusGeometry.
     *
     * This constructor initializes a torus with given major and minor radii, number of slices (segments around the major axis),
     * and number of sides (segments around the minor axis). The torus is tessellated based on these parameters.
     *
     * @param majorRadius The major radius of the torus.
     * @param minorRadius The minor radius (tube radius) of the torus.
     * @param slices The number of slices (segments) around the major axis.
     * @param sides The number of sides (segments) around the minor radius (tube).
     */
    TorusGeometry(int majorRadius, int minorRadius, int slices, int sides);

    /**
     * @brief Alternative constructor for TorusGeometry when vertices, normals, and indices are pre-computed.
     *
     * @param vertices A vector containing the vertices of the torus.
     * @param normals A vector containing the normals of the torus.
     * @param indices A vector containing the indices that define the geometry.
     */
    TorusGeometry(std::vector<Point3D> vertices, std::vector<Vector3<float>> normals, std::vector<unsigned int> indices);

    /**
     * @brief Copies the vertices of the torus.
     *
     * @return A vector of Point3D representing the vertices of the torus.
     */
    virtual std::vector<Point3D> copyVertices() override;

    /**
     * @brief Copies the normals of the torus.
     *
     * @return A vector of Vector3<float> representing the normals of the torus.
     */
    virtual std::vector<Vector3<float>> copyNormals() override;

    /**
     * @brief Copies the indices of the torus.
     *
     * @return A vector of unsigned integers representing the indices of the torus.
     */
    virtual std::vector<unsigned int> copyIndices() override;
};