#include <cmath>
#include "static.h"
#include <iostream>
#include <GL/glut.h>
#include <windows.h>
#include <mmsystem.h>

using namespace std;

void init();
void idle();
void reset();
void display();
void drawSeat();
void drawTyre();
void landmarks();
void drawFrame();
void drawChain();
void drawPedals();
void updateScene();
void setupCallBacks();
void motion(int, int);
void reshape(int, int);
void printInstruction();
GLfloat radToDeg(GLfloat);
GLfloat degToRad(GLfloat);
void mouse(int, int, int, int);
void keyboardKey(int, int, int);
void ZCylinder(GLfloat, GLfloat);
void XCylinder(GLfloat, GLfloat);
GLfloat angleSum(GLfloat, GLfloat);
void keyboard(unsigned char, int, int);
void gear(GLfloat, GLfloat, GLfloat, GLint, GLfloat);

GLenum Mouse;
int mousePrevX, mousePrevY;

GLfloat camPosX, camPosY, camPosZ;
GLfloat pedalAngle, speed, steering;
GLfloat camAngleX, camAngleY, camAngleZ;
GLfloat bicyclePosX, bicyclePosY, bicyclePosZ, bicycleDirection;

int main(int argc, char *argv[]) {

    printInstruction();

    glutInit(&argc, argv);

    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);

    glutInitWindowPosition(WINDOW_POSITION_X, WINDOW_POSITION_Y);
    glutInitWindowSize(WINDOW_WIDTH, WINDOW_HEIGHT);
    glutCreateWindow(WINDOW_NAME);

    init();

    setupCallBacks();

    glClearColor(0.0, 1.0, 1.0, 1.0);

    glutMainLoop();

    return 0;
}

void ZCylinder(GLfloat radius, GLfloat length) {
    GLUquadricObj *cylinder;
    cylinder = gluNewQuadric();
    glPushMatrix();
    glTranslatef(0, 0, 0);
    gluCylinder(cylinder, radius, radius, length, 15, 5);
    glPopMatrix();
}

void XCylinder(GLfloat radius, GLfloat length) {
    glPushMatrix();
    glRotatef(90, 0, 1, 0);
    ZCylinder(radius, length);
    glPopMatrix();
}

void updateScene() {
    GLfloat rotation, xDelta, zDelta, sin_steering, cos_steering;

    if (-INC_SPEED < speed && speed < INC_SPEED) return;
    if (speed < 0) pedalAngle = speed = 0;

    xDelta = speed * cos(degToRad(bicycleDirection + steering));
    zDelta = speed * sin(degToRad(bicycleDirection + steering));
    bicyclePosX += xDelta;
    bicyclePosY -= zDelta;
    pedalAngle = radToDeg(angleSum(degToRad(pedalAngle), speed / WHEEL_RADIUS * 0.5));

    sin_steering = sin(degToRad(steering));
    cos_steering = cos(degToRad(steering));

    rotation = atan2(speed * sin_steering, BICYCLE_LENGTH + speed * cos_steering);
    bicycleDirection = radToDeg(angleSum(degToRad(bicycleDirection), rotation));
}

GLfloat angleSum(GLfloat m, GLfloat n) {
    m += n;
    if (m < 0) return m + 2 * M_PI;
    else if (m > 2 * M_PI) return m - 2 * M_PI;
    else return m;
}

