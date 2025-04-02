#include <GL/freeglut.h>
#include <iostream>
#include <vector>
#include <cmath>
#include <algorithm> // For std::clamp (C++17) or manual clamping

// Define M_PI if not available
#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

// Manual clamp for pre-C++17
template<class T>
const T& clamp( const T& v, const T& lo, const T& hi ) {
    return std::min(hi, std::max(lo, v));
}


using namespace std;

// Structure to hold state for one viewport
struct ViewportState {
    // ... (transformations etc. unchanged) ...
    float transX = 0.0f, transY = 0.0f, transZ = 0.0f;
    float scale = 1.0f;
    float angle = 0.0f;
    bool useCustomAxis = false;
    // v1 and v2 are now the actual endpoints P and -P
    float v1[3] = {0.0f, 0.0f, 0.0f};
    float v2[3] = {0.0f, 0.0f, 1.0f}; // Initial placeholder
    // Store the normalized direction separately for rotation
    float customAxisDirection[3] = {0.0f, 0.0f, 1.0f}; // Default Z

    // Camera properties (unchanged)
    float eyeX = 5.0f, eyeY = 5.0f, eyeZ = 10.0f;
    float centerX = 0.0f, centerY = 0.0f, centerZ = 0.0f;
    float upX = 0.0f, upY = 1.0f, upZ = 0.0f;
};

// --- Global Variables ---
ViewportState viewports[2];
int activeViewport = 0;
int windowWidth = 1200;
int windowHeight = 600;
// Removed AXIS_LINE_LENGTH

// --- Utility Functions --- (Normalize, Matrix functions unchanged)
void Normalize(float* v) {
    float len = sqrt(v[0]*v[0] + v[1]*v[1] + v[2]*v[2]);
    if (len > 1e-6) {
        v[0] /= len; v[1] /= len; v[2] /= len;
    } else {
        v[0] = 0; v[1] = 0; v[2] = 1;
    }
}
void BuildRotationMatrix(float* M, float angleDeg, float* axis) {
    float normAxis[3] = {axis[0], axis[1], axis[2]};
    Normalize(normAxis);

    float angleRad = angleDeg * M_PI / 180.0f;
    float c = cos(angleRad);
    float s = sin(angleRad);
    float t = 1.0f - c;
    float x = normAxis[0], y = normAxis[1], z = normAxis[2];

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
    M[0] = s; M[5] = s; M[10] = s;
    M[15] = 1.0f;
}
void MultiplyMatrix(const float* A, const float* B, float* result) {
    float temp[16];
    for (int row = 0; row < 4; ++row) {
        for (int col = 0; col < 4; ++col) {
            temp[row * 4 + col] = 0.0f;
            for (int k = 0; k < 4; ++k) {
                temp[row * 4 + col] += A[row * 4 + k] * B[k * 4 + col];
            }
        }
    }
    for (int i = 0; i < 16; ++i) result[i] = temp[i];
}


// --- Drawing Functions ---
void DrawAxes() { // World axes
    glLineWidth(1.0f); // Make world axes thinner
    glBegin(GL_LINES);
    // Make axes very long to provide frame of reference
    float axisLen = 100.0f;
    glColor3f(0.5f, 0.0f, 0.0f); glVertex3f(-axisLen, 0.0f, 0.0f); glVertex3f(axisLen, 0.0f, 0.0f); // Dim Red X
    glColor3f(0.0f, 0.5f, 0.0f); glVertex3f(0.0f, -axisLen, 0.0f); glVertex3f(0.0f, axisLen, 0.0f); // Dim Green Y
    glColor3f(0.0f, 0.0f, 0.5f); glVertex3f(0.0f, 0.0f, -axisLen); glVertex3f(0.0f, 0.0f, axisLen); // Dim Blue Z
    glEnd();
}

