#include <GL/glut.h>
#include <math.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

// M3D_PI can be used for conversion from degrees to radians
#define M3D_PI (3.14159265358979323846)

// 3D Vector and Matrix types
typedef float M3DVector3f[3];
typedef float M3DVector4f[4];
typedef float M3DMatrix44f[16];

// Frame class for managing transformations (camera and objects)
class GLFrame
{
public:
    GLFrame() {
        vOrigin[0] = 0.0f; vOrigin[1] = 0.0f; vOrigin[2] = 0.0f;
        vForward[0] = 0.0f; vForward[1] = 0.0f; vForward[2] = -1.0f;
        vUp[0] = 0.0f; vUp[1] = 1.0f; vUp[2] = 0.0f;
    }

    void SetOrigin(float x, float y, float z) {
        vOrigin[0] = x; vOrigin[1] = y; vOrigin[2] = z;
    }

    void GetOrigin(M3DVector3f vPoint) {
        memcpy(vPoint, vOrigin, sizeof(M3DVector3f));
    }

    void MoveForward(float fDelta) {
        vOrigin[0] += vForward[0] * fDelta;
        vOrigin[1] += vForward[1] * fDelta;
        vOrigin[2] += vForward[2] * fDelta;
    }

    void RotateLocalY(float fAngle) {
        float rad = (fAngle * M3D_PI) / 180.0f;
        float cos_a = cos(rad);
        float sin_a = sin(rad);
        float new_x = vForward[0] * cos_a + vForward[2] * sin_a;
        float new_z = vForward[2] * cos_a - vForward[0] * sin_a;
        vForward[0] = new_x;
        vForward[2] = new_z;
    }

    void ApplyCameraTransform() {
        gluLookAt(vOrigin[0], vOrigin[1], vOrigin[2],
                  vOrigin[0] + vForward[0], vOrigin[1] + vForward[1], vOrigin[2] + vForward[2],
                  vUp[0], vUp[1], vUp[2]);
    }

    void ApplyActorTransform() {
        glTranslatef(vOrigin[0], vOrigin[1], vOrigin[2]);
    }

protected:
    M3DVector3f vOrigin;
    M3DVector3f vForward;
    M3DVector3f vUp;
};


// TGA File Loading function
//- Minimal TGA loader. Supports 24/32 bit uncompressed TGA files.
GLbyte* gltLoadTGA(const char *szFileName, GLint *iWidth, GLint *iHeight, GLint *iComponents, GLenum *eFormat)
{
    FILE *pFile;
    #pragma pack(1)
    typedef struct {
        GLbyte identsize;
        GLbyte colormaptype;
        GLbyte imagetype;
        unsigned short colormapstart;
        unsigned short colormaplength;
        unsigned char colormapbits;
        unsigned short xstart;
        unsigned short ystart;
        unsigned short width;
        unsigned short height;
        GLbyte bits;
        GLbyte descriptor;
    } TGAHEADER;
    #pragma pack(8)
    TGAHEADER tgaHeader;
    unsigned long lImageSize;
    GLbyte *pBits = NULL;

    pFile = fopen(szFileName, "rb");
    if(pFile == NULL) return NULL;

    fread(&tgaHeader, sizeof(TGAHEADER), 1, pFile);

    *iWidth = tgaHeader.width;
    *iHeight = tgaHeader.height;

    if(tgaHeader.imagetype != 2) { fclose(pFile); return NULL; }

    if(tgaHeader.bits == 24) {
        *iComponents = GL_RGB;
        *eFormat = GL_BGR_EXT;
    } else if(tgaHeader.bits == 32) {
        *iComponents = GL_RGBA;
        *eFormat = GL_BGRA_EXT;
    } else {
        fclose(pFile);
        return NULL;
    }

    lImageSize = (unsigned long)*iWidth * (unsigned long)*iHeight * (*iComponents == GL_RGB ? 3 : 4);
    pBits = (GLbyte*)malloc(lImageSize * sizeof(GLbyte));
    if(pBits == NULL) { fclose(pFile); return NULL; }

    if(fread(pBits, lImageSize, 1, pFile) != 1) {
        free(pBits);
        pBits = NULL;
    }

    fclose(pFile);
    return pBits;
}

