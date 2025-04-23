#include <GL/freeglut.h>
#include <iostream>
#include <vector>
#include <cmath>

#define MENU_GRID_10 1
#define MENU_GRID_15 2
#define MENU_GRID_20 3
#define MENU_EXIT    4

int gridSize = 10;
int windowWidth = 600, windowHeight = 600;

struct Pixel {
    int x, y;
    int direction; // 0 = endpoint, 1 = E, 2 = NE
};

struct Line {
    int x0, y0, x1, y1;
};

std::vector<Pixel> pixels;
std::vector<Line> lines;
std::vector<std::pair<int, int>> clickPoints;

void init() {
    pixels.clear();
    pixels.push_back({0, 0, 0});
    lines.clear();
    clickPoints.clear();
    glutPostRedisplay();
}

void setupProjection() {
    glClearColor(0.0, 0.0, 0.0, 0.0);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D(-gridSize, gridSize + 1, -gridSize, gridSize + 1);
}

void drawGrid() {
    glColor3f(1.0, 1.0, 1.0);
    glBegin(GL_LINES);
    for (int i = -gridSize; i <= gridSize + 1; i++) {
        glVertex2f(i, -gridSize);
        glVertex2f(i, gridSize + 1);
        glVertex2f(-gridSize, i);
        glVertex2f(gridSize + 1, i);
    }
    glEnd();
}

void drawPixels() {
    for (auto& p : pixels) {
        if (p.x == 0 && p.y == 0) {
            glColor3f(1.0, 1.0, 0.0);
        } else if (p.direction == 1) {
            glColor3f(0.0, 1.0, 0.0);
        } else if (p.direction == 2) {
            glColor3f(0.0, 0.5, 1.0);
        } else {
            glColor3f(1.0, 0.0, 0.0);
        }

        glBegin(GL_POLYGON);
        glVertex2f(p.x, p.y);
        glVertex2f(p.x + 1, p.y);
        glVertex2f(p.x + 1, p.y + 1);
        glVertex2f(p.x, p.y + 1);
        glEnd();
    }
}

void drawMidpointLine(Line line) {
    int x0 = line.x0, y0 = line.y0;
    int x1 = line.x1, y1 = line.y1;

    int dx = abs(x1 - x0);
    int dy = abs(y1 - y0);
    int sx = (x0 < x1) ? 1 : -1;
    int sy = (y0 < y1) ? 1 : -1;
    int err = dx - dy;

    while (true) {
        int direction = 0;
        if ((x0 == line.x0 && y0 == line.y0) || (x0 == line.x1 && y0 == line.y1)) {
            direction = 0;
        } else {
            int e2 = 2 * err;
            if (e2 > -dy && e2 < dx) direction = 2;
            else direction = 1;
        }

        pixels.push_back({x0, y0, direction});
        std::cout << "Pixel: (" << x0 << ", " << y0 << ")";
        std::cout << " -> " << (direction == 1 ? "E" : direction == 2 ? "NE" : "Endpoint") << std::endl;

        if (x0 == x1 && y0 == y1) break;
        int e2 = 2 * err;
        if (e2 > -dy) {
            err -= dy;
            x0 += sx;
        }
        if (e2 < dx) {
            err += dx;
            y0 += sy;
        }
    }
}

void display() {
    glClear(GL_COLOR_BUFFER_BIT);
    drawGrid();
    for (auto& line : lines) drawMidpointLine(line);
    drawPixels();
    glFlush();
}

std::pair<int, int> screenToGrid(int x, int y) {
    float normX = (float)x / windowWidth;
    float normY = (float)(windowHeight - y) / windowHeight;
    float worldX = normX * (2 * gridSize + 1) - gridSize;
    float worldY = normY * (2 * gridSize + 1) - gridSize;
    return {(int)floor(worldX), (int)floor(worldY)};
}

void mouse(int button, int state, int x, int y) {
    if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN) {
        auto [gx, gy] = screenToGrid(x, y);
        std::cout << "Clicked at: (" << gx << ", " << gy << ")\n";
        clickPoints.push_back({gx, gy});

        if (clickPoints.size() == 5) {
            clickPoints.clear();
            clickPoints.push_back({gx, gy});
        }

        if (clickPoints.size() > 1) {
            int last = clickPoints.size() - 1;
            lines.push_back({clickPoints[last - 1].first, clickPoints[last - 1].second,
                             clickPoints[last].first, clickPoints[last].second});
        }

        if (clickPoints.size() == 4) {
            lines.push_back({clickPoints[3].first, clickPoints[3].second,
                             clickPoints[0].first, clickPoints[0].second});
        }

        pixels.push_back({gx, gy, 0});
        glutPostRedisplay();
    }
}

void keyboard(unsigned char key, int x, int y) {
    if (key == 'r' || key == 'R') {
        init();
    }
}

void selectFromMenu(int choice) {
    switch (choice) {
        case MENU_GRID_10: gridSize = 10; break;
        case MENU_GRID_15: gridSize = 15; break;
        case MENU_GRID_20: gridSize = 20; break;
        case MENU_EXIT: exit(0);
    }
    setupProjection();
    init();
    glutPostRedisplay();
}

int buildPopupMenu() {
    int menu = glutCreateMenu(selectFromMenu);
    glutAddMenuEntry("Grid 10x10", MENU_GRID_10);
    glutAddMenuEntry("Grid 15x15", MENU_GRID_15);
    glutAddMenuEntry("Grid 20x20", MENU_GRID_20);
    glutAddMenuEntry("Exit", MENU_EXIT);
    return menu;
}

int main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
    glutInitWindowSize(windowWidth, windowHeight);
    glutCreateWindow("Midpoint Line Drawing");

    setupProjection();
    init();
    glutDisplayFunc(display);
    glutMouseFunc(mouse);
    glutKeyboardFunc(keyboard);

    int menu = buildPopupMenu();
    glutAttachMenu(GLUT_RIGHT_BUTTON);

    glutMainLoop();
    return 0;
}
