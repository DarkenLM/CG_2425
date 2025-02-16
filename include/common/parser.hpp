#pragma once
#include <string>
#include <vector>
#include "common/geometry/point.hpp"

#define PARSER_VERSION 1

class Parser3D {
    public:
        Parser3D();

        static void saveToFile(const std::string& filename, uint8_t version, std::vector<Point3D>& triangles);
};