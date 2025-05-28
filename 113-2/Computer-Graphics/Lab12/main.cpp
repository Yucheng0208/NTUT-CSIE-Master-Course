// icosahedron_subdivision.cpp
// Author: Yu-Cheng Chang (Modified by AI)
// Requirement: C++ with FreeGLUT + OpenCV
#include <iostream>
#include <string> // For std::string
#include <math.h>
#include <stdlib.h>

// For getcwd (debugging paths)
#ifdef _WIN32
#include <direct.h>
#define GetCurrentDir _getcwd
#else
#include <unistd.h>
#define GetCurrentDir getcwd
#endif

// Standard OpenGL and GLUT headers
#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/freeglut.h>
#endif
// glext.h might not be needed if we convert to GL_RGB/GL_RGBA
// #include <GL/glext.h> // For extension definitions like GL_BGR_EXT

// OpenCV 4 Headers (though not directly used in this rendering logic)
#include <opencv2/core.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/imgproc.hpp>

#define X .525731112119133606
#define Z .850650808352039932

static GLfloat vdata[12][3] = {
    {-X, 0.0, Z}, {X, 0.0, Z}, {-X, 0.0, -Z}, {X, 0.0, -Z},
    {0.0, Z, X}, {0.0, Z, -X}, {0.0, -Z, X}, {0.0, -Z, -X},
    {Z, X, 0.0}, {-Z, X, 0.0}, {Z, -X, 0.0}, {-Z, -X, 0.0}
};

static GLuint tindices[20][3] = {
    {1,4,0}, {4,9,0}, {4,9,5}, {8,5,4}, {1,8,4},
    {1,10,8}, {10,3,8}, {8,3,5}, {3,2,5}, {3,7,2},
    {3,10,7}, {10,6,7}, {6,11,7}, {6,0,11}, {6,1,0},
    {10,1,6}, {11,0,9}, {2,11,9}, {5,2,9}, {11,2,7}
};

// Global variables for control
int subdivisionDepth = 0;
GLenum polygonMode = GL_FILL;
float rotationX = 0.0f;
float rotationY = 0.0f;

// Window dimensions
int windowWidth = 900;
int windowHeight = 300; // Three viewports side-by-side

// --- Utility Functions ---

// Normalize a 3D vector
void normalize(GLfloat v[3]) {
    GLfloat len = sqrt(v[0] * v[0] + v[1] * v[1] + v[2] * v[2]);
    v[0] /= len;
    v[1] /= len;
    v[2] /= len;
}

// Calculate cross product of two vectors (v1 x v2) and store in result
void cross(GLfloat v1[3], GLfloat v2[3], GLfloat result[3]) {
    result[0] = v1[1] * v2[2] - v1[2] * v2[1];
    result[1] = v1[2] * v2[0] - v1[0] * v2[2];
    result[2] = v1[0] * v2[1] - v1[1] * v2[0];
}

// Calculate the normal of a triangle defined by three vertices
void calculateFaceNormal(GLfloat p1[3], GLfloat p2[3], GLfloat p3[3], GLfloat normal[3]) {
    GLfloat v_a[3] = {p2[0] - p1[0], p2[1] - p1[1], p2[2] - p1[2]};
    GLfloat v_b[3] = {p3[0] - p1[0], p3[1] - p1[1], p3[2] - p1[2]};
    cross(v_a, v_b, normal);
    normalize(normal);
}

// --- Drawing Functions ---

// Function to draw a single triangle with flat shading
void drawTriangleFlat(GLfloat v1[3], GLfloat v2[3], GLfloat v3[3]) {
    GLfloat normal[3];
    calculateFaceNormal(v1, v2, v3, normal);
    glNormal3fv(normal); // One normal per face for flat shading

    glBegin(GL_TRIANGLES);
        glVertex3fv(v1);
        glVertex3fv(v2);
        glVertex3fv(v3);
    glEnd();
}

