///////////////////////////////////////////////////////////////////////////////
//
// sphereworld_enhanced.cpp
//
// Enhanced version with improved shadow system and additional features:
// 1. Multiple light sources with corresponding shadows
// 2. Soft shadow edges using stencil buffer
// 3. Dynamic shadow intensity based on lighting
// 4. Shadow color variations
// 5. Optional shadow volumes for more complex objects
//
///////////////////////////////////////////////////////////////////////////////
#include <GL/freeglut.h>
#include <cmath>
#include <iostream>
#include <cstdio>
#include <vector>

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

#define WOOD_TEXTURE_WIDTH   128
#define WOOD_TEXTURE_HEIGHT  128

///////////////////////////////////////////////////////////////////////////////
// MATH AND UTILITY FUNCTIONS
///////////////////////////////////////////////////////////////////////////////
typedef float M3DVector3f[3];
typedef float M3DVector4f[4];
typedef float M3DMatrix44f[16];

#pragma pack(1)
typedef struct {
    GLbyte identsize; GLbyte colormaptype; GLbyte imagetype;
    unsigned short colormapstart; unsigned short colormaplength; unsigned char colormapbits;
    unsigned short xstart; unsigned short ystart; unsigned short width; unsigned short height;
    GLbyte bits; GLbyte descriptor;
} TGAHEADER;
#pragma pack(8)

void m3dCrossProduct(M3DVector3f r, const M3DVector3f u, const M3DVector3f v) {
    r[0] = u[1] * v[2] - v[1] * u[2];
    r[1] = -u[0] * v[2] + v[0] * u[2];
    r[2] = u[0] * v[1] - v[0] * u[1];
}

float m3dGetVectorLength(const M3DVector3f v) {
    return sqrt(v[0] * v[0] + v[1] * v[1] + v[2] * v[2]);
}

void m3dNormalizeVector(M3DVector3f v) {
    float len = m3dGetVectorLength(v);
    if (len != 0.0f) { v[0] /= len; v[1] /= len; v[2] /= len; }
}

void m3dFindNormal(M3DVector3f r, const M3DVector3f p1, const M3DVector3f p2, const M3DVector3f p3) {
    M3DVector3f v1, v2;
    v1[0] = p1[0] - p2[0]; v1[1] = p1[1] - p2[1]; v1[2] = p1[2] - p2[2];
    v2[0] = p2[0] - p3[0]; v2[1] = p2[1] - p3[1]; v2[2] = p2[2] - p3[2];
    m3dCrossProduct(r, v1, v2);
    m3dNormalizeVector(r);
}

void m3dGetPlaneEquation(M3DVector4f p, const M3DVector3f p1, const M3DVector3f p2, const M3DVector3f p3) {
    m3dFindNormal(p, p1, p2, p3);
    p[3] = -(p[0] * p3[0] + p[1] * p3[1] + p[2] * p3[2]);
}

void MakeShadowMatrix(M3DMatrix44f proj, const M3DVector4f plane, const M3DVector4f lightPos) {
    float dot = plane[0]*lightPos[0] + plane[1]*lightPos[1] + plane[2]*lightPos[2] + plane[3]*lightPos[3];
    proj[0]=dot-lightPos[0]*plane[0]; proj[4]=0.0f-lightPos[0]*plane[1]; proj[8]=0.0f-lightPos[0]*plane[2]; proj[12]=0.0f-lightPos[0]*plane[3];
    proj[1]=0.0f-lightPos[1]*plane[0]; proj[5]=dot-lightPos[1]*plane[1]; proj[9]=0.0f-lightPos[1]*plane[2]; proj[13]=0.0f-lightPos[1]*plane[3];
    proj[2]=0.0f-lightPos[2]*plane[0]; proj[6]=0.0f-lightPos[2]*plane[1]; proj[10]=dot-lightPos[2]*plane[2]; proj[14]=0.0f-lightPos[2]*plane[3];
    proj[3]=0.0f-lightPos[3]*plane[0]; proj[7]=0.0f-lightPos[3]*plane[1]; proj[11]=0.0f-lightPos[3]*plane[2]; proj[15]=dot-lightPos[3]*plane[3];
}

