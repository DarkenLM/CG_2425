#pragma once
#include "engine/scene/Camera.hpp"
#include "engine/scene/Group.hpp"
#include "engine/scene/Light.hpp"
#include "engine/scene/Window.hpp"

/**
 * @class Scene
 * @brief Represents a 3D scene containing a camera, lights, and groups of objects.
 *
 * Manages rendering, updates, and camera controls for the scene.
 */
class Scene {
   public:
    /**
     * @brief Constructs a new Scene object.
     * @param window Pointer to the rendering window.
     * @param camera Pointer to the scene camera.
     * @param groups Vector of object groups in the scene.
     * @param lights Vector of lights in the scene.
     */
    Scene(
        Window* window,
        Camera* camera,
        std::vector<Group*> groups,
        std::vector<Light*> lights);

    /**
     * @brief Gets the width of the rendering window.
     * @return Width in pixels.
     */
    int getWindowWidth() const;

    /**
     * @brief Gets the height of the rendering window.
     * @return Height in pixels.
     */
    int getWindowHeight() const;

    /**
     * @brief Returns a pointer to the scene's camera.
     * @return Pointer to Camera object.
     */
    Camera* getCamera();

    /**
     * @brief Creates a Scene instance from a file.
     * @param filePath Path to the scene configuration file.
     * @return Pointer to the loaded Scene.
     */
    static Scene* fromFile(const char* filePath);

    /** @brief Loads the scene data (groups, models, etc.). */
    void load();

    /** @brief Renders the scene, including all groups and lights. */
    void render();

    /** @brief Configures the camera based on current scene settings. */
    void setupCamera();

    /**
     * @brief Sets the current camera mode.
     * @param camMode The desired camera mode.
     */
    void setCameraMode(CameraMode camMode);

    /**
     * @brief Sets the aspect ratio of the camera.
     * @param aspectRatio Width-to-height ratio.
     */
    void setCameraAspectRatio(float aspectRatio);

    /**
     * @brief Updates the scene based on elapsed time.
     * @param deltaTime Time since last update in seconds.
     */
    void update(float deltaTime);

    /**
     * @brief Handles keyboard input events.
     * @param key Pressed key.
     * @param mx Mouse X position when key was pressed.
     * @param my Mouse Y position when key was pressed.
     */
    void onKeypress(unsigned char key, int mx, int my);

   private:
    Window* window;             /**< Pointer to the rendering window. */
    Camera* camera;             /**< Pointer to the camera used in the scene. */
    std::vector<Group*> groups; /**< Groups of objects within the scene. */
    std::vector<Light*> lights; /**< Light sources in the scene. */
};