void DrawArbitraryAxis(const ViewportState& state) {
    if (!state.useCustomAxis) return;
    // Draw the line
    glLineWidth(2.5f); // Make custom axis line thicker
    glColor3f(1.0f, 0.0f, 1.0f); // Magenta line
    glBegin(GL_LINES);
    glVertex3fv(state.v1);
    glVertex3fv(state.v2);
    glEnd();

    // Draw points at the ends for clarity
    glPointSize(8.0f);
    glBegin(GL_POINTS);
    glColor3f(1.0f, 1.0f, 0.0f); // Yellow point at v1 (P)
    glVertex3fv(state.v1);
    glColor3f(0.0f, 1.0f, 1.0f); // Cyan point at v2 (-P)
    glVertex3fv(state.v2);
    glEnd();
}

void DrawCube() { // Unchanged
    glBegin(GL_QUADS);
    float s = 0.7f;
    GLfloat vertices[][3] = {
        {-s, -s, -s}, { s, -s, -s}, { s,  s, -s}, {-s,  s, -s},
        {-s, -s,  s}, { s, -s,  s}, { s,  s,  s}, {-s,  s,  s}
    };
    GLint faces[][4] = {
        {0, 3, 2, 1}, {4, 5, 6, 7}, {0, 4, 7, 3}, {1, 2, 6, 5}, {3, 7, 6, 2}, {0, 1, 5, 4}
    };
    GLfloat colors[][3] = {
        {1.0, 0.0, 0.0}, {0.0, 1.0, 0.0}, {0.0, 0.0, 1.0}, {1.0, 1.0, 0.0}, {1.0, 0.0, 1.0}, {0.0, 1.0, 1.0}
    };
    for (int i = 0; i < 6; ++i) {
        glColor3fv(colors[i]);
        for (int j = 0; j < 4; ++j) glVertex3fv(vertices[faces[i][j]]);
    }
    glEnd();
}

// --- GLUT Callbacks ---

void RenderScene() { // Mostly unchanged logic, just adjusted far plane
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    for (int i = 0; i < 2; ++i) {
        int vpX = (i == 0) ? 0 : windowWidth / 2;
        int vpWidth = windowWidth / 2;
        glViewport(vpX, 0, vpWidth, windowHeight);

        ViewportState& currentState = viewports[i];

        glMatrixMode(GL_PROJECTION);
        glLoadIdentity();
        if (windowHeight > 0 && vpWidth > 0) {
             // Increased far plane significantly
             gluPerspective(45.0, (double)vpWidth / (double)windowHeight, 0.1, 500.0);
        }

        glMatrixMode(GL_MODELVIEW);
        glLoadIdentity();
        gluLookAt(currentState.eyeX, currentState.eyeY, currentState.eyeZ,
                  currentState.centerX, currentState.centerY, currentState.centerZ,
                  currentState.upX, currentState.upY, currentState.upZ);

        DrawAxes();
        DrawArbitraryAxis(currentState); // Draws line from v1 to v2

        // --- Object Transformation ---
        float axisForRotation[3];
        float pivotPoint[3] = {0.0f, 0.0f, 0.0f}; // Rotation pivot is always origin

        if (currentState.useCustomAxis) {
            axisForRotation[0] = currentState.customAxisDirection[0];
            axisForRotation[1] = currentState.customAxisDirection[1];
            axisForRotation[2] = currentState.customAxisDirection[2];
        } else {
            axisForRotation[0] = 0.0f; axisForRotation[1] = 0.0f; axisForRotation[2] = 1.0f; // Default Z
        }

        // Build transformation matrices (Pivot is origin, T1/T2 are identity)
        float T_trans[16], R_axis[16], S_scale[16];
        float M1[16], FinalTransform[16];

        BuildTranslationMatrix(T_trans, currentState.transX, currentState.transY, currentState.transZ);
        BuildScaleMatrix(S_scale, currentState.scale);
        BuildRotationMatrix(R_axis, currentState.angle, axisForRotation);

        // Combine: Final = T_trans * S_scale * R_axis
        MultiplyMatrix(S_scale, R_axis, M1);             // M1 = S_scale * R_axis
        MultiplyMatrix(T_trans, M1, FinalTransform);     // Final = T_trans * M1

        glPushMatrix();
        glMultMatrixf(FinalTransform);
        DrawCube();
        glPopMatrix();

        // Draw active viewport border (unchanged)
         if (i == activeViewport) {
            glMatrixMode(GL_PROJECTION); glPushMatrix(); glLoadIdentity();
            glOrtho(0, vpWidth, 0, windowHeight, -1, 1);
            glMatrixMode(GL_MODELVIEW); glPushMatrix(); glLoadIdentity();
            glColor3f(1.0, 0.5, 0.0); glLineWidth(3.0);
            glBegin(GL_LINE_LOOP);
            glVertex2i(2, 2); glVertex2i(vpWidth - 2, 2);
            glVertex2i(vpWidth - 2, windowHeight - 2); glVertex2i(2, windowHeight - 2);
            glEnd();
            glPopMatrix(); glMatrixMode(GL_PROJECTION); glPopMatrix();
            glMatrixMode(GL_MODELVIEW);
        }
    }
    glutSwapBuffers();
}