void drawFrame() {
    glColor3f(1, 0, 0);

    glPushMatrix();

    glPushMatrix();

    glColor3f(0, 0, 0);

    glPushMatrix();
    glTranslatef(0, 0, 0.06f);
    glRotatef(-2 * pedalAngle, 0, 0, 1);
    gear(0.08f, 0.3f, 0.03f, 30, 0.03f);
    glPopMatrix();

    glColor3f(1, 1, 1);
    glTranslatef(0, 0, -0.25f);
    ZCylinder(0.08f, 0.47f);
    glPopMatrix();

    glRotatef(RIGHT_ANGLE, 0, 0, 1);
    XCylinder(ROD_RADIUS, RIGHT_ROD);

    glRotatef(MIDDLE_ANGLE - RIGHT_ANGLE, 0, 0, 1);
    XCylinder(ROD_RADIUS, MIDDLE_ROD);

    glColor3f(1, 1, 0);
    glTranslatef(MIDDLE_ROD, 0, 0);
    glRotatef(-MIDDLE_ANGLE, 0, 0, 1);
    glScalef(0.3f, ROD_RADIUS, 0.25f);

    glColor3f(0, 0, 0);
    drawSeat();

    glColor3f(0.439, 0, 0.658);
    glPopMatrix();

    glPushMatrix();
    glRotatef(-180, 0, 1, 0);

    glPushMatrix();
    glTranslatef(-0.05f, 0, WHEEL_OFFSET);
    glRotatef(-4.0f, 0, 1, 0);
    XCylinder(ROD_RADIUS, WHEEL_RADIUS + TUBE_WIDTH + 0.6);
    glPopMatrix();
    glPushMatrix();
    glTranslatef(-0.05f, 0, -WHEEL_OFFSET);
    glRotatef(4.0f, 0, 1, 0);
    XCylinder(ROD_RADIUS, WHEEL_RADIUS + TUBE_WIDTH + 0.6);
    glPopMatrix();
    glPopMatrix();

    glPushMatrix();
    glTranslatef(-(BACK_CONNECTOR + WHEEL_RADIUS + TUBE_WIDTH), 0, 0);

    glPushMatrix();
    glRotatef(-2 * pedalAngle, 0, 0, 1);
    drawTyre();
    gear(0.03f, 0.15f, 0.03f, 20, 0.03f);
    glColor3f(1, 0.64, 0.168);
    glPopMatrix();
    glRotatef(LEFT_ANGLE, 0, 0, 1);

    glPushMatrix();
    glColor3f(0.439, 0, 0.658);
    glTranslatef(0, 0, -WHEEL_OFFSET-0.1);
    glRotatef(-6.5f, 0, 1, 0);
    XCylinder(ROD_RADIUS, WHEEL_LEN+0.7);
    glPopMatrix();
    glPushMatrix();
    glTranslatef(0, 0, WHEEL_OFFSET+0.1);
    glRotatef(6.5f, 0, 1, 0);
    XCylinder(ROD_RADIUS, WHEEL_LEN+0.7);
    glPopMatrix();

    glTranslatef(WHEEL_LEN, 0, 0);

    glTranslatef(CRANK_ROD, 0, 0);
    glRotatef(-LEFT_ANGLE, 0, 0, 1);
    glColor3f(1, 1, 1);
    XCylinder(ROD_RADIUS, TOP_LEN);

    glTranslatef(TOP_LEN, 0, 0);
    glRotatef(-FRONT_INCLINE, 0, 0, 1);

    glPushMatrix();
    glTranslatef(-0.1f, 0, 0);
    XCylinder(ROD_RADIUS, 0.45f);
    glPopMatrix();

    glPushMatrix();
    glRotatef(-steering, 1, 0, 0);

    glTranslatef(-0.3f, 0, 0);

    glPushMatrix();
    glRotatef(FRONT_INCLINE, 0, 0, 1);

    glPushMatrix();
    glTranslatef(0, 0, -HANDLE_ROD / 2);
    glColor3f(0, 0, 0);
    ZCylinder(ROD_RADIUS, HANDLE_ROD);
    glPopMatrix();

    glPushMatrix();
    glColor3f(1, 0, 0);
    glTranslatef(0, 0, -HANDLE_ROD / 2);
    ZCylinder(0.07f, HANDLE_ROD / 4);

    glColor3f(0, 0, 0);

    glPushMatrix();
    glTranslatef(0.07,0,0.47);
    glutSolidCube(0.1);
    glTranslatef(0.07,0,-0.37);
    ZCylinder(0.02, HANDLE_ROD / 4);
    glPopMatrix();

    glColor3f(1, 0, 0);

    glTranslatef(0, 0, HANDLE_ROD * 3 / 4);
    ZCylinder(0.07f, HANDLE_ROD / 4);
    glColor3f(0, 0, 0);

    glPushMatrix();
    glTranslatef(0.07,0,-0.05);
    glutSolidCube(0.1);
    glTranslatef(0.07,0,-0.05);
    ZCylinder(0.02, HANDLE_ROD / 4);
    glPopMatrix();

    glPopMatrix();
    glPopMatrix();

    glPushMatrix();

    XCylinder(ROD_RADIUS, CRANK_ROD);

    glTranslatef(CRANK_ROD, 0, 0);
    glRotatef(CRANK_ANGLE, 0, 0, 1);

    glPushMatrix();
    glColor3f(0.439, 0, 0.658);
    glTranslatef(0, 0, WHEEL_OFFSET);
    glRotatef(-5.0f, 0, 1, 0);
    XCylinder(ROD_RADIUS, CRANK_RODS);
    glPopMatrix();
    glPushMatrix();
    glTranslatef(0, 0, -WHEEL_OFFSET);
    glRotatef(5.0f, 0, 1, 0);
    XCylinder(ROD_RADIUS, CRANK_RODS);
    glPopMatrix();

    glPushMatrix();
    glRotatef(90, 0, 1, 0);
    glTranslatef(-0.15f, 0, 0);
    XCylinder(ROD_RADIUS, 0.3);
    glPopMatrix();

    glTranslatef(CRANK_RODS, 0, 0);
    glRotatef(-2 * pedalAngle, 0, 0, 1);
    drawTyre();
    glPopMatrix();
    glPopMatrix();
    glPopMatrix();
}

