#pragma once

#include "common/util/xmlutil.hpp"
using namespace tinyxml2;

class Window {
    public:
        Window(int width, int height) {
            this->width = width;
            this->height = height;
        };

        static Window* fromXML(XMLElement* xml) {
            GET_XML_ELEMENT_ATTRIB_OR_FAIL(xml, "width", width, int);
            GET_XML_ELEMENT_ATTRIB_OR_FAIL(xml, "height", height, int);

            return new Window(width, height);
        };
    
    private:
        int width;
        int height;
};