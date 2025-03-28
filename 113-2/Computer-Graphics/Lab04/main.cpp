#include <GL/freeglut.h>
#include <iostream>
using namespace std;

float rotateX = 0.0f, rotateY = 0.0f, rotateZ = 0.0f;
float transX = 0.0f, transY = 0.0f, transZ = 0.0f;
float scale = 1.0f;
float angle = 0.0f;

bool useCustomAxis = false;
float v1[3] = {0, 0, 0};
float v2[3] = {0, 0, 1};

void Normalize(float* v) {
    float len = sqrt(v[0]*v[0] + v[1]*v[1] + v[2]*v[2]);
    if (len > 0.0f) {
        v[0] /= len; v[1] /= len; v[2] /= len;
    }
}

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

void BuildTranslationMatrix(float* M, float x, float y, float z) {
    for (int i = 0; i < 16; ++i) M[i] = (i % 5 == 0) ? 1.0f : 0.0f;
    M[12] = x; M[13] = y; M[14] = z;
}

void BuildScaleMatrix(float* M, float s) {
    for (int i = 0; i < 16; ++i) M[i] = 0.0f;
    M[0] = M[5] = M[10] = s;
    M[15] = 1.0f;
}

void MultiplyMatrix(const float* A, const float* B, float* result) {
    for (int row = 0; row < 4; ++row) {
        for (int col = 0; col < 4; ++col) {
            result[row * 4 + col] = 0.0f;
            for (int k = 0; k < 4; ++k) {
                result[row * 4 + col] += A[row * 4 + k] * B[k * 4 + col];
            }
        }
    }
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
    if (!useCustomAxis) return;
    glLineWidth(3.0f);
    glColor3f(1, 0, 1);
    glBegin(GL_LINES);
    float ext = 5.0f;
    float dir[3] = { v2[0] - v1[0], v2[1] - v1[1], v2[2] - v1[2] };
    Normalize(dir);
    glVertex3f(v1[0] - dir[0]*ext, v1[1] - dir[1]*ext, v1[2] - dir[2]*ext);
    glVertex3f(v2[0] + dir[0]*ext, v2[1] + dir[1]*ext, v2[2] + dir[2]*ext);
    glEnd();
}

void DrawCube() {
    glBegin(GL_QUADS);
    glColor3f(1, 1, 0);
    float s = 1.0f;
    float v[8][3] = {
        {-s, -s, -s}, { s, -s, -s}, { s,  s, -s}, {-s,  s, -s},
        {-s, -s,  s}, { s, -s,  s}, { s,  s,  s}, {-s,  s,  s}
    };
    int faces[6][4] = {
        {0, 1, 2, 3}, {1, 5, 6, 2}, {5, 4, 7, 6},
        {4, 0, 3, 7}, {3, 2, 6, 7}, {4, 5, 1, 0}
    };
    for (int i = 0; i < 6; ++i) {
        for (int j = 0; j < 4; ++j)
            glVertex3fv(v[faces[i][j]]);
    }
    glEnd();
}

void RenderScene() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glLoadIdentity();
    gluLookAt(5, 5, 10, 0, 0, 0, 0, 1, 0);

    DrawAxes();
    DrawArbitraryAxis();

    float axis[3];
    if (useCustomAxis) {
        axis[0] = v2[0] - v1[0];
        axis[1] = v2[1] - v1[1];
        axis[2] = v2[2] - v1[2];
    } else {
        axis[0] = 0;
        axis[1] = 0;
        axis[2] = 1; // default Z-axis
    }
    Normalize(axis);

    float T1[16], R[16], T2[16], S[16], Trans[16];
    float M1[16], M2[16], M3[16], Final[16];

    BuildTranslationMatrix(Trans, transX, transY, transZ);
    BuildTranslationMatrix(T1, -v1[0], -v1[1], -v1[2]);
    BuildTranslationMatrix(T2, v1[0], v1[1], v1[2]);
    BuildRotationMatrix(R, angle, axis);
    BuildScaleMatrix(S, scale);

    MultiplyMatrix(R, T1, M1);
    MultiplyMatrix(T2, M1, M2);
    MultiplyMatrix(S, M2, M3);
    MultiplyMatrix(Trans, M3, Final);

    glPushMatrix();
    glMultMatrixf(Final);
    DrawCube();
    glPopMatrix();

    glutSwapBuffers();
}

void KeyPress(unsigned char key, int x, int y) {
    switch (key) {
        case 'U': angle += 0.5f; break;
        case 'u': angle -= 0.5f; break;
        case 'W': transY += 0.5f; break;
        case 'S': transY -= 0.5f; break;
        case 'A': transX -= 0.5f; break;
        case 'D': transX += 0.5f; break;
        case 'Q': transZ -= 0.5f; break;
        case 'E': transZ += 0.5f; break;
        case 'T': scale *= 1.1f; break;
        case 'R': scale /= 1.1f; break;
        case 'X': cout << "Bye~"; exit(0);
        case 'I':
            cout << "請輸入 v1(x y z) 與 v2(x y z)：";
            cin >> v1[0] >> v1[1] >> v1[2] >> v2[0] >> v2[1] >> v2[2];
            useCustomAxis = true;
            angle = 0.0f;
            break;
        case ' ':
            transX = transY = transZ = 0;
            angle = 0.0f;
            scale = 1.0f;
            useCustomAxis = false;
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

    glutMainLoop();
    return 0;
}
