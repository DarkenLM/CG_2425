#pragma once

#include "common/debug.hpp"
#include "lib/tinyxml2.h"

/**
 * @brief Macro for safely getting the first child element of a source XML element.
 *
 * This macro looks for the specified child element by name. If it's not found, an error is logged and `nullptr` is returned.
 *
 * @param source The source XML element.
 * @param target The name of the target child element.
 * @param elem The variable to hold the element pointer.
 */
#define GET_XML_DOC_ELEMENT_OR_FAIL(source, target, elem)                     \
    XMLElement* elem;                                                         \
    if ((elem = source.FirstChildElement(target)) == NULL) {                  \
        ERROR << "Could not find element '" target "' at root." << std::endl; \
        return nullptr;                                                       \
    }

/**
 * @brief Macro for safely getting the first child element of a source XML element.
 *
 * Similar to the previous macro but operates on a pointer to an `XMLElement`.
 *
 * @param source The source XML element pointer.
 * @param target The name of the target child element.
 * @param elem The variable to hold the element pointer.
 */
#define GET_XML_ELEMENT_OR_FAIL(source, target, elem)                         \
    XMLElement* elem;                                                         \
    if ((elem = source->FirstChildElement(target)) == NULL) {                 \
        ERROR << "Could not find element '" target "' at root." << std::endl; \
        return nullptr;                                                       \
    }

/**
 * @brief Macro to get the first child element of a source XML element without error handling.
 *
 * This macro retrieves the first child element with the specified name. It does not perform any error checking.
 *
 * @param source The source XML element pointer.
 * @param target The name of the target child element.
 * @param elem The variable to hold the element pointer.
 */
#define GET_XML_ELEMENT(source, target, elem) \
    XMLElement* elem = source->FirstChildElement(target)

/**
 * @brief Macro for safely getting an attribute from an XML element.
 *
 * This macro retrieves the specified attribute from the given XML element. If the attribute is not found, an error is logged.
 *
 * @param source The source XML element.
 * @param target The name of the attribute.
 * @param var The variable to store the attribute value.
 * @param type The type of the attribute value.
 */
#define GET_XML_ELEMENT_ATTRIB_OR_FAIL(source, target, var, type)                    \
    type var;                                                                        \
    if (!QueryAttribute(source, target, var)) {                                      \
        /* if (!source->QueryAttribute(target, var)) { */                            \
        ERROR << "Could not find element '" target "' at " #source "." << std::endl; \
        return nullptr;                                                              \
    }

/**
 * @brief Macro for getting an attribute from an XML element without error handling.
 *
 * This macro retrieves the specified attribute from the XML element without performing error checking.
 *
 * @param source The source XML element.
 * @param target The name of the attribute.
 * @param var The variable to store the attribute value.
 * @param type The type of the attribute value.
 */
#define GET_XML_ELEMENT_ATTRIB(source, target, var, type) \
    type var = (type)NULL;                                \
    QueryAttribute(source, target, var);

/**
 * @brief Queries an attribute from an XML element and retrieves the value as a string.
 *
 * @param element The XML element to query.
 * @param name The name of the attribute.
 * @param out The variable to store the attribute value.
 * @return `true` if the attribute was found and successfully retrieved, `false` otherwise.
 */
bool QueryAttribute(tinyxml2::XMLElement* element, const char* name, const char*& out);

/**
 * @brief Queries an attribute from an XML element and retrieves the value as an integer.
 *
 * @param element The XML element to query.
 * @param name The name of the attribute.
 * @param out The variable to store the attribute value.
 * @return `true` if the attribute was found and successfully retrieved, `false` otherwise.
 */
bool QueryAttribute(tinyxml2::XMLElement* element, const char* name, int& out);

/**
 * @brief Queries an attribute from an XML element and retrieves the value as an unsigned integer.
 *
 * @param element The XML element to query.
 * @param name The name of the attribute.
 * @param out The variable to store the attribute value.
 * @return `true` if the attribute was found and successfully retrieved, `false` otherwise.
 */
bool QueryAttribute(tinyxml2::XMLElement* element, const char* name, unsigned int& out);

/**
 * @brief Queries an attribute from an XML element and retrieves the value as a 64-bit integer.
 *
 * @param element The XML element to query.
 * @param name The name of the attribute.
 * @param out The variable to store the attribute value.
 * @return `true` if the attribute was found and successfully retrieved, `false` otherwise.
 */
bool QueryAttribute(tinyxml2::XMLElement* element, const char* name, int64_t& out);

/**
 * @brief Queries an attribute from an XML element and retrieves the value as an unsigned 64-bit integer.
 *
 * @param element The XML element to query.
 * @param name The name of the attribute.
 * @param out The variable to store the attribute value.
 * @return `true` if the attribute was found and successfully retrieved, `false` otherwise.
 */
bool QueryAttribute(tinyxml2::XMLElement* element, const char* name, uint64_t& out);

/**
 * @brief Queries an attribute from an XML element and retrieves the value as a boolean.
 *
 * @param element The XML element to query.
 * @param name The name of the attribute.
 * @param out The variable to store the attribute value.
 * @return `true` if the attribute was found and successfully retrieved, `false` otherwise.
 */
bool QueryAttribute(tinyxml2::XMLElement* element, const char* name, bool& out);

/**
 * @brief Queries an attribute from an XML element and retrieves the value as a double.
 *
 * @param element The XML element to query.
 * @param name The name of the attribute.
 * @param out The variable to store the attribute value.
 * @return `true` if the attribute was found and successfully retrieved, `false` otherwise.
 */
bool QueryAttribute(tinyxml2::XMLElement* element, const char* name, double& out);

/**
 * @brief Queries an attribute from an XML element and retrieves the value as a float.
 *
 * @param element The XML element to query.
 * @param name The name of the attribute.
 * @param out The variable to store the attribute value.
 * @return `true` if the attribute was found and successfully retrieved, `false` otherwise.
 */
bool QueryAttribute(tinyxml2::XMLElement* element, const char* name, float& out);
