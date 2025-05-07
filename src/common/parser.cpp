#include "common/parser.hpp"
// #include "common/geometry/GeometryCatalog.hpp"
#include <fstream>
#include <iostream>
#include <sstream>

#include "common/util/vectors.hpp"
#include "engine/geometry/GeometryCatalog.hpp"

struct _parser3d_load_result {
    uint8_t _guard;
    std::vector<Point3D> vertices;
    std::vector<Vector3<float>> normals;
    std::vector<unsigned int> indices;
    BaseGeometryKind kind;
};

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

struct _parser3d_load_result _parser3DLoadFile(const std::string& filename) {
    std::ifstream file(("Models/" + filename), std::ios::binary);
    if (!file) {
        std::cerr << "Error opening file for reading: " << filename << std::endl;
        return (struct _parser3d_load_result){0};
    }

    uint8_t file_version;
    file.read(reinterpret_cast<char*>(&file_version), sizeof(file_version));
    if (file_version != PARSER_VERSION) {
        std::cerr << "Error: Unsupported file version " << static_cast<int>(file_version)
                  << " (expected) " << static_cast<int>(PARSER_VERSION) << ").\n";
        return (struct _parser3d_load_result){0};
    }

    BaseGeometryKind kind;
    file.read(reinterpret_cast<char*>(&kind), sizeof(kind));
    if (kind > _GEOMETRY_KIND_UPPER_BOUND) {
        std::cerr << "Error: Unsupported geometry " << static_cast<int>(kind) << ".\n";
        return (struct _parser3d_load_result){0};
    }

    uint16_t nVertices;
    file.read(reinterpret_cast<char*>(&nVertices), sizeof(nVertices));
    uint16_t nTriangles;
    file.read(reinterpret_cast<char*>(&nTriangles), sizeof(nTriangles));

    // Resize vertex vector for better performance
    std::vector<Point3D> vertices;
    std::vector<Vector3<float>> normals;
    std::vector<unsigned int> indices;

    // READ VERTICES
    for (int i = 0; i < nVertices; i++) {
        float x, y, z;
        file.read(reinterpret_cast<char*>(&x), sizeof(x));
        file.read(reinterpret_cast<char*>(&y), sizeof(y));
        file.read(reinterpret_cast<char*>(&z), sizeof(z));

        vertices.push_back(Point3D(x, y, z));
    }

    // READ NORMALS
    for (int i = 0; i < nVertices; i++) {
        float x, y, z;
        file.read(reinterpret_cast<char*>(&x), sizeof(x));
        file.read(reinterpret_cast<char*>(&y), sizeof(y));
        file.read(reinterpret_cast<char*>(&z), sizeof(z));

        normals.push_back(Vector3(x, y, z));
    }

    // READ INDICES
    for (int i = 0; i < nTriangles * 3; i++) {
        unsigned int index;
        file.read(reinterpret_cast<char*>(&index), sizeof(index));

        indices.push_back(index);
    }

    std::cout << "---------- PARSER ----------\n";
    std::cout << "Loading from " << filename << " ...\n";
    std::cout << "Loaded " << nVertices << " vertices!" << std::endl;
    std::cout << "Loaded " << nVertices << " normals!" << std::endl;
    std::cout << "Loaded " << nTriangles << " triangles! " << std::endl;
    std::cout << "---------- ------ ----------\n";

    return (struct _parser3d_load_result){
        ._guard = 1,
        .vertices = vertices,
        .normals = normals,
        .indices = indices,
        .kind = kind};
}

// TODO: Acrescentar as alterações do novo generator/loader a esta função
struct _parser3d_load_result _parserObjLoadFile(const std::string& filename) {
    std::ifstream file(("Models/" + filename), std::ios::binary);
    if (!file) {
        std::cerr << "Error opening file for reading: " << filename << std::endl;
        return (struct _parser3d_load_result){0};
    }

    std::vector<Point3D> triangles;
    std::vector<Point3D> vertices;

    std::string line;
    while (getline(file, line)) {
        std::stringstream ss(line);
        std::string type;
        ss >> type;

        if (type == "v") {
            float x, y, z;
            if (!(ss >> x >> y >> z)) {
                std::cerr << "Error loading vertices at line: " << line << std::endl;
                continue;
            }

            vertices.push_back(Point3D(x, y, z));
        } else if (type == "vt") {
            // float u, v;
            // if (!(ss >> u >> v)) {
            // 	std::cerr << "Error loading texture coordinate at line: " << line << std::endl;
            // 	continue;
            // }

            // textCoords.push_back(Point2D(u, v));
        } else if (type == "vn") {
            // float x, y, z;
            // if (!(ss >> x >> y >> z)) {
            // 	std::cerr << "Error loading vertice normal at line: " << line << std::endl;
            // 	continue;
            // }

            // normals.push_back(Point3D(x, y, z));
        } else if (type == "f") {
            std::vector<int> vIndices, vtIndices, vnIndices;
            std::string vertexData;

            while (ss >> vertexData) {
                std::stringstream vertexStream(vertexData);
                std::string vIdx, vtIdx, vnIdx;

                // v/vt/vn
                getline(vertexStream, vIdx, '/');
                getline(vertexStream, vtIdx, '/');
                getline(vertexStream, vnIdx, '/');

                vIndices.push_back(!vIdx.empty() ? std::stoi(vIdx) - 1 : -1);
                vtIndices.push_back(!vtIdx.empty() ? std::stoi(vtIdx) - 1 : -1);
                vnIndices.push_back(!vnIdx.empty() ? std::stoi(vnIdx) - 1 : -1);
            }

            if (vIndices.size() == 3) {
                for (int i = 0; i < 3; i++) {
                    int vertexIndex = vIndices[i];
                    triangles.push_back(Point3D(
                        vertices[vertexIndex].getX(),
                        vertices[vertexIndex].getY(),
                        vertices[vertexIndex].getZ()));
                }
            } else {
                std::cerr << "Error while loading face at line: " << line << std::endl;
            }
        }
    }

    std::cout << "Loaded " << triangles.size() << " triangles from " << filename << ".\n";

    return (struct _parser3d_load_result){
        ._guard = 1,
        .vertices = triangles,
        .kind = GEOMETRY_OBJ};
}

BaseGeometry* Parser3D::load3DFile(const std::string& filename) {
    struct _parser3d_load_result res = _parser3DLoadFile(filename);
    if (res._guard == 0) return nullptr;

    return createGeometryFromKind(res.kind, res.vertices, res.normals, res.indices);
    return nullptr;
}

BaseGeometry* Parser3D::loadObjFile(const std::string& filename) {
    struct _parser3d_load_result res = _parserObjLoadFile(filename);
    if (res._guard == 0) return nullptr;

    return createGeometryFromKind(res.kind, res.vertices, res.normals, res.indices);
    return nullptr;
}