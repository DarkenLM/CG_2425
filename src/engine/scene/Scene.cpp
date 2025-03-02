#include "engine/scene/Scene.hpp"
#include "common/util/xmlutil.hpp"
#include "common/debug.hpp"
#include "lib/tinyxml2.h"
#include <filesystem>
#include <iostream>
#include <vector>

namespace fs = std::filesystem;
using namespace tinyxml2;

Scene::Scene(Window* window, Camera* camera, std::vector<Group*> groups) {
    this->window = window;
    this->camera = camera;
    this->groups = groups;
}

int Scene::getWindowWidth() const { return this->window->getWidth(); }
int Scene::getWindowHeight() const { return this->window->getHeight(); }

Scene* Scene::fromFile(const char* filePath) {
    fs::path fullPath = fs::absolute(filePath);

    if (!fs::exists(fullPath)) {
        ERROR << "Error opening file for reading: " << fullPath << std::endl;
        return nullptr;
    }

    XMLDocument doc;
    if (doc.LoadFile(fullPath.c_str()) != XML_SUCCESS) {
        ERROR << "Error reading XML file: "; doc.PrintError();
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
    sceneCamera->setCameraMode(CAMERA_EX);

    std::vector<Group*> groups;

    GET_XML_ELEMENT_OR_FAIL(root, "group", _group);
    do {
        Group* group = Group::fromXML(_group);
        if (!group) return nullptr; // TODO: Outright break or silently ignore?

        groups.push_back(group);
    } while ((_group = _group->NextSiblingElement("group")));

    Scene* scene = new Scene(sceneWindow, sceneCamera, groups);
    return scene;
}

void Scene::load() {
    for (Group* g : this->groups) {
        fuckAround {
            g->load();
        } findOut(std::string e) {
            yeet std::string("Unable to load group: ") + e;
        }
    }

    this->setCameraAspectRatio(this->getWindowWidth() / this->getWindowHeight());
}

void Scene::render() {
    for (Group* g : this->groups) {
        g->render();
    }   
}

void Scene::setupCamera() {
    this->camera->render();
}

void Scene::setCameraAspectRatio(float aspectRatio) {
    this->camera->setAspectRatio(aspectRatio);
}

void Scene::update(float deltaTime) {

}

void Scene::onKeypress(unsigned char key, int mx, int my) {
    switch (key) {
        case 'q': case 'Q': {
			this->camera->rotateTo(0, Camera::CAMERA_STEP, 0.0f);
			break;
		}
		case 'e': case 'E': {
			this->camera->rotateTo(0, -Camera::CAMERA_STEP, 0.0f);
			break;
		}
		case 'z': case 'Z': {
			this->camera->rotateTo(0, 0.0f, Camera::CAMERA_STEP);
			break;
		}
		case 'c': case 'C': {
			this->camera->rotateTo(0, 0.0f, -Camera::CAMERA_STEP);
			break;
		}
        default: {
            // Didn't to shit, do not reprocess.
            return;
        }
    }

    glutPostRedisplay();
}