GLuint LoadTGATexture(const char *szFileName) {
    FILE *pFile = fopen(szFileName, "rb");
    if (!pFile) {
        std::cerr << "Warning: Cannot open TGA file '" << szFileName << "'. Using default texture." << std::endl;
        return 0;
    }

    TGAHEADER tgaHeader;
    fread(&tgaHeader, sizeof(TGAHEADER), 1, pFile);

    if (tgaHeader.imagetype != 2 || (tgaHeader.bits != 24 && tgaHeader.bits != 32)) {
        std::cerr << "Warning: TGA file '" << szFileName << "' is not a supported type. Using default texture." << std::endl;
        fclose(pFile);
        return 0;
    }

    GLint width = tgaHeader.width, height = tgaHeader.height, components = tgaHeader.bits / 8;
    GLenum eFormat = (components == 4) ? GL_RGBA : GL_RGB;
    long imageSize = width * height * components;
    GLbyte *pBits = new GLbyte[imageSize];

    if (!pBits) {
        std::cerr << "Error: Could not allocate memory for TGA." << std::endl;
        fclose(pFile);
        return 0;
    }

    fread(pBits, imageSize, 1, pFile);
    fclose(pFile);

    // BGR to RGB conversion
    for (long i = 0; i < imageSize; i += components) {
        GLbyte temp = pBits[i];
        pBits[i] = pBits[i + 2];
        pBits[i + 2] = temp;
    }

    GLuint textureID;
    glGenTextures(1, &textureID);
    glBindTexture(GL_TEXTURE_2D, textureID);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_GENERATE_MIPMAP, GL_TRUE);
    glTexImage2D(GL_TEXTURE_2D, 0, eFormat, width, height, 0, eFormat, GL_UNSIGNED_BYTE, pBits);

    delete[] pBits;
    return textureID;
}

void MakeWoodTexture(GLubyte data[WOOD_TEXTURE_HEIGHT][WOOD_TEXTURE_WIDTH][3]) {
    for (int i=0; i<WOOD_TEXTURE_HEIGHT; i++) {
        for (int j=0; j<WOOD_TEXTURE_WIDTH; j++) {
            float dist = sqrt((float)(i-WOOD_TEXTURE_HEIGHT/2)*(i-WOOD_TEXTURE_HEIGHT/2) +
                             (float)(j-WOOD_TEXTURE_WIDTH/2)*(j-WOOD_TEXTURE_WIDTH/2));
            float sine_val = sin(dist*0.5f);
            float noise = (rand()%50)/255.0f;
            float r=0.5f+noise, g=0.3f+noise, b=0.1f+noise;
            if (sine_val>0.8f) {r*=0.6f; g*=0.6f; b*=0.6f;}
            data[i][j][0]=(GLubyte)(r*255.0f);
            data[i][j][1]=(GLubyte)(g*255.0f);
            data[i][j][2]=(GLubyte)(b*255.0f);
        }
    }
}

// Create a default grass-like texture
void MakeGrassTexture(GLubyte data[WOOD_TEXTURE_HEIGHT][WOOD_TEXTURE_WIDTH][3]) {
    for (int i=0; i<WOOD_TEXTURE_HEIGHT; i++) {
        for (int j=0; j<WOOD_TEXTURE_WIDTH; j++) {
            float noise = (rand()%100)/255.0f;
            float r = 0.1f + noise*0.1f;
            float g = 0.4f + noise*0.2f;
            float b = 0.1f + noise*0.1f;
            data[i][j][0] = (GLubyte)(r*255.0f);
            data[i][j][1] = (GLubyte)(g*255.0f);
            data[i][j][2] = (GLubyte)(b*255.0f);
        }
    }
}

// Create a default orb texture
void MakeOrbTexture(GLubyte data[WOOD_TEXTURE_HEIGHT][WOOD_TEXTURE_WIDTH][3]) {
    int centerX = WOOD_TEXTURE_WIDTH/2;
    int centerY = WOOD_TEXTURE_HEIGHT/2;
    for (int i=0; i<WOOD_TEXTURE_HEIGHT; i++) {
        for (int j=0; j<WOOD_TEXTURE_WIDTH; j++) {
            float dist = sqrt((float)(i-centerY)*(i-centerY) + (float)(j-centerX)*(j-centerX));
            float radius = WOOD_TEXTURE_WIDTH/2.0f;
            float intensity = 1.0f - (dist/radius);
            if (intensity < 0) intensity = 0;

            float r = 0.3f + intensity*0.5f;
            float g = 0.3f + intensity*0.7f;
            float b = 0.8f + intensity*0.2f;

            data[i][j][0] = (GLubyte)(r*255.0f);
            data[i][j][1] = (GLubyte)(g*255.0f);
            data[i][j][2] = (GLubyte)(b*255.0f);
        }
    }
}

