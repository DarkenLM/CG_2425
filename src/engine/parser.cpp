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

struct _parser3d_load_result _parserObjLoadFile(const std::string& filename) {
    std::ifstream file(("Models/" + filename), std::ios::binary);
    if (!file) {
        std::cerr << "Error opening file for reading: " << filename << std::endl;
        return (struct _parser3d_load_result){ 0 };
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
				for (int i = 0 ; i < 3 ; i++) {
					int vertexIndex = vIndices[i];  
					triangles.push_back(Point3D(  
						vertices[vertexIndex].getX(),  
						vertices[vertexIndex].getY(),  
						vertices[vertexIndex].getZ()  
					));
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
        .kind = GEOMETRY_OBJ
    };
}

BaseGeometry* Parser3D::load3DFile(const std::string& filename) {
    struct _parser3d_load_result res = _parser3DLoadFile(filename);
    if (res._guard == 0) return nullptr;
    
    return createGeometryFromKind(res.kind, res.vertices);
    return nullptr;
}

BaseGeometry* Parser3D::loadObjFile(const std::string& filename) {
    struct _parser3d_load_result res = _parserObjLoadFile(filename);
    if (res._guard == 0) return nullptr;
    
    return createGeometryFromKind(res.kind, res.vertices);
    return nullptr; 
}