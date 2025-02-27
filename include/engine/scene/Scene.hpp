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

        static Scene* fromFile(char* filePath);

        void load();

    private:
        Window* window;
        Camera* camera;
        std::vector<Group*> groups;
};