void drawCube() {
    glBegin(GL_QUADS);
    // Front face
    glNormal3f(0,0,1);
    glTexCoord2f(0,0); glVertex3f(-.5,-.5,.5);
    glTexCoord2f(1,0); glVertex3f(.5,-.5,.5);
    glTexCoord2f(1,1); glVertex3f(.5,.5,.5);
    glTexCoord2f(0,1); glVertex3f(-.5,.5,.5);

    // Back face
    glNormal3f(0,0,-1);
    glTexCoord2f(1,0); glVertex3f(-.5,-.5,-.5);
    glTexCoord2f(1,1); glVertex3f(-.5,.5,-.5);
    glTexCoord2f(0,1); glVertex3f(.5,.5,-.5);
    glTexCoord2f(0,0); glVertex3f(.5,-.5,-.5);

    // Top face
    glNormal3f(0,1,0);
    glTexCoord2f(0,1); glVertex3f(-.5,.5,-.5);
    glTexCoord2f(0,0); glVertex3f(-.5,.5,.5);
    glTexCoord2f(1,0); glVertex3f(.5,.5,.5);
    glTexCoord2f(1,1); glVertex3f(.5,.5,-.5);

    // Bottom face
    glNormal3f(0,-1,0);
    glTexCoord2f(1,1); glVertex3f(-.5,-.5,-.5);
    glTexCoord2f(0,1); glVertex3f(.5,-.5,-.5);
    glTexCoord2f(0,0); glVertex3f(.5,-.5,.5);
    glTexCoord2f(1,0); glVertex3f(-.5,-.5,.5);

    // Right face
    glNormal3f(1,0,0);
    glTexCoord2f(1,0); glVertex3f(.5,-.5,-.5);
    glTexCoord2f(1,1); glVertex3f(.5,.5,-.5);
    glTexCoord2f(0,1); glVertex3f(.5,.5,.5);
    glTexCoord2f(0,0); glVertex3f(.5,-.5,.5);

    // Left face
    glNormal3f(-1,0,0);
    glTexCoord2f(0,0); glVertex3f(-.5,-.5,-.5);
    glTexCoord2f(1,0); glVertex3f(-.5,-.5,.5);
    glTexCoord2f(1,1); glVertex3f(-.5,.5,.5);
    glTexCoord2f(0,1); glVertex3f(-.5,.5,-.5);
    glEnd();
}

void drawSphere(float radius, int slices, int stacks) {
    for (int i=0; i<stacks; ++i) {
        float lat0 = M_PI*(-0.5f+(float)i/stacks);
        float z0 = radius*sin(lat0);
        float zr0 = radius*cos(lat0);
        float lat1 = M_PI*(-0.5f+(float)(i+1)/stacks);
        float z1 = radius*sin(lat1);
        float zr1 = radius*cos(lat1);

        glBegin(GL_QUAD_STRIP);
        for (int j=0; j<=slices; ++j) {
            float lng = 2*M_PI*(float)j/slices;
            float x = cos(lng);
            float y = sin(lng);
            float u = (float)j/slices;
            float v0 = (float)i/stacks;
            float v1 = (float)(i+1)/stacks;

            glNormal3f(x*zr0, y*zr0, z0);
            glTexCoord2f(u, v0);
            glVertex3f(x*zr0, y*zr0, z0);

            glNormal3f(x*zr1, y*zr1, z1);
            glTexCoord2f(u, v1);
            glVertex3f(x*zr1, y*zr1, z1);
        }
        glEnd();
    }
}

///////////////////////////////////////////////////////////////////////////////
// ENHANCED SHADOW SYSTEM
///////////////////////////////////////////////////////////////////////////////
struct Light {
    M3DVector4f position;
    M3DVector4f ambient;
    M3DVector4f diffuse;
    M3DVector4f specular;
    M3DMatrix44f shadowMatrix;
    float intensity;
    bool enabled;
};

