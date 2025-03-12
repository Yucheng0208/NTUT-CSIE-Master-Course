#include <GL/freeglut.h>
#include <iostream>

float rotateX = 0.0f, rotateY = 0.0f, rotateZ = 0.0f; // 旋轉角度
float transX = 0.0f, transY = 0.0f, transZ = 0.0f; // 平移
float scale = 1.0f;  // 縮放比例

// **繪製 XYZ 軸**
void DrawAxes() {
    glLineWidth(2.0f);
    glBegin(GL_LINES);

    // X 軸（紅色）
    glColor3f(1, 0, 0);
    glVertex3f(-10, 0, 0);
    glVertex3f(10, 0, 0);

    // Y 軸（綠色）
    glColor3f(0, 1, 0);
    glVertex3f(0, -10, 0);
    glVertex3f(0, 10, 0);

    // Z 軸（藍色）
    glColor3f(0, 0, 1);
    glVertex3f(0, 0, -10);
    glVertex3f(0, 0, 10);

    glEnd();
}

// **繪製立方體**
void DrawCube() {
    glColor3f(1, 1, 0); // 黃色
    glutSolidCube(2.0);
}

// **渲染場景**
void RenderScene() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glLoadIdentity();

    gluLookAt(5, 5, 10, 0, 0, 0, 0, 1, 0);

    DrawAxes();  // **畫 XYZ 軸**

    // **應用變換**
    glTranslatef(transX, transY, transZ);  // **平移**
    glRotatef(rotateX, 1, 0, 0); // **X 軸旋轉**
    glRotatef(rotateY, 0, 1, 0); // **Y 軸旋轉**
    glRotatef(rotateZ, 0, 0, 1); // **Z 軸旋轉**
    glScalef(scale, scale, scale); // **縮放**

    DrawCube(); // 畫立方體

    glutSwapBuffers();
}

// **鍵盤輸入**
void KeyPress(unsigned char key, int x, int y) {
    switch (key) {
        // **旋轉**
        case 'X': rotateX += 5; break;
        case 'x': rotateX -= 5; break;
        case 'Y': rotateY += 5; break;
        case 'y': rotateY -= 5; break;
        case 'Z': rotateZ += 5; break;
        case 'z': rotateZ -= 5; break;

        // **平移**
        case 'A': transX -= 0.5; break;
        case 'D': transX += 0.5; break;
        case 'W': transY += 0.5; break;
        case 'S': transY -= 0.5; break;
        case 'Q': transZ -= 0.5; break;
        case 'E': transZ += 0.5; break;

        // **縮放**
        case 'T': scale *= 1.1f; break;
        case 'R': scale /= 1.1f; break;
        // **重置**
        case ' ':
            rotateX = rotateY = rotateZ = 0;
            transX = transY = transZ = 0;
            scale = 1.0f;
            break;
    }
    glutPostRedisplay();
}

// **視窗大小變更**
void ChangeSize(int w, int h) {
    glViewport(0, 0, w, h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(45, (float)w / (float)h, 1, 100);
    glMatrixMode(GL_MODELVIEW);
}

// **主函數**
int main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
    glutInitWindowSize(600, 600);
    glutCreateWindow("Lab 03 Cube with Transformations");

    glEnable(GL_DEPTH_TEST);
    glClearColor(1.0, 1.0, 1.0, 1.0); // 設定背景為白色

    glutDisplayFunc(RenderScene);
    glutReshapeFunc(ChangeSize);
    glutKeyboardFunc(KeyPress);

    glutMainLoop();
    return 0;
}
