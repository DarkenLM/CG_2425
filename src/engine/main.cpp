#ifndef GLUT
#include <GL/glew.h>
#include <GL/glut.h>
#endif

#include "common/geometry/BaseGeometry.hpp"
#include "common/parser.hpp"
#include "engine/engineUI/engineUI.hpp"
#include "engine/input.hpp"
#include "engine/scene/Scene.hpp"
#include "engine/scene/SceneState.hpp"

struct scenestate STATE;

#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif

#define _USE_MATH_DEFINES
#include <math.h>
#include <stdio.h>

#include <chrono>
#include <sstream>
#include <vector>

#define MAX_TICK 10000000
#define TICK_MS 16  // ~ 60 FPS (16ms * 60 = 960ms, 17ms * 60 = 1020ms)

EngineUI engineUI;

double clamp(double value, double min, double max) {
    if (value > max) return max;
    if (value < min) return min;
    return value;
}

void changeSize(int w, int h) {
    // Prevent a divide by zero, when window is too short
    // (you can�t make a window with zero width).
    if (h == 0) h = 1;

    // compute window's aspect ratio
    float ratio = w * 1.0f / h;
    STATE.scene->setCameraAspectRatio(ratio);

    // finally set the viewport to be the entire window
    glViewport(0, 0, w, h);
}

void validateGlutSettings() {
    // Fullscreen validation
    if (STATE.fullscreen != engineUI.fullscreen) {
        STATE.fullscreen = engineUI.fullscreen;
        if (STATE.fullscreen) {
            glutFullScreen();
        } else {
            glutReshapeWindow(STATE.scene->getWindowWidth(), STATE.scene->getWindowHeight());
            changeSize(STATE.scene->getWindowWidth(), STATE.scene->getWindowHeight());
        }
    }

    // Vsync validation
    if (STATE.vsync != engineUI.vsync) {
        STATE.vsync = engineUI.vsync;
        if (STATE.vsync) {
            // glutswap(1);
        } else {
            // glutSwapInterval(0);
        }
    };

    // PolygonMode Validation
    if (STATE.polygonMode != engineUI.polygonMode) {
        STATE.polygonMode = engineUI.polygonMode;
        switch (STATE.polygonMode) {
            case 0:
                glPolygonMode(GL_FRONT, GL_FILL);
                break;
            case 1:
                glPolygonMode(GL_FRONT, GL_LINE);
                break;
            case 2:
                glPolygonMode(GL_FRONT, GL_POINT);
                break;
            default:
                break;
        }
    }

    if (STATE.cameraMode != engineUI.cameraMode) {
        STATE.cameraMode = engineUI.cameraMode;
        switch (STATE.cameraMode) {
            case 0:
                STATE.scene->setCameraMode(CAMERA_EX);
                break;
            case 1:
                STATE.scene->setCameraMode(CAMERA_FP);
                break;
            case 2:
                STATE.scene->setCameraMode(CAMERA_TP);
                break;
            default:
                break;
        }
    }
}

void loadfps() {
    STATE.frames++;
    int time = glutGet(GLUT_ELAPSED_TIME);
    if (time - STATE.timebase > 50) {
        STATE.fps = STATE.frames * 1000 / (time - STATE.timebase);
        STATE.timebase = time;
        STATE.frames = 0;
    }
}

void handleInputEvents(InputEvent* e) {
    KeypressEvent* ke = (KeypressEvent*)(e);
    if (e->isKeypressEvent() && ke->up) {
        ImGui_ImplGLUT_KeyboardUpFunc(ke->key.key, ke->mouseX, ke->mouseY);
        ke->process();
    } else if (e->isKeypressEvent()) {
        // ImGui_ImplGLUT_KeyboardFunc(ke->key.key, ke->mouseX, ke->mouseY);
    }
}

void renderScene(void) {
    // Update ImGUI display size
    ImGuiIO& io = ImGui::GetIO();
    int width = glutGet(GLUT_WINDOW_WIDTH);
    int height = glutGet(GLUT_WINDOW_HEIGHT);
    io.DisplaySize = ImVec2((float)width, (float)height);

    // clear buffers
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // Validates of glut is fullscreen or not
    validateGlutSettings();

    // calculates the fps.
    loadfps();

    // set camera
    STATE.scene->setupCamera();

    // Setup Axis

    glBegin(GL_LINES);
    // x in red
    glColor3f(1.0f, 0.0f, 0.0f);
    glVertex3f(-1000.f, 0.0f, 0.0f);
    glVertex3f(1000.0f, 0.0f, 0.0f);
    // y in green
    glColor3f(0.0f, 1.0f, 0.0f);
    glVertex3f(0.0f, -1000.0f, 0.0f);
    glVertex3f(0.0f, 1000.0f, 0.0f);
    // z in blue
    glColor3f(0.0f, 0.0f, 1.0f);
    glVertex3f(0.f, 0.0f, -1000.0f);
    glVertex3f(0.0f, 0.0f, 1000.0f);

    // Changes color back to white
    glColor3f(1.0f, 1.0f, 1.0f);
    glEnd();

    STATE.scene->render();

    // Render ImGui UI
    engineUI.render();

    // End of frame
    glutSwapBuffers();
}