void gear(GLfloat inner_radius, GLfloat outer_radius, GLfloat width, GLint teeth, GLfloat tooth_depth) {

    GLint i;
    GLfloat r0, r1, r2;
    GLfloat angle, da;
    GLfloat u, v, len;

    r0 = inner_radius;
    r1 = outer_radius - tooth_depth / 2.0;
    r2 = outer_radius + tooth_depth / 2.0;

    da = 2.0 * M_PI / teeth / 4.0;

    glTranslatef(0, 0, 0.12f);
    glColor3f(0, 0, 0);

    glShadeModel(GL_FLAT);

    glNormal3f(0.0, 0.0, 1.0);

    glBegin(GL_QUAD_STRIP);
    for (i = 0; i <= teeth; i++) {
        angle = i * 2.0 * M_PI / teeth;
        glVertex3f(r0 * cos(angle), r0 * sin(angle), width * 0.5);
        glVertex3f(r1 * cos(angle), r1 * sin(angle), width * 0.5);
        glVertex3f(r0 * cos(angle), r0 * sin(angle), width * 0.5);
        glVertex3f(r1 * cos(angle + 3 * da), r1 * sin(angle + 3 * da), width * 0.5);
    }
    glEnd();

    glBegin(GL_QUADS);
    da = 2.0 * M_PI / teeth / 4.0;
    for (i = 0; i < teeth; i++) {
        angle = i * 2.0 * M_PI / teeth;

        glVertex3f(r1 * cos(angle), r1 * sin(angle), width * 0.5);
        glVertex3f(r2 * cos(angle + da), r2 * sin(angle + da), width * 0.5);
        glVertex3f(r2 * cos(angle + 2 * da), r2 * sin(angle + 2 * da), width * 0.5);
        glVertex3f(r1 * cos(angle + 3 * da), r1 * sin(angle + 3 * da), width * 0.5);
    }
    glEnd();


    glNormal3f(0.0, 0.0, -1.0);

    glBegin(GL_QUAD_STRIP);
    for (i = 0; i <= teeth; i++) {
        angle = i * 2.0 * M_PI / teeth;
        glVertex3f(r1 * cos(angle), r1 * sin(angle), -width * 0.5);
        glVertex3f(r0 * cos(angle), r0 * sin(angle), -width * 0.5);
        glVertex3f(r1 * cos(angle + 3 * da), r1 * sin(angle + 3 * da), -width * 0.5);
        glVertex3f(r0 * cos(angle), r0 * sin(angle), -width * 0.5);
    }
    glEnd();

    glBegin(GL_QUADS);
    da = 2.0 * M_PI / teeth / 4.0;
    for (i = 0; i < teeth; i++) {
        angle = i * 2.0 * M_PI / teeth;

        glVertex3f(r1 * cos(angle + 3 * da), r1 * sin(angle + 3 * da), -width * 0.5);
        glVertex3f(r2 * cos(angle + 2 * da), r2 * sin(angle + 2 * da), -width * 0.5);
        glVertex3f(r2 * cos(angle + da), r2 * sin(angle + da), -width * 0.5);
        glVertex3f(r1 * cos(angle), r1 * sin(angle), -width * 0.5);
    }
    glEnd();

    glBegin(GL_QUAD_STRIP);
    for (i = 0; i < teeth; i++) {
        angle = i * 2.0 * M_PI / teeth;

        glVertex3f(r1 * cos(angle), r1 * sin(angle), width * 0.5);
        glVertex3f(r1 * cos(angle), r1 * sin(angle), -width * 0.5);
        u = r2 * cos(angle + da) - r1 * cos(angle);
        v = r2 * sin(angle + da) - r1 * sin(angle);
        len = sqrt(u * u + v * v);
        u /= len;
        v /= len;
        glNormal3f(v, -u, 0.0);
        glVertex3f(r2 * cos(angle + da), r2 * sin(angle + da), width * 0.5);
        glVertex3f(r2 * cos(angle + da), r2 * sin(angle + da), -width * 0.5);
        glNormal3f(cos(angle), sin(angle), 0.0);
        glVertex3f(r2 * cos(angle + 2 * da), r2 * sin(angle + 2 * da), width * 0.5);
        glVertex3f(r2 * cos(angle + 2 * da), r2 * sin(angle + 2 * da), -width * 0.5);
        u = r1 * cos(angle + 3 * da) - r2 * cos(angle + 2 * da);
        v = r1 * sin(angle + 3 * da) - r2 * sin(angle + 2 * da);
        glNormal3f(v, -u, 0.0);
        glVertex3f(r1 * cos(angle + 3 * da), r1 * sin(angle + 3 * da), width * 0.5);
        glVertex3f(r1 * cos(angle + 3 * da), r1 * sin(angle + 3 * da), -width * 0.5);
        glNormal3f(cos(angle), sin(angle), 0.0);
    }

    glVertex3f(r1 * cos(0.0), r1 * sin(0.0), width * 0.5);
    glVertex3f(r1 * cos(0.0), r1 * sin(0.0), -width * 0.5);

    glEnd();


    glShadeModel(GL_SMOOTH);

    glBegin(GL_QUAD_STRIP);
    for (i = 0; i <= teeth; i++) {
        angle = i * 2.0 * M_PI / teeth;
        glNormal3f(-cos(angle), -sin(angle), 0.0);
        glVertex3f(r0 * cos(angle), r0 * sin(angle), -width * 0.5);
        glVertex3f(r0 * cos(angle), r0 * sin(angle), width * 0.5);
    }
    glEnd();
}

