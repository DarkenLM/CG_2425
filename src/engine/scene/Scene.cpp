#include "engine/scene/Scene.hpp"

#include <filesystem>
#include <iostream>
#include <vector>

#include "common/debug.hpp"
#include "common/util/xmlutil.hpp"
#include "lib/tinyxml2.h"

namespace fs = std::filesystem;
using namespace tinyxml2;

Scene::Scene(Window* window, Camera* camera, std::vector<Group*> groups, std::vector<Light*> lights) {
    this->window = window;
    this->camera = camera;
    this->groups = groups;
    this->lights = lights;
}

int Scene::getWindowWidth() const { return this->window->getWidth(); }

int Scene::getWindowHeight() const { return this->window->getHeight(); }

std::vector<Group*> Scene::getGroups() const { return this->groups; }

const Model* Scene::getObjectById(const char* id) const {
    for (const auto& group : this->groups) {
        const Model* target = group->getObjectById(id);
        if (target != nullptr) return target;
    }

    return nullptr;
}

// TODO: Corrigir esta funçao, esta a devolver o group da scene
Group* Scene::getGroupWithObjectId(const char* id) const {
    for (const auto& group : this->groups) {
        const Model* target = group->getObjectById(id);
        if (target != nullptr) return group;
    }

    return nullptr;
}

Scene* Scene::fromFile(const char* filePath) {
    fs::path fullPath = fs::absolute(filePath);

    if (!fs::exists(fullPath)) {
        ERROR << "Error opening file for reading: " << fullPath << std::endl;
        return nullptr;
    }

    XMLDocument doc;
    if (doc.LoadFile(fullPath.c_str()) != XML_SUCCESS) {
        ERROR << "Error reading XML file: ";
        doc.PrintError();
        return nullptr;
    }

    // XMLElement* root;
    // if ((root = doc.FirstChildElement("world")) == NULL) {
    //     ERROR << "Could not find element 'world' at root." << std::endl;
    //     return nullptr;
    // }

    GET_XML_DOC_ELEMENT_OR_FAIL(doc, "world", root);

    GET_XML_ELEMENT_OR_FAIL(root, "window", window);
    Window* sceneWindow = Window::fromXML(window);

    GET_XML_ELEMENT_OR_FAIL(root, "camera", camera);
    Camera* sceneCamera = Camera::fromXML(camera);

    GET_XML_ELEMENT(root, "lights", _lights);
    std::vector<Light*> lights;
    if (_lights != NULL) {
        GET_XML_ELEMENT(_lights, "light", _light);
        do {
            Light* light = Light::fromXML(_light);
            if (!light) return nullptr;  // TODO: Outright break or silently ignore?

            lights.push_back(light);
        } while ((_light = _light->NextSiblingElement("light")));
    }

    std::vector<Group*> groups;
    GET_XML_ELEMENT_OR_FAIL(root, "group", _group);
    do {
        Group* group = Group::fromXML(_group);
        if (!group) return nullptr;  // TODO: Outright break or silently ignore?

        groups.push_back(group);
    } while ((_group = _group->NextSiblingElement("group")));

    Scene* scene = new Scene(sceneWindow, sceneCamera, groups, lights);
    return scene;
}

void Scene::load() {
    for (Group* g : this->groups) {
        fuckAround {
            g->load();
        }
        findOut(std::string e) {
            yeet std::string("Unable to load group: ") + e;
        }
    }

    fuckAround {
        if (!this->camera->isTrackingFallback()) {
            std::string targetId = this->camera->getTrackingId();
            const Model* targetModel = this->getObjectById(targetId.c_str());

            Group* targetGroup = this->getGroupWithObjectId(targetId.c_str());

            this->camera->track(targetModel, targetId);
        }

        this->camera->load();
    }
    findOut(std::string e) {
        yeet std::string("Unable to load camera: ") + e;
    }

    this->setCameraAspectRatio(this->getWindowWidth() / this->getWindowHeight());
}

void Scene::loadLights() {
    fuckAround {
        float amb[4] = {1.0f, 1.0f, 1.0f, 1.0f};
        glLightModelfv(GL_LIGHT_MODEL_AMBIENT, amb);
        for (int i = 0; i < this->lights.size(); i++) {
            if (i == 8) {
                yeet std::string("Program not ready for that many light sources");
                exit;
            }
            this->lights[i]->gl_id = GL_LIGHT0 + i;
            glEnable(GL_LIGHT0 + i);
        }
    }
    findOut(std::string e) {
        yeet std::string("Unable to load lights: ") + e;
    }
}

void Scene::render() {
    for (Light* l : this->lights) {
        l->render();
    }
    for (Group* g : this->groups) {
        g->render();
    }
}

void Scene::setupCamera() {
    this->camera->render();
}

void Scene::setCameraMode(CameraMode camMode) {
    this->camera->setCameraMode(camMode);
}

void Scene::setCameraAspectRatio(float aspectRatio) {
    this->camera->setAspectRatio(aspectRatio);
}

Camera* Scene::getCamera() {
    return this->camera;
}

void Scene::update(float deltaTime) {
}

/**
 * This Function serves as a bridge inside the scene, to send keyboard input from the main(glut) to the camera object
 */
void Scene::onKeypress(unsigned char key, int mx, int my) {
    this->camera->processMovement(key, mx, my);
    return;
}
