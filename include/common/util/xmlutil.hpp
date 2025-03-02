#pragma once

#include "common/debug.hpp"
#include "lib/tinyxml2.h"

#define GET_XML_DOC_ELEMENT_OR_FAIL(source, target, elem) \
    XMLElement* elem; \
    if ((elem = source.FirstChildElement(target)) == NULL) { \
        ERROR << "Could not find element '" target "' at root." << std::endl; \
        return nullptr; \
    }

#define GET_XML_ELEMENT_OR_FAIL(source, target, elem) \
    XMLElement* elem; \
    if ((elem = source->FirstChildElement(target)) == NULL) { \
        ERROR << "Could not find element '" target "' at root." << std::endl; \
        return nullptr; \
    }

#define GET_XML_ELEMENT(source, target, elem) XMLElement* elem = source->FirstChildElement(target)

bool QueryAttribute(tinyxml2::XMLElement* element, const char* name, const char*& out);
bool QueryAttribute(tinyxml2::XMLElement* element, const char* name, int& out);
bool QueryAttribute(tinyxml2::XMLElement* element, const char* name, unsigned int& out);
bool QueryAttribute(tinyxml2::XMLElement* element, const char* name, int64_t& out);
bool QueryAttribute(tinyxml2::XMLElement* element, const char* name, uint64_t& out);
bool QueryAttribute(tinyxml2::XMLElement* element, const char* name, bool& out);
bool QueryAttribute(tinyxml2::XMLElement* element, const char* name, double& out);
bool QueryAttribute(tinyxml2::XMLElement* element, const char* name, float& out);

#define GET_XML_ELEMENT_ATTRIB_OR_FAIL(source, target, var, type) \
    type var; \
    if (!QueryAttribute(source, target, var)) { \
    /* if (!source->QueryAttribute(target, var)) { */ \
        ERROR << "Could not find element '" target "' at " #source "." << std::endl; \
        return nullptr; \
    }

#define GET_XML_ELEMENT_ATTRIB(source, target, var, type) \
    type var = (type)NULL; \
    QueryAttribute(source, target, var);
