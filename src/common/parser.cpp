#include "common/parser.hpp"
// #include "common/geometry/GeometryCatalog.hpp"

#include <fstream>
#include <iostream>
#include <sstream>

Parser3D::Parser3D() {
}

int Parser3D::saveToFile(const std::string& filename, BaseGeometry* geometry) {
    std::ofstream file(("Models/" + filename), std::ios::binary);

    if (!file) {
        std::cerr << "Error opening file for writing: " << filename << std::endl;
        return 1;
    }

    file.write(reinterpret_cast<const char*>(&PARSER_VERSION), sizeof(PARSER_VERSION));
    // file.write((const char*)(geometry->getKind()), sizeof(uint8_t));
    file << static_cast<uint8_t>(geometry->getKind());

    std::vector<Point3D> vertices = geometry->serialize();

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