// Math functions for shadow matrix
void m3dCrossProduct(M3DVector3f result, const M3DVector3f u, const M3DVector3f v) {
    result[0] = u[1]*v[2] - v[1]*u[2];
    result[1] = -u[0]*v[2] + v[0]*u[2];
    result[2] = u[0]*v[1] - v[0]*u[1];
}

void m3dNormalizeVector(M3DVector3f v) {
    float mag = sqrt(v[0]*v[0] + v[1]*v[1] + v[2]*v[2]);
    if (mag > 0.0f) { v[0] /= mag; v[1] /= mag; v[2] /= mag; }
}

void m3dGetPlaneEquation(M3DVector4f planeEq, const M3DVector3f p1, const M3DVector3f p2, const M3DVector3f p3) {
    M3DVector3f v1, v2;
    v1[0] = p2[0] - p1[0]; v1[1] = p2[1] - p1[1]; v1[2] = p2[2] - p1[2];
    v2[0] = p3[0] - p1[0]; v2[1] = p3[1] - p1[1]; v2[2] = p3[2] - p1[2];
    m3dCrossProduct(planeEq, v1, v2);
    m3dNormalizeVector(planeEq);
    planeEq[3] = -(planeEq[0]*p1[0] + planeEq[1]*p1[1] + planeEq[2]*p1[2]);
}

void m3dMakePlanarShadowMatrix(M3DMatrix44f proj, const M3DVector4f planeEq, const M3DVector3f vLightPos) {
    float dot = planeEq[0] * vLightPos[0] + planeEq[1] * vLightPos[1] + planeEq[2] * vLightPos[2] + planeEq[3];
    proj[0] = dot - vLightPos[0] * planeEq[0];
    proj[4] = 0.0f - vLightPos[0] * planeEq[1];
    proj[8] = 0.0f - vLightPos[0] * planeEq[2];
    proj[12] = 0.0f - vLightPos[0] * planeEq[3];
    proj[1] = 0.0f - vLightPos[1] * planeEq[0];
    proj[5] = dot - vLightPos[1] * planeEq[1];
    proj[9] = 0.0f - vLightPos[1] * planeEq[2];
    proj[13] = 0.0f - vLightPos[1] * planeEq[3];
    proj[2] = 0.0f - vLightPos[2] * planeEq[0];
    proj[6] = 0.0f - vLightPos[2] * planeEq[1];
    proj[10] = dot - vLightPos[2] * planeEq[2];
    proj[14] = 0.0f - vLightPos[2] * planeEq[3];
    proj[3] = 0.0f - 1.0f * planeEq[0];
    proj[7] = 0.0f - 1.0f * planeEq[1];
    proj[11] = 0.0f - 1.0f * planeEq[2];
    proj[15] = dot - 1.0f * planeEq[3];
}

#define NUM_SPHERES      1
GLFrame    spheres[NUM_SPHERES];
GLFrame    frameCamera;
bool       bPaused = false;
GLUquadricObj *pSphere = NULL;

// 用於公轉的軌道參數
M3DVector3f vRobotPos = {0.0f, 0.0f, -2.5f}; // 機器人中心位置 (公轉中心)
GLfloat sphereRadii[NUM_SPHERES];    // 每個球體的公轉半徑
GLfloat sphereAngles[NUM_SPHERES];   // 每個球體當前的公轉角度
GLfloat sphereSpeeds[NUM_SPHERES];   // 每個球體的公轉速度

