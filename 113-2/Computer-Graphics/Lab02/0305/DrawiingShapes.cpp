#include <stdio.h> 
#include <stdlib.h>
#include <GL/freeglut.h>

void ChangeSize(int, int);
void RenderScene(void);
void SetupRC();
void TimerFunction(int);

// 旋轉角度變數
GLfloat angleX = 0.0f; // X 軸旋轉
GLfloat angleY = 0.0f; // Y 軸旋轉

int main(int argc, char** argv)
{
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
    glutInitWindowSize(600, 600);
    glutInitWindowPosition(600, 80);
    glutCreateWindow("Full View Rotating Triangular Prism");

    glutReshapeFunc(ChangeSize);
    glutDisplayFunc(RenderScene);
    glutTimerFunc(16, TimerFunction, 1); // 60 FPS

    SetupRC(); // 設定 OpenGL 參數

    glutMainLoop();
    return 0;
}

// 設定 OpenGL 參數
void SetupRC()
{
    glEnable(GL_DEPTH_TEST);   // 啟用深度測試
    glEnable(GL_COLOR_MATERIAL);
    glEnable(GL_LIGHTING);     // 啟用光照
    glEnable(GL_LIGHT0);       // 使用預設光源
    glEnable(GL_NORMALIZE);    // 確保光照在旋轉時正確作用
    glDisable(GL_CULL_FACE);   // **關閉面剔除，確保底面可見**

    glShadeModel(GL_SMOOTH);
}

// 視窗大小變更時呼叫
void ChangeSize(int w, int h)
{
    glViewport(0, 0, w, h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(45.0, (float)w / (float)h, 1.0, 100.0);
    glMatrixMode(GL_MODELVIEW);
}

// 計時器函數，讓物體旋轉
void TimerFunction(int value)
{
    angleX += 1.0f; // X 軸旋轉
    angleY += 1.5f; // Y 軸旋轉

    glutPostRedisplay(); // 重新繪製場景
    glutTimerFunc(16, TimerFunction, 1); // 設定下一次計時
}

// 繪製旋轉中的三角柱
void RenderScene(void)
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glLoadIdentity();

    // **調整視角，從稍高角度觀看，確保底面也能看到**
    gluLookAt(3.0, 3.5, 6.0,  // 攝影機位置 (eyeX, eyeY, eyeZ)
        0.0, 0.0, 0.0,   // 觀看的目標點 (centerX, centerY, centerZ)
        0.0, 1.0, 0.0);  // 上方向 (upX, upY, upZ)

    glRotatef(angleX, 1.0f, 0.0f, 0.0f); // 繞 X 軸旋轉
    glRotatef(angleY, 0.0f, 1.0f, 0.0f); // 繞 Y 軸旋轉

    glBegin(GL_TRIANGLES);

    // **三角柱頂點**
    GLfloat vertices[6][3] = {
        { -1.0, -1.0, -1.0 },  // 0 左下角
        {  1.0, -1.0, -1.0 },  // 1 右下角
        {  0.0,  1.0, -1.0 },  // 2 上頂點
        { -1.0, -1.0,  1.0 },  // 3 左下角 (後方)
        {  1.0, -1.0,  1.0 },  // 4 右下角 (後方)
        {  0.0,  1.0,  1.0 }   // 5 上頂點 (後方)
    };

    // **頂面 (紅色)**
    glColor3f(1.0, 0.0, 0.0);
    glVertex3fv(vertices[0]);
    glVertex3fv(vertices[1]);
    glVertex3fv(vertices[2]);

    // **底面 (藍色)**
    glColor3f(0.0, 0.0, 1.0);
    glVertex3fv(vertices[3]);
    glVertex3fv(vertices[5]);
    glVertex3fv(vertices[4]);

    // **側面 1 (綠色)**
    glColor3f(0.0, 1.0, 0.0);
    glVertex3fv(vertices[0]);
    glVertex3fv(vertices[3]);
    glVertex3fv(vertices[1]);

    glVertex3fv(vertices[1]);
    glVertex3fv(vertices[3]);
    glVertex3fv(vertices[4]);

    // **側面 2 (黃色)**
    glColor3f(1.0, 1.0, 0.0);
    glVertex3fv(vertices[1]);
    glVertex3fv(vertices[4]);
    glVertex3fv(vertices[2]);

    glVertex3fv(vertices[2]);
    glVertex3fv(vertices[4]);
    glVertex3fv(vertices[5]);

    // **側面 3 (紫色)**
    glColor3f(1.0, 0.0, 1.0);
    glVertex3fv(vertices[0]);
    glVertex3fv(vertices[2]);
    glVertex3fv(vertices[3]);

    glVertex3fv(vertices[3]);
    glVertex3fv(vertices[2]);
    glVertex3fv(vertices[5]);

    glEnd();

    glutSwapBuffers();
}
