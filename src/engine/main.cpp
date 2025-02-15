// #include <stdio.h>
// #include "common/common.h"

// int main(int argc, char** argv) {
//     commonTest();
//     printf("Hello from ENGINE.\n");
// }


#include "common/common.h"
#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif

#define _USE_MATH_DEFINES
#include <math.h>
#include <stdio.h>

#define MAX_TICK 10000000

int TICK;

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
	// gluLookAt(
	// 	5.0f, 5.0f, 5.0f,
	// 	0.0f, 0.0f, -1.0f,
	// 	0.0f, 1.0f, 0.0f
	// );

	double dtick = TICK;
	double factor = 20;
	double multFactor = 5;
	double camX = cos(dtick / factor) * multFactor;
	double camY = cos(dtick / factor) * multFactor;
	double camZ = sin(dtick / factor) * multFactor;
	// double camX = 1, camZ = 0;
	printf("CAM: %f %f\n", camX, camZ);

	gluLookAt(
		camX, camY, camZ,
		0.0f, 0.0f, 0.0f,
		0.0f, 1.0f, 0.0f
	);
		
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
	glutWireTeapot(abs(sin(mult)) + 0.5);
	// glutWireTeapot(1);
	
	// End of frame
	glutSwapBuffers();
}


void printInfo() {
	printf("Vendor: %s\n", glGetString(GL_VENDOR));
	printf("Renderer: %s\n", glGetString(GL_RENDERER));
	printf("Version: %s\n", glGetString(GL_VERSION));
}


int main(int argc, char** argv) {
    commonTest();

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
	glutIdleFunc(renderScene);

	
	// some OpenGL settings
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
	
	// enter GLUT�s main cycle
	glutMainLoop();
	
	return 1;
}