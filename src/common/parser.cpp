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

    std::vector<Point3D> vertices = geometry->getVertices();
    std::vector<Vector3<float>> normals = geometry->getNormals();
    std::vector<unsigned int> indices = geometry->getIndices();

    if (indices.size() % 3 != 0) {
        std::cerr << "Number of indices are probably incorrect: <indices>%3 != 0";
        return 1;
    }

    uint16_t nVertices = vertices.size();
    uint16_t nNormals = normals.size();
    uint16_t nTriangles = indices.size() / 3;

    // WRITE NUMBER OF VERTICES
    file.write(reinterpret_cast<const char*>(&nVertices), sizeof(nVertices));

    // WRITE NUMBER OF TRIANGLES
    file.write(reinterpret_cast<const char*>(&nTriangles), sizeof(nTriangles));

    // WRITE VERTICES
    for (const Point3D& p : vertices) {
        float x = p.getX();
        float y = p.getY();
        float z = p.getZ();

        file.write(reinterpret_cast<const char*>(&x), sizeof(float));
        file.write(reinterpret_cast<const char*>(&y), sizeof(float));
        file.write(reinterpret_cast<const char*>(&z), sizeof(float));
    }

    // WRITE NORMALS
    for (const Vector3<float> n : normals) {
        float x = n.first;
        float y = n.second;
        float z = n.third;

        file.write(reinterpret_cast<const char*>(&x), sizeof(float));
        file.write(reinterpret_cast<const char*>(&y), sizeof(float));
        file.write(reinterpret_cast<const char*>(&z), sizeof(float));
    }

    // WRITE INDICES
    for (const unsigned int& i : indices) {
        file.write(reinterpret_cast<const char*>(&i), sizeof(unsigned int));
    }

    file.close();

    std::cout << "---------- PARSER ----------\n";
    std::cout << "Generated " << nVertices << " vertices!" << std::endl;
    std::cout << "Generated " << nNormals << " normals!" << std::endl;
    std::cout << "Generated " << nTriangles << " triangles! " << std::endl;
    std::cout << "Information saved successfully at: " << filename << "\n";
    std::cout << "---------- ------ ----------\n";
    return 0;
}