void drawChain() {
    GLfloat depth;
    static int mode = 0;

    glColor3f(0, 0, 0);
    glTranslatef(0, 0, 0.1f);
    glEnable(GL_LINE);
    mode = (mode + 1) % 2;

    if (mode == 0 && speed > 0) glLineStipple(1, 0x1c47);
    else if (mode == 1 && speed > 0) glLineStipple(1, 0x00FF);

    glBegin(GL_LINES);
    for (depth = 0.06f; depth <= 0.12f; depth += 0.01f) {
        glVertex3f(-1.6f, 0.15f, ROD_RADIUS);
        glVertex3f(0, 0.3f, depth);

        glVertex3f(-1.6f, -0.15f, ROD_RADIUS);
        glVertex3f(0, -0.3f, depth);
    }
    glEnd();
    glDisable(GL_LINE_STIPPLE);
    glTranslatef(0, 0, -0.1f);
}

void drawSeat() {

    glBegin(GL_POLYGON);
    glVertex3f(-0.1f, 1, -0.5f);
    glVertex3f(1, 1, -0.3f);
    glVertex3f(1.1f, 1, -0.1f);
    glVertex3f(1.1f, 1, 0.1f);
    glVertex3f(1, 1, 0.3f);
    glVertex3f(-0.1f, 1, 0.5f);
    glVertex3f(-0.5f, 1, 1);
    glVertex3f(-0.6f, 1, 1.1f);
    glVertex3f(-0.7f, 1, 1.2f);
    glVertex3f(-0.8f, 1, 1.2f);
    glVertex3f(-0.9f, 1, 1.1f);
    glVertex3f(-1, 1, 1);
    glVertex3f(-1, 1, -1);
    glVertex3f(-0.9f, 1, -1.1f);
    glVertex3f(-0.8f, 1, -1.2f);
    glVertex3f(-0.7f, 1, -1.2f);
    glVertex3f(-0.6f, 1, -1.1f);
    glVertex3f(-0.5f, 1, -1);
    glEnd();

    glBegin(GL_POLYGON);
    glVertex3f(-0.1f, -1, -0.5f);
    glVertex3f(1, -1, -0.3f);
    glVertex3f(1.1f, -1, -0.1f);
    glVertex3f(1.1f, -1, 0.1f);
    glVertex3f(1, -1, 0.3f);
    glVertex3f(-0.1f, -1, 0.5f);
    glVertex3f(-0.5f, -1, 1);
    glVertex3f(-0.6f, -1, 1.1f);
    glVertex3f(-0.7f, -1, 1.2f);
    glVertex3f(-0.8f, -1, 1.2f);
    glVertex3f(-0.9f, -1, 1.1f);
    glVertex3f(-1, -1, 1);
    glVertex3f(-1, -1, -1);
    glVertex3f(-0.9f, -1, -1.1f);
    glVertex3f(-0.8f, -1, -1.2f);
    glVertex3f(-0.7f, -1, -1.2f);
    glVertex3f(-0.6f, -1, -1.1f);
    glVertex3f(-0.5f, -1, -1);
    glEnd();

    glBegin(GL_QUADS);
    glVertex3f(1, 1, -0.3f);
    glVertex3f(1, 1, 0.3f);
    glVertex3f(1, -1, 0.3f);
    glVertex3f(1, -1, -0.3f);

    glVertex3f(1, 1, 0.3f);
    glVertex3f(-0.1f, 1, 0.5f);
    glVertex3f(-0.1f, -1, 0.5f);
    glVertex3f(1, -1, 0.3f);

    glVertex3f(1, 1, -0.3f);
    glVertex3f(-0.1f, 1, -0.5f);
    glVertex3f(-0.1f, -1, -0.5f);
    glVertex3f(1, -1, -0.3f);

    glVertex3f(-0.1f, 1, 0.5f);
    glVertex3f(-0.5f, 1, 1);
    glVertex3f(-0.5f, -1, 1);
    glVertex3f(-0.1f, -1, 0.5f);

    glVertex3f(-0.1f, 1, -0.5f);
    glVertex3f(-0.5f, 1, -1);
    glVertex3f(-0.5f, -1, -1);
    glVertex3f(-0.1f, -1, -0.5f);

    glVertex3f(-0.5f, 1, 1);
    glVertex3f(-1, 1, 1);
    glVertex3f(-1, -1, 1);
    glVertex3f(-0.5f, -1, 1);

    glVertex3f(-0.5f, 1, -1);
    glVertex3f(-1, 1, -1);
    glVertex3f(-1, -1, -1);
    glVertex3f(-0.5f, -1, -1);

    glVertex3f(-1, 1, 1);
    glVertex3f(-1, 1, -1);
    glVertex3f(-1, -1, -1);
    glVertex3f(-1, -1, 1);

    glEnd();
}

