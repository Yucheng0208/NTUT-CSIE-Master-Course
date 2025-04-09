#include <GL/freeglut.h>
#include <iostream>
#include <vector>
#include <cmath>

// Menu 定義
#define MENU_GRID_10 1
#define MENU_GRID_15 2
#define MENU_GRID_20 3
#define MENU_EXIT    4

int gridSize = 10;                 // 預設格線尺寸
int windowWidth = 600, windowHeight = 600;

// 用 pair<int, int> 記錄已經被點擊的格子
std::vector<std::pair<int, int>> filledCells;

// 初始化
void init() {
    glClearColor(0.0, 0.0, 0.0, 0.0); // 背景黑色
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D(-gridSize, gridSize + 1, -gridSize, gridSize + 1); // 注意 +1 讓格線完整
    filledCells.clear();
    filledCells.push_back({0, 0}); // 預設填色 (0,0)，黃色格子
}

// 畫格線
void drawGrid() {
    glColor3f(1.0, 1.0, 1.0); // 白色格線
    glBegin(GL_LINES);
    for (int i = -gridSize; i <= gridSize + 1; i++) {
        glVertex2f(i, -gridSize);
        glVertex2f(i, gridSize + 1);
        glVertex2f(-gridSize, i);
        glVertex2f(gridSize + 1, i);
    }
    glEnd();
}

// 畫被填色的格子
void drawFilledCells() {
    for (auto& cell : filledCells) {
        int x = cell.first;
        int y = cell.second;

        if (x == 0 && y == 0) {
            glColor3f(1.0, 1.0, 0.0); // (0,0) 畫黃色
        } else {
            glColor3f(0.0, 1.0, 0.0); // 其他格子畫綠色
        }

        glBegin(GL_POLYGON);
        glVertex2f(x, y);
        glVertex2f(x + 1, y);
        glVertex2f(x + 1, y + 1);
        glVertex2f(x, y + 1);
        glEnd();
    }
}

// 顯示
void display() {
    glClear(GL_COLOR_BUFFER_BIT);
    drawGrid();
    drawFilledCells();
    glFlush();
}

// 螢幕座標 -> 格子座標
std::pair<int, int> screenToGrid(int x, int y) {
    float normX = (float)x / windowWidth;
    float normY = (float)(windowHeight - y) / windowHeight; // 注意Y軸倒轉
    float worldX = normX * (2 * gridSize + 1) - gridSize;
    float worldY = normY * (2 * gridSize + 1) - gridSize;
    int gridX = (int)floor(worldX);
    int gridY = (int)floor(worldY);
    return {gridX, gridY};
}

// 滑鼠點擊事件
void mouse(int button, int state, int x, int y) {
    if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN) {
        auto [gridX, gridY] = screenToGrid(x, y);
        std::cout << "Clicked at grid: (" << gridX << ", " << gridY << ")" << std::endl;

        // 避免重複填色 (0,0)
        if (!(gridX == 0 && gridY == 0)) {
            filledCells.push_back({gridX, gridY});
        }
        glutPostRedisplay();
    }
}

// 選單事件處理
void selectFromMenu(int choice) {
    switch (choice) {
        case MENU_GRID_10:
            gridSize = 10;
            break;
        case MENU_GRID_15:
            gridSize = 15;
            break;
        case MENU_GRID_20:
            gridSize = 20;
            break;
        case MENU_EXIT:
            exit(0);
            break;
    }
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D(-gridSize, gridSize + 1, -gridSize, gridSize + 1); // +1，修正右邊和上邊缺格
    filledCells.clear();
    filledCells.push_back({0, 0}); // 每次切換後，重設 (0,0) 為黃色
    glutPostRedisplay();
}

// 建立Popup Menu
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
    glutCreateWindow("Clickable 2D Grid");

    init();
    glutDisplayFunc(display);
    glutMouseFunc(mouse);

    int menu = buildPopupMenu();
    glutAttachMenu(GLUT_RIGHT_BUTTON); // 右鍵選單

    glutMainLoop();
    return 0;
}