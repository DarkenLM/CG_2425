#pragma once

#include <unordered_map>

#include "common/geometry/BaseGeometry.hpp"
#include "common/util/maputil.hpp"
#include "common/util/xmlutil.hpp"
#include "common/parser.hpp"
#include "engine/geometry/GeometryCatalog.hpp"

using namespace tinyxml2;

// class Model {
//     public:
//         Model(const char* source) {
//             this->source = source;
//             this->texture = "";
//             this->color = "";
//             this->geometry = nullptr;
//             this->_loaded = false;
//         }

//         static Model* fromXML(XMLElement* xml) {
//             GET_XML_ELEMENT_ATTRIB_OR_FAIL(xml, "file", file, const char*);
//             GET_XML_ELEMENT_ATTRIB(xml, "texture", texture, const char*);
//             GET_XML_ELEMENT_ATTRIB(xml, "color", color, const char*);

//             Model* obj = new Model(file);
//             if (texture) obj->setTexture(texture);
//             if (color) obj->setTexture(color);

//             return obj;
//         }

//         const char* getTexture() {
//             return this->texture.c_str();
//         }

//         Model* setTexture(const char* texture) {
//             this->texture = texture;
//             return this;
//         }

//         const char* getColor() {
//             return this->color.c_str();
//         }

//         Model* setColor(const char* color) {
//             this->color = color;
//             return this;
//         }

//         void load() {
//             this->geometry = this->getOrLoadModel(this->source);
//             this->_loaded = true;
//         }

//     private:
//         std::string source;
//         std::string texture;
//         std::string color;
//         BaseGeometry* geometry;
//         bool _loaded;

//         static Map<BaseGeometry*, std::string> _geometryCache;
//         static BaseGeometry* getOrLoadModel(std::string modelName) {
//             auto model = _geometryCache.get(modelName);
//             if (model.has_value()) return model.value();

//             // BaseGeometry* geometry = new BaseGeometry();
//             BaseGeometry* geometry = Parser3D::load3DFile(modelName);
//             _geometryCache.add(modelName, geometry);

//             return geometry;
//         }
// };

class Model {
    public:
        Model(const char* source);

        static Model* fromXML(XMLElement* xml);

        const char* getTexture();
        Model* setTexture(const char* texture);

        const char* getColor();
        Model* setColor(const char* color);

        void load();

    private:
        std::string source;
        std::string texture;
        std::string color;
        BaseGeometry* geometry;
        bool _loaded;

        static Map<BaseGeometry*, std::string> _geometryCache;
        static BaseGeometry* getOrLoadModel(std::string modelName);
};
