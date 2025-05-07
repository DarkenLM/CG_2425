#pragma once

#include <chrono>

#include "engine/scene/Scene.hpp"

/**
 * @struct scenestate
 * @brief Holds the current runtime state of the scene and rendering context.
 *
 * Contains references to the scene, rendering buffers, timing metrics,
 * performance statistics, and rendering/viewing settings.
 */
struct scenestate {
    Scene* scene; /**< Pointer to the current Scene object. */

    std::vector<GLuint> VBObuffers; /**< Vector of Vertex Buffer Object (VBO) identifiers. */
    std::vector<GLuint> IBObuffers; /**< Vector of Index Buffer Object (IBO) identifiers. */

    std::chrono::steady_clock::time_point lastUpdate; /**< Timestamp of the last update frame. */
    float deltaTime;                                  /**< Time in seconds since the last frame. */
    float ellapsedTime;                               /**< Total elapsed time since the start of the application. */

    float timebase; /**< Time reference for FPS calculation. */
    float frames;   /**< Number of frames rendered since last FPS calculation. */
    float fps;      /**< Frames per second (FPS) counter. */

    // Inner State (UI Validation)

    bool vsync = false;           /**< Enables/disables vertical sync. */
    bool fullscreen = false;      /**< Indicates whether fullscreen mode is enabled. */
    bool showGroupCurves = false; /**< Toggles visibility of group curves for debugging. */

    int polygonMode = 1; /**< Polygon rendering mode: 0 - Fill, 1 - Wireframe, 2 - Points. */
    int cameraMode = 0;  /**< Camera control mode: 0 - Explorer, 1 - First Person, 2 - Third Person. */
};