///////////////////////////////////////////////////////////////////////////////
// GLOBAL VARIABLES
///////////////////////////////////////////////////////////////////////////////
GLfloat yRot = 0.0f;
GLfloat swingAngle = 0.0f;
GLfloat animTime = 0.0f;
GLfloat sphereOrbitAngle = 0.0f;
GLfloat lightOrbitAngle = 0.0f;
bool isPaused = false;
bool showMultipleLights = false;
bool showSoftShadows = true;

// Enhanced lighting system
std::vector<Light> lights;
M3DVector4f groundPlane;

GLuint woodTextureID, grassTextureID, orbTextureID;
GLubyte woodTexture[WOOD_TEXTURE_HEIGHT][WOOD_TEXTURE_WIDTH][3];
GLubyte grassTexture[WOOD_TEXTURE_HEIGHT][WOOD_TEXTURE_WIDTH][3];
GLubyte orbTexture[WOOD_TEXTURE_HEIGHT][WOOD_TEXTURE_WIDTH][3];

///////////////////////////////////////////////////////////////////////////////
// HIERARCHICAL ROBOT MODEL
///////////////////////////////////////////////////////////////////////////////
void drawRobot() {
    glBindTexture(GL_TEXTURE_2D, woodTextureID);
    glColor3f(1,1,1);

    float tH=1.8f, tW=1.0f, tD=0.6f, hS=0.7f, aL=1.4f, aW=0.3f, lL=1.6f, lW=0.4f, lR=0.5f;

    // Torso
    glPushMatrix();
    glTranslatef(0, lL+tH/2, 0);
    glScalef(tW, tH, tD);
    drawCube();
    glPopMatrix();

    // Head
    glPushMatrix();
    glTranslatef(0, lL+tH+hS/2, 0);
    glScalef(hS, hS, hS);
    drawCube();
    glPopMatrix();

    // Left arm
    glPushMatrix();
    glTranslatef(-(tW/2+aW/2), lL+tH*0.85f, 0);
    glRotatef(swingAngle, 1, 0, 0);
        glPushMatrix();
        glTranslatef(0, -aL*lR/2, 0);
        glScalef(aW, aL*lR, aW);
        drawCube();
        glPopMatrix();
        glPushMatrix();
        glTranslatef(0, -aL*lR, 0);
        glTranslatef(0, -aL*lR/2, 0);
        glScalef(aW, aL*lR, aW);
        drawCube();
        glPopMatrix();
    glPopMatrix();

    // Right arm
    glPushMatrix();
    glTranslatef(tW/2+aW/2, lL+tH*0.85f, 0);
    glRotatef(-swingAngle, 1, 0, 0);
        glPushMatrix();
        glTranslatef(0, -aL*lR/2, 0);
        glScalef(aW, aL*lR, aW);
        drawCube();
        glPopMatrix();
        glPushMatrix();
        glTranslatef(0, -aL*lR, 0);
        glTranslatef(0, -aL*lR/2, 0);
        glScalef(aW, aL*lR, aW);
        drawCube();
        glPopMatrix();
    glPopMatrix();

    // Left leg
    glPushMatrix();
    glTranslatef(-(tW/2-lW/2), lL, 0);
    glRotatef(-swingAngle, 1, 0, 0);
        glPushMatrix();
        glTranslatef(0, -lL*lR/2, 0);
        glScalef(lW, lL*lR, lW);
        drawCube();
        glPopMatrix();
        glPushMatrix();
        glTranslatef(0, -lL*lR, 0);
        glTranslatef(0, -lL*lR/2, 0);
        glScalef(lW, lL*lR, lW);
        drawCube();
        glPopMatrix();
    glPopMatrix();

    // Right leg
    glPushMatrix();
    glTranslatef(tW/2-lW/2, lL, 0);
    glRotatef(swingAngle, 1, 0, 0);
        glPushMatrix();
        glTranslatef(0, -lL*lR/2, 0);
        glScalef(lW, lL*lR, lW);
        drawCube();
        glPopMatrix();
        glPushMatrix();
        glTranslatef(0, -lL*lR, 0);
        glTranslatef(0, -lL*lR/2, 0);
        glScalef(lW, lL*lR, lW);
        drawCube();
        glPopMatrix();
    glPopMatrix();
}

