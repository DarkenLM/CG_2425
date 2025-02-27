#include "common/util/xmlutil.hpp"

bool QueryAttribute(tinyxml2::XMLElement* element, const char* name, const char*& out) {
    return element->QueryStringAttribute(name, (&out)) == tinyxml2::XML_SUCCESS;
}

bool QueryAttribute(tinyxml2::XMLElement* element, const char* name, int& out) {
    return element->QueryIntAttribute(name, &out) == tinyxml2::XML_SUCCESS;
}

bool QueryAttribute(tinyxml2::XMLElement* element, const char* name, unsigned int& out) {
    return element->QueryUnsignedAttribute(name, &out) == tinyxml2::XML_SUCCESS;
}

bool QueryAttribute(tinyxml2::XMLElement* element, const char* name, int64_t& out) {
    return element->QueryInt64Attribute(name, &out) == tinyxml2::XML_SUCCESS;
}

bool QueryAttribute(tinyxml2::XMLElement* element, const char* name, uint64_t& out) {
    return element->QueryUnsigned64Attribute(name, &out) == tinyxml2::XML_SUCCESS;
}

bool QueryAttribute(tinyxml2::XMLElement* element, const char* name, bool& out) {
    return element->QueryBoolAttribute(name, &out) == tinyxml2::XML_SUCCESS;
}

bool QueryAttribute(tinyxml2::XMLElement* element, const char* name, double& out) {
    return element->QueryDoubleAttribute(name, &out) == tinyxml2::XML_SUCCESS;
}

bool QueryAttribute(tinyxml2::XMLElement* element, const char* name, float& out) {
    return element->QueryFloatAttribute(name, &out) == tinyxml2::XML_SUCCESS;
}