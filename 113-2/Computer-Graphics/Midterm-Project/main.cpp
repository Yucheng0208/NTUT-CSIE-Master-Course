#include <GL/freeglut.h>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>
#include <cmath>
#include <ctime>
#include <algorithm>

// 結構定義
struct Vertex {
    float x, y, z;
};

struct Face {
    std::vector<int> indices;
};

std::vector<Vertex> vertices;
std::vector<Face> faces;

float rotX = 0.0f, rotY = 0.0f, rotZ = 0.0f;
float transX = 0.0f, transY = 0.0f, transZ = -5.0f;
float autoScale = 1.0f;

// 任意軸旋轉控制
float customAxisX = 0.0f, customAxisY = 0.0f, customAxisZ = 1.0f;
float customAngle = 0.0f;

int renderMode = 2; // 0: points, 1: lines, 2: faces
bool useRandomColor = false;
bool hasModel = false;
std::string currentModelName = "None";

void resetTransform() {
    rotX = rotY = rotZ = 0.0f;
    transX = transY = 0.0f;
    transZ = -2.0f;
    customAxisX = 0.0f;
    customAxisY = 0.0f;
    customAxisZ = 1.0f;
    customAngle = 0.0f;
}

void clearModel() {
    vertices.clear();
    faces.clear();
    hasModel = false;
    currentModelName = "None";
    resetTransform();
}

void adjustViewToBoundingBox() {
    if (vertices.empty()) return;

    float minX = vertices[0].x, maxX = vertices[0].x;
    float minY = vertices[0].y, maxY = vertices[0].y;
    float minZ = vertices[0].z, maxZ = vertices[0].z;

    for (const auto& v : vertices) {
        minX = std::min(minX, v.x); maxX = std::max(maxX, v.x);
        minY = std::min(minY, v.y); maxY = std::max(maxY, v.y);
        minZ = std::min(minZ, v.z); maxZ = std::max(maxZ, v.z);
    }

    float centerX = (minX + maxX) / 2;
    float centerY = (minY + maxY) / 2;
    float centerZ = (minZ + maxZ) / 2;

    for (auto& v : vertices) {
        v.x -= centerX;
        v.y -= centerY;
        v.z -= centerZ;
    }

    float maxDim = std::max({ maxX - minX, maxY - minY, maxZ - minZ });
    autoScale = 1.4f / maxDim;
    transZ = -2.0f;
}

bool loadOBJ(const std::string& path) {
    clearModel();
    std::ifstream file(path);
    if (!file.is_open()) return false;

    std::string line;
    while (std::getline(file, line)) {
        std::istringstream iss(line);
        std::string type;
        iss >> type;
        if (type == "v") {
            Vertex v{};
            iss >> v.x >> v.y >> v.z;
            vertices.push_back(v);
        } else if (type == "f") {
            Face f;
            int idx;
            while (iss >> idx) {
                f.indices.push_back(idx - 1);
            }
            faces.push_back(f);
        }
    }
    file.close();
    hasModel = true;
    adjustViewToBoundingBox();
    return true;
}

void printText(float x, float y, const std::string& text) {
    glRasterPos2f(x, y);
    for (char c : text) {
        glutBitmapCharacter(GLUT_BITMAP_8_BY_13, c);
    }
}

void drawModel() {
    if (!hasModel) return;

    if (renderMode == 0) glBegin(GL_POINTS);
    else if (renderMode == 1) glBegin(GL_LINES);
    else glBegin(GL_TRIANGLES);

    for (const auto& face : faces) {
        if (useRandomColor) {
            glColor3f((rand() % 100) / 100.0f, (rand() % 100) / 100.0f, (rand() % 100) / 100.0f);
        } else {
            glColor3f(0.8f, 0.8f, 0.8f);
        }

        if (renderMode == 1 && face.indices.size() >= 2) {
            for (size_t i = 0; i < face.indices.size(); ++i) {
                const auto& v1 = vertices[face.indices[i]];
                const auto& v2 = vertices[face.indices[(i + 1) % face.indices.size()]];
                glVertex3f(v1.x, v1.y, v1.z);
                glVertex3f(v2.x, v2.y, v2.z);
            }
        } else {
            for (int idx : face.indices) {
                const auto& v = vertices[idx];
                glVertex3f(v.x, v.y, v.z);
            }
        }
    }
    glEnd();
}

void drawHUD() {
    glMatrixMode(GL_PROJECTION);
    glPushMatrix();
    glLoadIdentity();
    gluOrtho2D(0, 800, 0, 600);
    glMatrixMode(GL_MODELVIEW);
    glPushMatrix();
    glLoadIdentity();

    glColor3f(1.0, 1.0, 1.0);
    char buffer[100];
    printText(10, 110, "Model: " + currentModelName);
    sprintf(buffer, "X Angle: %.2f", rotX); printText(10, 96, buffer);
    sprintf(buffer, "Y Angle: %.2f", rotY); printText(10, 82, buffer);
    sprintf(buffer, "Z Angle: %.2f", rotZ); printText(10, 68, buffer);
    sprintf(buffer, "Depth (Z): %.2f", -transZ); printText(10, 54, buffer);
    sprintf(buffer, "Trans X: %.2f", transX); printText(10, 40, buffer);
    sprintf(buffer, "Trans Y: %.2f", transY); printText(10, 26, buffer);
    sprintf(buffer, "Custom Axis: (%.1f, %.1f, %.1f)", customAxisX, customAxisY, customAxisZ); printText(10, 12, buffer);
    sprintf(buffer, "Custom Angle: %.1f", customAngle); printText(10, -2, buffer);

    glPopMatrix();
    glMatrixMode(GL_PROJECTION);
    glPopMatrix();
    glMatrixMode(GL_MODELVIEW);
}

