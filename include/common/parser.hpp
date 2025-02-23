#pragma once
#include <string>
#include <vector>
#include <cstdint>

#include "common/geometry/point.hpp"
#include "common/geometry/BaseGeometry.hpp"

/**
 *  @brief File format version number (1 byte)
 */
constexpr uint8_t PARSER_VERSION = 2;

/**
 *  @class Parser3D
 *  @brief Handles serialization and deserialization of 3D geometry data in binary format.
 *
 * The binary file format includes:
 * - 1 byte version number
 * - 2 byte triangle count
 * - 3 vertices per triangle
 * - 3 floats per vertex (x, y, z coordinates)
 */
class Parser3D {
   public:
    /**
     *  @brief Default constructor
     */
    Parser3D();

    /**
     *  @brief Serializes 3D vertices to a binary file inside a Models folder.
     *  @param filename Output filename
     *  @param vertices Vector of Point3D vertices (must contain 3 vertices per triangle)
     *  @return 0 on success, 1 on failure
     *  @throws Does not throw but returns error code - fails if:
     *          - File cannot be opened
     *          - Vertex count not divisible by 3
     */
    static int saveToFile(const std::string& filename, std::vector<Point3D>& triangles);

    /**
     *  @brief Deserializes 3D vertices from a binary file
     *  @param filename Input filename
     *  @param vertices Output vector to populate with loaded Point3D vertices
     *  @return 0 on success, 1 on failure
     *  @throws Does not throw but returns error code - fails if:
     *         - File cannot be opened
     *         - Version mismatch
     *         - Corrupted data
     */
    static int load3DFile(const std::string& filename, std::vector<Point3D>& triangles);

    /**
     *  @brief Deserializes a Geometry object from a binary file
     *  @param filename Input filename
     *  @return A geometry object, inheriting from BaseGeometry.
     *  @throws Fails if:
     *         - File cannot be opened
     *         - Version mismatch
     *         - Corrupted data
     */
    static BaseGeometry* loadGeometryFromFile(const std::string& filename);
};