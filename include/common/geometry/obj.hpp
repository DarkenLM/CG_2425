#pragma once

#include "common/geometry/BaseGeometry.hpp"

/**
 * @brief Geometry class for importing and handling OBJ file data.
 *
 * This class allows importing geometry from OBJ files, consisting of
 * vertices, normals, and triangle indices. It provides methods for
 * copying this data into a new instance.
 */
class ObjGeometry : public BaseGeometry {
   public:
    /**
     * @brief Destructor.
     */
    virtual ~ObjGeometry();

    /**
     * @brief Constructs an ObjGeometry from precomputed data.
     *
     * @param vertices List of vertex positions loaded from an OBJ file.
     * @param normals List of normal vectors loaded from an OBJ file.
     * @param indices List of triangle indices loaded from an OBJ file.
     */
    ObjGeometry(std::vector<Point3D> vertices, std::vector<Vector3<float>> normals, std::vector<unsigned int> indices);

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