void KeyPress(unsigned char key, int x, int y) { // Unchanged
    ViewportState& current = viewports[activeViewport];
    switch (key) {
        case 'U': current.angle += 5.0f; break;
        case 'u': current.angle -= 5.0f; break;
        case 'W': case 'w': current.transY += 0.1f; break;
        case 'S': case 's': current.transY -= 0.1f; break;
        case 'A': case 'a': current.transX -= 0.1f; break;
        case 'D': case 'd': current.transX += 0.1f; break;
        case 'Q': case 'q': current.transZ += 0.1f; break;
        case 'E': case 'e': current.transZ -= 0.1f; break;
        case 'T': case 't': current.scale *= 1.1f; break;
        case 'R': case 'r': current.scale /= 1.1f; break;
        case ' ':
            current.transX = current.transY = current.transZ = 0.0f;
            current.angle = 0.0f;
            current.scale = 1.0f;
            current.useCustomAxis = false;
            break;
        case 27: case 'X': case 'x':
             cout << "Exiting..." << endl;
             exit(0);
             break;
        case '\t':
            activeViewport = (activeViewport + 1) % 2;
            cout << "Active Viewport: " << (activeViewport == 0 ? "Left" : "Right") << endl;
            break;
    }
    glutPostRedisplay();
}

void MouseClick(int button, int state, int x, int y) {
    if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN) {
        activeViewport = (x < windowWidth / 2) ? 0 : 1;
        cout << "Clicked in Viewport: " << (activeViewport == 0 ? "Left" : "Right") << endl;

        // 1. Get matrices and viewport for the active viewport
        GLdouble modelviewMatrix[16], projectionMatrix[16];
        GLint viewportParams[4];
        int vpX = (activeViewport == 0) ? 0 : windowWidth / 2;
        int vpWidth = windowWidth / 2;

        // Set context before getting matrices
        glViewport(vpX, 0, vpWidth, windowHeight);
        glMatrixMode(GL_PROJECTION); glLoadIdentity();
         if (windowHeight > 0 && vpWidth > 0) {
             gluPerspective(45.0, (double)vpWidth / (double)windowHeight, 0.1, 500.0); // Match RenderScene
         }
        glGetDoublev(GL_PROJECTION_MATRIX, projectionMatrix);
        glMatrixMode(GL_MODELVIEW); glLoadIdentity();
        gluLookAt(viewports[activeViewport].eyeX, viewports[activeViewport].eyeY, viewports[activeViewport].eyeZ,
                  viewports[activeViewport].centerX, viewports[activeViewport].centerY, viewports[activeViewport].centerZ,
                  viewports[activeViewport].upX, viewports[activeViewport].upY, viewports[activeViewport].upZ);
        glGetDoublev(GL_MODELVIEW_MATRIX, modelviewMatrix);
        glGetIntegerv(GL_VIEWPORT, viewportParams);

        // 2. --- Calculate winZ corresponding to world origin (0,0,0) ---
        GLdouble originWinX, originWinY, originWinZ;
        gluProject(0.0, 0.0, 0.0, // World origin
                   modelviewMatrix, projectionMatrix, viewportParams,
                   &originWinX, &originWinY, &originWinZ);

        // Clamp winZ between 0 and 1 (it should be, but safety first)
        // Use the manual clamp function defined at the top
        GLfloat targetWinZ = clamp((GLfloat)originWinZ, 0.0f, 1.0f);
        // If origin is behind camera, originWinZ might be < 0 or > 1.
        // Defaulting to a mid-range value might be better in that edge case.
        if (originWinZ < 0.0 || originWinZ > 1.0) {
             cout << "Warning: World origin projected outside depth range. Using winZ=0.5." << endl;
             targetWinZ = 0.5f;
        }


        // 3. Unproject the actual mouse click using the calculated targetWinZ
        GLfloat winX = (GLfloat)x;
        GLfloat winY = (GLfloat)viewportParams[3] - (GLfloat)y; // Invert Y
        GLdouble worldX, worldY, worldZ; // This will be point P

        int result = gluUnProject(winX, winY, targetWinZ, // Use targetWinZ
                                  modelviewMatrix, projectionMatrix, viewportParams,
                                  &worldX, &worldY, &worldZ);

        if (result == GL_TRUE) {
            ViewportState& current = viewports[activeViewport];

            // Store the calculated point P as v1
            current.v1[0] = (float)worldX;
            current.v1[1] = (float)worldY;
            current.v1[2] = (float)worldZ;

            // Store -P as v2
            current.v2[0] = -current.v1[0];
            current.v2[1] = -current.v1[1];
            current.v2[2] = -current.v1[2];

            // Calculate and store the normalized direction P-Origin (which is just P)
            float direction[3] = {current.v1[0], current.v1[1], current.v1[2]};
            Normalize(direction);
            current.customAxisDirection[0] = direction[0];
            current.customAxisDirection[1] = direction[1];
            current.customAxisDirection[2] = direction[2];

            current.useCustomAxis = true;
            current.angle = 0.0f; // Reset angle

            cout << "Set Custom Axis Point P (v1): (" << current.v1[0] << ", " << current.v1[1] << ", " << current.v1[2] << ")" << endl;
            cout << "                     -P (v2): (" << current.v2[0] << ", " << current.v2[1] << ", " << current.v2[2] << ")" << endl;
            cout << "        Normalized Direction: (" << current.customAxisDirection[0] << ", "
                 << current.customAxisDirection[1] << ", " << current.customAxisDirection[2] << ")" << endl;

        } else {
            cerr << "Error in gluUnProject!" << endl;
        }

        glutPostRedisplay();
    }
}

