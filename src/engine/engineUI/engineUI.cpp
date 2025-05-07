#include "engine/engineUI/engineUI.hpp"

#include <iostream>

#include "engine/scene/SceneState.hpp"

extern struct scenestate STATE;  ///< External reference to the global scene state

EngineUI::EngineUI()
    : show_demo_window(false),
      show_another_window(false),
      show_performance_window(true),
      clear_color(ImVec4(0.45f, 0.55f, 0.60f, 1.00f)),
      fullscreen(false),
      vsync(false),
      polygonMode(1),
      cameraMode(0) {}

EngineUI::~EngineUI() {
    shutdown();
}

/**
 * @brief Sets up the ImGui style (rounded edges, padding, etc.).
 */
void setupStyle() {
    ImGui::StyleColorsClassic();

    ImGuiStyle& style = ImGui::GetStyle();
    // Sets window design
    style.WindowRounding = 8.0f;
    style.WindowPadding = (ImVec2(15.0f, 15.0f));
    style.WindowTitleAlign = (ImVec2(0.5f, 0.5f));
    // Sets frame design
    style.FrameRounding = 8.0f;
    style.FramePadding = (ImVec2(5.0f, 5.0f));
    style.FrameBorderSize = 1.0f;
    // Sets misc design
    style.PopupRounding = 8.0f;
    style.ItemSpacing = (ImVec2(8.0f, 5.0f));
    style.TabBorderSize = 1.0f;
    style.ScrollbarRounding = 10.0f;
    style.GrabRounding = 10.0f;
    style.GrabMinSize = 18.0f;
}

void EngineUI::init() {
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();

    ImGuiIO& io = ImGui::GetIO();
    (void)io;
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;

    setupStyle();

    ImGui_ImplGLUT_Init();
    ImGui_ImplOpenGL3_Init();
}

/**
 * @brief Displays the performance overlay with FPS and camera information.
 *
 * @param p_open Pointer to a boolean that controls whether the overlay is open.
 */
static void showPerformanceOverlay(bool* p_open) {
    static int location = 1;
    ImGuiIO& io = ImGui::GetIO();
    ImGuiWindowFlags window_flags = ImGuiWindowFlags_NoDecoration |
                                    ImGuiWindowFlags_AlwaysAutoResize |
                                    ImGuiWindowFlags_NoSavedSettings |
                                    ImGuiWindowFlags_NoFocusOnAppearing |
                                    ImGuiWindowFlags_NoNav;
    if (location >= 0) {
        const float PAD = 10.0f;
        const ImGuiViewport* viewport = ImGui::GetMainViewport();
        ImVec2 work_pos = viewport->WorkPos;  // Use work area to avoid menu-bar/task-bar, if any!
        ImVec2 work_size = viewport->WorkSize;
        ImVec2 window_pos, window_pos_pivot;
        window_pos.x = (location & 1) ? (work_pos.x + work_size.x - PAD) : (work_pos.x + PAD);
        window_pos.y = (location & 2) ? (work_pos.y + work_size.y - PAD) : (work_pos.y + PAD);
        window_pos_pivot.x = (location & 1) ? 1.0f : 0.0f;
        window_pos_pivot.y = (location & 2) ? 1.0f : 0.0f;
        ImGui::SetNextWindowPos(window_pos, ImGuiCond_Always, window_pos_pivot);
        window_flags |= ImGuiWindowFlags_NoMove;
    } else if (location == -2) {
        ImGui::SetNextWindowPos(ImGui::GetMainViewport()->GetCenter(), ImGuiCond_Always, ImVec2(0.5f, 0.5f));
        window_flags |= ImGuiWindowFlags_NoMove;
    }
    ImGui::SetNextWindowBgAlpha(0.35f);  // Transparent background
    if (ImGui::Begin("Example: Simple overlay", p_open, window_flags)) {
        ImGui::Text(
            "Overlay with performance Information\n"
            "(right-click to change position)");
        ImGui::Separator();
        if (ImGui::IsMousePosValid())
            ImGui::Text("Mouse Position: (%.1f,%.1f)", io.MousePos.x, io.MousePos.y);
        else
            ImGui::Text("Mouse Position: <invalid>");
        if (ImGui::BeginPopupContextWindow()) {
            if (ImGui::MenuItem("Custom", NULL, location == -1)) location = -1;
            if (ImGui::MenuItem("Center", NULL, location == -2)) location = -2;
            if (ImGui::MenuItem("Top-left", NULL, location == 0)) location = 0;
            if (ImGui::MenuItem("Top-right", NULL, location == 1)) location = 1;
            if (ImGui::MenuItem("Bottom-left", NULL, location == 2)) location = 2;
            if (ImGui::MenuItem("Bottom-right", NULL, location == 3)) location = 3;
            if (p_open && ImGui::MenuItem("Close")) *p_open = false;
            ImGui::EndPopup();
        }
        ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / STATE.fps, STATE.fps);
        ImGui::Separator();
        char* cameraType;
        if (STATE.scene->getCamera()->getCameraMode() == CAMERA_EX) {
            ImGui::Text("[Explorer Camera]");
        } else if (STATE.scene->getCamera()->getCameraMode() == CAMERA_FP) {
            ImGui::Text("[First Person Camera]");
        } else if (STATE.scene->getCamera()->getCameraMode() == CAMERA_TP) {
            ImGui::Text("[Third Person Camera]");
        } else {
            ImGui::Text("[ERROR]");
        }
        Vector3<float> position = STATE.scene->getCamera()->getCurrentPos();
        Vector3<float> lookAt = STATE.scene->getCamera()->getCurrentLookAt();
        ImGui::Text("POS: %.2fx %.2fy %.2fz", position.first, position.second, position.third);
        ImGui::Text("LOOKAT: %.2fx %.2fy %.2fz", lookAt.first, lookAt.second, lookAt.third);
    }
    ImGui::End();
}