void drawPedals() {
    glColor3f(0, 0, 0);

    glPushMatrix();
    glTranslatef(0, 0, 0.105f);
    glRotatef(-pedalAngle, 0, 0, 1);
    glTranslatef(0.25f, 0, 0.15f);

    glPushMatrix();
    glScalef(0.5f, 0.05f, 0.1f);
    glutSolidCube(1);
    glPopMatrix();

    glPushMatrix();
    glTranslatef(0.25f, 0, 0.15f);
    glRotatef(pedalAngle, 0, 0, 1);
    glScalef(0.2f, 0.08f, 0.3f);
    glutSolidCube(1);
    glPopMatrix();

    glPopMatrix();

    glPushMatrix();
    glTranslatef(0, 0, -0.23f);
    glRotatef(180 - pedalAngle, 0, 0, 1);
    glTranslatef(0.25f, 0, 0);

    glPushMatrix();
    glScalef(0.5f, 0.05f, 0.1f);
    glutSolidCube(1);
    glPopMatrix();

    glPushMatrix();
    glTranslatef(0.25f, 0, -0.15f);
    glRotatef(pedalAngle - 180, 0, 0, 1);
    glScalef(0.2f, 0.08f, 0.3f);
    glutSolidCube(1);
    glPopMatrix();

    glPopMatrix();

    glColor3f(1, 0, 0);
}