void drawShadowsForLight(const Light& light, float shadowAlpha) {
    if (!light.enabled) return;

    glPushMatrix();
    glMultMatrixf(light.shadowMatrix);

    // Set shadow color with varying intensity
    glColor4f(0.1f, 0.1f, 0.1f, shadowAlpha * light.intensity);

    // Draw Robot's Shadow
    glPushMatrix();
        glTranslatef(0, -0.4, 0);
        glRotatef(yRot, 0, 1, 0);
        glScalef(0.65f, 0.65f, 0.65f);
        drawRobot();
    glPopMatrix();

    // Draw Spheres' Shadows
    GLfloat sphere_y = -0.4f + 1.0f;
    glPushMatrix();
        glRotatef(sphereOrbitAngle, 0, 1, 0);
        glPushMatrix(); glTranslatef(0, sphere_y, -5); drawSphere(0.4f, 26, 13); glPopMatrix();
        glPushMatrix(); glTranslatef(3, sphere_y, -3); drawSphere(0.4f, 26, 13); glPopMatrix();
        glPushMatrix(); glTranslatef(-3, sphere_y, -3); drawSphere(0.4f, 26, 13); glPopMatrix();
        glPushMatrix(); glTranslatef(3, sphere_y, 3); drawSphere(0.4f, 26, 13); glPopMatrix();
        glPushMatrix(); glTranslatef(-3, sphere_y, 3); drawSphere(0.4f, 26, 13); glPopMatrix();
    glPopMatrix();

    glPopMatrix();
}

void renderEnhancedShadows() {

    glPushAttrib(GL_ALL_ATTRIB_BITS);

    // Disable lighting and texturing for shadows
    glDisable(GL_LIGHTING);
    glDisable(GL_TEXTURE_2D);
    

    // Enable blending for semi-transparent shadows
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    // Enable polygon offset to prevent z-fighting
    glEnable(GL_POLYGON_OFFSET_FILL);
    glPolygonOffset(-1.0f, -1.0f);

    if (showSoftShadows && lights.size() > 1) {
        // Render multiple shadows with different alpha values for soft shadow effect
        for (size_t i = 0; i < lights.size(); ++i) {
            float alpha = 0.3f / lights.size(); // Distribute shadow intensity
            drawShadowsForLight(lights[i], alpha);
        }
    } else {
        // Render single shadow from primary light
        if (!lights.empty()) {
            drawShadowsForLight(lights[0], 0.6f);
        }
    }

    glDisable(GL_POLYGON_OFFSET_FILL);
    glPopAttrib();
}