// Creates a node in gui for the settings
void EngineUI::settings() {
    if (ImGui::TreeNodeEx("Settings", ImGuiTreeNodeFlags_Framed)) {
        if (ImGui::CollapsingHeader("More Windows")) {
            ImGui::Checkbox("Performance Overlay", &this->show_performance_window);
        }
        if (ImGui::CollapsingHeader("OpenGL Settings")) {
            ImGui::Checkbox("Fullscreen", &this->fullscreen);
            ImGui::Checkbox("Vsync", &this->vsync);
            ImGui::RadioButton("Filled", &this->polygonMode, 0);
            ImGui::SameLine();
            ImGui::RadioButton("Wireframed", &this->polygonMode, 1);
            ImGui::SameLine();
            ImGui::RadioButton("Points", &this->polygonMode, 2);
        }
        ImGui::TreePop();
    }
}

// Creates a node in gui for the camera options
void EngineUI::camera() {
    if (ImGui::TreeNodeEx("Camara", ImGuiTreeNodeFlags_Framed)) {
        ImGui::RadioButton("Explorer", &this->cameraMode, 0);
        ImGui::SameLine();
        ImGui::RadioButton("First Person", &this->cameraMode, 1);
        ImGui::SameLine();
        ImGui::RadioButton("Third Person", &this->cameraMode, 2);
        ImGui::InputFloat("Rotation Speed", &(STATE.scene->getCamera()->ROTATION_SPEED), 0.01f, 0.5f, "%.2f", 1);
        if (this->cameraMode == 1) {
            ImGui::InputFloat("Movement Speed", &(STATE.scene->getCamera()->MOVEMENT_SPEED), 0.01f, 0.5f, "%.2f", 1);
        }
        if (this->cameraMode == 0) {
            ImGui::SliderFloat("Zoom Speed", &(STATE.scene->getCamera()->ZOOM_STEP), 0.01f, STATE.scene->getCamera()->ZOOM_STEP + 10, "%.2f", 1);
        }
        ImGui::TreePop();
    }
}

// Creates a node in gui for the sceneModels options
void EngineUI::models() {
    if (ImGui::TreeNodeEx("Models", ImGuiTreeNodeFlags_Framed)) {
        ImGui::Text("To be implemented!!");
        ImGui::TreePop();
    }
}

// This function is the responsible to render the ui in the window
void EngineUI::render() {
    // Start the Dear ImGui frame
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGLUT_NewFrame();
    ImGui::NewFrame();

    // Simple ImGUI window
    if (show_demo_window)
        ImGui::ShowDemoWindow(&show_demo_window);

    static float f = 0.0f;
    static int counter = 0;
    ImGuiIO& io = ImGui::GetIO();

    ImGui::Begin("Our CG engine!", nullptr, ImGuiWindowFlags_NoMove | ImGuiWindowFlags_AlwaysAutoResize);
    ImGui::Text("This project was developed as part\n of the Computer Graphics course.");

    ImGui::SetWindowPos(ImVec2(0, 0), ImGuiCond_Always);

    // ImGui::Checkbox("Demo Window", &show_demo_window);  // Edit bools storing our window open/close state

    if (this->show_performance_window) {
        showPerformanceOverlay(&this->show_performance_window);
    }

    EngineUI::models();

    EngineUI::camera();

    EngineUI::settings();

    ImGui::End();

    // Render ImGUI
    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

// This shuts down the ui
void EngineUI::shutdown() {
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGLUT_Shutdown();
    ImGui::DestroyContext();
}