void display() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glLoadIdentity();

    glTranslatef(transX, transY, transZ);
    glRotatef(rotX, 1.0f, 0.0f, 0.0f);
    glRotatef(rotY, 0.0f, 1.0f, 0.0f);
    glRotatef(rotZ, 0.0f, 0.0f, 1.0f);
    glRotatef(customAngle, customAxisX, customAxisY, customAxisZ);
    glScalef(autoScale, autoScale, autoScale);

    drawModel();
    drawHUD();
    glutSwapBuffers();
}

void reshape(int w, int h) {
    glViewport(0, 0, w, h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(45.0, (double)w / h, 0.1, 100.0);
    glMatrixMode(GL_MODELVIEW);
}

void keyboard(unsigned char key, int x, int y) {
    switch (key) {
        case 'w': rotX -= 5; break;
        case 's': rotX += 5; break;
        case 'a': rotY -= 5; break;
        case 'd': rotY += 5; break;
        case 'q': rotZ -= 5; break;
        case 'e': rotZ += 5; break;
        case 'z': transZ += 0.2f; break;
        case 'x': transZ -= 0.2f; break;
        case 'r': resetTransform(); break;

        case 'u': customAxisX += 0.1f; break;
        case 'i': customAxisY += 0.1f; break;
        case 'o': customAxisZ += 0.1f; break;
        case 'j': customAngle -= 5.0f; break;
        case 'l': customAngle += 5.0f; break;
        case 'k': customAxisX = 0.0f; customAxisY = 0.0f; customAxisZ = 1.0f; customAngle = 0.0f; break;
    }
    glutPostRedisplay();
}

void arrowKeys(int key, int x, int y) {
    switch (key) {
        case GLUT_KEY_LEFT:  transX -= 0.2f; break;
        case GLUT_KEY_RIGHT: transX += 0.2f; break;
        case GLUT_KEY_UP:    transY += 0.2f; break;
        case GLUT_KEY_DOWN:  transY -= 0.2f; break;
        case GLUT_KEY_PAGE_UP:   transZ += 0.2f; break;
        case GLUT_KEY_PAGE_DOWN: transZ -= 0.2f; break;
    }
    glutPostRedisplay();
}

void menu(int choice) {
    switch (choice) {
        case 1: loadOBJ("models/teapot.obj"); currentModelName = "Teapot"; break;
        case 2: loadOBJ("models/teddy.obj"); currentModelName = "Teddy"; break;
        case 3: loadOBJ("models/gourd.obj"); currentModelName = "Gourd"; break;
        case 4: loadOBJ("models/octahedron.obj"); currentModelName = "Octahedron"; break;
        case 5: renderMode = 0; break;
        case 6: renderMode = 1; break;
        case 7: renderMode = 2; break;
        case 8: useRandomColor = true; break;
        case 9: useRandomColor = false; break;
        case 10: clearModel(); break;
        case 11: exit(0); break;
    }
    glutPostRedisplay();
}

void createMenu() {
    int submenuModel = glutCreateMenu(menu);
    glutAddMenuEntry("Load Teapot", 1);
    glutAddMenuEntry("Load Teddy", 2);
    glutAddMenuEntry("Load Gourd", 3);
    glutAddMenuEntry("Load Octahedron", 4);

    int submenuRender = glutCreateMenu(menu);
    glutAddMenuEntry("Render: Point", 5);
    glutAddMenuEntry("Render: Line", 6);
    glutAddMenuEntry("Render: Face", 7);

    int submenuColor = glutCreateMenu(menu);
    glutAddMenuEntry("Random Color", 8);
    glutAddMenuEntry("Solid Color", 9);

    int mainMenu = glutCreateMenu(menu);
    glutAddSubMenu("Model", submenuModel);
    glutAddSubMenu("Render Mode", submenuRender);
    glutAddSubMenu("Color Mode", submenuColor);
    glutAddMenuEntry("Clear Model", 10);
    glutAddMenuEntry("Exit", 11);

    glutAttachMenu(GLUT_RIGHT_BUTTON);
}

int main(int argc, char** argv) {
    srand(static_cast<unsigned int>(time(nullptr)));
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
    glutInitWindowSize(800, 600);
    glutCreateWindow("OBJ Model Viewer");

    glEnable(GL_DEPTH_TEST);

    glutDisplayFunc(display);
    glutReshapeFunc(reshape);
    glutKeyboardFunc(keyboard);
    glutSpecialFunc(arrowKeys);
    createMenu();

    glutMainLoop();
    return 0;
}
