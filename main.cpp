#include <iostream>
#include <GL/glut.h>

#define WINDOW_HEIGHT 500
#define WINDOW_WIDTH 500
#define WINDOW_POSITION_X 650
#define WINDOW_POSITION_Y 250
#define WINDOW_NAME "Bicycle"

using namespace std;

void displayCB();

int main(int argc, char *argv[]) {

    glutInit(&argc, argv);

    glutInitDisplayMode(GLUT_RGB);

    glutInitWindowPosition(WINDOW_POSITION_X, WINDOW_POSITION_Y);
    glutInitWindowSize(WINDOW_WIDTH, WINDOW_HEIGHT);

    glutCreateWindow(WINDOW_NAME);

    glutDisplayFunc(displayCB);

    glutMainLoop();

    return 0;
}

void displayCB() {
    glClear(GL_COLOR_BUFFER_BIT);
    glLoadIdentity();

    //TODO Do some stuff here

    glFlush();
}
