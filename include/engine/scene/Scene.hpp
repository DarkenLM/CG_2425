#pragma once
#include "engine/scene/Camera.hpp"
#include "engine/scene/Group.hpp"
#include "engine/scene/Window.hpp"
#include "engine/scene/Light.hpp"

// TODO: Add the groups.
class Scene {
    public:
        Scene(
            Window* window,
            Camera* camera,
            std::vector<Group*> groups,
            std::vector<Light*> lights
        );

        int getWindowWidth() const;
        int getWindowHeight() const;
        Camera* getCamera();

        static Scene* fromFile(const char* filePath);
        void load();

        void render();
        void setupCamera();
        void setCameraMode(CameraMode camMode);
        void setCameraAspectRatio(float aspectRatio);
        void update(float deltaTime);

        void onKeypress(unsigned char key, int mx, int my);
        void onKeypress2(unsigned char key, int mx, int my);

    private:
        Window* window;
        Camera* camera;
        std::vector<Group*> groups;
        std::vector<Light*> lights;
};