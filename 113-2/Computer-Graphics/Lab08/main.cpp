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
    float r, g, b;
};

struct Line {
    int x0, y0, x1, y1;
};

std::vector<Pixel> pixels;
std::vector<Line> lines;
std::vector<std::pair<int, int>> clickPoints;

void init() {
    pixels.clear();
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

    // 標記原點 0,0 為紅色小方塊
    glColor3f(1.0, 0.0, 0.0);
    glBegin(GL_POLYGON);
    glVertex2f(-0.2f, -0.2f);
    glVertex2f(0.2f, -0.2f);
    glVertex2f(0.2f, 0.2f);
    glVertex2f(-0.2f, 0.2f);
    glEnd();
}

void drawPixels() {
    for (auto& p : pixels) {
        glColor3f(p.r, p.g, p.b);
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
        pixels.push_back({x0, y0, 1.0f, 1.0f, 1.0f}); // 白色線段
        if (x0 == x1 && y0 == y1) break;
        int e2 = 2 * err;
        if (e2 > -dy) { err -= dy; x0 += sx; }
        if (e2 < dx) { err += dx; y0 += sy; }
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

bool isInsideTriangle(std::pair<int, int> p, std::vector<std::pair<int, int>> tri) {
    auto [x, y] = p;
    auto sign = [](int x0, int y0, int x1, int y1, int x2, int y2) {
        return (x0 - x2) * (y1 - y2) - (x1 - x2) * (y0 - y2);
    };

    int d1 = sign(x, y, tri[0].first, tri[0].second, tri[1].first, tri[1].second);
    int d2 = sign(x, y, tri[1].first, tri[1].second, tri[2].first, tri[2].second);
    int d3 = sign(x, y, tri[2].first, tri[2].second, tri[0].first, tri[0].second);

    bool has_neg = (d1 < 0) || (d2 < 0) || (d3 < 0);
    bool has_pos = (d1 > 0) || (d2 > 0) || (d3 > 0);

    return !(has_neg && has_pos);
}

void fillTriangleArea(std::vector<std::pair<int, int>> tri) {
    for (int x = -gridSize; x <= gridSize; x++) {
        for (int y = -gridSize; y <= gridSize; y++) {
            if (isInsideTriangle({x, y}, tri)) {
                pixels.push_back({x, y, 0.2f, 0.6f, 1.0f}); // 淺藍填色
            }
        }
    }
}

void mouse(int button, int state, int x, int y) {
    if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN) {
        auto [gx, gy] = screenToGrid(x, y);
        std::cout << "Clicked at: (" << gx << ", " << gy << ")\n";

        if (clickPoints.size() < 3) {
            clickPoints.push_back({gx, gy});
            pixels.push_back({gx, gy, 1.0f, 1.0f, 0.0f}); // 黃點

            if (clickPoints.size() > 1) {
                int last = clickPoints.size() - 1;
                lines.push_back({clickPoints[last - 1].first, clickPoints[last - 1].second,
                                 clickPoints[last].first, clickPoints[last].second});
            }

            if (clickPoints.size() == 3) {
                lines.push_back({clickPoints[2].first, clickPoints[2].second,
                                 clickPoints[0].first, clickPoints[0].second});
                fillTriangleArea(clickPoints); // 三角形內部填色
            }

        } else {
            std::vector<std::pair<int, int>> triangle = {
                clickPoints[0], clickPoints[1], clickPoints[2]
            };
            bool inside = isInsideTriangle({gx, gy}, triangle);
            if (inside) {
                pixels.push_back({gx, gy, 0.2f, 0.6f, 1.0f}); // 藍色點
            } else {
                pixels.push_back({gx, gy, 0.5f, 0.5f, 0.5f}); // 灰色點
            }
        }

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
    glutCreateWindow("Half-Space Triangle Fill");

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
