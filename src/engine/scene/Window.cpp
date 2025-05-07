#include "engine/scene/Window.hpp"

Window::Window(int width, int height) {
    this->width = width;
    this->height = height;
}

int Window::getWidth() const {
    return this->width;
}

int Window::getHeight() const {
    return this->height;
}

Window* Window::fromXML(XMLElement* xml) {
    GET_XML_ELEMENT_ATTRIB_OR_FAIL(xml, "width", width, int);
    GET_XML_ELEMENT_ATTRIB_OR_FAIL(xml, "height", height, int);

    return new Window(width, height);
}