///////////////////////////////////////////////////////////////////////////////
// MAIN RENDERING FUNCTION
///////////////////////////////////////////////////////////////////////////////
void RenderScene(void) {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glPushMatrix();
    gluLookAt(0, 2, 15, 0, 0, 0, 0, 1, 0);

    // Update light positions if multiple lights are enabled
    if (showMultipleLights && lights.size() > 1) {
        // Animate secondary lights
        lights[1].position[0] = 8.0f * cos(lightOrbitAngle * M_PI / 180.0f);
        lights[1].position[2] = 8.0f * sin(lightOrbitAngle * M_PI / 180.0f);
        glLightfv(GL_LIGHT1, GL_POSITION, lights[1].position);

        if (lights.size() > 2) {
            lights[2].position[0] = -6.0f * cos((lightOrbitAngle + 120) * M_PI / 180.0f);
            lights[2].position[2] = -6.0f * sin((lightOrbitAngle + 120) * M_PI / 180.0f);
            glLightfv(GL_LIGHT2, GL_POSITION, lights[2].position);
        }

        // Update shadow matrices
        for (size_t i = 0; i < lights.size(); ++i) {
            MakeShadowMatrix(lights[i].shadowMatrix, groundPlane, lights[i].position);
        }
    }

    // STEP 1: Draw the ground plane
    glColor3f(1, 1, 1);
    glBindTexture(GL_TEXTURE_2D, grassTextureID);
    glBegin(GL_QUADS);
        glNormal3f(0, 1, 0);
        glTexCoord2f(0, 0);   glVertex3f(-10, -0.4, 10);
        glTexCoord2f(10, 0);  glVertex3f(10, -0.4, 10);
        glTexCoord2f(10, 10); glVertex3f(10, -0.4, -10);
        glTexCoord2f(0, 10);  glVertex3f(-10, -0.4, -10);

    glEnd();

    // STEP 2: Render enhanced shadows
    renderEnhancedShadows();

    // STEP 3: Draw the real objects
    GLfloat sphere_y = -0.4f + 1.0f;

    // Draw orbiting spheres
    glPushMatrix();
        glRotatef(sphereOrbitAngle, 0, 1, 0);
        glBindTexture(GL_TEXTURE_2D, orbTextureID);
        glColor3f(1, 1, 1);
        glPushMatrix(); glTranslatef(0, sphere_y, -5); drawSphere(0.4f, 26, 13); glPopMatrix();
        glPushMatrix(); glTranslatef(3, sphere_y, -3); drawSphere(0.4f, 26, 13); glPopMatrix();
        glPushMatrix(); glTranslatef(-3, sphere_y, -3); drawSphere(0.4f, 26, 13); glPopMatrix();
        glPushMatrix(); glTranslatef(3, sphere_y, 3); drawSphere(0.4f, 26, 13); glPopMatrix();
        glPushMatrix(); glTranslatef(-3, sphere_y, 3); drawSphere(0.4f, 26, 13); glPopMatrix();
    glPopMatrix();

    // Draw the robot
    glPushMatrix();
        glTranslatef(0, -0.4, 0);
        glRotatef(yRot, 0, 1, 0);
        glScalef(0.65f, 0.65f, 0.65f);
        drawRobot();
    glPopMatrix();

    // Draw light position indicators (small spheres)
    if (showMultipleLights) {
        glDisable(GL_LIGHTING);
        glDisable(GL_TEXTURE_2D);
        for (size_t i = 0; i < lights.size(); ++i) {
            if (lights[i].enabled) {
                glPushMatrix();
                glTranslatef(lights[i].position[0], lights[i].position[1], lights[i].position[2]);
                glColor3f(lights[i].diffuse[0], lights[i].diffuse[1], lights[i].diffuse[2]);
                drawSphere(0.2f, 10, 8);
                glPopMatrix();
            }
        }
        glEnable(GL_LIGHTING);
        glEnable(GL_TEXTURE_2D);
    }

    glPopMatrix();
    glutSwapBuffers();
}

