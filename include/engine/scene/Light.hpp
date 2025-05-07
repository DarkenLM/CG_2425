#pragma once

#include <string>

#include "common/util/xmlutil.hpp"
#include "engine/scene/Object.hpp"

using namespace tinyxml2;

/**
 * @brief Enumeration for types of light sources.
 */
typedef enum light_type {
    LIGHT_DIRECTIONAL,  ///< Directional light
    LIGHT_POINT,        ///< Point light
    LIGHT_SPOT          ///< Spot light
} LightType;

/**
 * @brief Represents a light source in the scene.
 */
class Light : public Object {
   public:
    /**
     * @brief Constructs a default directional light.
     */
    Light();

    /**
     * @brief Constructs a light with specified type and direction.
     * @param type The type of light.
     * @param dirX X-component of the direction.
     * @param dirY Y-component of the direction.
     * @param dirZ Z-component of the direction.
     */
    Light(LightType type, float dirX, float dirY, float dirZ);

    /**
     * @brief Parses a Light object from an XML element.
     * @param xml Pointer to the XML element.
     * @return A new Light object, or nullptr on failure.
     */
    static Light* fromXML(XMLElement* xml);

    /**
     * @brief Renders the light (to be implemented).
     */
    void render() override;

   private:
    LightType type;
    float dirX, dirY, dirZ;
};