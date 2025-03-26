#include <GL/freeglut.h>
#include <iostream>
#include <cmath>

float rotateX = 0.0f, rotateY = 0.0f, rotateZ = 0.0f; // 原本的三軸旋轉角度
float transX = 0.0f, transY = 0.0f, transZ = 0.0f; // 平移
float scale = 1.0f;  // 縮放比例
float angle = 0.0f;  // 任意軸自訂角度控制

float v1[3] = {0, 0, 0};
float v2[3] = {1, 1, 0};

// 正規化向量
void Normalize(float* v) {
    float len = sqrt(v[0]*v[0] + v[1]*v[1] + v[2]*v[2]);
    if (len > 0.0f) {
        v[0] /= len; v[1] /= len; v[2] /= len;
    }
}

// 建構 Rodrigues rotation matrix
void BuildRotationMatrix(float* M, float angleDeg, float* axis) {
    float angleRad = angleDeg * M_PI / 180.0f;
    float c = cos(angleRad);
    float s = sin(angleRad);
    float t = 1 - c;
    float x = axis[0], y = axis[1], z = axis[2];

    M[0] = t*x*x + c;     M[1] = t*x*y - s*z; M[2] = t*x*z + s*y; M[3] = 0;
    M[4] = t*x*y + s*z;   M[5] = t*y*y + c;   M[6] = t*y*z - s*x; M[7] = 0;
    M[8] = t*x*z - s*y;   M[9] = t*y*z + s*x; M[10]= t*z*z + c;   M[11]= 0;
    M[12]= 0;             M[13]= 0;           M[14]= 0;           M[15]= 1;
}

void DrawAxes() {
    glLineWidth(2.0f);
    glBegin(GL_LINES);
    glColor3f(1, 0, 0); glVertex3f(-10, 0, 0); glVertex3f(10, 0, 0);
    glColor3f(0, 1, 0); glVertex3f(0, -10, 0); glVertex3f(0, 10, 0);
    glColor3f(0, 0, 1); glVertex3f(0, 0, -10); glVertex3f(0, 0, 10);
    glEnd();
}

void DrawArbitraryAxis() {
    glLineWidth(3.0f);
    glColor3f(1, 0, 1); // 紫色
    glBegin(GL_LINES);
    // 延伸線長度倍數，避免太短不好看
    float ext = 5.0f;
    float dir[3] = { v2[0] - v1[0], v2[1] - v1[1], v2[2] - v1[2] };
    Normalize(dir);
    glVertex3f(v1[0] - dir[0]*ext, v1[1] - dir[1]*ext, v1[2] - dir[2]*ext);
    glVertex3f(v2[0] + dir[0]*ext, v2[1] + dir[1]*ext, v2[2] + dir[2]*ext);
    glEnd();
}

void DrawCube() {
    glColor3f(1, 1, 0);
    glutSolidCube(2.0);
}

void RenderScene() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glLoadIdentity();

    gluLookAt(5, 5, 10, 0, 0, 0, 0, 1, 0);

    DrawAxes();            // 畫 XYZ 軸（世界座標）
    DrawArbitraryAxis();   // 畫任意軸（世界座標）

    // 任意軸旋轉處理
    float axis[3] = { v2[0] - v1[0], v2[1] - v1[1], v2[2] - v1[2] };
    Normalize(axis);
    float R[16];
    BuildRotationMatrix(R, angle, axis);

    glPushMatrix();

    // 任意軸旋轉變換
    glTranslatef(v1[0], v1[1], v1[2]);
    glMultMatrixf(R);
    glTranslatef(-v1[0], -v1[1], -v1[2]);

    // 原始旋轉和平移變換
    glRotatef(rotateX, 1, 0, 0);
    glRotatef(rotateY, 0, 1, 0);
    glRotatef(rotateZ, 0, 0, 1);

    glTranslatef(transX, transY, transZ);
    glScalef(scale, scale, scale);

    DrawCube();
    glPopMatrix();

    glutSwapBuffers();
}

void KeyPress(unsigned char key, int x, int y) {
    switch (key) {
        case 'X': rotateX += 5; break;
        case 'x': rotateX -= 5; break;
        case 'Y': rotateY += 5; break;
        case 'y': rotateY -= 5; break;
        case 'Z': rotateZ += 5; break;
        case 'z': rotateZ -= 5; break;

        case 'U': angle += 45; break; // 任意軸劇烈旋轉
        case 'u': angle -= 45; break;

        case 'A': transX -= 0.5; break;
        case 'D': transX += 0.5; break;
        case 'W': transY += 0.5; break;
        case 'S': transY -= 0.5; break;
        case 'Q': transZ -= 0.5; break;
        case 'E': transZ += 0.5; break;

        case 'T': scale *= 1.1f; break;
        case 'R': scale /= 1.1f; break;

        case 'I':
            std::cout << "請輸入 v1(x y z) 與 v2(x y z)：";
            std::cin >> v1[0] >> v1[1] >> v1[2] >> v2[0] >> v2[1] >> v2[2];
            angle = 0.0f;
            break;

        case ' ': // Reset 所有狀態
            rotateX = rotateY = rotateZ = 0;
            transX = transY = transZ = 0;
            scale = 1.0f;
            angle = 0.0f;
            break;
    }
    glutPostRedisplay();
}

void ChangeSize(int w, int h) {
    glViewport(0, 0, w, h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(45, (float)w / (float)h, 1, 1000);
    glMatrixMode(GL_MODELVIEW);
}

int main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
    glutInitWindowSize(600, 600);
    glutCreateWindow("Lab 04 Arbitrary Rotation");

    glEnable(GL_DEPTH_TEST);
    glClearColor(1.0, 1.0, 1.0, 1.0);

    glutDisplayFunc(RenderScene);
    glutReshapeFunc(ChangeSize);
    glutKeyboardFunc(KeyPress);

    std::cout << "初始請輸入 v1(x y z) 與 v2(x y z)：";
    std::cin >> v1[0] >> v1[1] >> v1[2] >> v2[0] >> v2[1] >> v2[2];

    glutMainLoop();
    return 0;
}