void drawTyre() {

    glColor3f(0, 0, 0);
    glutSolidTorus(0.06f, 0.92f, 4, 30);

    glColor3f(0, 0, 0);
    glPushMatrix();
    glTranslatef(0, 0, -0.25f);
    ZCylinder(0.08f, 0.5f);
    glPopMatrix();
    glutSolidTorus(0.08f, 0.05f, 3, 20);

    glColor3f(0, 0, 1);
    for (int i = 0; i < SPOKES_COUNT; ++i) {
        glPushMatrix();
        glRotatef(i * SPOKE_ANGLE, 0, 0, 1);
        glColor3f(0, 0, 0);
        glLineWidth(2.0f);
        glBegin(GL_LINES);
        glVertex3f(0, 0.02f, 0);
        glVertex3f(0, 0.86f, 0);
        glEnd();
        glPopMatrix();
    }

    glColor3f(0, 0, 0);
    glutSolidTorus(TUBE_WIDTH, WHEEL_RADIUS, 10, 30);
    glColor3f(1, 0, 0);
}

void init() {
    GLfloat light[] = {1.0, 1.0, 1.0};
    GLfloat mat_shininess[] = {100.0};
    GLfloat mat_specular[] = {1.0, 1.0, 1.0, 1.0};
    GLfloat light_directional[] = {1.0, 1.0, 1.0, 1.0};

    reset();

    glShadeModel(GL_SMOOTH);

    glLightfv(GL_LIGHT5, GL_AMBIENT, light);
    glLightfv(GL_LIGHT5, GL_DIFFUSE, light);
    glLightfv(GL_LIGHT5, GL_SPECULAR, light);
    glLightfv(GL_LIGHT5, GL_POSITION, light_directional);
    glMaterialfv(GL_FRONT, GL_SHININESS, mat_shininess);
    glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular);
    glColorMaterial(GL_FRONT, GL_DIFFUSE);

    glEnable(GL_LIGHT5);
    glEnable(GL_LIGHTING);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_COLOR_MATERIAL);
}

void landmarks() {
    glDisable(GL_LIGHTING);

    glColor3f(0, 1, 0);

    glBegin(GL_QUADS);
    glVertex3f(100, -WHEEL_RADIUS, 100);
    glVertex3f(-100, -WHEEL_RADIUS, 100);
    glVertex3f(-100, -WHEEL_RADIUS, -100);
    glVertex3f(100, -WHEEL_RADIUS, -100);
    glEnd();

    glPushMatrix();
    glColor3f(1,1,0);
    GLUquadric *quad;
    quad = gluNewQuadric();
    glTranslatef(20,20,20);
    gluSphere(quad,1,100,20);
    glPopMatrix();

    /*glPushMatrix();
    glColor3f(0.6,0.4,0.2);
    GLUquadric *quad1;
    quad1 = gluNewQuadric();
    glTranslatef(-50,-20,20);
    gluSphere(quad1,30,100,20);
    glPopMatrix();*/

    /*glPushMatrix();
    glColor3f(0.6,0.4,0.2);
    glTranslatef(10,0,10);
    glScaled(5,5,5);

    glBegin(GL_QUADS);

    glVertex3f(1, 0, 1);
    glVertex3f(1, 0, -1);
    glVertex3f(-1, 0, -1);
    glVertex3f(-1, 0, 1);

    glVertex3f(1, 1, 1);
    glVertex3f(1, 0, -1);
    glVertex3f(-1, 0, -1);
    glVertex3f(-1, 1, 1);

    glVertex3f(1, 0, 1);
    glVertex3f(1, 1, 1);
    glVertex3f(-1, 0, 1);
    glVertex3f(-1, 1, 1);

    glEnd();
    glPopMatrix();*/

    glEnable(GL_LIGHTING);
}

void display() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glEnable(GL_NORMALIZE);

    glPushMatrix();

    glRotatef(camAngleY, 1, 0, 0);
    glRotatef(camAngleX, 0, 1, 0);
    glRotatef(camAngleZ, 0, 0, 1);

    landmarks();

    glPushMatrix();
    glTranslatef(bicyclePosX, bicyclePosZ, bicyclePosY);
    glRotatef(bicycleDirection, 0, 1, 0);

    drawFrame();
    drawChain();
    drawPedals();
    glPopMatrix();

    glPopMatrix();

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    gluLookAt(camPosX, camPosY, camPosZ, camPosX, camPosY, 0.0, 0.0, 1.0, 0.0);

    glutSwapBuffers();
}

GLfloat radToDeg(GLfloat rad) {
    return rad * 180 / M_PI;
}

GLfloat degToRad(GLfloat deg) {
    return deg * M_PI / 180;
}