// Light and material Data
GLfloat fLightPos[4]   = { -100.0f, 100.0f, 50.0f, 1.0f };
GLfloat fNoLight[] = { 0.0f, 0.0f, 0.0f, 0.0f };
GLfloat fLowLight[] = { 0.25f, 0.25f, 0.25f, 1.0f };
GLfloat fBrightLight[] = { 1.0f, 1.0f, 1.0f, 1.0f };

M3DMatrix44f mShadowMatrix;

// Texture objects
#define GROUND_TEXTURE  0
#define WOOD_TEXTURE    1
#define SPHERE_TEXTURE  2
#define NUM_TEXTURES    3
GLuint  textureObjects[NUM_TEXTURES];
const char *szTextureFiles[] = {"grass.tga", "wood.tga", "orb.tga"};

// Robot animation variables
GLfloat fTorsoSpin = 0.0f;
GLfloat fArmSwing = 0.0f;
GLfloat fLegWalk = 0.0f;


// Draw a cube with texture coordinates on each face
void DrawTexturedCube(GLfloat size)
{
    GLfloat s = size * 0.5f;
    glBegin(GL_QUADS);
        // Front Face
        glNormal3f(0.0f, 0.0f, 1.0f);
        glTexCoord2f(0.0f, 0.0f); glVertex3f(-s, -s, s);
        glTexCoord2f(1.0f, 0.0f); glVertex3f(s, -s, s);
        glTexCoord2f(1.0f, 1.0f); glVertex3f(s, s, s);
        glTexCoord2f(0.0f, 1.0f); glVertex3f(-s, s, s);
        // Back Face
        glNormal3f(0.0f, 0.0f, -1.0f);
        glTexCoord2f(1.0f, 0.0f); glVertex3f(-s, -s, -s);
        glTexCoord2f(1.0f, 1.0f); glVertex3f(-s, s, -s);
        glTexCoord2f(0.0f, 1.0f); glVertex3f(s, s, -s);
        glTexCoord2f(0.0f, 0.0f); glVertex3f(s, -s, -s);
        // Top Face
        glNormal3f(0.0f, 1.0f, 0.0f);
        glTexCoord2f(0.0f, 1.0f); glVertex3f(-s, s, -s);
        glTexCoord2f(0.0f, 0.0f); glVertex3f(-s, s, s);
        glTexCoord2f(1.0f, 0.0f); glVertex3f(s, s, s);
        glTexCoord2f(1.0f, 1.0f); glVertex3f(s, s, -s);
        // Bottom Face
        glNormal3f(0.0f, -1.0f, 0.0f);
        glTexCoord2f(1.0f, 1.0f); glVertex3f(-s, -s, -s);
        glTexCoord2f(0.0f, 1.0f); glVertex3f(s, -s, -s);
        glTexCoord2f(0.0f, 0.0f); glVertex3f(s, -s, s);
        glTexCoord2f(1.0f, 0.0f); glVertex3f(-s, -s, s);
        // Right face
        glNormal3f(1.0f, 0.0f, 0.0f);
        glTexCoord2f(1.0f, 0.0f); glVertex3f(s, -s, -s);
        glTexCoord2f(1.0f, 1.0f); glVertex3f(s, s, -s);
        glTexCoord2f(0.0f, 1.0f); glVertex3f(s, s, s);
        glTexCoord2f(0.0f, 0.0f); glVertex3f(s, -s, s);
        // Left Face
        glNormal3f(-1.0f, 0.0f, 0.0f);
        glTexCoord2f(0.0f, 0.0f); glVertex3f(-s, -s, -s);
        glTexCoord2f(1.0f, 0.0f); glVertex3f(-s, -s, s);
        glTexCoord2f(1.0f, 1.0f); glVertex3f(-s, s, s);
        glTexCoord2f(0.0f, 1.0f); glVertex3f(-s, s, -s);
    glEnd();
}