void stepFunc(int timerId) {
    // Calculate delta time
    auto now = std::chrono::steady_clock::now();
    STATE.deltaTime = std::chrono::duration_cast<std::chrono::microseconds>(now - STATE.lastUpdate).count() / 1000000.0f;
    STATE.lastUpdate = now;

    STATE.scene->update(STATE.deltaTime);
    STATE.ellapsedTime += STATE.deltaTime;

    if (InputManager::hasInput()) {
        MouseEvent me = InputManager::getLastMousePos();

        // Handle input
        // printf("STEP KEYS: ");
        for (Key k : InputManager::getKeys()) {
            // printf("%s, ", k.toString().c_str());

            ImGui_ImplGLUT_KeyboardFunc(k.key, me.mouseX, me.mouseY);
            STATE.scene->onKeypress(k.key, me.mouseX, me.mouseY);
        }
    }
    glutPostRedisplay();
    glutTimerFunc(16, stepFunc, 0);
}

void printInfo() {
    printf("Vendor: %s\n", glGetString(GL_VENDOR));
    printf("Renderer: %s\n", glGetString(GL_RENDERER));
    printf("Version: %s\n", glGetString(GL_VERSION));
}

void loadScene(const char* sceneFile) {
    std::cout << "Loading scene '" << sceneFile << "'\n";

    Scene* scene = Scene::fromFile(sceneFile);
    if (scene == nullptr) {
        std::cout << "Unable to read scene.\n";
        exit(1);
    }

    fuckAround {
        scene->load();
        STATE.scene = scene;
    }
    findOut(std::string e) {
        std::cout << std::string("Unable to load scene: ") + e + "\n";
        exit(1);
    }
}

void genVBOs() {
    Map<BaseGeometry*, std::string> geometrys = Model::getGeometryCache();
    Map<GLuint, std::string>& VBOids = Model::getGeometryVBO();
    Map<GLuint, std::string>& IBOids = Model::getGeometryIBO();
    std::vector<std::string> keys = geometrys.keys();

    // Prepare buffers
    STATE.VBObuffers.resize(keys.size());
    STATE.IBObuffers.resize(keys.size());

    // Generate VBOs
    glGenBuffers(keys.size(), STATE.VBObuffers.data());
    // Generate IBO
    glGenBuffers(keys.size(), STATE.IBObuffers.data());

    // Assign buffers
    for (int i = 0; i < keys.size(); i++) {
        VBOids.add(keys[i], STATE.VBObuffers[i]);
        IBOids.add(keys[i], STATE.IBObuffers[i]);
    }

    for (std::string k : keys) {
        std::optional<GLuint> VBOindex = VBOids.get(k);
        std::optional<GLuint> IBOindex = IBOids.get(k);
        std::optional<BaseGeometry*> geometry = geometrys.get(k);
        std::vector<Point3D> modelVerticesArray = geometry.value()->getVertices();
        std::vector<unsigned int> modelIndicesArray = geometry.value()->getIndices();
        if (VBOindex.value()) {
            std::cout << "A criar vbo para: " << k << " com buffer em: " << VBOindex.value() << std::endl;

            glBindBuffer(GL_ARRAY_BUFFER, VBOindex.value());
            glBufferData(
                GL_ARRAY_BUFFER,
                sizeof(Point3D) * modelVerticesArray.size(),
                modelVerticesArray.data(),
                GL_STATIC_DRAW);

            glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBOindex.value());
            glBufferData(GL_ELEMENT_ARRAY_BUFFER,
                         sizeof(unsigned int) * modelIndicesArray.size(),
                         modelIndicesArray.data(),
                         GL_STATIC_DRAW);
        } else {
            std::cout << "Error at loading the vbo for: " << k << std::endl;
        }
    }
}

void processKeys(unsigned char key, int x, int y) {
    ImGui_ImplGLUT_KeyboardFunc(key, x, y);
    STATE.scene->onKeypress(key, x, y);

    glutPostRedisplay();
}

void processSpecialKeys(int key, int x, int y) {
    glutPostRedisplay();
}

// Callback function for mouse input
void processMouse(int button, int state, int x, int y) {
    ImGui_ImplGLUT_MouseFunc(button, state, x, y);
    if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN) {
    }
}

// Callback function for mouse motion
void motionFunc(int x, int y) {
    ImGuiIO& io = ImGui::GetIO();
    io.MousePos = ImVec2(x, y);
}

int main(int argc, char** argv) {
    if (argc < 2) {
        std::cerr << "Usage: <scene file>\n";
        return 1;
    }

    const char* sceneFile = argv[1];

    loadScene(sceneFile);

    // put GLUT init here
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGBA);
    glutInitWindowPosition(100, 100);
    glutInitWindowSize(STATE.scene->getWindowWidth(), STATE.scene->getWindowHeight());
    glutCreateWindow("CG@DI");
    glewInit();

    glEnableClientState(GL_VERTEX_ARRAY);
    genVBOs();

    // Inicializar fps counter
    STATE.timebase = glutGet(GLUT_ELAPSED_TIME);

    // Initialize ImGui
    engineUI.init();

    // put callback registry here
    glutDisplayFunc(renderScene);
    glutReshapeFunc(changeSize);
    glutIdleFunc(renderScene);
    glutTimerFunc(16, stepFunc, 0);

    // Keyboard and mouse input
    // glutKeyboardFunc(processKeys);
    // glutSpecialFunc(processSpecialKeys);
    InputManager::listen(handleInputEvents);

    glutMouseFunc(processMouse);
    glutMotionFunc(motionFunc);
    glutPassiveMotionFunc(motionFunc);

    // some OpenGL settings
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);
    glClearColor(0.0f, 0.0f, 0.0f, 0.0f);

    glPolygonMode(GL_FRONT, GL_LINE);

    // enter GLUT�s main cycle
    glutMainLoop();

    return 1;
}