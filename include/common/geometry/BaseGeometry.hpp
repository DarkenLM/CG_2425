#pragma once

#include <string>
#include <vector>
#include <cstdint>
#include <memory>
#include <unordered_map>

#include "point.hpp"
#include "common/util/maputil.hpp"

typedef enum basegeometry_kind: uint8_t {
    _GEOMETRY_BASE,
    GEOMETRY_CONE,
    GEOMETRY_PLANE,
    GEOMETRY_BOX,
    GEOMETRY_SPHERE,
    GEOMETRY_CYLINDER,
    GEOMETRY_TORUS,
    GEOMETRY_FLATRING,
    _GEOMETRY_KIND_UPPER_BOUND
} BaseGeometryKind;


// #define REGISTER_GEOMETRY(ClassName, KindValue) \
//     static struct ClassName##Register { \
//         ClassName##Register() { \
//             registry[KindValue] = []() -> std::unique_ptr<BaseGeometry> { return std::make_unique<ClassName>(); }; \
//         } \
//     } ClassName##Instance;

// #define REGISTER_GEOMETRY(className, kind) \
//     /*static_block {*/ \
//         BaseGeometry::registerType(kind, [](std::vector<Point3D> vertices) -> std::unique_ptr<BaseGeometry> { \
//             return std::make_unique<className>(vertices); \
//         }); \
//     /*}*/

class BaseGeometry {
   protected:
    BaseGeometryKind _kind = _GEOMETRY_BASE;
    std::vector<Point3D> vertices;

    public:
        virtual ~BaseGeometry() = default;
        std::vector<Point3D> getVertices() const { return this->vertices; };

        BaseGeometryKind getKind() const { return this->_kind; };

        virtual std::vector<Point3D> serialize() = 0;
        static BaseGeometry* deserialize(std::string filePath) { return nullptr; };


    //     static void registerType(BaseGeometryKind kind, std::unique_ptr<BaseGeometry> (*creator)(std::vector<Point3D>)) {
    //         registry[kind] = creator;
    //     }

    //     static std::unique_ptr<BaseGeometry> create(BaseGeometryKind kind, std::vector<Point3D> vertices) {
    //         if (registry.find(kind) != registry.end()) {
    //             return registry[kind](vertices);
    //         }

    //         return nullptr;
    //     }

    // private:
    //     static std::unordered_map<BaseGeometryKind, std::unique_ptr<BaseGeometry> (*)(std::vector<Point3D>)> registry;
};