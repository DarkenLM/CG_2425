#include "common/common.hpp"
#include "common/geometry/BaseGeometry.hpp"
#include "common/parser.hpp"
#include "engine/scene/Scene.hpp"

#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif

#define _USE_MATH_DEFINES
#include <math.h>
#include <stdio.h>

#include <sstream>
#include <vector>
#include <chrono>

#define MAX_TICK 10000000
#define TICK_MS 16 // ~ 60 FPS (16ms * 60 = 960ms, 17ms * 60 = 1020ms)

struct scenestate {
    Scene* scene;

    std::chrono::steady_clock::time_point lastUpdate;
    float deltaTime;
};
struct scenestate STATE;

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

void renderScene(void) {
    // clear buffers
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // set camera
    STATE.scene->setupCamera();

    // Setup Axis
    glBegin(GL_LINES);
        // x in red
        glColor3f(1.0f, 0.0f, 0.0f);
        glVertex3f(-100.f, 0.0f, 0.0f);
        glVertex3f(100.0f, 0.0f, 0.0f);
        // y in green
        glColor3f(0.0f, 1.0f, 0.0f);
        glVertex3f(0.0f, -100.0f, 0.0f);
        glVertex3f(0.0f, 100.0f, 0.0f);
        // z in blue
        glColor3f(0.0f, 0.0f, 1.0f);
        glVertex3f(0.f, 0.0f, -100.0f);
        glVertex3f(0.0f, 0.0f, 100.0f);

        // Changes color back to white
        glColor3f(1.0f, 1.0f, 1.0f);
    glEnd();

    STATE.scene->render();

    // End of frame
    glutSwapBuffers();
}

void stepFunc(int timerId) {
    // Calculate delta time
    auto now = std::chrono::steady_clock::now();
    STATE.deltaTime = std::chrono::duration_cast<std::chrono::microseconds>(now - STATE.lastUpdate).count() / 1000000.0f;
    STATE.lastUpdate = now;

    STATE.scene->update(STATE.deltaTime);
    glutPostRedisplay();
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
        exit(1);
    }

    fuckAround {
        scene->load();
        STATE.scene = scene;
    } findOut(std::string e) {
        std::cout << std::string("Unable to load scene: ") + e + "\n";
        exit(1);
    }
}

void processKeys(unsigned char key, int x, int y) {
    switch (key) {
        case 'h':  // Solid mode
            glPolygonMode(GL_FRONT, GL_FILL);
            break;
        case 'j':  // Wireframe mode
            glPolygonMode(GL_FRONT, GL_LINE);
            break;
        case 'k':  // Point mode
            glPolygonMode(GL_FRONT, GL_POINT);
            break;
        default:
            break;
    }

    STATE.scene->onKeypress(key, x, y);
    
    glutPostRedisplay();
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

    // printInfo();

    // put callback registry here
    glutDisplayFunc(renderScene);
    glutReshapeFunc(changeSize);
    // glutIdleFunc(renderScene);
    glutTimerFunc(16, stepFunc, 0);

    glutKeyboardFunc(processKeys);

    // some OpenGL settings
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);
    glClearColor(0.0f, 0.0f, 0.0f, 0.0f);

    glPolygonMode(GL_FRONT, GL_LINE);

    // enter GLUT�s main cycle
    glutMainLoop();

    return 1;
}