#pragma once

#include <unordered_map>

#include "common/geometry/BaseGeometry.hpp"
#include "common/util/maputil.hpp"
#include "common/util/xmlutil.hpp"
#include "common/parser.hpp"

#include "engine/geometry/GeometryCatalog.hpp"
#include "engine/scene/Object.hpp"

using namespace tinyxml2;

class Model: public Object {
    public:
        Model(const char* source);

        #pragma region ------- Overrides -------
        void render() override;
        #pragma endregion ------- Overrides -------

        const char* getTexture();
        Model* setTexture(const char* texture);

        const char* getColor();
        Model* setColor(const char* color);

        static Model* fromXML(XMLElement* xml);
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
