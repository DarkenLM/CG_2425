#include <stdio.h>

#include <cstdlib>
#include <iostream>
#include <string>
#include <vector>

#include "common/common.hpp"
#include "common/geometry/cone.hpp"
#include "common/geometry/cylinder.hpp"
#include "common/geometry/flatRing.hpp"
#include "common/geometry/plane.hpp"
#include "common/geometry/torus.hpp"
#include "common/parser.hpp"

int main(int argc, char* argv[]) {
    commonTest();
    printf("Hello from GENERATOR.\n");

#ifdef DEBUG_MODE
    printf("GENERATOR DEBUG MODE.\n");
    printf("ARGC: %d | ARGV: ", argc);
    for (int i = 0; i < argc; i++) printf("%s ", argv[i]);
    printf("\n");
#endif

    if (argc < 2) {
        std::cerr << "Usage: " << argv[0] << " <figure>\n";
        return 1;
    }

    std::string figure = argv[1];

    if (figure == "cone") {
        if (argc < 7) {
            std::cerr << "Usage: " << argv[0] << " " << argv[1] << " <radius> <height> <slices> <stacks> <output_file>\n";
            return 1;
        }

        int radius = std::atoi(argv[2]);
        int height = std::atoi(argv[3]);
        int slices = std::atoi(argv[4]);
        int stacks = std::atoi(argv[5]);

        // coneGenerator(radius, height, slices, stacks);
        ConeGeometry cone(radius, height, slices, stacks);
        std::vector<Point3D> vertices = cone.getVertices();
        Parser3D::saveToFile(argv[6], vertices);

        std::cout << "Cone information stored at Models/" << argv[6] << std::endl;
        return 0;
    } else if (figure == "box") {
        if (argc < 5) {
            std::cerr << "Usage: " << argv[0] << " " << argv[1] << " <length> <grid_size> <output_file>";
            return 1;
        }

        int length = std::atoi(argv[2]);
        int grid_size = std::atoi(argv[3]);

        std::cout << "Box information stored at Models/" << argv[4] << std::endl;
        return 0;
    } else if (figure == "plane") {
        if (argc < 5) {
            std::cerr << "Usage: " << argv[0] << " " << argv[1] << " <length> <grid_size> <output_file>";
            return 1;
        }

        int length = std::atoi(argv[2]);
        int grid_size = std::atoi(argv[3]);

        PlaneGeometry plane(length, grid_size);
        std::vector<Point3D> vertices = plane.getVertices();
        Parser3D::saveToFile(argv[4], vertices);

        std::cout << "Plane information stored at Models/" << argv[4] << std::endl;
        return 0;
    } else if (figure == "sphere") {
        if (argc < 6) {
            std::cerr << "Usage: " << argv[0] << " " << argv[1] << " <radius> <slices> <stacks> <output_file>";
            return 1;
        }

        int radius = std::atoi(argv[2]);
        int slices = std::atoi(argv[3]);
        int stacks = std::atoi(argv[4]);

        std::cout << "Sphere information stored at Models/" << argv[5] << std::endl;
        return 0;
    } else if (figure == "cylinder") {  // A PARTIR DAQUI TEMOS PRIMITVAS EXTRAS
        if (argc < 6) {
            std::cerr << "Usage: " << argv[0] << " " << argv[1] << " <radius> <height> <slices> <output_file>";
            return 1;
        }

        int radius = std::atoi(argv[2]);
        int height = std::atoi(argv[3]);
        int slices = std::atoi(argv[4]);

        CylinderGeometry cylinder(radius, height, slices);
        std::vector<Point3D> vertices = cylinder.getVertices();
        Parser3D::saveToFile(argv[5], vertices);

        std::cout << "Cylinder information stored at Models/" << argv[5] << std::endl;
        return 0;
    } else if (figure == "torus") {
        if (argc < 7) {
            std::cerr << "Usage: " << argv[0] << " " << argv[1] << " <majorRadius> <minorRadius> <slices> <sides> <output_file>";
            return 1;
        }

        int majorRadius = std::atoi(argv[2]);
        int minorRadius = std::atoi(argv[3]);
        int slices = std::atoi(argv[4]);
        int sides = std::atoi(argv[5]);

        TorusGeometry torus(majorRadius, minorRadius, slices, sides);
        std::vector<Point3D> vertices = torus.getVertices();
        Parser3D::saveToFile(argv[6], vertices);

        std::cout << "Torus information stored at Models/" << argv[6] << std::endl;
        return 0;
    } else if (figure == "flatRing") {
        if (argc < 6) {
            std::cerr << "Usage: " << argv[0] << " " << argv[1] << " <majorRadius> <minorRadius> <slices> <output_file>";
            return 1;
        }

        int majorRadius = std::atoi(argv[2]);
        int minorRadius = std::atoi(argv[3]);
        int slices = std::atoi(argv[4]);

        FlatRingGeometry torus(majorRadius, minorRadius, slices);
        std::vector<Point3D> vertices = torus.getVertices();
        Parser3D::saveToFile(argv[5], vertices);

        std::cout << "FlatRing information stored at Models/" << argv[5] << std::endl;
        return 0;
    } else {
        std::cerr << "Error: Primitive not supported.\n";
        return 1;
    }
}