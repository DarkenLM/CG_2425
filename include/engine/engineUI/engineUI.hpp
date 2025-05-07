#pragma once
#include <engine/imGUI/imgui.h>
#include <engine/imGUI/imgui_impl_glut.h>
#include <engine/imGUI/imgui_impl_opengl3.h>

/**
 * @class EngineUI
 * @brief Manages the user interface for the engine using ImGui.
 */
class EngineUI {
   public:
    /**
     * @brief Constructor for EngineUI class.
     */
    EngineUI();

    /**
     * @brief Destructor for EngineUI class.
     */
    ~EngineUI();

    /**
     * @brief Initializes the ImGui context and settings.
     */
    void init();

    /**
     * @brief Renders the user interface.
     */
    void render();

    /**
     * @brief Shuts down the ImGui context.
     */
    void shutdown();

    bool fullscreen;  ///< Whether fullscreen mode is enabled.
    bool vsync;       ///< Whether V-Sync is enabled.
    int polygonMode;  ///< Polygon mode (0: Fill, 1: Wireframe, 2: Points).
    int cameraMode;   ///< Camera mode (0: Explorer, 1: First Person, 2: Third Person).

   private:
    // ImGUI Settings
    bool show_demo_window;
    bool show_another_window;
    bool show_performance_window;
    ImVec4 clear_color;

    /**
     * @brief Displays the settings for the models in the scene.
     */
    void models();

    /**
     * @brief Displays the settings for the engine UI.
     */
    void settings();

    /**
     * @brief Displays the settings for the camera.
     */
    void camera();
};