// Function to draw a single triangle with interpolated shading (requires vertex normals)
void drawTriangleInterpolated(GLfloat v1[3], GLfloat v2[3], GLfloat v3[3]) {
    glBegin(GL_TRIANGLES);
        glNormal3fv(v1); // Assuming vertex position is also the normal for a sphere
        glVertex3fv(v1);
        glNormal3fv(v2);
        glVertex3fv(v2);
        glNormal3fv(v3);
        glVertex3fv(v3);
    glEnd();
}

// Recursive subdivision function
void subdivideTriangle(GLfloat v1[3], GLfloat v2[3], GLfloat v3[3], int depth) {
    if (depth == 0) {
        // Draw the current triangle
        glNormal3fv(v1); glVertex3fv(v1);
        glNormal3fv(v2); glVertex3fv(v2);
        glNormal3fv(v3); glVertex3fv(v3);
        return;
    }

    GLfloat v12[3], v23[3], v31[3]; // Midpoints

    // Calculate midpoints and normalize them to lie on the sphere
    for (int i = 0; i < 3; ++i) {
        v12[i] = (v1[i] + v2[i]) / 2.0f;
        v23[i] = (v2[i] + v3[i]) / 2.0f;
        v31[i] = (v3[i] + v1[i]) / 2.0f;
    }
    normalize(v12);
    normalize(v23);
    normalize(v31);

    // Recursively subdivide the four new triangles
    subdivideTriangle(v1, v12, v31, depth - 1);
    subdivideTriangle(v12, v2, v23, depth - 1);
    subdivideTriangle(v31, v23, v3, depth - 1);
    subdivideTriangle(v12, v23, v31, depth - 1);
}

// --- Display Functions for each mode ---

void drawFlatMode() {
    glShadeModel(GL_FLAT);
    glPolygonMode(GL_FRONT_AND_BACK, polygonMode);

    for (int i = 0; i < 20; ++i) {
        drawTriangleFlat(
            vdata[tindices[i][0]],
            vdata[tindices[i][1]],
            vdata[tindices[i][2]]
        );
    }
}

void drawInterpolateMode() {
    glShadeModel(GL_SMOOTH);
    glPolygonMode(GL_FRONT_AND_BACK, polygonMode);

    for (int i = 0; i < 20; ++i) {
        drawTriangleInterpolated(
            vdata[tindices[i][0]],
            vdata[tindices[i][1]],
            vdata[tindices[i][2]]
        );
    }
}

void drawSubdivideMode() {
    glShadeModel(GL_SMOOTH); // Subdivision typically uses smooth shading
    glPolygonMode(GL_FRONT_AND_BACK, polygonMode);

    glBegin(GL_TRIANGLES);
    for (int i = 0; i < 20; ++i) {
        subdivideTriangle(
            vdata[tindices[i][0]],
            vdata[tindices[i][1]],
            vdata[tindices[i][2]],
            subdivisionDepth
        );
    }
    glEnd();
}

// --- OpenGL Callbacks ---

void initGL() {
    glClearColor(0.2f, 0.2f, 0.2f, 1.0f); // Dark background
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);
    glEnable(GL_COLOR_MATERIAL); // Allow glColor to set material properties

    // Set up light properties
    GLfloat light_position[] = {1.0f, 1.0f, 1.0f, 0.0f}; // Directional light
    GLfloat light_ambient[] = {0.2f, 0.2f, 0.2f, 1.0f};
    GLfloat light_diffuse[] = {1.0f, 1.0f, 1.0f, 1.0f};
    GLfloat light_specular[] = {1.0f, 1.0f, 1.0f, 1.0f};

    glLightfv(GL_LIGHT0, GL_POSITION, light_position);
    glLightfv(GL_LIGHT0, GL_AMBIENT, light_ambient);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, light_diffuse);
    glLightfv(GL_LIGHT0, GL_SPECULAR, light_specular);

    // Set material properties (can be overridden by glColor when GL_COLOR_MATERIAL is enabled)
    GLfloat material_ambient[] = {0.0f, 0.5f, 0.8f, 1.0f}; // Blueish ambient
    GLfloat material_diffuse[] = {0.0f, 0.5f, 0.8f, 1.0f}; // Blueish diffuse
    GLfloat material_specular[] = {1.0f, 1.0f, 1.0f, 1.0f};
    GLfloat material_shininess[] = {50.0f};

    glMaterialfv(GL_FRONT, GL_AMBIENT, material_ambient);
    glMaterialfv(GL_FRONT, GL_DIFFUSE, material_diffuse);
    glMaterialfv(GL_FRONT, GL_SPECULAR, material_specular);
    glMaterialfv(GL_FRONT, GL_SHININESS, material_shininess);

    glColor3f(0.0f, 0.5f, 0.8f); // Default color for the object
}

