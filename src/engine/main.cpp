// #include <stdio.h>
// #include "common/common.hpp"

// int main(int argc, char** argv) {
//     commonTest();
//     printf("Hello from ENGINE.\n");
// }

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

#define MAX_TICK 10000000

int TICK;

//----- Camera settings -----//

// Camera distance
GLfloat distance = 10.0f;

// Using distance and 45 degrees to calculate initial position
GLfloat alpha = M_PI / 4;  // Vertical angle
GLfloat beta = M_PI / 4;   // Horizontal angle

// Setting gluLookAt
GLfloat posX = distance * cos(alpha) * sin(beta);
GLfloat posY = distance * sin(alpha);
GLfloat posZ = distance * cos(alpha) * cos(beta);
GLfloat atX = 0.0f, atY = 0.0f, atZ = 0.0f;  // LookAt target
GLfloat upX = 0.0f, upY = 1.0f, upZ = 0.0f;  // Up vector

//----- end -----//

struct scenestate {
    std::vector<BaseGeometry> objects;
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
    // Set the projection matrix as current
    glMatrixMode(GL_PROJECTION);
    // Load the identity matrix
    glLoadIdentity();
    // Set the perspective
    gluPerspective(45.0f, ratio, 1.0f, 1000.0f);
    // return to the model view matrix mode
    glMatrixMode(GL_MODELVIEW);

    // finally set the viewport to be the entire window
    glViewport(0, 0, w, h);
}

void renderScene(void) {
    // clear buffers
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // set camera
    glLoadIdentity();
    gluLookAt(
        posX, posY, posZ,
        atX, atY, atZ,
        upX, upY, upZ);

    // put axis drawing in here
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
    // double camX = 1, camZ = 0;
    // printf("CAM: %f %f\n", camX, camZ);

    // put drawing instructions here
    // glutSolidCube(1.0f);
    // glutWireCube(1.0f);
    // glutWireSphere(1.0f, 10, 10);
    // glutWireTorus(0.5f, 1.0f, 10, 10);

    TICK = (TICK + 1) % MAX_TICK;
    // printf("TICK: %d\n", *TICK);

    double mult = (TICK * (M_PI / 4)) / 20;
    // double mult = *TICK / 20;
    // double _mult = clamp(mult, 0, M_PI);
    double _mult = 0;

    // printf("MULT: %f %f\n", mult, _mult);
    glutWireTeapot(2);
    // glutWireTeapot(1);

    // End of frame
    glutSwapBuffers();
}

void printInfo() {
    // printf("Vendor: %s\n", glGetString(GL_VENDOR));
    // printf("Renderer: %s\n", glGetString(GL_RENDERER));
    // printf("Version: %s\n", glGetString(GL_VERSION));
}

/**
 * Function used to test the parser
 void testingParser() {
    std::vector<Point3D> vertices;
    if (Parser3D::load3DFile("cylinder.3d", vertices) == 0) {
        for (int i = 0; i < vertices.size(); i++) {
            std::cout << "Vertice " << i << ": " << vertices[i] << std::endl;
        }
    } else {
        std::cerr << "Erro a ler o ficheiro";
        return;
    }
}
*/

// TODO: Load scene file. Meanwhile, hardcode that shit
void loadScene(char* sceneFile, char* modelDir) {
    Scene* scene = Scene::fromFile(sceneFile);
    if (scene == nullptr) {
        exit(1);
    }

    fuckAround {
        scene->load();
    } findOut(std::string e) {
        std::cout << std::string("Unable to load scene: ") + e + "\n";
        exit(1);
    }
}

// Function to update camera position using spherical coordinates
void updateCameraPosition() {
    posX = distance * cos(alpha) * sin(beta);
    posY = distance * sin(alpha);
    posZ = distance * cos(alpha) * cos(beta);
}

void processKeys(unsigned char key, int x, int y) {
    switch (key) {
        case 'w':  // Tilt up
            if (alpha < M_PI / 2 - 0.02f) alpha += 0.01f;
            break;
        case 's':  // Tilt down
            if (alpha > -M_PI / 2 + 0.02f) alpha -= 0.01f;
            break;
        case 'a':  // Rotate left
            beta -= 0.01f;
            break;
        case 'd':  // Rotate right
            beta += 0.01f;
            break;
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
    
    updateCameraPosition();
    glutPostRedisplay();
}

int main(int argc, char** argv) {
    commonTest();

#ifdef DEBUG_MODE
    printf("ENGINE DEBUG MODE.\n");
#endif

    // testingParser();
    loadScene("Scenes/phase1/test_1_1.xml", "");

    // put GLUT init here
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGBA);
    glutInitWindowPosition(100, 100);
    glutInitWindowSize(800, 800);
    glutCreateWindow("CG@DI");

    // int tick = 0;
    // TICK = &tick;

    printInfo();

    // put callback registry here
    glutDisplayFunc(renderScene);
    glutReshapeFunc(changeSize);
    // glutIdleFunc(renderScene);

    glutKeyboardFunc(processKeys);

    // some OpenGL settings
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);
    glClearColor(0.0f, 0.0f, 0.0f, 0.0f);

    // enter GLUT�s main cycle
    glutMainLoop();

    return 1;
}