// Draw the animated robot
void DrawRobot(GLint nShadow)
{
    if(nShadow == 0) {
        // With no shadow, robot is textured and specular.
        // Bind the wood texture once for the entire robot.
        glBindTexture(GL_TEXTURE_2D, textureObjects[WOOD_TEXTURE]);
        glMaterialfv(GL_FRONT, GL_SPECULAR, fBrightLight);
    }

    glPushMatrix();
    // Center the robot and spin its torso
    glTranslatef(vRobotPos[0], vRobotPos[1], vRobotPos[2]); // 使用 vRobotPos 變數來定位
    glRotatef(fTorsoSpin, 0.0f, 1.0f, 0.0f);

    // Draw Torso
    glPushMatrix();
    glTranslatef(0.0f, 0.5f, 0.0f);
    glScalef(0.6f, 0.8f, 0.5f);
    DrawTexturedCube(0.9f);
    glPopMatrix();

    // Draw Head
    glPushMatrix();
    glTranslatef(0.0f, 1.0f, 0.0f);
    gluSphere(pSphere, 0.2f, 21, 11);
    glPopMatrix();

    // Draw Right Arm
    glPushMatrix();
    glTranslatef(0.45f, 0.5f, 0.0f);
    glRotatef(fArmSwing, 1.0f, 0.0f, 0.0f);
    glScalef(0.2f, 0.5f, 0.2f);
    DrawTexturedCube(1.0f);
    glPopMatrix();

    // Draw Left Arm
    glPushMatrix();
    glTranslatef(-0.45f, 0.5f, 0.0f);
    glRotatef(-fArmSwing, 1.0f, 0.0f, 0.0f);
    glScalef(0.2f, 0.5f, 0.2f);
    DrawTexturedCube(1.0f);
    glPopMatrix();

    // Draw Right Leg
    glPushMatrix();
    glTranslatef(0.2f, -0.1f, 0.0f);
    glRotatef(fLegWalk, 1.0f, 0.0f, 0.0f);
    glScalef(0.25f, 0.5f, 0.25f);
    DrawTexturedCube(1.0f);
    glPopMatrix();

    // Draw Left Leg
    glPushMatrix();
    glTranslatef(-0.2f, -0.1f, 0.0f);
    glRotatef(-fLegWalk, 1.0f, 0.0f, 0.0f);
    glScalef(0.25f, 0.5f, 0.25f);
    DrawTexturedCube(1.0f);
    glPopMatrix();

    glPopMatrix(); // Pop the main robot matrix

    if(nShadow == 0) {
        // Reset material properties
        glMaterialfv(GL_FRONT, GL_SPECULAR, fNoLight);
    }
}