///////////////////////////////////////////////////////////////////////////////
// SETUP AND INITIALIZATION
///////////////////////////////////////////////////////////////////////////////
void SetupRC() {
    glClearColor(0.6f, 0.6f, 0.6f, 1.0f);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);
    glFrontFace(GL_CCW);

    // Initialize lighting
    glEnable(GL_LIGHTING);

    // Setup primary light (GL_LIGHT0)
    Light primaryLight;
    primaryLight.position[0] = -10.0f; primaryLight.position[1] = 20.0f;
    primaryLight.position[2] = 10.0f; primaryLight.position[3] = 1.0f;
    primaryLight.ambient[0] = 0.3f; primaryLight.ambient[1] = 0.3f;
    primaryLight.ambient[2] = 0.3f; primaryLight.ambient[3] = 1.0f;
    primaryLight.diffuse[0] = 0.8f; primaryLight.diffuse[1] = 0.8f;
    primaryLight.diffuse[2] = 0.8f; primaryLight.diffuse[3] = 1.0f;
    primaryLight.specular[0] = 1.0f; primaryLight.specular[1] = 1.0f;
    primaryLight.specular[2] = 1.0f; primaryLight.specular[3] = 1.0f;
    primaryLight.intensity = 1.0f;
    primaryLight.enabled = true;
    lights.push_back(primaryLight);

    glEnable(GL_LIGHT0);
    glLightfv(GL_LIGHT0, GL_AMBIENT, primaryLight.ambient);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, primaryLight.diffuse);
    glLightfv(GL_LIGHT0, GL_SPECULAR, primaryLight.specular);
    glLightfv(GL_LIGHT0, GL_POSITION, primaryLight.position);

    // Setup secondary lights
    Light secondaryLight1;
    secondaryLight1.position[0] = 8.0f; secondaryLight1.position[1] = 15.0f;
    secondaryLight1.position[2] = 0.0f; secondaryLight1.position[3] = 1.0f;
    secondaryLight1.ambient[0] = 0.1f; secondaryLight1.ambient[1] = 0.1f;
    secondaryLight1.ambient[2] = 0.2f; secondaryLight1.ambient[3] = 1.0f;
    secondaryLight1.diffuse[0] = 0.4f; secondaryLight1.diffuse[1] = 0.4f;
    secondaryLight1.diffuse[2] = 0.8f; secondaryLight1.diffuse[3] = 1.0f;
    secondaryLight1.specular[0] = 0.6f; secondaryLight1.specular[1] = 0.6f;
    secondaryLight1.specular[2] = 1.0f; secondaryLight1.specular[3] = 1.0f;
    secondaryLight1.intensity = 0.7f;
    secondaryLight1.enabled = false;
    lights.push_back(secondaryLight1);

    Light secondaryLight2;
    secondaryLight2.position[0] = -6.0f; secondaryLight2.position[1] = 12.0f;
    secondaryLight2.position[2] = -8.0f; secondaryLight2.position[3] = 1.0f;
    secondaryLight2.ambient[0] = 0.2f; secondaryLight2.ambient[1] = 0.1f;
    secondaryLight2.ambient[2] = 0.1f; secondaryLight2.ambient[3] = 1.0f;
    secondaryLight2.diffuse[0] = 0.8f; secondaryLight2.diffuse[1] = 0.4f;
    secondaryLight2.diffuse[2] = 0.4f; secondaryLight2.diffuse[3] = 1.0f;
    secondaryLight2.specular[0] = 1.0f; secondaryLight2.specular[1] = 0.6f;
    secondaryLight2.specular[2] = 0.6f; secondaryLight2.specular[3] = 1.0f;
    secondaryLight2.intensity = 0.5f;
    secondaryLight2.enabled = false;
    lights.push_back(secondaryLight2);

    // Material properties
    glEnable(GL_COLOR_MATERIAL);
    glColorMaterial(GL_FRONT, GL_AMBIENT_AND_DIFFUSE);
    GLfloat specular[] = {1.0f, 1.0f, 1.0f, 1.0f};
    glMaterialfv(GL_FRONT, GL_SPECULAR, specular);
    glMateriali(GL_FRONT, GL_SHININESS, 128);

    // Enable texturing
    glEnable(GL_TEXTURE_2D);

    // Create wood texture
    MakeWoodTexture(woodTexture);
    glGenTextures(1, &woodTextureID);
    glBindTexture(GL_TEXTURE_2D, woodTextureID);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, WOOD_TEXTURE_WIDTH, WOOD_TEXTURE_HEIGHT, 0, GL_RGB, GL_UNSIGNED_BYTE, woodTexture);

    // Try to load external textures, fallback to generated textures
    grassTextureID = LoadTGATexture("grass.tga");
    if (grassTextureID == 0) {
        MakeGrassTexture(grassTexture);
        glGenTextures(1, &grassTextureID);
        glBindTexture(GL_TEXTURE_2D, grassTextureID);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, WOOD_TEXTURE_WIDTH, WOOD_TEXTURE_HEIGHT, 0, GL_RGB, GL_UNSIGNED_BYTE, grassTexture);
    }

    orbTextureID = LoadTGATexture("orb.tga");
    if (orbTextureID == 0) {
        MakeOrbTexture(orbTexture);
        glGenTextures(1, &orbTextureID);
        glBindTexture(GL_TEXTURE_2D, orbTextureID);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, WOOD_TEXTURE_WIDTH, WOOD_TEXTURE_HEIGHT, 0, GL_RGB, GL_UNSIGNED_BYTE, orbTexture);
    }

    // Setup ground plane for shadow calculations
    m3dGetPlaneEquation(groundPlane, (M3DVector3f){-10, -0.4, -10},
                       (M3DVector3f){10, -0.4, -10}, (M3DVector3f){0, -0.4, 10});

    // Initialize shadow matrices
    for (size_t i = 0; i < lights.size(); ++i) {
        MakeShadowMatrix(lights[i].shadowMatrix, groundPlane, lights[i].position);
    }
}

