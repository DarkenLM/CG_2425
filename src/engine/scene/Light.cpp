#include "engine/scene/Light.hpp"

#include <cstring>

Light::Light()
    : type(LIGHT_DIRECTIONAL), dirX(0.0f), dirY(0.0f), dirZ(0.0f) {}

Light::Light(LightType type, float dirX, float dirY, float dirZ)
    : type(type), dirX(dirX), dirY(dirY), dirZ(dirZ) {}

Light* Light::fromXML(XMLElement* xml) {
    GET_XML_ELEMENT_ATTRIB_OR_FAIL(xml, "type", _type, const char*);
    GET_XML_ELEMENT_ATTRIB_OR_FAIL(xml, "dirx", _dirX, float);
    GET_XML_ELEMENT_ATTRIB_OR_FAIL(xml, "diry", _dirY, float);
    GET_XML_ELEMENT_ATTRIB_OR_FAIL(xml, "dirz", _dirZ, float);

    LightType type;
    if (strcmp(_type, "directional") == 0)
        type = LIGHT_DIRECTIONAL;
    else if (strcmp(_type, "point") == 0)
        type = LIGHT_POINT;
    else if (strcmp(_type, "spot") == 0)
        type = LIGHT_SPOT;
    else
        yeet std::string("Unknown light type: ") + _type;

    return new Light(type, _dirX, _dirY, _dirZ);
}

void Light::render() {
    // TODO: Implement light rendering
}