void SetupRC()
{
    M3DVector3f vPoints[3] = {{ 0.0f, -0.4f, 0.0f },
                             { 10.0f, -0.4f, 0.0f },
                             { 5.0f, -0.4f, -5.0f }};
    int i;

    glClearColor(fLowLight[0], fLowLight[1], fLowLight[2], fLowLight[3]);

    glStencilOp(GL_INCR, GL_INCR, GL_INCR);
    glClearStencil(0);
    glStencilFunc(GL_EQUAL, 0x0, 0x01);

    glCullFace(GL_BACK);
    glFrontFace(GL_CCW);
    glEnable(GL_CULL_FACE);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_MULTISAMPLE_ARB);

    glLightModelfv(GL_LIGHT_MODEL_AMBIENT, fNoLight);
    glLightfv(GL_LIGHT0, GL_AMBIENT, fLowLight);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, fBrightLight);
    glLightfv(GL_LIGHT0, GL_SPECULAR, fBrightLight);
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);

    M3DVector4f pPlane;
    m3dGetPlaneEquation(pPlane, vPoints[0], vPoints[1], vPoints[2]);
    m3dMakePlanarShadowMatrix(mShadowMatrix, pPlane, fLightPos);

    glEnable(GL_COLOR_MATERIAL);
    glColorMaterial(GL_FRONT, GL_AMBIENT_AND_DIFFUSE);
    glMaterialfv(GL_FRONT, GL_SPECULAR, fBrightLight);
    glMateriali(GL_FRONT, GL_SHININESS, 128);

    // << 修改: 為唯一的球體設定固定的軌道參數 >>
    // 由於只有一顆球，我們不再使用隨機值，而是給它一個固定的軌道
    sphereRadii[0] = 5.0f;    // 公轉半徑為 5.0
    sphereAngles[0] = 0.0f;   // 初始角度為 0
    sphereSpeeds[0] = 0.01f;  // 設定一個適中的公轉速度

    glEnable(GL_TEXTURE_2D);
    glGenTextures(NUM_TEXTURES, textureObjects);
    glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);

    for(i = 0; i < NUM_TEXTURES; i++)
    {
        GLbyte *pBytes;
        GLint iWidth, iHeight, iComponents;
        GLenum eFormat;

        glBindTexture(GL_TEXTURE_2D, textureObjects[i]);

        pBytes = gltLoadTGA(szTextureFiles[i], &iWidth, &iHeight, &iComponents, &eFormat);
        if(pBytes == NULL) {
            fprintf(stderr, "Error loading texture: %s\n", szTextureFiles[i]);
            exit(1);
        }

        gluBuild2DMipmaps(GL_TEXTURE_2D, iComponents, iWidth, iHeight, eFormat, GL_UNSIGNED_BYTE, pBytes);
        free(pBytes);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    }

    pSphere = gluNewQuadric();
    gluQuadricDrawStyle(pSphere, GLU_FILL);
    gluQuadricNormals(pSphere, GLU_SMOOTH);
    gluQuadricTexture(pSphere, GL_TRUE);
}

void ShutdownRC(void)
{
    if(pSphere != NULL)
        gluDeleteQuadric(pSphere);
    glDeleteTextures(NUM_TEXTURES, textureObjects);
}

void DrawGround(void)
{
    GLfloat fExtent = 20.0f;
    GLfloat fStep = 1.0f;
    GLfloat y = -0.4f;
    GLfloat iStrip, iRun;
    GLfloat s = 0.0f;
    GLfloat t = 0.0f;
    GLfloat texStep = 1.0f / (fExtent * .075f);

    glBindTexture(GL_TEXTURE_2D, textureObjects[GROUND_TEXTURE]);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

    for(iStrip = -fExtent; iStrip <= fExtent; iStrip += fStep)
    {
        t = 0.0f;
        glBegin(GL_TRIANGLE_STRIP);
            for(iRun = fExtent; iRun >= -fExtent; iRun -= fStep)
            {
                glTexCoord2f(s, t);
                glNormal3f(0.0f, 1.0f, 0.0f);
                glVertex3f(iStrip, y, iRun);

                glTexCoord2f(s + texStep, t);
                glNormal3f(0.0f, 1.0f, 0.0f);
                glVertex3f(iStrip + fStep, y, iRun);

                t += texStep;
            }
        glEnd();
        s += texStep;
    }
}

void DrawInhabitants(GLint nShadow)
{
    GLint i;

    if(nShadow == 0)
        glColor4f(1.0f, 1.0f, 1.0f, 1.0f);
    else
        glColor4f(0.0f, 0.0f, 0.0f, 0.6f);

    if(nShadow == 0) {
        glMaterialfv(GL_FRONT, GL_SPECULAR, fBrightLight);
        glBindTexture(GL_TEXTURE_2D, textureObjects[SPHERE_TEXTURE]);
    }

    // 這個迴圈現在只會執行一次
    for(i = 0; i < NUM_SPHERES; i++)
    {
        glPushMatrix();
        spheres[i].ApplyActorTransform();
        gluSphere(pSphere, 0.3f, 21, 11);
        glPopMatrix();
    }

    if(nShadow == 0)
        glMaterialfv(GL_FRONT, GL_SPECULAR, fNoLight);

    DrawRobot(nShadow);
}

