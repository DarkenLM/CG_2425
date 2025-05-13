#pragma once

#include "common/geometry/BaseGeometry.hpp"

/**
 *  @brief File format version number (1 byte)
 */
constexpr uint8_t PARSER_VERSION = 6;

/**
 *  @class Parser3D
 *  @brief Handles serialization and deserialization of 3D geometry data in binary format.
 *
 * The binary file format includes:
 * - 1 byte version number
 * - 1 byte geometry type
 * - 2 byte triangle count
 * - 3 vertices per triangle
 * - 3 floats per vertex (x, y, z coordinates)
 * - 3 floats per normal vector (x, y, z)
 * - 2 floats per UV coordinate (u, v)
 * - Indices for triangles (unsigned ints)
 */
class Parser3D {
   public:
    /**
     *  @brief Default constructor
     */
    Parser3D();

    /**
     *  @brief Serializes 3D vertices, normals, UVs, and indices to a binary file inside a Models folder.
     *  @param filename Output filename
     *  @param geometry Geometry object to be serialized (must contain vertices, normals, UVs, and indices)
     *  @return 0 on success, 1 on failure
     *  @throws Does not throw but returns error code - fails if:
     *          - File cannot be opened
     *          - Vertex count not divisible by 3
     */
    static int saveToFile(const std::string& filename, BaseGeometry* geometry);

    /**
     *  @brief Deserializes 3D vertices, normals, UVs, and indices from a binary file
     *  @param filename Input filename
     *  @return A geometry object, inheriting from BaseGeometry.
     *  @throws Does not throw but returns error code - fails if:
     *         - File cannot be opened
     *         - Version mismatch
     *         - Corrupted data
     */
    static BaseGeometry* load3DFile(const std::string& filename);

    /**
     *  @brief Reads data from an OBJ file and loads geometry (including UVs, normals, and vertices)
     *  @param filename Input filename
     *  @return A geometry object, inheriting from BaseGeometry.
     *  @throws Does not throw but returns error code - fails if:
     *         - File cannot be opened
     *         - Corrupted data
     */
    static BaseGeometry* loadObjFile(const std::string& filename);
};