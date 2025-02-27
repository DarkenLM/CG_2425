#pragma once

#include "common/util/xmlutil.hpp"
using namespace tinyxml2;

class Camera {
    public:
        Camera(
            int posX, int posY, int posZ,
            int lookX, int lookY, int lookZ,
            int upX, int upY, int upZ,
            int fov, int near, int far
        ) {
            this->posX = posX;
            this->posY = posY;
            this->posZ = posZ,

            this->lookX = lookX;
            this->lookY = lookY;
            this->lookZ = lookZ,

            this->upX = upX;
            this->upY = upY;
            this->upZ = upZ;

            this->fov = fov;
            this->near = near;
            this->far = far;
        };

        static Camera* fromXML(XMLElement* xml) {
            GET_XML_ELEMENT_OR_FAIL(xml, "position", position);
            GET_XML_ELEMENT_ATTRIB_OR_FAIL(position, "x", posX, int);
            GET_XML_ELEMENT_ATTRIB_OR_FAIL(position, "y", posY, int);
            GET_XML_ELEMENT_ATTRIB_OR_FAIL(position, "z", posZ, int);

            GET_XML_ELEMENT_OR_FAIL(xml, "lookAt", lookAt);
            GET_XML_ELEMENT_ATTRIB_OR_FAIL(lookAt, "x", lookX, int);
            GET_XML_ELEMENT_ATTRIB_OR_FAIL(lookAt, "y", lookY, int);
            GET_XML_ELEMENT_ATTRIB_OR_FAIL(lookAt, "z", lookZ, int);

            GET_XML_ELEMENT_OR_FAIL(xml, "up", up);
            GET_XML_ELEMENT_ATTRIB_OR_FAIL(up, "x", upX, int);
            GET_XML_ELEMENT_ATTRIB_OR_FAIL(up, "y", upY, int);
            GET_XML_ELEMENT_ATTRIB_OR_FAIL(up, "z", upZ, int);


            GET_XML_ELEMENT_OR_FAIL(xml, "projection", projection);
            GET_XML_ELEMENT_ATTRIB_OR_FAIL(projection, "fov",  fov, int);
            GET_XML_ELEMENT_ATTRIB_OR_FAIL(projection, "near", near, int);
            GET_XML_ELEMENT_ATTRIB_OR_FAIL(projection, "far",  far, int);

            return new Camera(
                posX, posY, posZ,
                lookX, lookY, lookZ,
                upX, upY, upZ,
                fov, near, far
            );
        }

    private:
        int posX; int posY; int posZ;
        int lookX; int lookY; int lookZ;
        int upX; int upY; int upZ;
        int fov; int near; int far;
};