void RenderScene(void)
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

    glPushMatrix();
        frameCamera.ApplyCameraTransform();
        glLightfv(GL_LIGHT0, GL_POSITION, fLightPos);

        glColor3f(1.0f, 1.0f, 1.0f);
        DrawGround();

        glDisable(GL_DEPTH_TEST);
        glDisable(GL_LIGHTING);
        glDisable(GL_TEXTURE_2D);
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        glEnable(GL_STENCIL_TEST);
        glPushMatrix();
            glMultMatrixf(mShadowMatrix);
            DrawInhabitants(1);
        glPopMatrix();
        glDisable(GL_STENCIL_TEST);
        glDisable(GL_BLEND);
        glEnable(GL_LIGHTING);
        glEnable(GL_TEXTURE_2D);
        glEnable(GL_DEPTH_TEST);

        DrawInhabitants(0);

    glPopMatrix();
    glutSwapBuffers();
}

void SpecialKeys(int key, int x, int y)
{
    if(key == GLUT_KEY_UP) frameCamera.MoveForward(0.1f);
    if(key == GLUT_KEY_DOWN) frameCamera.MoveForward(-0.1f);
    if(key == GLUT_KEY_LEFT) frameCamera.RotateLocalY(0.1f);
    if(key == GLUT_KEY_RIGHT) frameCamera.RotateLocalY(-0.1f);
}

void Keyboard(unsigned char key, int x, int y)
{
    if(key == 'p' || key == 'P' || key == ' ')
        bPaused = !bPaused;
}

void TimerFunction(int value)
{
    if(!bPaused)
    {
        // 機器人動畫
        fTorsoSpin += 0.5f;
        if(fTorsoSpin > 360.0f) fTorsoSpin = 0.0f;

        fArmSwing = sin(glutGet(GLUT_ELAPSED_TIME) / 500.0) * 45.0;
        fLegWalk = sin(glutGet(GLUT_ELAPSED_TIME) / 500.0) * -35.0;

        // 更新球體的位置，讓它公轉 (迴圈只會跑一次)
        for(int i = 0; i < NUM_SPHERES; i++)
        {
            // 1. 更新角度 (角度 = 角度 + 速度)
            sphereAngles[i] += sphereSpeeds[i];

            // 2. 使用三角函數計算新的 x 和 z 坐標
            GLfloat x = vRobotPos[0] + sphereRadii[i] * cos(sphereAngles[i]);
            GLfloat z = vRobotPos[2] + sphereRadii[i] * sin(sphereAngles[i]);

            // 3. 更新球體的位置
            spheres[i].SetOrigin(x, 0.0f, z);
        }
    }

    glutPostRedisplay();
    glutTimerFunc(16, TimerFunction, 1);
}

void ChangeSize(int w, int h)
{
    if(h == 0) h = 1;
    glViewport(0, 0, w, h);
    GLfloat fAspect = (GLfloat)w / (GLfloat)h;
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(35.0f, fAspect, 1.0f, 50.0f);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
}

int main(int argc, char* argv[])
{
    // srand() 在這裡不是必須的，因為我們不再使用隨機值了，但保留也無妨
    srand(glutGet(GLUT_ELAPSED_TIME));

    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH | GLUT_STENCIL | GLUT_MULTISAMPLE);
    glutInitWindowSize(800, 600);
    glutCreateWindow("SphereWorld: Robot with a single Moon");

    glutReshapeFunc(ChangeSize);
    glutDisplayFunc(RenderScene);
    glutSpecialFunc(SpecialKeys);
    glutKeyboardFunc(Keyboard);

    SetupRC();
    glutTimerFunc(33, TimerFunction, 1);

    glutMainLoop();
    ShutdownRC();
    return 0;
}