void display() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // Viewport 1: Flat Shading
    glViewport(0, 0, windowWidth / 3, windowHeight);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(45.0, (double)(windowWidth / 3) / windowHeight, 0.1, 100.0);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    gluLookAt(0.0, 0.0, 3.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0);
    glRotatef(rotationX, 1.0f, 0.0f, 0.0f);
    glRotatef(rotationY, 0.0f, 1.0f, 0.0f);
    drawFlatMode();

    // Viewport 2: Interpolated Shading
    glViewport(windowWidth / 3, 0, windowWidth / 3, windowHeight);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(45.0, (double)(windowWidth / 3) / windowHeight, 0.1, 100.0);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    gluLookAt(0.0, 0.0, 3.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0);
    glRotatef(rotationX, 1.0f, 0.0f, 0.0f);
    glRotatef(rotationY, 0.0f, 1.0f, 0.0f);
    drawInterpolateMode();

    // Viewport 3: Subdivided Shading
    glViewport(2 * windowWidth / 3, 0, windowWidth / 3, windowHeight);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(45.0, (double)(windowWidth / 3) / windowHeight, 0.1, 100.0);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    gluLookAt(0.0, 0.0, 3.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0);
    glRotatef(rotationX, 1.0f, 0.0f, 0.0f);
    glRotatef(rotationY, 0.0f, 1.0f, 0.0f);
    drawSubdivideMode();

    glutSwapBuffers();
}

void reshape(int w, int h) {
    windowWidth = w;
    windowHeight = h;
    // Viewports will be set in display()
    glutPostRedisplay();
}

void keyboard(unsigned char key, int x, int y) {
    switch (key) {
        case 'a': // Rotate left
            rotationY -= 5.0f;
            break;
        case 'd': // Rotate right
            rotationY += 5.0f;
            break;
        case 'w': // Rotate up
            rotationX -= 5.0f;
            break;
        case 's': // Rotate down
            rotationX += 5.0f;
            break;
        case 'l': // Polygon mode: Line
            polygonMode = GL_LINE;
            std::cout << "Polygon Mode: GL_LINE" << std::endl;
            break;
        case 'f': // Polygon mode: Fill
            polygonMode = GL_FILL;
            std::cout << "Polygon Mode: GL_FILL" << std::endl;
            break;
        case '+': // Increase subdivision depth
        case '=':
            subdivisionDepth++;
            std::cout << "Subdivision Depth: " << subdivisionDepth << std::endl;
            break;
        case '-': // Decrease subdivision depth
            if (subdivisionDepth > 0) {
                subdivisionDepth--;
            }
            std::cout << "Subdivision Depth: " << subdivisionDepth << std::endl;
            break;
        case 27: // ESC key to exit
            exit(0);
            break;
    }
    glutPostRedisplay(); // Redraw the scene
}

// --- Main Function ---

int main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
    glutInitWindowSize(windowWidth, windowHeight);
    glutCreateWindow("Lab 12: Icosahedron Lighting and Subdivision");

    initGL();

    glutDisplayFunc(display);
    glutReshapeFunc(reshape);
    glutKeyboardFunc(keyboard);

    glutMainLoop();

    return 0;
}