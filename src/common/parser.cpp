#include "common/parser.hpp"

#include <fstream>
#include <iostream>
#include <sstream>

Parser3D::Parser3D() {
}

// /**
//  *  @brief File format version number (1 byte)
//  */
// constexpr uint8_t PARSER_VERSION = 1;

int Parser3D::saveToFile(const std::string& filename, std::vector<Point3D>& vertices) {
    std::ofstream file(("Models/" + filename), std::ios::binary);

    if (!file) {
        std::cerr << "Error opening file for writing: " << filename << std::endl;
        return 1;
    }

    file.write(reinterpret_cast<const char*>(&PARSER_VERSION), sizeof(PARSER_VERSION));

    if (vertices.size() % 3 != 0) {
        std::cerr << "Number of vertices are insuficient: <vertices>%3 != 0";
        return 1;
    }
    uint16_t nTriangles = vertices.size() / 3;

    file.write(reinterpret_cast<const char*>(&nTriangles), sizeof(nTriangles));

    for (const Point3D& p : vertices) {
        float x = p.getX();
        float y = p.getY();
        float z = p.getZ();

        file.write(reinterpret_cast<const char*>(&x), sizeof(float));
        file.write(reinterpret_cast<const char*>(&y), sizeof(float));
        file.write(reinterpret_cast<const char*>(&z), sizeof(float));
    }

    file.close();

    std::cout << "Saved " << nTriangles << " triangles to " << filename << " successfully!\n";
    return 0;
}

int Parser3D::load3DFile(const std::string& filename, std::vector<Point3D>& vertices) {
    std::ifstream file(("Models/" + filename), std::ios::binary);
    if (!file) {
        std::cerr << "Error opening file for reading: " << filename << std::endl;
        return 1;
    }

    uint8_t file_version;
    file.read(reinterpret_cast<char*>(&file_version), sizeof(file_version));
    if (file_version != PARSER_VERSION) {
        std::cerr << "Error: Unsupported file version " << static_cast<int>(file_version)
                  << " (expected) " << static_cast<int>(PARSER_VERSION) << ").\n";
        return 1;
    }

    uint16_t nTriangles;
    file.read(reinterpret_cast<char*>(&nTriangles), sizeof(nTriangles));

    // Resize vertex vector for better performance
    vertices.resize(nTriangles * 3);

    for (Point3D& p : vertices) {
        float x, y, z;
        file.read(reinterpret_cast<char*>(&x), sizeof(x));
        file.read(reinterpret_cast<char*>(&y), sizeof(y));
        file.read(reinterpret_cast<char*>(&z), sizeof(z));
        p.set(x, y, z);
    }

    std::cout << "Loaded " << nTriangles << " triangles from " << filename << ".\n";
    return 0;
}
