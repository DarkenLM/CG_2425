#pragma once

#include <cstdint>
#include <memory>
#include <string>
#include <unordered_map>
#include <vector>

#include "common/util/maputil.hpp"
#include "common/util/vectors.hpp"
#include "point.hpp"

/**
 * @brief Enumerates different kinds of geometric primitives.
 */
typedef enum basegeometry_kind : uint8_t {
    _GEOMETRY_BASE,
    GEOMETRY_CONE,
    GEOMETRY_PLANE,
    GEOMETRY_BOX,
    GEOMETRY_SPHERE,
    GEOMETRY_CYLINDER,
    GEOMETRY_TORUS,
    GEOMETRY_FLATRING,
    GEOMETRY_ICOSPHERE,
    GEOMETRY_BEZIER,
    _GEOMETRY_KIND_UPPER_BOUND,
    GEOMETRY_OBJ
} BaseGeometryKind;

/**
 * @brief Abstract base class for 3D geometric primitives.
 *
 * This class provides the base interface for managing and accessing
 * geometry data such as vertices, normals, and indices.
 */
class BaseGeometry {
   protected:
    /**< Type of geometry */
    BaseGeometryKind _kind = _GEOMETRY_BASE;
    /**< Vertex positions */
    std::vector<Point3D> vertices;
    /**< Normal vectors */
    std::vector<Vector3<float>> normals;
    /**< Triangle indices */
    std::vector<unsigned int> indices;

   public:
    /**
     * @brief Virtual destructor.
     */
    virtual ~BaseGeometry() = default;

    /**
     * @brief Get the geometry's vertices.
     * @return Vector of 3D points representing vertices.
     */
    std::vector<Point3D> getVertices() const { return this->vertices; };

    /**
     * @brief Get the geometry's normals.
     * @return Vector of 3D float vectors representing normals.
     */
    std::vector<Vector3<float>> getNormals() const { return this->normals; };

    /**
     * @brief Get the geometry's triangle indices.
     * @return Vector of unsigned integers representing indices.
     */
    std::vector<unsigned int> getIndices() const { return this->indices; };

    /**
     * @brief Get the type of geometry.
     * @return BaseGeometryKind enum value.
     */
    BaseGeometryKind getKind() const { return this->_kind; };

    /**
     * @brief Create a copy of the vertex data.
     * @return Vector of copied 3D points.
     */
    virtual std::vector<Point3D> copyVertices() = 0;

    /**
     * @brief Create a copy of the normal data.
     * @return Vector of copied normals.
     */
    virtual std::vector<Vector3<float>> copyNormals() = 0;

    /**
     * @brief Create a copy of the index data.
     * @return Vector of copied indices.
     */
    virtual std::vector<unsigned int> copyIndices() = 0;
};