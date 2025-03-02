#pragma once
#include "engine/scene/Window.hpp"
#include "engine/scene/Camera.hpp"
#include "engine/scene/Group.hpp"

// TODO: Add the groups.
class Scene {
    public:
        Scene(
            Window* window,
            Camera* camera,
            std::vector<Group*> groups
        );

        int getWindowWidth() const;
        int getWindowHeight() const;

        static Scene* fromFile(const char* filePath);
        void load();

        void render();
        void setupCamera();
        void setCameraAspectRatio(float aspectRatio);
        void update(float deltaTime);

        void onKeypress(unsigned char key, int mx, int my);

    private:
        Window* window;
        Camera* camera;
        std::vector<Group*> groups;
};