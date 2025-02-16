#pragma once

#include <string>
#include <vector>
#include "point.hpp"

class BaseGeometry {
    protected:
        std::vector<Point3D> vertices;

    public:
        virtual ~BaseGeometry() = default;
        std::vector<Point3D> getVertices() const { return this->vertices; };

        virtual std::vector<Point3D> serialize() = 0;
        static BaseGeometry* deserialize(std::string filePath) { return nullptr; };
};