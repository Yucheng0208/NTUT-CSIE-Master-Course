// magical_scene_project.cpp
// OpenGL + FreeGLUT + OpenCV inspired by fantasy scene

#include <GL/freeglut.h>
#include <opencv2/opencv.hpp>
#include <iostream>
#include <cmath>

float angleA = 0.0f;
float angleB = 0.0f;
float timeStep = 0.0f;

float objectBX = 0.0f, objectBY = 0.0f;
cv::Mat textureImg;
GLuint earthTexture;

GLuint loadTexture(const char* filename) {
    cv::Mat image = cv::imread(filename);
    if (image.empty()) {
        std::cerr << "Failed to load texture: " << filename << std::endl;
        return 0;
    }
    cv::cvtColor(image, image, cv::COLOR_BGR2RGB);
    GLuint texID;
    glGenTextures(1, &texID);
    glBindTexture(GL_TEXTURE_2D, texID);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, image.cols, image.rows, 0, GL_RGB, GL_UNSIGNED_BYTE, image.ptr());
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    return texID;
}

void setup() {
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);
    glEnable(GL_COLOR_MATERIAL);
    glEnable(GL_TEXTURE_2D);
    glShadeModel(GL_SMOOTH);
    glClearColor(0.0f, 0.0f, 0.05f, 1.0f);

    earthTexture = loadTexture("earth.jpg"); // 使用地球貼圖
}

void update(int value) {
    angleA += 2.0f;
    angleB += 1.0f;
    timeStep += 0.05f;

    objectBX = cos(timeStep) * 3.0f;
    objectBY = sin(timeStep) * 0.5f;

    glutPostRedisplay();
    glutTimerFunc(16, update, 0);
}

void drawTexturedSphere() {
    GLUquadric* quad = gluNewQuadric();
    gluQuadricTexture(quad, GL_TRUE);
    glBindTexture(GL_TEXTURE_2D, earthTexture);
    gluSphere(quad, 0.5, 30, 30);
    gluDeleteQuadric(quad);
}

void drawObjectA() {
    glPushMatrix();
    glRotatef(angleA, 0, 1, 0);
    glTranslatef(0.0, 0.0, 0.0);
    glColor3f(1.0, 1.0, 0.0); // 黃綠核心球
    glutSolidSphere(0.2, 20, 20);
    glPopMatrix();
}

void drawObjectB() {
    glPushMatrix();
    glRotatef(angleB, 0, 1, 0);
    glTranslatef(objectBX, objectBY, 0.0);
    drawTexturedSphere(); // 地球紋理的球體
    glPopMatrix();
}

void drawObjectC() {
    glPushMatrix();
    glTranslatef(2.0, sin(timeStep * 2.0f) * 0.5f, 0.0);
    glColor3f(1.0, 0.0, 0.0);
    glutSolidTeapot(0.4);
    glPopMatrix();
}

void drawPlatform() {
    glPushMatrix();
    glTranslatef(0.0, -0.3, 0.0);
    glColor3f(0.3, 0.25, 0.2);
    glutSolidTorus(0.1, 4.0, 30, 50);
    glPopMatrix();
}

void display() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glLoadIdentity();
    gluLookAt(6, 6, 6, 0, 0, 0, 0, 1, 0);

    drawPlatform();
    drawObjectA();
    drawObjectB();
    drawObjectC();

    glutSwapBuffers();
}

void reshape(int w, int h) {
    glViewport(0, 0, w, h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(45.0, (float)w/h, 1.0, 100.0);
    glMatrixMode(GL_MODELVIEW);
}

int main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
    glutInitWindowSize(1000, 700);
    glutCreateWindow("Fantasy Scene - OpenCV + FreeGLUT");
    setup();
    glutDisplayFunc(display);
    glutReshapeFunc(reshape);
    glutTimerFunc(0, update, 0);
    glutMainLoop();
    return 0;
}