void ChangeSize(int w, int h) { // Unchanged
    windowWidth = w;
    windowHeight = h;
    glutPostRedisplay();
}

void InitGL() { // Unchanged
    glEnable(GL_DEPTH_TEST);
    glClearColor(0.2f, 0.2f, 0.2f, 1.0f);
    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);
    glFrontFace(GL_CCW);
    glShadeModel(GL_FLAT);
    // Enable point smoothing for nicer points
    glEnable(GL_POINT_SMOOTH);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

}

int main(int argc, char** argv) { // Adjusted instructions
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH | GLUT_MULTISAMPLE); // Added Multisample
    glutInitWindowSize(windowWidth, windowHeight);
    glutInitWindowPosition(100, 100);
    glutCreateWindow("Dual Viewport - Arbitrary Rotation - Corrected Point");

    InitGL();

    glutDisplayFunc(RenderScene);
    glutReshapeFunc(ChangeSize);
    glutKeyboardFunc(KeyPress);
    glutMouseFunc(MouseClick);

    cout << " --- Controls ---" << endl;
    cout << " Mouse Left Click: Define axis from P(x,y,z) to -P(-x,-y,-z) in clicked viewport." << endl;
    cout << "                   P is the world point corresponding to the click at the depth of the world origin." << endl;
    cout << " Tab Key: Switch active viewport." << endl;
    cout << " U / u: Rotate object around current axis (origin) in active viewport." << endl;
    cout << " W/A/S/D/Q/E: Translate object in active viewport." << endl;
    cout << " T / R: Scale object in active viewport." << endl;
    cout << " Spacebar: Reset active viewport's object transforms and axis." << endl;
    cout << " X / Esc: Exit." << endl;
    cout << " Active Viewport: Left" << endl;

    glutMainLoop();
    return 0;
}