void idle() {
    updateScene();
    glutPostRedisplay();
}

void keyboardKey(int key, int x, int y) {
    switch (key) {
        case GLUT_KEY_UP:
            camPosZ -= 0.1f;
            break;
        case GLUT_KEY_DOWN:
            camPosZ += 0.1f;
            break;
        case GLUT_KEY_LEFT:
            camPosX -= 0.1f;
            break;
        case GLUT_KEY_RIGHT:
            camPosX += 0.1f;
            break;
        case GLUT_KEY_PAGE_UP:
            camPosY += 0.1f;
            break;
        case GLUT_KEY_PAGE_DOWN:
            camPosY -= 0.1f;
            if (camPosY <= 0) camPosY = 0;
            break;
    }
    glutPostRedisplay();
}

void reset() {
    camAngleX = 0;
    camAngleY = 0;
    camAngleZ = 0;
    pedalAngle = 0;
    steering = 0;
    Mouse = GLUT_UP;
    pedalAngle = 0;
    speed = 0;
    steering = 0;
    camPosX = 0;
    camPosY = 0;
    camPosZ = 5.0f;
    bicyclePosX = 0;
    bicyclePosY = 0;
    bicyclePosZ = 0;
    bicycleDirection = 0;
}

void keyboard(unsigned char key, int x, int y) {
    switch (key) {
        case 'r':
        case 'R':
            reset();
            break;
        case 'a':
        case 'A':
            if (steering < HANDLE_LIMIT)
                steering += INC_STEERING;
            break;
        case 'd':
        case 'D':
            if (steering > -HANDLE_LIMIT)
                steering -= INC_STEERING;
            break;
        case 'w':
        case 'W':
            speed += INC_SPEED;
            break;
        case 's':
        case 'S':
            speed -= INC_SPEED;
            break;
        case 'h':
        case 'H':
            PlaySound(TEXT("../Resource/ring.wav"), NULL, SND_FILENAME);
            break;
        case 27:
            exit(1);
    }

    pedalAngle += speed;
    if (speed < 0) speed = 0;
    if (pedalAngle < 0) pedalAngle = 0;
    if (pedalAngle >= 360) pedalAngle -= 360;

    glutPostRedisplay();
}

void mouse(int button, int state, int x, int y) {
    if (button == GLUT_LEFT_BUTTON){
        if (state == GLUT_UP) Mouse = GLUT_UP;
        if (state == GLUT_DOWN) {
            Mouse = GLUT_DOWN;
            mousePrevX = x;
            mousePrevY = y;
        }
    }
    glutPostRedisplay();
}

void motion(int x, int y) {
    if (Mouse == GLUT_DOWN) {
        int deltaX, deltaY;
        deltaX = mousePrevX - x;
        deltaY = mousePrevY - y;
        camAngleX += 0.5 * deltaX;
        camAngleY += 0.5 * deltaY;
        if (camAngleY <= 0) camAngleY = 0;
        if (camAngleY >= 180) camAngleY = 180;
    } else {
        Mouse = GLUT_UP;
    }
    mousePrevX = x;
    mousePrevY = y;
    glutPostRedisplay();
}

void reshape(int w, int h) {
    glViewport(0, 0, (GLsizei) w, (GLsizei) h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(100.0, (GLfloat) w / (GLfloat) h, 0.1, 100.0);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    gluLookAt(camPosX, camPosY, camPosZ, camPosX, camPosY, 0.0, 0.0, 1.0, 0.0);
}

void setupCallBacks() {
    glutDisplayFunc(display);
    glutReshapeFunc(reshape);
    glutIdleFunc(idle);
    glutSpecialFunc(keyboardKey);
    glutKeyboardFunc(keyboard);
    glutMouseFunc(mouse);
    glutMotionFunc(motion);
    glutSetCursor(GLUT_CURSOR_CROSSHAIR);
}

void printInstruction() {
    cout << "CG-2022" << endl;
    cout << "Seyed Mohammad Amin Atyabi" << endl;
    cout << "810198559" << endl;
    cout << "'A' to increase the speed" << endl;
    cout << "'S' to decrease the speed" << endl;
    cout << "'D' to turn right" << endl;
    cout << "'A' to turn left" << endl;
    cout << "'R' to reset the scene" << endl;
    cout << "Arrow keys to move the camera" << endl;
    cout << "Mouse to move the scene" << endl;
}
