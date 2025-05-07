#pragma once

#include "common/util/xmlutil.hpp"

using namespace tinyxml2;

/**
 * @class Window
 * @brief Represents the rendering window's dimensions.
 *
 * Stores the width and height of the rendering window and allows construction
 * from an XML element.
 */
class Window {
   public:
    /**
     * @brief Constructs a Window with the given dimensions.
     * @param width Width of the window in pixels.
     * @param height Height of the window in pixels.
     */
    Window(int width, int height);

    /**
     * @brief Returns the width of the window.
     * @return Width in pixels.
     */
    int getWidth() const;

    /**
     * @brief Returns the height of the window.
     * @return Height in pixels.
     */
    int getHeight() const;

    /**
     * @brief Creates a Window object from an XML element.
     *
     * Expects the XML element to contain "width" and "height" attributes.
     *
     * @param xml Pointer to the XML element.
     * @return A pointer to the newly created Window object.
     */
    static Window* fromXML(XMLElement* xml);

   private:
    int width;  /**< Width of the window in pixels. */
    int height; /**< Height of the window in pixels. */
};
