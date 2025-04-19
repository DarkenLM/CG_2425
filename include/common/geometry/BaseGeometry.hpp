#pragma once

#include <cstdint>
#include <memory>
#include <string>
#include <unordered_map>
#include <vector>

#include "common/util/maputil.hpp"
#include "point.hpp"

typedef enum basegeometry_kind : uint8_t {
    _GEOMETRY_BASE,
    GEOMETRY_CONE,
    GEOMETRY_PLANE,
    GEOMETRY_BOX,
    GEOMETRY_SPHERE,
    GEOMETRY_CYLINDER,
    GEOMETRY_TORUS,
    GEOMETRY_FLATRING,
    GEOMETRY_ICOSPHERE,
    _GEOMETRY_KIND_UPPER_BOUND,
    GEOMETRY_OBJ
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
    std::vector<Vector3<float>> normals;
    std::vector<unsigned int> indices;

   public:
    virtual ~BaseGeometry() = default;
    std::vector<Point3D> getVertices() const { return this->vertices; };
    std::vector<Vector3<float>> getNormals() const { return this->normals; };
    std::vector<unsigned int> getIndices() const { return this->indices; };

    BaseGeometryKind getKind() const { return this->_kind; };

    virtual std::vector<Point3D> serielizeVertices() = 0;
    virtual std::vector<Vector3<float>> serielizeNormals() = 0;
    virtual std::vector<unsigned int> serielizeIndices() = 0;

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