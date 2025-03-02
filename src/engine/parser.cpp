#include "common/parser.hpp"
#include "engine/geometry/GeometryCatalog.hpp"

#include <fstream>
#include <iostream>
#include <sstream>

struct _parser3d_load_result {
    uint8_t _guard;
    std::vector<Point3D> vertices;
    BaseGeometryKind kind;
};

struct _parser3d_load_result _parser3DLoadFile(const std::string& filename) {
    std::ifstream file(("Models/" + filename), std::ios::binary);
    if (!file) {
        std::cerr << "Error opening file for reading: " << filename << std::endl;
        return (struct _parser3d_load_result){ 0 };
    }

    uint8_t file_version;
    file.read(reinterpret_cast<char*>(&file_version), sizeof(file_version));
    if (file_version != PARSER_VERSION) {
        std::cerr << "Error: Unsupported file version " << static_cast<int>(file_version)
                  << " (expected) " << static_cast<int>(PARSER_VERSION) << ").\n";
        return (struct _parser3d_load_result){ 0 };
    }

    BaseGeometryKind kind;
    file.read(reinterpret_cast<char*>(&kind), sizeof(kind));
    if (kind > _GEOMETRY_KIND_UPPER_BOUND) {
        std::cerr << "Error: Unsupported geometry " << static_cast<int>(kind) << ".\n";
        return (struct _parser3d_load_result){ 0 };
    }

    uint16_t nTriangles;
    file.read(reinterpret_cast<char*>(&nTriangles), sizeof(nTriangles));

    // Resize vertex vector for better performance
    std::vector<Point3D> vertices;

    // for (Point3D& p : vertices) {
    for (int i = 0; i < nTriangles * 3; i++) {
        float x, y, z;
        file.read(reinterpret_cast<char*>(&x), sizeof(x));
        file.read(reinterpret_cast<char*>(&y), sizeof(y));
        file.read(reinterpret_cast<char*>(&z), sizeof(z));
        // p.set(x, y, z);
        vertices.push_back(Point3D(x, y, z));
    }

    std::cout << "Loaded " << nTriangles << " triangles from " << filename << ".\n";

    return (struct _parser3d_load_result){ 
        ._guard = 1,
        .vertices = vertices,
        .kind = kind
    };
}

BaseGeometry* Parser3D::load3DFile(const std::string& filename) {
    struct _parser3d_load_result res = _parser3DLoadFile(filename);
    if (res._guard == 0) return nullptr;
    
    return createGeometryFromKind(res.kind, res.vertices);
    return nullptr;
}