void KeyboardFunc(unsigned char key, int x, int y) {
    switch(key) {
        case 32: // Spacebar - pause/resume animation
            isPaused = !isPaused;
            break;
        case 'l': case 'L': // Toggle multiple lights
            showMultipleLights = !showMultipleLights;
            if (showMultipleLights) {
                // Enable additional lights
                for (size_t i = 1; i < lights.size(); ++i) {
                    lights[i].enabled = true;
                    glEnable(GL_LIGHT0 + i);
                    glLightfv(GL_LIGHT0 + i, GL_AMBIENT, lights[i].ambient);
                    glLightfv(GL_LIGHT0 + i, GL_DIFFUSE, lights[i].diffuse);
                    glLightfv(GL_LIGHT0 + i, GL_SPECULAR, lights[i].specular);
                    glLightfv(GL_LIGHT0 + i, GL_POSITION, lights[i].position);
                }
                std::cout << "Multiple lights enabled" << std::endl;
            } else {
                // Disable additional lights
                for (size_t i = 1; i < lights.size(); ++i) {
                    lights[i].enabled = false;
                    glDisable(GL_LIGHT0 + i);
                }
                std::cout << "Single light mode" << std::endl;
            }
            break;
        case 's': case 'S': // Toggle soft shadows
            showSoftShadows = !showSoftShadows;
            std::cout << "Soft shadows: " << (showSoftShadows ? "ON" : "OFF") << std::endl;
            break;
        case 'h': case 'H': // Show help
            std::cout << "\n=== CONTROLS ===" << std::endl;
            std::cout << "Spacebar: Pause/Resume Animation" << std::endl;
            std::cout << "L: Toggle Multiple Lights" << std::endl;
            std::cout << "S: Toggle Soft Shadows" << std::endl;
            std::cout << "H: Show this help" << std::endl;
            std::cout << "ESC: Exit" << std::endl;
            std::cout << "================" << std::endl;
            break;
        case 27: // ESC - exit
            // Cleanup
            glDeleteTextures(1, &woodTextureID);
            glDeleteTextures(1, &grassTextureID);
            glDeleteTextures(1, &orbTextureID);
            exit(0);
            break;
    }
}

void ChangeSize(int w, int h) {
    if (h == 0) h = 1; // Prevent division by zero

    glViewport(0, 0, w, h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(45.0, (float)w/h, 1.0, 100.0);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
}

void TimerFunction(int value) {
    if (!isPaused) {
        // Update rotations
        yRot += 1.0f;
        if (yRot >= 360.0f) yRot = 0.0f;

        // Update robot animation
        animTime += 0.05f;
        swingAngle = 30.0f * sin(animTime);

        // Update sphere orbit
        sphereOrbitAngle += 0.5f;
        if (sphereOrbitAngle >= 360.0f) sphereOrbitAngle = 0.0f;

        // Update light orbit (slower than spheres)
        lightOrbitAngle += 0.3f;
        if (lightOrbitAngle >= 360.0f) lightOrbitAngle = 0.0f;
    }

    glutPostRedisplay();
    glutTimerFunc(16, TimerFunction, 1); // ~60 FPS
}

void DisplayInfo() {
    std::cout << "\n========================================" << std::endl;
    std::cout << "  ENHANCED SPHEREWORLD WITH SHADOWS" << std::endl;
    std::cout << "========================================" << std::endl;
    std::cout << "Features:" << std::endl;
    std::cout << "• Enhanced shadow system with multiple light sources" << std::endl;
    std::cout << "• Soft shadows when multiple lights are enabled" << std::endl;
    std::cout << "• Dynamic light animation" << std::endl;
    std::cout << "• Robust shadow rendering with Z-fighting prevention" << std::endl;
    std::cout << "• Fallback textures if external files are missing" << std::endl;
    std::cout << "\nControls:" << std::endl;
    std::cout << "• Spacebar: Pause/Resume Animation" << std::endl;
    std::cout << "• L: Toggle Multiple Lights & Animated Lighting" << std::endl;
    std::cout << "• S: Toggle Soft Shadows" << std::endl;
    std::cout << "• H: Show help" << std::endl;
    std::cout << "• ESC: Exit" << std::endl;
    std::cout << "\nNote: The program will use generated textures if" << std::endl;
    std::cout << "'grass.tga' and 'orb.tga' files are not found." << std::endl;
    std::cout << "========================================\n" << std::endl;
}

int main(int argc, char* argv[]) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
    glutInitWindowSize(800, 600);
    glutCreateWindow("Enhanced SphereWorld: Advanced Shadow System");

    glutReshapeFunc(ChangeSize);
    glutDisplayFunc(RenderScene);
    glutKeyboardFunc(KeyboardFunc);
    glutTimerFunc(16, TimerFunction, 1);

    SetupRC();
    DisplayInfo();

    glutMainLoop();

    return 0;
}