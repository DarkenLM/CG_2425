#pragma once

#include <limits>

#include "common/util/xmlutil.hpp"
#include "engine/scene/Object.hpp"
using namespace tinyxml2;

typedef enum light_type {
    LIGHT_DIRECTIONAL,
    LIGHT_POINT,
    LIGHT_SPOT
} LightType;

class Light : public Object {
    public:
        LightType type;
        float dirX, dirY, dirZ;

        Light(): type(LIGHT_DIRECTIONAL), dirX(0), dirY(0), dirZ(0) {};
        Light(LightType type, float dirX, float dirY, float dirZ): 
            type(type), dirX(dirX), dirY(dirY), dirZ(dirZ) {};

        static Light* fromXML(XMLElement* xml) {
            GET_XML_ELEMENT_ATTRIB_OR_FAIL(xml, "type", _type, const char*);
            GET_XML_ELEMENT_ATTRIB_OR_FAIL(xml, "dirx", _dirX, float);
            GET_XML_ELEMENT_ATTRIB_OR_FAIL(xml, "diry", _dirY, float);
            GET_XML_ELEMENT_ATTRIB_OR_FAIL(xml, "dirz", _dirZ, float);

            LightType type;
            if (strcmp(_type, "directional") == 0) type = LIGHT_DIRECTIONAL;
            else if (strcmp(_type, "point") == 0) type = LIGHT_POINT;
            else if (strcmp(_type, "spot") == 0) type = LIGHT_SPOT; 
            else yeet std::string("Unknown light type: ") + _type;

            return new Light(type, _dirX, _dirY, _dirZ);
        }

        #pragma region------- Overrides -------
            void render() override {
                // TODO: Implement light
            }
        #pragma endregion------- Overrides -------
};