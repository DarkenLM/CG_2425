#include <chrono>

#include "engine/scene/Scene.hpp"

struct scenestate {
    Scene* scene;
    std::vector<GLuint> vboBuffers;

    std::chrono::steady_clock::time_point lastUpdate;
    float deltaTime;

    float timebase;
    float frames;
    float fps;

    // inner State (UI Validation)
    bool vsync = false;
    bool fullscreen = false;
    int polygonMode = 1;  // 0 - Fill; 1 - WireFrame; 2 - Points
    int cameraMode = 0;   // 0 - Explorer; 1 - First Person; 2 - Third Person
};