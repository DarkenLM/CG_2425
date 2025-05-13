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
    GLenum gl_id;
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
    Light(LightType type, float dirX, float dirY, float dirZ,
          float posX, float posY, float posZ, float cutoff = 0.0f);

    // Getters
    // TODO: doxygen
    LightType getType() const;
    float getDirX() const;
    float getDirY() const;
    float getDirZ() const;
    float getPosX() const;
    float getPosY() const;
    float getPosZ() const;
    float getCutoff() const;
    float* getAmbient() const;
    float* getDiffuse() const;
    float* getSpecular() const;

    // Setters
    void setType(LightType type);
    void setDirection(float x, float y, float z);
    void setPosition(float x, float y, float z);
    void setDirX(float x);
    void setDirY(float y);
    void setDirZ(float z);
    void setPosX(float x);
    void setPosY(float y);
    void setPosZ(float z);
    void setCutoff(float cutoff);
    void setAmbient(float amb1, float amb2, float amb3, float amb4);
    void setDiffuse(float dif1, float dif2, float dif3, float dif4);
    void setSpecular(float spec1, float spec2, float spec3, float spec4);

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
    float posX, posY, posZ;
    float cutoff;
    float ambient[4];
    float diffuse[4];
    float specular[4];
};