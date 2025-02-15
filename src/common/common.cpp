#include "common/common.h"
#include <stdio.h>
#include <iostream>
#include <vector>
#include <fstream>
#include "include/generator/point.hpp"

void saveToFile(const std::string& filename, uint8_t version, std::vector<Point3D>& triangles){
    std::ofstream file(filename, std::ios::binary);

    if(!file){
        std::cerr << "Error opening file for writing: " << filename << std::endl;
        return;
    }

    file.write(reinterpret_cast<const char*>(&version), sizeof(version));

    uint16_t nTriangles = triangles.size() / 3;

    file.write(reinterpret_cast<const char*>(&nTriangles), sizeof(nTriangles));

    for (const Point3D& p : triangles) {
        float x = p.getX();
        float y = p.getY();
        float z = p.getZ();

        file.write(reinterpret_cast<const char*>(&x), sizeof(float));
        file.write(reinterpret_cast<const char*>(&y), sizeof(float));
        file.write(reinterpret_cast<const char*>(&z), sizeof(float));
    }

    file.close();
    std::cout << "Saved " << nTriangles << " triangles to " << filename << " successfully!\n";
}

void commonTest() {
    printf("Hello from COMMON.\n");
}