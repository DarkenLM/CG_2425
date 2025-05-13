#include "engine/scene/Light.hpp"

#include <cstring>

Light::Light()
    : type(LIGHT_DIRECTIONAL),
      dirX(0.0f),
      dirY(0.0f),
      dirZ(0.0f),
      posX(0.0f),
      posY(0.0f),
      posZ(0.0f),
      cutoff(0.0f) {}

Light::Light(LightType type, float dirX, float dirY, float dirZ)
    : type(type),
      dirX(dirX),
      dirY(dirY),
      dirZ(dirZ),
      posX(0.0f),
      posY(0.0f),
      posZ(0.0f),
      cutoff(0.0f) {}

Light::Light(LightType type, float dirX, float dirY, float dirZ,
             float posX, float posY, float posZ, float cutoff)
    : type(type),
      dirX(dirX),
      dirY(dirY),
      dirZ(dirZ),
      posX(posX),
      posY(posY),
      posZ(posZ),
      cutoff(cutoff) {}

// Getters
LightType Light::getType() const { return type; }
float Light::getDirX() const { return dirX; }
float Light::getDirY() const { return dirY; }
float Light::getDirZ() const { return dirZ; }
float Light::getPosX() const { return posX; }
float Light::getPosY() const { return posY; }
float Light::getPosZ() const { return posZ; }
float Light::getCutoff() const { return cutoff; }
float* Light::getAmbient() const {
    return const_cast<float*>(&ambient[0]);
}
float* Light::getDiffuse() const {
    return const_cast<float*>(&diffuse[0]);
}
float* Light::getSpecular() const {
    return const_cast<float*>(&specular[0]);
}
// Setters
void Light::setType(LightType newType) { type = newType; }

void Light::setDirection(float x, float y, float z) {
    dirX = x;
    dirY = y;
    dirZ = z;
}

void Light::setPosition(float x, float y, float z) {
    posX = x;
    posY = y;
    posZ = z;
}

void Light::setDirX(float x) { dirX = x; }
void Light::setDirY(float y) { dirY = y; }
void Light::setDirZ(float z) { dirZ = z; }

void Light::setPosX(float x) { posX = x; }
void Light::setPosY(float y) { posY = y; }
void Light::setPosZ(float z) { posZ = z; }

void Light::setCutoff(float c) { cutoff = c; }

void Light::setAmbient(float amb1, float amb2, float amb3, float amb4) {
    this->ambient[0] = amb1;
    this->ambient[1] = amb2;
    this->ambient[2] = amb3;
    this->ambient[3] = amb4;
};
void Light::setDiffuse(float dif1, float dif2, float dif3, float dif4) {
    this->diffuse[0] = dif1;
    this->diffuse[1] = dif2;
    this->diffuse[2] = dif3;
    this->diffuse[3] = dif4;
};
void Light::setSpecular(float spec1, float spec2, float spec3, float spec4) {
    this->specular[0] = spec1;
    this->specular[1] = spec2;
    this->specular[2] = spec3;
    this->specular[3] = spec4;
};

Light* Light::fromXML(XMLElement* xml) {
    GET_XML_ELEMENT_ATTRIB_OR_FAIL(xml, "type", _type, const char*);

    LightType type;
    if (strcmp(_type, "directional") == 0) {
        type = LIGHT_DIRECTIONAL;
        GET_XML_ELEMENT_ATTRIB_OR_FAIL(xml, "dirx", _dirX, float);
        GET_XML_ELEMENT_ATTRIB_OR_FAIL(xml, "diry", _dirY, float);
        GET_XML_ELEMENT_ATTRIB_OR_FAIL(xml, "dirz", _dirZ, float);
        return new Light(type, _dirX, _dirY, _dirZ);

    } else if (strcmp(_type, "point") == 0) {
        type = LIGHT_POINT;
        GET_XML_ELEMENT_ATTRIB_OR_FAIL(xml, "posx", _posX, float);
        GET_XML_ELEMENT_ATTRIB_OR_FAIL(xml, "posy", _posY, float);
        GET_XML_ELEMENT_ATTRIB_OR_FAIL(xml, "posz", _posZ, float);
        return new Light(type, 0.0f, 0.0f, 0.0f, _posX, _posY, _posZ);
    } else if (strcmp(_type, "spot") == 0) {
        type = LIGHT_SPOT;
        GET_XML_ELEMENT_ATTRIB_OR_FAIL(xml, "dirx", _dirX, float);
        GET_XML_ELEMENT_ATTRIB_OR_FAIL(xml, "diry", _dirY, float);
        GET_XML_ELEMENT_ATTRIB_OR_FAIL(xml, "dirz", _dirZ, float);
        GET_XML_ELEMENT_ATTRIB_OR_FAIL(xml, "posx", _posX, float);
        GET_XML_ELEMENT_ATTRIB_OR_FAIL(xml, "posy", _posY, float);
        GET_XML_ELEMENT_ATTRIB_OR_FAIL(xml, "posz", _posZ, float);
        GET_XML_ELEMENT_ATTRIB_OR_FAIL(xml, "cutoff", _cutoff, float);
        return new Light(type, _dirX, _dirY, _dirZ, _posX, _posY, _posZ, _cutoff);
    } else
        yeet std::string("Unknown light type: ") + _type;
}

void Light::render() {
    GLenum lightID = this->gl_id;
    switch (this->type) {
        case LIGHT_DIRECTIONAL: {
            float dir[4] = {this->getDirX(), this->getDirY(), this->getDirZ(), 0.0f};
            glLightfv(lightID, GL_POSITION, dir);
            break;
        }
        case LIGHT_POINT: {
            float pos[4] = {this->getPosX(), this->getPosY(), this->getPosZ(), 1.0f};
            glLightfv(lightID, GL_POSITION, pos);
            break;
        }
        case LIGHT_SPOT: {
            float spotPos[4] = {this->getPosX(), this->getPosY(), this->getPosZ(), 1.0f};
            float spotDir[3] = {this->getDirX(), this->getDirY(), this->getDirZ()};
            glLightfv(lightID, GL_POSITION, spotPos);
            glLightfv(lightID, GL_SPOT_DIRECTION, spotDir);
            glLightf(lightID, GL_SPOT_CUTOFF, this->getCutoff());
            break;
        }
    }
}
