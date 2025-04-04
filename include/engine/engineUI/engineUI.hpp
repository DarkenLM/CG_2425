#pragma once
#include <engine/imGUI/imgui.h>
#include <engine/imGUI/imgui_impl_glut.h>
#include <engine/imGUI/imgui_impl_opengl3.h>

class EngineUI {
   public:
    EngineUI();
    ~EngineUI();

    void init();
    void render();
    void shutdown();

    // Opengl Settings
    bool fullscreen;
    bool vsync;
    int polygonMode;  // 0 - Fill; 1 - WireFrame; 2 - Points
    int cameraMode;   // 0 - Explorer; 1 - First Person; 2 - Third Person

   private:
    // ImGUI Settings
    bool show_demo_window;
    bool show_another_window;
    bool show_performance_window;
    ImVec4 clear_color;
    // Scene Settings

    void models();
    void settings();
    void camera();
};