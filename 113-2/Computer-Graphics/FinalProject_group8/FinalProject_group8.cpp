// SphereWorld_Robot_NoGLEW.cpp
// OpenGL SuperBible
// Demonstrates an immersive 3D environment with an animated robot.
// Original program by Richard S. Wright Jr.
// Modified to be a single file, include a robot, pause functionality,
// and operate without the GLEW library.
//
// Added a sun sphere to represent the light source.
// Added keyboard controls (1, 2, 3) to move the sun (East, Noon, West).
// Modified the sun to be a textured sphere using sun.tga.
// Corrected sun texture color by using GL_REPLACE texture environment.
// Added W/S for camera pitch and R to reset the scene.
// Added key '4' to trigger a 5-second firework show, which then resets the scene.
// Firework show is denser, and world animations continue during the show.
// MODIFIED: Firework show is now a "sky-filling" spectacular.

// Disable fopen security warning on MSVC
#define _CRT_SECURE_NO_WARNINGS

// Standard Library Includes
#include <GL/glut.h> // For GLUT, GL, GLU functions
#include <iostream>
#include <cmath>
#include <cstdlib>
#include <cstdio>
#include <vector>    // For OBJ loading
#include <string>    // For OBJ loading
#include <fstream>   // For OBJ loading
#include <sstream>   // For OBJ loading
#include <algorithm> // For replace
#include <map>       // For map used in OBJ loading
#include <tuple>     // For tuple

// Manual definitions for TGA loading, typically provided by GLEW
#define GL_BGR_EXT 0x80E0
#define GL_BGRA_EXT 0x80E1
#define GL_CLAMP_TO_EDGE 0x812F
#define M_PI 3.1415926



///////////////////////////////////////////////////////////////////////////////
// BEGIN math3d.h
///////////////////////////////////////////////////////////////////////////////
#define M3D_H

using namespace std;

// 3D Math Types
typedef float M3DVector3f[3];
typedef float M3DMatrix44f[16];
typedef float M3DVector4f[4];

// Vector functions
void m3dCopyVector3(M3DVector3f dst, const M3DVector3f src) {
    dst[0] = src[0]; dst[1] = src[1]; dst[2] = src[2];
}

void m3dLoadVector3(M3DVector3f v, float x, float y, float z) {
    v[0] = x; v[1] = y; v[2] = z;
}

void m3dCrossProduct(M3DVector3f result, const M3DVector3f u, const M3DVector3f v) {
    result[0] = u[1] * v[2] - v[1] * u[2];
    result[1] = -u[0] * v[2] + v[0] * u[2];
    result[2] = u[0] * v[1] - v[0] * u[1];
}

void m3dNormalizeVector(M3DVector3f u) {
    float A = u[0] * u[0] + u[1] * u[1] + u[2] * u[2];
    if (A == 0.0f) return;
    float B = 1.0f / sqrt(A);
    u[0] *= B; u[1] *= B; u[2] *= B;
}

// Matrix functions
void m3dSetMatrixColumn44(M3DMatrix44f m, const M3DVector3f v, int col) {
    m[col * 4] = v[0]; m[col * 4 + 1] = v[1]; m[col * 4 + 2] = v[2];
}

void m3dRotationMatrix44(M3DMatrix44f m, float angle, float x, float y, float z) {
    float mag = sqrt(x * x + y * y + z * z);
    if (mag == 0.0f) return;
    x /= mag; y /= mag; z /= mag;
    float c = cos(angle); float s = sin(angle);
    m[0] = x * x * (1 - c) + c;      m[4] = x * y * (1 - c) - z * s; m[8] = x * z * (1 - c) + y * s; m[12] = 0.0f;
    m[1] = y * x * (1 - c) + z * s; m[5] = y * y * (1 - c) + c;      m[9] = y * z * (1 - c) - x * s; m[13] = 0.0f;
    m[2] = z * x * (1 - c) - y * s; m[6] = z * y * (1 - c) + x * s; m[10] = z * z * (1 - c) + c;      m[14] = 0.0f;
    m[3] = 0.0f;                      m[7] = 0.0f;                       m[11] = 0.0f;                       m[15] = 1.0f;
}

void m3dTransformVector3(M3DVector3f out, const M3DVector3f in, const M3DMatrix44f m) {
    out[0] = in[0] * m[0] + in[1] * m[4] + in[2] * m[8] + m[12];
    out[1] = in[0] * m[1] + in[1] * m[5] + in[2] * m[9] + m[13];
    out[2] = in[0] * m[2] + in[1] * m[6] + in[2] * m[10] + m[14];
}

void m3dGetPlaneEquation(M3DVector4f planeEq, const M3DVector3f p1, const M3DVector3f p2, const M3DVector3f p3) {
    M3DVector3f v1, v2;
    v1[0] = p2[0] - p1[0]; v1[1] = p2[1] - p1[1]; v1[2] = p2[2] - p1[2];
    v2[0] = p3[0] - p1[0]; v2[1] = p3[1] - p1[1]; v2[2] = p3[2] - p1[2];
    m3dCrossProduct(planeEq, v1, v2);
    m3dNormalizeVector(planeEq);
    planeEq[3] = -(planeEq[0] * p1[0] + planeEq[1] * p1[1] + planeEq[2] * p1[2]);
}

void m3dMakePlanarShadowMatrix(M3DMatrix44f proj, const M3DVector4f planeEq, const GLfloat* lightPos) {
    float dot = planeEq[0] * lightPos[0] + planeEq[1] * lightPos[1] + planeEq[2] * lightPos[2] + planeEq[3] * lightPos[3];
    proj[0] = dot - lightPos[0] * planeEq[0]; proj[4] = 0.0f - lightPos[0] * planeEq[1]; proj[8] = 0.0f - lightPos[0] * planeEq[2]; proj[12] = 0.0f - lightPos[0] * planeEq[3];
    proj[1] = 0.0f - lightPos[1] * planeEq[0]; proj[5] = dot - lightPos[1] * planeEq[1]; proj[9] = 0.0f - lightPos[1] * planeEq[2]; proj[13] = 0.0f - lightPos[1] * planeEq[3];
    proj[2] = 0.0f - lightPos[2] * planeEq[0]; proj[6] = 0.0f - lightPos[2] * planeEq[1]; proj[10] = dot - lightPos[2] * planeEq[2]; proj[14] = 0.0f - lightPos[2] * planeEq[3];
    proj[3] = 0.0f - lightPos[3] * planeEq[0]; proj[7] = 0.0f - lightPos[3] * planeEq[1]; proj[11] = 0.0f - lightPos[3] * planeEq[2]; proj[15] = dot - lightPos[3] * planeEq[3];
}

void m3dInvertMatrix44(M3DMatrix44f dst, const M3DMatrix44f src) {
    // A simple implementation for rotation matrices
    dst[0] = src[0]; dst[1] = src[4]; dst[2] = src[8]; dst[3] = 0.0f;
    dst[4] = src[1]; dst[5] = src[5]; dst[6] = src[9]; dst[7] = 0.0f;
    dst[8] = src[2]; dst[9] = src[6]; dst[10] = src[10]; dst[11] = 0.0f;
    dst[12] = 0.0f;    dst[13] = 0.0f;    dst[14] = 0.0f;    dst[15] = 1.0f;
}

///////////////////////////////////////////////////////////////////////////////
// END math3d.h
///////////////////////////////////////////////////////////////////////////////


///////////////////////////////////////////////////////////////////////////////
// BEGIN glframe.h
///////////////////////////////////////////////////////////////////////////////

#define _ORTHO_FRAME_

class GLFrame {
protected:
    M3DVector3f vOrigin;
    M3DVector3f vForward;
    M3DVector3f vUp;

public:
    GLFrame(void) {
        m3dLoadVector3(vOrigin, 0.0f, 0.0f, 0.0f);
        m3dLoadVector3(vUp, 0.0f, 1.0f, 0.0f);
        m3dLoadVector3(vForward, 0.0f, 0.0f, -1.0f);
    }

    inline void SetOrigin(const M3DVector3f vPoint) { m3dCopyVector3(vOrigin, vPoint); }
    inline void SetOrigin(float x, float y, float z) { m3dLoadVector3(vOrigin, x, y, z); }
    inline void GetOrigin(M3DVector3f vPoint) { m3dCopyVector3(vPoint, vOrigin); }

    inline void SetForwardVector(const M3DVector3f vDirection) { m3dCopyVector3(vForward, vDirection); }
    inline void SetForwardVector(float x, float y, float z) { m3dLoadVector3(vForward, x, y, z); }
    inline void GetForwardVector(M3DVector3f vVector) { m3dCopyVector3(vVector, vForward); }

    inline void SetUpVector(const M3DVector3f vDirection) { m3dCopyVector3(vUp, vDirection); }
    inline void SetUpVector(float x, float y, float z) { m3dLoadVector3(vUp, x, y, z); }
    inline void GetUpVector(M3DVector3f vVector) { m3dCopyVector3(vVector, vUp); }

    inline void GetZAxis(M3DVector3f vVector) { GetForwardVector(vVector); }
    inline void GetYAxis(M3DVector3f vVector) { GetUpVector(vVector); }
    inline void GetXAxis(M3DVector3f vVector) { m3dCrossProduct(vVector, vUp, vForward); }

    inline void MoveForward(float fDelta) {
        vOrigin[0] += vForward[0] * fDelta;
        vOrigin[1] += vForward[1] * fDelta;
        vOrigin[2] += vForward[2] * fDelta;
    }

    void GetMatrix(M3DMatrix44f matrix, bool bRotationOnly = false) {
        M3DVector3f vXAxis;
        m3dCrossProduct(vXAxis, vUp, vForward);
        m3dSetMatrixColumn44(matrix, vXAxis, 0); matrix[3] = 0.0f;
        m3dSetMatrixColumn44(matrix, vUp, 1);         matrix[7] = 0.0f;
        m3dSetMatrixColumn44(matrix, vForward, 2); matrix[11] = 0.0f;
        if (bRotationOnly) {
            matrix[12] = 0.0f; matrix[13] = 0.0f; matrix[14] = 0.0f;
        }
        else {
            m3dSetMatrixColumn44(matrix, vOrigin, 3);
        }
        matrix[15] = 1.0f;
    }

    inline void GetCameraOrientation(M3DMatrix44f m) {
        M3DVector3f x, z;
        z[0] = -vForward[0]; z[1] = -vForward[1]; z[2] = -vForward[2];
        m3dCrossProduct(x, vUp, z);
#define M(row,col) m[col*4+row]
        M(0, 0) = x[0]; M(0, 1) = x[1]; M(0, 2) = x[2]; M(0, 3) = 0.0;
        M(1, 0) = vUp[0]; M(1, 1) = vUp[1]; M(1, 2) = vUp[2]; M(1, 3) = 0.0;
        M(2, 0) = z[0]; M(2, 1) = z[1]; M(2, 2) = z[2]; M(2, 3) = 0.0;
        M(3, 0) = 0.0; M(3, 1) = 0.0; M(3, 2) = 0.0; M(3, 3) = 1.0;
#undef M
    }

    inline void ApplyCameraTransform(bool bRotOnly = false) {
        M3DMatrix44f m;
        GetCameraOrientation(m);
        glMultMatrixf(m);
        if (!bRotOnly) glTranslatef(-vOrigin[0], -vOrigin[1], -vOrigin[2]);
    }

    void ApplyActorTransform(bool bRotationOnly = false) {
        M3DMatrix44f rotMat;
        GetMatrix(rotMat, bRotationOnly);
        glMultMatrixf(rotMat);
    }

    void RotateLocalY(float fAngle) {
        M3DMatrix44f rotMat;
        m3dRotationMatrix44(rotMat, fAngle, vUp[0], vUp[1], vUp[2]);
        M3DVector3f newVect;
        newVect[0] = rotMat[0] * vForward[0] + rotMat[4] * vForward[1] + rotMat[8] * vForward[2];
        newVect[1] = rotMat[1] * vForward[0] + rotMat[5] * vForward[1] + rotMat[9] * vForward[2];
        newVect[2] = rotMat[2] * vForward[0] + rotMat[6] * vForward[1] + rotMat[10] * vForward[2];
        m3dCopyVector3(vForward, newVect);
    }

    void RotateLocalX(float fAngle) {
        M3DMatrix44f rotMat;
        M3DVector3f vCross;
        m3dCrossProduct(vCross, vUp, vForward);
        m3dRotationMatrix44(rotMat, fAngle, vCross[0], vCross[1], vCross[2]);
        M3DVector3f newVect;
        newVect[0] = rotMat[0] * vForward[0] + rotMat[4] * vForward[1] + rotMat[8] * vForward[2];
        newVect[1] = rotMat[1] * vForward[0] + rotMat[5] * vForward[1] + rotMat[9] * vForward[2];
        newVect[2] = rotMat[2] * vForward[0] + rotMat[6] * vForward[1] + rotMat[10] * vForward[2];
        m3dCopyVector3(vForward, newVect);
        newVect[0] = rotMat[0] * vUp[0] + rotMat[4] * vUp[1] + rotMat[8] * vUp[2];
        newVect[1] = rotMat[1] * vUp[0] + rotMat[5] * vUp[1] + rotMat[9] * vUp[2];
        newVect[2] = rotMat[2] * vUp[0] + rotMat[6] * vUp[1] + rotMat[10] * vUp[2];
        m3dCopyVector3(vUp, newVect);
    }
};

///////////////////////////////////////////////////////////////////////////////
// END glframe.h
///////////////////////////////////////////////////////////////////////////////


///////////////////////////////////////////////////////////////////////////////
// BEGIN gltools.h - relevant parts
///////////////////////////////////////////////////////////////////////////////

#pragma pack(1)
typedef struct {
    GLbyte     identsize;
    GLbyte     colorMapType;
    GLbyte     imageType;
    unsigned short     colorMapStart;
    unsigned short     colorMapLength;
    unsigned char      colorMapBits;
    unsigned short     xstart;
    unsigned short     ystart;
    unsigned short     width;
    unsigned short     height;
    GLbyte     bits;
    GLbyte     descriptor;
} TGAHEADER;
#pragma pack(8)

GLbyte* gltLoadTGA(const char* szFileName, GLint* iWidth, GLint* iHeight, GLint* iComponents, GLenum* eFormat) {
    FILE* pFile;
    TGAHEADER tgaHeader;
    unsigned long lImageSize;
    short sDepth;
    GLbyte* pBits = NULL;

    *iWidth = 0; *iHeight = 0; *eFormat = GL_BGR_EXT; *iComponents = GL_RGB8;

    pFile = fopen(szFileName, "rb");
    if (pFile == NULL) return NULL;

    fread(&tgaHeader, sizeof(TGAHEADER), 1, pFile);
    *iWidth = tgaHeader.width;
    *iHeight = tgaHeader.height;
    sDepth = tgaHeader.bits / 8;

    if (tgaHeader.bits != 8 && tgaHeader.bits != 24 && tgaHeader.bits != 32) {
        fclose(pFile);
        return NULL;
    }

    lImageSize = tgaHeader.width * tgaHeader.height * sDepth;
    pBits = (GLbyte*)malloc(lImageSize * sizeof(GLbyte));
    if (pBits == NULL) {
        fclose(pFile); // Close file on malloc failure
        return NULL;
    }

    if (fread(pBits, lImageSize, 1, pFile) != 1) {
        free(pBits);
        fclose(pFile); // Close file on fread failure
        return NULL;
    }

    switch (sDepth) {
    case 3:
        *eFormat = GL_BGR_EXT;
        *iComponents = GL_RGB;
        break;
    case 4:
        *eFormat = GL_BGRA_EXT;
        *iComponents = GL_RGBA;
        break;
    case 1:
        *eFormat = GL_LUMINANCE;
        *iComponents = GL_LUMINANCE;
        break;
    };

    fclose(pFile);
    return pBits;
}

///////////////////////////////////////////////////////////////////////////////
// END gltools.h
///////////////////////////////////////////////////////////////////////////////

// --- OBJ Loader Definitions ---
struct OBJModel {
    vector<GLfloat> vertices;
    vector<GLfloat> normals;
    vector<GLfloat> texCoords;
    vector<GLuint> indices; // Stores indices for final interleaved vertex data

    // Original data for parsing
    vector<GLfloat> temp_vertices;
    vector<GLfloat> temp_normals;
    vector<GLfloat> temp_texCoords;
};

// Function to parse a face line like "v1/vt1/vn1 v2/vt2/vn2 v3/vt3/vn3"
void parseFacePart(const string& facePart, GLuint& vIdx, GLuint& vtIdx, GLuint& vnIdx) {
    string s = facePart;
    replace(s.begin(), s.end(), '/', ' '); // Replace / with spaces for easier parsing
    stringstream ss(s);
    ss >> vIdx;
    if (!ss.eof()) ss >> vtIdx;
    if (!ss.eof()) ss >> vnIdx;
}

// Basic OBJ Loader - Handles v, vt, vn, and f (triangle faces only, with full v/vt/vn indexing)
bool LoadOBJ(const char* filename, OBJModel& model) {
    ifstream file(filename);
    if (!file.is_open()) {
        cerr << "Error: Could not open OBJ file " << filename << endl;
        return false;
    }

    vector<GLuint> vertexIndices, uvIndices, normalIndices;

    string line;
    while (getline(file, line)) {
        stringstream ss(line);
        string line_type;
        ss >> line_type;

        if (line_type == "v") {
            GLfloat x, y, z;
            ss >> x >> y >> z;
            model.temp_vertices.push_back(x);
            model.temp_vertices.push_back(y);
            model.temp_vertices.push_back(z);
        }
        else if (line_type == "vt") {
            GLfloat u, v;
            ss >> u >> v;
            model.temp_texCoords.push_back(u);
            model.temp_texCoords.push_back(1.0f - v); // Invert V for OpenGL (TGA usually has 0,0 at bottom-left)
        }
        else if (line_type == "vn") {
            GLfloat x, y, z;
            ss >> x >> y >> z;
            model.temp_normals.push_back(x);
            model.temp_normals.push_back(y);
            model.temp_normals.push_back(z);
        }
        else if (line_type == "f") {
            string facePart[3]; // Assuming triangles
            ss >> facePart[0] >> facePart[1] >> facePart[2];

            // Handle potential quad faces by converting to triangles
            string facePart4;
            bool isQuad = false;
            if (ss >> facePart4) { // Check if there's a fourth vertex
                isQuad = true;
            }

            GLuint vIdx[4] = { 0 }, vtIdx[4] = { 0 }, vnIdx[4] = { 0 };

            // Parse first triangle (always exists for valid f lines)
            parseFacePart(facePart[0], vIdx[0], vtIdx[0], vnIdx[0]);
            parseFacePart(facePart[1], vIdx[1], vtIdx[1], vnIdx[1]);
            parseFacePart(facePart[2], vIdx[2], vtIdx[2], vnIdx[2]);

            // Add first triangle
            vertexIndices.push_back(vIdx[0]); uvIndices.push_back(vtIdx[0]); normalIndices.push_back(vnIdx[0]);
            vertexIndices.push_back(vIdx[1]); uvIndices.push_back(vtIdx[1]); normalIndices.push_back(vnIdx[1]);
            vertexIndices.push_back(vIdx[2]); uvIndices.push_back(vtIdx[2]); normalIndices.push_back(vnIdx[2]);

            if (isQuad) {
                parseFacePart(facePart4, vIdx[3], vtIdx[3], vnIdx[3]);
                // Add second triangle for quad (v0, v2, v3)
                vertexIndices.push_back(vIdx[0]); uvIndices.push_back(vtIdx[0]); normalIndices.push_back(vnIdx[0]);
                vertexIndices.push_back(vIdx[2]); uvIndices.push_back(vtIdx[2]); normalIndices.push_back(vnIdx[2]);
                vertexIndices.push_back(vIdx[3]); uvIndices.push_back(vtIdx[3]); normalIndices.push_back(vnIdx[3]);
            }
        }
    }

    map<tuple<GLuint, GLuint, GLuint>, GLuint> uniqueVertexMap;
    GLuint current_index = 0;

    for (size_t i = 0; i < vertexIndices.size(); ++i) {
        GLuint vIndex = vertexIndices[i];
        GLuint uvIndex = uvIndices[i];
        GLuint normalIndex = normalIndices[i];

        vIndex--;
        if (uvIndex > 0) uvIndex--;
        if (normalIndex > 0) normalIndex--;

        tuple<GLuint, GLuint, GLuint> key = make_tuple(vIndex, uvIndex, normalIndex);

        if (uniqueVertexMap.find(key) == uniqueVertexMap.end()) {
            uniqueVertexMap[key] = current_index;
            model.indices.push_back(current_index);
            model.vertices.push_back(model.temp_vertices[vIndex * 3]);
            model.vertices.push_back(model.temp_vertices[vIndex * 3 + 1]);
            model.vertices.push_back(model.temp_vertices[vIndex * 3 + 2]);
            if (uvIndex < model.temp_texCoords.size() / 2) {
                model.texCoords.push_back(model.temp_texCoords[uvIndex * 2]);
                model.texCoords.push_back(model.temp_texCoords[uvIndex * 2 + 1]);
            }
            else {
                model.texCoords.push_back(0.0f); model.texCoords.push_back(0.0f);
            }
            if (normalIndex < model.temp_normals.size() / 3) {
                model.normals.push_back(model.temp_normals[normalIndex * 3]);
                model.normals.push_back(model.temp_normals[normalIndex * 3 + 1]);
                model.normals.push_back(model.temp_normals[normalIndex * 3 + 2]);
            }
            else {
                model.normals.push_back(0.0f); model.normals.push_back(1.0f); model.normals.push_back(0.0f);
            }
            current_index++;
        }
        else {
            model.indices.push_back(uniqueVertexMap[key]);
        }
    }
    model.temp_vertices.clear();
    model.temp_normals.clear();
    model.temp_texCoords.clear();
    return true;
}
// --- End OBJ Loader Definitions ---
enum CameraMode { MODE_FIRST, MODE_THIRD, MODE_FAR, MODE_TOP };
CameraMode camMode = MODE_FIRST;
bool isFirstPerson = true;
float thirdPersonYaw = 0.0f;
float thirdPersonPitch = 0.0f;
const float thirdPersonDist = 9.0f;
const float thirdPersonHeight = 2.0f;
const float thirdPersonPitchMin = -0.5f;
const float thirdPersonPitchMax = 1.4f;

float firstPersonPitch = 0.0f;
const float firstPersonPitchMin = -0.5f;
const float firstPersonPitchMax = 1.4f;
const float pitchDelta = 0.05f;

float farPitchANGLE = M_PI / 48;
float   farYaw = 0, farPitch = farPitchANGLE, farDist = 50, farHeight = 7;
float   topYaw = 0, topPitch = -M_PI / 2, topDist = 40, topHeight = 50;

float lastYawD = 0.0f;
float lastPitchD = 0.0f;
const float topPitchMin = -M_PI;
const float topPitchMax = 0.0f;

M3DVector3f characterPos = { 0.0f, 1.5f, 15.0f };

M3DVector3f savedForward = { 0.0f, 0.0f, 1.0f };
M3DVector3f savedUp;

M3DVector3f firstForward, firstUp;

M3DVector3f sceneCenter = { 0.0f, 0.0f, 0.0f };
M3DVector3f freeCamPos;

bool animationEnabled = true;
float animationSpeed = 30.0f;
float animTime = 0.0f;

// Robot joint angles
float headRot = 0.0f;
bool moving = false;
float headYaw = 0.0f, headPitch = 0.0f;
const float headPitchMin = -0.5f, headPitchMax = 1.4f;

bool swingReverse = false;
float armSwingAngle = 0.0f;
const float swingStep = 30.0f;
const float swingSpeed = 2.0f;

const float walkSwing = 30.0f;
float torsoRot = 0.0f;
float rightArmRot = 30.0f;
float rightElbowRot = -20.0f;
float leftArmRot = -30.0f;
float leftElbowRot = 20.0f;
float leftLegRot = 0.0f;
float rightLegRot = 0.0f;

// Main Application Globals
GLFrame frameCamera;

GLfloat fLightPos[4] = { 0.0f, 100.0f, 0.0f, 1.0f }; // Noon
GLfloat fNoLight[] = { 0.0f, 0.0f, 0.0f, 0.0f };
GLfloat fLowLight[] = { 0.25f, 0.25f, 0.25f, 1.0f };
GLfloat fBrightLight[] = { 1.0f, 1.0f, 1.0f, 1.0f };

M3DMatrix44f mShadowMatrix;
M3DVector4f g_pPlane;

void UpdateCameraView() {
    switch (camMode) {
    case MODE_FIRST:
        frameCamera.SetOrigin(characterPos);
        frameCamera.SetForwardVector(firstForward);
        frameCamera.SetUpVector(firstUp);
        m3dCopyVector3(savedForward, firstForward);
        m3dCopyVector3(savedUp, firstUp);
        break;

    case MODE_THIRD: {
        savedForward[0] = sin(thirdPersonYaw) * cos(thirdPersonPitch);
        savedForward[1] = sin(thirdPersonPitch);
        savedForward[2] = cos(thirdPersonYaw) * cos(thirdPersonPitch);
        m3dNormalizeVector(savedForward);
        M3DVector3f worldUp = { 0,1,0 }, right, newUp;
        m3dCrossProduct(right, worldUp, savedForward);
        m3dNormalizeVector(right);
        m3dCrossProduct(newUp, savedForward, right);
        m3dNormalizeVector(newUp);
        m3dCopyVector3(savedUp, newUp);
        M3DVector3f camPos = {
            characterPos[0] - savedForward[0] * thirdPersonDist,
            characterPos[1] + thirdPersonHeight,
            characterPos[2] - savedForward[2] * thirdPersonDist
        };
        frameCamera.SetOrigin(camPos);
        frameCamera.SetForwardVector(savedForward);
        frameCamera.SetUpVector(savedUp);
        m3dCopyVector3(firstForward, savedForward);
        m3dCopyVector3(firstUp, savedUp);
        break;
    }

    case MODE_FAR: {
        float cp = cosf(farPitch), sp = sinf(farPitch);
        float sy = sinf(farYaw), cy = cosf(farYaw);
        M3DVector3f offset = { farDist * cp * sy, farDist * sp, farDist * cp * cy };
        freeCamPos[0] = sceneCenter[0] + offset[0];
        freeCamPos[1] = sceneCenter[1] + offset[1] + farHeight;
        freeCamPos[2] = sceneCenter[2] + offset[2];
        savedForward[0] = sceneCenter[0] - freeCamPos[0];
        savedForward[1] = sceneCenter[1] - freeCamPos[1];
        savedForward[2] = sceneCenter[2] - freeCamPos[2];
        m3dNormalizeVector(savedForward);
        M3DVector3f worldUp = { 0, 1, 0 };
        m3dCopyVector3(savedUp, worldUp);
        frameCamera.SetOrigin(freeCamPos);
        frameCamera.SetForwardVector(savedForward);
        frameCamera.SetUpVector(savedUp);
        break;
    }
    case MODE_TOP: {
        freeCamPos[0] = sceneCenter[0];
        freeCamPos[1] = sceneCenter[1] + topHeight;
        freeCamPos[2] = sceneCenter[2];
        savedForward[0] = 0; savedForward[1] = -1; savedForward[2] = 0;
        savedUp[0] = 0; savedUp[1] = 0; savedUp[2] = -1;
        frameCamera.SetOrigin(freeCamPos);
        frameCamera.SetForwardVector(savedForward);
        frameCamera.SetUpVector(savedUp);
        break;
    }
    }

    glutPostRedisplay();
}


#define GROUND_TEXTURE       0
#define DINING_TABLE_TEXTURE 1
#define BUSH_OBJ_TEXTURE     2
#define POTTED_PLANT_TEXTURE 3
#define SUN_TEXTURE          4
#define DIAMOND_CARD_TEXTURE 5
#define HEART_CARD_TEXTURE   6
#define NUM_TEXTURES         7

GLuint  textureObjects[NUM_TEXTURES];

const char* szTextureFiles[] = { "TGA/blackandwhiteTiles.tga", "TGA/wood.tga", "TGA/bush.tga", "TGA/pottedPlant.tga", "TGA/sun.tga",  "TGA/diamond.tga", "TGA/heart.tga" };

bool isPaused = false;
GLfloat yRot = 0.0f;

// Firework Mode Globals
bool isFireworkMode = false;
float fireworkTimer = 0.0f;

// Global OBJ models
OBJModel diningTable_obj;
OBJModel grass_obj_model;
OBJModel pottedPlant_obj;
OBJModel playingCard_obj;

// Fireworks Settings
struct FireworkParticle {
    float x, y, z;
    float vx, vy, vz;
    float r, g, b;
    float life;
};

vector<FireworkParticle> particles;

// Windows Size
int winWidth = 800, winHeight = 600;

// Forward Declarations
void DrawOBJModel(const OBJModel& model, GLuint textureID, GLint nShadow);
void DrawInhabitants(GLint nShadow);
void Keyboard(unsigned char key, int x, int y);
void DrawSun(void);
void ResetScene(void);

// Function to launch a firework burst for the automatic show
void LaunchAutomaticFirework() {
    float worldX = ((float)rand() / RAND_MAX) * 80.0f - 40.0f;
    float worldY = ((float)rand() / RAND_MAX) * 30.0f + 25.0f; // Vary height more
    float worldZ = ((float)rand() / RAND_MAX) * 80.0f - 40.0f;

    // MODIFIED: Create a much denser and more spectacular burst for "sky-filling" effect.
    for (int i = 0; i < 350; ++i) { // Increased particle count
        FireworkParticle p;
        p.x = worldX;
        p.y = worldY;
        p.z = worldZ;

        float angle1 = ((float)rand() / RAND_MAX) * 2 * M_PI;
        float angle2 = ((float)rand() / RAND_MAX) * M_PI;
        float speed = ((float)rand() / RAND_MAX) * 0.15f; // Increased speed for larger bursts
        p.vx = speed * cos(angle1) * sin(angle2);
        p.vy = speed * sin(angle1) * sin(angle2);
        p.vz = speed * cos(angle2);
        p.r = 0.5f + ((float)rand() / RAND_MAX) * 0.5f; // Brighter colors
        p.g = 0.5f + ((float)rand() / RAND_MAX) * 0.5f;
        p.b = 0.5f + ((float)rand() / RAND_MAX) * 0.5f;
        p.life = 2.0f + ((float)rand() / RAND_MAX) * 2.0f; // Longer life for bigger, more persistent bursts
        particles.push_back(p);
    }
}


void SetupRC() {
    M3DVector3f vPoints[3] = { { 0.0f, -0.4f, 0.0f },
                               { 10.0f, -0.4f, 0.0f },
                               { 5.0f, -0.4f, -5.0f } };
    int i;

    glClearColor(fLowLight[0], fLowLight[1], fLowLight[2], fLowLight[3]);

    glStencilOp(GL_INCR, GL_INCR, GL_INCR);
    glClearStencil(0);
    glStencilFunc(GL_EQUAL, 0x0, 0x01);

    glCullFace(GL_BACK);
    glFrontFace(GL_CCW);
    glEnable(GL_CULL_FACE);
    glEnable(GL_DEPTH_TEST);

    glLightModelfv(GL_LIGHT_MODEL_AMBIENT, fNoLight);
    glLightfv(GL_LIGHT0, GL_AMBIENT, fLowLight);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, fBrightLight);
    glLightfv(GL_LIGHT0, GL_SPECULAR, fBrightLight);
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);

    m3dGetPlaneEquation(g_pPlane, vPoints[0], vPoints[1], vPoints[2]);
    m3dMakePlanarShadowMatrix(mShadowMatrix, g_pPlane, fLightPos);

    glEnable(GL_COLOR_MATERIAL);
    glColorMaterial(GL_FRONT, GL_AMBIENT_AND_DIFFUSE);
    glMaterialfv(GL_FRONT, GL_SPECULAR, fBrightLight);
    glMateriali(GL_FRONT, GL_SHININESS, 128);

    glEnable(GL_TEXTURE_2D);
    glGenTextures(NUM_TEXTURES, textureObjects);
    glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);

    for (i = 0; i < NUM_TEXTURES; i++) {
        GLbyte* pBytes;
        GLint iWidth, iHeight, iComponents;
        GLenum eFormat;

        glBindTexture(GL_TEXTURE_2D, textureObjects[i]);

        pBytes = gltLoadTGA(szTextureFiles[i], &iWidth, &iHeight, &iComponents, &eFormat);
        if (pBytes == NULL) {
            cerr << "Error loading TGA file: " << szTextureFiles[i] << endl;
            continue;
        }
        gluBuild2DMipmaps(GL_TEXTURE_2D, iComponents, iWidth, iHeight, eFormat, GL_UNSIGNED_BYTE, pBytes);
        free(pBytes);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    }

    glBindTexture(GL_TEXTURE_2D, textureObjects[GROUND_TEXTURE]);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

    if (!LoadOBJ("OBJ/diningTable.obj", diningTable_obj)) {
        cerr << "Failed to load OBJ/diningTable.obj" << endl;
    }
    if (!LoadOBJ("OBJ/bush.obj", grass_obj_model)) {
        cerr << "Failed to load OBJ/bush.obj" << endl;
    }
    if (!LoadOBJ("OBJ/pottedPlant.obj", pottedPlant_obj)) {
        cerr << "Failed to load OBJ/pottedPlant.obj" << endl;
    }
    if (!LoadOBJ("OBJ/playingCard.obj", playingCard_obj)) {
        cerr << "Failed to load OBJ/playingCard.obj" << endl;
    }

    frameCamera.GetForwardVector(savedForward);
    frameCamera.GetUpVector(savedUp);

    m3dCopyVector3(firstForward, savedForward);
    m3dCopyVector3(firstUp, savedUp);

    firstPersonPitch = asin(savedForward[1]);
    thirdPersonYaw = atan2(savedForward[0], savedForward[2]);
    thirdPersonPitch = firstPersonPitch;
}

void ShutdownRC(void) {
    glDeleteTextures(NUM_TEXTURES, textureObjects);
}



void DrawGround(void) {
    GLfloat fExtent = 20.0f;
    GLfloat fStep = 1.0f;
    GLfloat y = -0.4f;
    GLfloat s = 0.0f;
    GLfloat t = 0.0f;
    GLfloat texStep = 1.0f / (fExtent * .075f);

    glBindTexture(GL_TEXTURE_2D, textureObjects[GROUND_TEXTURE]);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

    for (GLfloat iStrip = -fExtent; iStrip <= fExtent; iStrip += fStep) {
        t = 0.0f;
        glBegin(GL_TRIANGLE_STRIP);
        for (GLfloat iRun = fExtent; iRun >= -fExtent; iRun -= fStep) {
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

void DrawOBJModel(const OBJModel& model, GLuint textureID, GLint nShadow) {
    if (nShadow == 0) {
        glEnable(GL_TEXTURE_2D);
        glBindTexture(GL_TEXTURE_2D, textureID);
        glColor4f(1.0f, 1.0f, 1.0f, 1.0f);
        glMaterialfv(GL_FRONT, GL_SPECULAR, fBrightLight);
    }
    else {
        glDisable(GL_TEXTURE_2D);
        glColor4f(0.0f, 0.0f, 0.0f, 0.6f);
        glMaterialfv(GL_FRONT, GL_SPECULAR, fNoLight);
    }

    if (!model.vertices.empty()) {
        glEnableClientState(GL_VERTEX_ARRAY);
        glVertexPointer(3, GL_FLOAT, 0, model.vertices.data());
    }

    if (!model.normals.empty()) {
        glEnableClientState(GL_NORMAL_ARRAY);
        glNormalPointer(GL_FLOAT, 0, model.normals.data());
    }
    else {
        glNormal3f(0.0f, 1.0f, 0.0f);
    }

    if (nShadow == 0 && !model.texCoords.empty()) {
        glEnableClientState(GL_TEXTURE_COORD_ARRAY);
        glTexCoordPointer(2, GL_FLOAT, 0, model.texCoords.data());
    }

    if (!model.indices.empty()) {
        glDrawElements(GL_TRIANGLES, model.indices.size(), GL_UNSIGNED_INT, model.indices.data());
    }
    else {
        glDrawArrays(GL_TRIANGLES, 0, model.vertices.size() / 3);
    }

    glDisableClientState(GL_VERTEX_ARRAY);
    if (!model.normals.empty()) {
        glDisableClientState(GL_NORMAL_ARRAY);
    }
    if (nShadow == 0 && !model.texCoords.empty()) {
        glDisableClientState(GL_TEXTURE_COORD_ARRAY);
    }
}

void DrawSun(void) {
    glDisable(GL_LIGHTING);
    glColor3f(1.0f, 1.0f, 1.0f);
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, textureObjects[SUN_TEXTURE]);
    glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);

    glPushMatrix();
    glTranslatef(fLightPos[0], fLightPos[1], fLightPos[2]);
    static GLUquadric* pSun = NULL;
    if (pSun == NULL) {
        pSun = gluNewQuadric();
        gluQuadricDrawStyle(pSun, GLU_FILL);
        gluQuadricNormals(pSun, GLU_SMOOTH);
        gluQuadricTexture(pSun, GL_TRUE);
    }
    glRotatef(180.0f, 0.0f, 1.0f, 0.0f);
    glRotatef(-90.0f, 1.0f, 0.0f, 0.0f);
    gluSphere(pSun, 5.0f, 30, 30);
    glPopMatrix();

    glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
    glEnable(GL_LIGHTING);
}

void DrawRobot() {
    glBindTexture(GL_TEXTURE_2D, textureObjects[DINING_TABLE_TEXTURE]);

    glPushMatrix();
    glRotatef(torsoRot, 0, 1, 0);

    // Torso
    glPushMatrix();
    glScalef(1.0f, 1.5f, 0.5f);
    glutSolidCube(1.0f);
    glPopMatrix();

    // Head
    glPushMatrix();
    glTranslatef(0.0f, 0.75f + 0.15f, 0.0f);
    glRotatef(headYaw * 180.0f / M_PI, 0, 1, 0);
    glRotatef(headPitch * 180.0f / M_PI, 1, 0, 0);
    glutSolidCube(0.4f);
    glPopMatrix();

    // Right Arm
    glPushMatrix();
    glTranslatef(-0.6f, 0.75f, 0);
    glRotatef(rightArmRot, 1, 0, 0);
    glutSolidSphere(0.15f, 12, 12);
    glTranslatef(0, -0.1f - 0.5f, 0);
    glPushMatrix();
    glScalef(0.2f, 1.0f, 0.2f);
    glutSolidCube(1.0f);
    glPopMatrix();
    glTranslatef(0, -0.5f - 0.08f, 0);
    glRotatef(rightElbowRot, 1, 0, 0);
    glutSolidSphere(0.1f, 12, 12);
    glTranslatef(0, -0.08f - 0.5f, 0);
    glPushMatrix();
    glScalef(0.2f, 1.0f, 0.2f);
    glutSolidCube(1.0f);
    glPopMatrix();
    glPopMatrix();

    // Left Arm
    glPushMatrix();
    glTranslatef(0.6f, 0.75f, 0);
    glRotatef(leftArmRot, 1, 0, 0);
    glutSolidSphere(0.15f, 12, 12);
    glTranslatef(0, -0.1f - 0.5f, 0);
    glPushMatrix();
    glScalef(0.2f, 1.0f, 0.2f);
    glutSolidCube(1.0f);
    glPopMatrix();
    glTranslatef(0, -0.5f - 0.08f, 0);
    glRotatef(leftElbowRot, 1, 0, 0);
    glutSolidSphere(0.1f, 12, 12);
    glTranslatef(0, -0.08f - 0.5f, 0);
    glPushMatrix();
    glScalef(0.2f, 1.0f, 0.2f);
    glutSolidCube(1.0f);
    glPopMatrix();
    glPopMatrix();

    // Legs
    float legX[2] = { -0.3f,0.3f }, legRot[2] = { leftLegRot, rightLegRot };
    for (int i = 0; i < 2; ++i) {
        glPushMatrix();
        glTranslatef(legX[i], -0.75f - 0.1f, 0);
        glRotatef(legRot[i], 1, 0, 0);
        glutSolidSphere(0.15f, 12, 12);
        glTranslatef(0, -0.1f - 0.5f, 0);
        glPushMatrix();
        glScalef(0.3f, 1.0f, 0.3f);
        glutSolidCube(1.0f);
        glPopMatrix();
        glPopMatrix();
    }

    glPopMatrix();
}

void DrawInhabitants(GLint nShadow) {
    static float orbitSpeed = 1.0f;

    // Draw diningTable.obj
    glPushMatrix();
    glTranslatef(0.0f, -0.4f, -2.5f);
    glRotatef(270, 1.0f, 0.0f, 0.0f);
    glScalef(0.3f, 0.3f, 0.3f);
    DrawOBJModel(diningTable_obj, textureObjects[DINING_TABLE_TEXTURE], nShadow);
    glPopMatrix();

    // Draw 5 Draw bush.obj
    for (int i = 0; i < 5; ++i) {
        glPushMatrix();
        float amplitude = 5.0f;
        float frequency = 0.5f;
        float offsetZ = 2.0f;
        float xOffset = amplitude * cos(i * frequency);
        float zPosition = i * 1.5f - offsetZ;

        glTranslatef(0.0f, -0.4f, -2.5f);
        glRotatef(yRot * orbitSpeed * 2.0f, 0.0f, 1.0f, 0.0f);
        glTranslatef(10.0f + xOffset, -0.4f, zPosition);
        glScalef(0.3f, 0.3f, 0.3f);
        DrawOBJModel(grass_obj_model, textureObjects[BUSH_OBJ_TEXTURE], nShadow);
        glPopMatrix();

        // Draw playing cards behind the bushes
        glPushMatrix();
        glTranslatef(0.0f, -0.4f, -2.5f);
        glRotatef(yRot * orbitSpeed * 2.0f, 0.0f, 1.0f, 0.0f);
        float cardOffsetFromBushZ = -0.5f;
        xOffset = amplitude * cos(i * frequency);
        zPosition = i * 1.5f - offsetZ;

        if (i % 2 == 0) {
            glTranslatef(5.0f + xOffset, 1.25f, zPosition - cardOffsetFromBushZ);
            glScalef(0.3f, 0.3f, 0.3f);
            glRotatef(90.0f, 0.0f, 1.0f, 0.0f);
            DrawOBJModel(playingCard_obj, textureObjects[DIAMOND_CARD_TEXTURE], nShadow);
        }
        else {
            glTranslatef(5.0f + xOffset, 1.25f, zPosition - cardOffsetFromBushZ);
            glScalef(0.3f, 0.3f, 0.3f);
            glRotatef(90.0f, 0.0f, 1.0f, 0.0f);
            DrawOBJModel(playingCard_obj, textureObjects[HEART_CARD_TEXTURE], nShadow);
        }
        glPopMatrix();
    }

    // Draw pottedPlant.obj
    glPushMatrix();
    glTranslatef(0.0f, -0.2f, -2.5f);
    glRotatef(yRot, 0.0f, 1.0f, 0.0f);
    glTranslatef(0.0f, 0.5f, 0.0f);
    glScalef(0.02f, 0.02f, 0.02f);
    DrawOBJModel(pottedPlant_obj, textureObjects[POTTED_PLANT_TEXTURE], nShadow);
    glPopMatrix();

    // Draw Robot
    glPushMatrix();
    if (nShadow == 1) {
        glColor4f(0.0f, 0.0f, 0.0f, 0.6f);
        glDisable(GL_TEXTURE_2D);
    }
    else {
        glColor4f(1.0f, 1.0f, 1.0f, 1.0f);
        glEnable(GL_TEXTURE_2D);
    }
    glPushMatrix();
    glTranslatef(characterPos[0], characterPos[1], characterPos[2]);
    glRotatef(thirdPersonYaw * 180.0f / M_PI, 0.0f, 1.0f, 0.0f);
    DrawRobot();
    glPopMatrix();

    if (nShadow == 0 && camMode == MODE_FIRST) {
        glColor4f(1.0f, 1.0f, 1.0f, 1.0f);
        glEnable(GL_TEXTURE_2D);
        glPushMatrix();
        glTranslatef(0.0f, -1.5f, -2.0f);
        DrawRobot();
        glPopMatrix();
    }
    glPopMatrix();
}


void UpdateParticles(float dt) {
    for (auto& p : particles) {
        p.x += p.vx;
        p.y += p.vy;
        p.z += p.vz;
        p.life -= dt;
    }
    particles.erase(std::remove_if(particles.begin(), particles.end(),
        [](FireworkParticle& p) { return p.life <= 0.0f; }),
        particles.end());
}

void DrawParticles() {
    glDisable(GL_LIGHTING);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glDisable(GL_TEXTURE_2D);

    // MODIFIED: Slightly smaller points for a finer, spark-like appearance with high density.
    glPointSize(2.5f);
    glBegin(GL_POINTS);
    for (const auto& p : particles) {
        glColor4f(p.r, p.g, p.b, p.life);
        glVertex3f(p.x, p.y, p.z);
    }
    glEnd();

    glDisable(GL_BLEND);
    glEnable(GL_LIGHTING);
    glEnable(GL_TEXTURE_2D);
}

void RenderScene(void) {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

    glPushMatrix();
    frameCamera.ApplyCameraTransform();
    glLightfv(GL_LIGHT0, GL_POSITION, fLightPos);
    DrawSun();
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

    DrawParticles();

    glPopMatrix();
    glutSwapBuffers();
}

void MouseClick(int button, int state, int x, int y) {
    if (isFireworkMode) return;

    if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN) {
        float winX = (float)x;
        float winY = (float)(winHeight - y);
        float winZ;

        glReadPixels(x, winHeight - y, 1, 1, GL_DEPTH_COMPONENT, GL_FLOAT, &winZ);

        GLdouble modelview[16], projection[16];
        GLint viewport[4];

        glGetDoublev(GL_PROJECTION_MATRIX, projection);
        glGetIntegerv(GL_VIEWPORT, viewport);

        glMatrixMode(GL_MODELVIEW);
        glPushMatrix();
        glLoadIdentity();
        frameCamera.ApplyCameraTransform();
        glGetDoublev(GL_MODELVIEW_MATRIX, modelview);
        glPopMatrix();

        GLdouble worldX, worldY, worldZ;
        if (winZ < 1.0f) {
            gluUnProject(winX, winY, winZ, modelview, projection, viewport,
                &worldX, &worldY, &worldZ);
        }
        else {
            GLdouble worldX_near, worldY_near, worldZ_near;
            GLdouble worldX_far, worldY_far, worldZ_far;

            gluUnProject(winX, winY, 0.0, modelview, projection, viewport,
                &worldX_near, &worldY_near, &worldZ_near); // Near
            gluUnProject(winX, winY, 1.0, modelview, projection, viewport,
                &worldX_far, &worldY_far, &worldZ_far);     // Far

            float dy = worldY_far - worldY_near;
            if (fabs(dy) < 0.0001f) return;

            float targetY = 5.0f;
            float t = (targetY - worldY_near) / dy;

            if (t < 0.0f || t > 2.0f) return;

            worldX = worldX_near + t * (worldX_far - worldX_near);
            worldY = targetY;
            worldZ = worldZ_near + t * (worldZ_far - worldZ_near);
        }

        for (int i = 0; i < 100; ++i) {
            FireworkParticle p;
            p.x = worldX;
            p.y = worldY;
            p.z = worldZ;
            float angle1 = ((float)rand() / RAND_MAX) * 2 * M_PI;
            float angle2 = ((float)rand() / RAND_MAX) * M_PI;
            float speed = ((float)rand() / RAND_MAX) * 0.05f;
            p.vx = speed * cos(angle1) * sin(angle2);
            p.vy = speed * sin(angle1) * sin(angle2);
            p.vz = speed * cos(angle2);
            p.r = (float)rand() / RAND_MAX;
            p.g = (float)rand() / RAND_MAX;
            p.b = (float)rand() / RAND_MAX;
            p.life = 1.0f;
            particles.push_back(p);
        }
    }
}

void ApplyFirstPersonRotation(float yawD, float pitchD) {
    if (pitchD != 0.0f) {
        float np = firstPersonPitch + pitchD;
        np = std::max(firstPersonPitchMin, std::min(firstPersonPitchMax, np));
        frameCamera.RotateLocalX(firstPersonPitch - np);
        firstPersonPitch = np;
    }
    if (yawD != 0.0f) {
        M3DMatrix44f rot;
        m3dRotationMatrix44(rot, yawD, 0, 1, 0);
        m3dTransformVector3(savedForward, savedForward, rot);
        m3dNormalizeVector(savedForward);
        m3dTransformVector3(savedUp, savedUp, rot);
        m3dNormalizeVector(savedUp);
        frameCamera.SetForwardVector(savedForward);
        frameCamera.SetUpVector(savedUp);
    }
    frameCamera.GetForwardVector(savedForward);
    frameCamera.GetUpVector(savedUp);
    m3dCopyVector3(firstForward, savedForward);
    m3dCopyVector3(firstUp, savedUp);
}

void ApplyThirdPersonRotation(float yawD, float pitchD) {
    thirdPersonYaw += yawD;
    thirdPersonPitch += pitchD;
    thirdPersonPitch = std::max(firstPersonPitchMin,
        std::min(firstPersonPitchMax, thirdPersonPitch));
    savedForward[0] = sin(thirdPersonYaw) * cos(thirdPersonPitch);
    savedForward[1] = sin(thirdPersonPitch);
    savedForward[2] = cos(thirdPersonYaw) * cos(thirdPersonPitch);
    m3dNormalizeVector(savedForward);
    M3DVector3f worldUp = { 0,1,0 }, right, newUp;
    m3dCrossProduct(right, savedForward, worldUp);
    m3dNormalizeVector(right);
    m3dCrossProduct(newUp, right, savedForward);
    m3dNormalizeVector(newUp);
    m3dCopyVector3(savedUp, newUp);
    M3DVector3f camPos = {
        characterPos[0] - savedForward[0] * thirdPersonDist,
        characterPos[1] + thirdPersonHeight,
        characterPos[2] - savedForward[2] * thirdPersonDist
    };
    frameCamera.SetOrigin(camPos);
    frameCamera.SetForwardVector(savedForward);
    frameCamera.SetUpVector(savedUp);
    m3dCopyVector3(firstForward, savedForward);
    m3dCopyVector3(firstUp, savedUp);
}

void ApplyFarCameraRotation(float yawD, float pitchD) {
    M3DMatrix44f rot;
    M3DVector3f offset, newOffset, worldUp = { 0,1,0 };
    m3dRotationMatrix44(rot, -yawD, 0, 1, 0);
    m3dCopyVector3(offset, freeCamPos);
    offset[0] -= sceneCenter[0];
    offset[1] -= sceneCenter[1];
    offset[2] -= sceneCenter[2];
    m3dTransformVector3(newOffset, offset, rot);
    freeCamPos[0] = newOffset[0] + sceneCenter[0];
    freeCamPos[1] = newOffset[1] + sceneCenter[1];
    freeCamPos[2] = newOffset[2] + sceneCenter[2];
    m3dRotationMatrix44(rot, pitchD, 1, 0, 0);
    m3dCopyVector3(offset, freeCamPos);
    offset[0] -= sceneCenter[0];
    offset[1] -= sceneCenter[1];
    offset[2] -= sceneCenter[2];
    m3dTransformVector3(newOffset, offset, rot);
    freeCamPos[0] = newOffset[0] + sceneCenter[0];
    freeCamPos[1] = newOffset[1] + sceneCenter[1];
    freeCamPos[2] = newOffset[2] + sceneCenter[2];
    savedForward[0] = sceneCenter[0] - freeCamPos[0];
    savedForward[1] = sceneCenter[1] - freeCamPos[1];
    savedForward[2] = sceneCenter[2] - freeCamPos[2];
    m3dNormalizeVector(savedForward);
    m3dCopyVector3(savedUp, worldUp);
    frameCamera.SetOrigin(freeCamPos);
    frameCamera.SetForwardVector(savedForward);
    frameCamera.SetUpVector(savedUp);
}

void ApplyTopCameraRotation(float yawD, float pitchD) {
    M3DMatrix44f rot;
    M3DVector3f offset, newOffset;
    m3dRotationMatrix44(rot, yawD, 0, 1, 0);
    m3dCopyVector3(offset, freeCamPos);
    offset[0] -= sceneCenter[0];
    offset[1] -= sceneCenter[1];
    offset[2] -= sceneCenter[2];
    m3dTransformVector3(newOffset, offset, rot);
    freeCamPos[0] = newOffset[0] + sceneCenter[0];
    freeCamPos[1] = newOffset[1] + sceneCenter[1];
    freeCamPos[2] = newOffset[2] + sceneCenter[2];
    M3DVector3f right;
    m3dCrossProduct(right, savedForward, savedUp);
    m3dNormalizeVector(right);
    float tmp_topPitch = topPitch;
    topPitch += pitchD;
    topPitch = std::max(topPitchMin,
        std::min(topPitchMax, topPitch));
    pitchD = topPitch - tmp_topPitch;
    m3dRotationMatrix44(rot, -pitchD, right[0], right[1], right[2]);
    m3dCopyVector3(offset, freeCamPos);
    offset[0] -= sceneCenter[0];
    offset[1] -= sceneCenter[1];
    offset[2] -= sceneCenter[2];
    m3dTransformVector3(newOffset, offset, rot);
    freeCamPos[0] = newOffset[0] + sceneCenter[0];
    freeCamPos[1] = newOffset[1] + sceneCenter[1];
    freeCamPos[2] = newOffset[2] + sceneCenter[2];
    savedForward[0] = sceneCenter[0] - freeCamPos[0];
    savedForward[1] = sceneCenter[1] - freeCamPos[1];
    savedForward[2] = sceneCenter[2] - freeCamPos[2];
    m3dNormalizeVector(savedForward);
    M3DVector3f newUp;
    m3dCrossProduct(newUp, right, savedForward);
    m3dNormalizeVector(newUp);
    m3dCopyVector3(savedUp, newUp);
    frameCamera.SetOrigin(freeCamPos);
    frameCamera.SetForwardVector(savedForward);
    frameCamera.SetUpVector(savedUp);
}

void SpecialKeys(int key, int x, int y) {
    if (isFireworkMode) return;

    float yawD = 0, pitchD = 0;
    switch (key) {
    case GLUT_KEY_UP:    if (camMode == MODE_FAR) return; pitchD = +pitchDelta; break;
    case GLUT_KEY_DOWN:  if (camMode == MODE_FAR) return; pitchD = -pitchDelta; break;
    case GLUT_KEY_LEFT:  if (camMode == MODE_TOP) return; yawD = +pitchDelta; break;
    case GLUT_KEY_RIGHT: if (camMode == MODE_TOP) return; yawD = -pitchDelta; break;
    }

    if (camMode == MODE_FIRST) ApplyFirstPersonRotation(yawD, pitchD);
    else if (camMode == MODE_THIRD) ApplyThirdPersonRotation(yawD, pitchD);
    else if (camMode == MODE_FAR) ApplyFarCameraRotation(yawD, pitchD);
    else if (camMode == MODE_TOP) ApplyTopCameraRotation(yawD, pitchD);


    glutPostRedisplay();
}

void movefunction(unsigned char key)
{
    const float step = 0.3f;
    const float swing = walkSwing;

    if (camMode == MODE_TOP) {
        float fy = topYaw;
        M3DVector3f fwTop = { sinf(fy), 0.0f, cosf(fy) };
        M3DVector3f worldUp = { 0,1,0 }, rightTop;
        m3dCrossProduct(rightTop, fwTop, worldUp);
        m3dNormalizeVector(fwTop);
        m3dNormalizeVector(rightTop);
        if (key == 'w' || key == 'W') { characterPos[0] -= fwTop[0] * step; characterPos[2] -= fwTop[2] * step; rightArmRot = -swing;  leftArmRot = swing; rightLegRot = swing;   leftLegRot = -swing; }
        else if (key == 's' || key == 'S') { characterPos[0] += fwTop[0] * step; characterPos[2] += fwTop[2] * step; rightArmRot = swing;   leftArmRot = -swing; rightLegRot = -swing;  leftLegRot = swing; }
        else if (key == 'a' || key == 'A') { characterPos[0] += rightTop[0] * step; characterPos[2] += rightTop[2] * step; rightArmRot = swing;   leftArmRot = -swing; rightLegRot = -swing;  leftLegRot = swing; }
        else if (key == 'd' || key == 'D') { characterPos[0] -= rightTop[0] * step; characterPos[2] -= rightTop[2] * step; rightArmRot = -swing;  leftArmRot = swing; rightLegRot = swing;   leftLegRot = -swing; }
        glutPostRedisplay();
        return;
    }
    else if (camMode == MODE_FAR) {
        M3DVector3f fwFar = { sinf(farYaw), 0.0f, cosf(farYaw) };
        M3DVector3f worldUp = { 0,1,0 }, rightFar;
        m3dCrossProduct(rightFar, fwFar, worldUp);
        m3dNormalizeVector(fwFar);
        m3dNormalizeVector(rightFar);
        if (key == 'w' || key == 'W') { characterPos[0] += fwFar[0] * step; characterPos[2] += fwFar[2] * step; rightArmRot = -swing; leftArmRot = swing; rightLegRot = swing; leftLegRot = -swing; }
        else if (key == 's' || key == 'S') { characterPos[0] -= fwFar[0] * step; characterPos[2] -= fwFar[2] * step; rightArmRot = swing; leftArmRot = -swing; rightLegRot = -swing; leftLegRot = swing; }
        else if (key == 'a' || key == 'A') { characterPos[0] -= rightFar[0] * step; characterPos[2] -= rightFar[2] * step; rightArmRot = swing; leftArmRot = -swing; rightLegRot = -swing; leftLegRot = swing; }
        else if (key == 'd' || key == 'D') { characterPos[0] += rightFar[0] * step; characterPos[2] += rightFar[2] * step; rightArmRot = -swing; leftArmRot = swing; rightLegRot = swing; leftLegRot = -swing; }
        return;
    }

    M3DVector3f fw, xa;
    if (camMode == MODE_FIRST) { frameCamera.GetForwardVector(fw); frameCamera.GetXAxis(xa); }
    else { m3dCopyVector3(fw, savedForward); m3dCrossProduct(xa, savedUp, fw); }
    fw[1] = 0; xa[1] = 0;
    m3dNormalizeVector(fw);
    m3dNormalizeVector(xa);

    if (key == 'w' || key == 'W') { characterPos[0] += fw[0] * step; characterPos[2] += fw[2] * step; rightArmRot = -swing;  leftArmRot = swing; rightLegRot = swing;   leftLegRot = -swing; }
    else if (key == 's' || key == 'S') { characterPos[0] -= fw[0] * step; characterPos[2] -= fw[2] * step; rightArmRot = swing;   leftArmRot = -swing; rightLegRot = -swing;  leftLegRot = swing; }
    else if (key == 'a' || key == 'A') { characterPos[0] += xa[0] * step; characterPos[2] += xa[2] * step; rightArmRot = swing;   leftArmRot = -swing; rightLegRot = -swing;  leftLegRot = swing; }
    else if (key == 'd' || key == 'D') { characterPos[0] -= xa[0] * step; characterPos[2] -= xa[2] * step; rightArmRot = -swing;  leftArmRot = swing; rightLegRot = swing;   leftLegRot = -swing; }

    if (camMode == MODE_FIRST) {
        frameCamera.SetOrigin(characterPos);
    }
    else if (camMode == MODE_THIRD) {
        M3DVector3f camPos = {
            characterPos[0] - savedForward[0] * thirdPersonDist,
            characterPos[1] + thirdPersonHeight,
            characterPos[2] - savedForward[2] * thirdPersonDist
        };
        frameCamera.SetOrigin(camPos);
    }
    glutPostRedisplay();
}

void ResetScene(void) {
    isFireworkMode = false;
    particles.clear();

    characterPos[0] = 0.0f;
    characterPos[1] = 1.5f;
    characterPos[2] = 15.0f;

    camMode = MODE_FIRST;
    isFirstPerson = true;

    firstPersonPitch = 0.0f;
    thirdPersonYaw = M_PI;
    thirdPersonPitch = 0.0f;

    frameCamera.SetOrigin(characterPos);
    frameCamera.SetForwardVector(0, 0, -1);
    frameCamera.SetUpVector(0, 1, 0);

    frameCamera.GetForwardVector(savedForward);
    frameCamera.GetUpVector(savedUp);
    m3dCopyVector3(firstForward, savedForward);
    m3dCopyVector3(firstUp, savedUp);

    UpdateCameraView();
}

void Keyboard(unsigned char key, int x, int y) {
    if (isFireworkMode) {
        if (key == 'r' || key == 'R') {
            ResetScene();
        }
        return;
    }

    switch (key) {
    case 'p': case 'P': isPaused = !isPaused; break;
    case '1': fLightPos[0] = 100; fLightPos[1] = 25;  fLightPos[2] = 0;
        m3dMakePlanarShadowMatrix(mShadowMatrix, g_pPlane, fLightPos);
        break;
    case '2': fLightPos[0] = 0;   fLightPos[1] = 100; fLightPos[2] = 0;
        m3dMakePlanarShadowMatrix(mShadowMatrix, g_pPlane, fLightPos);
        break;
    case '3': fLightPos[0] = -100; fLightPos[1] = 25;  fLightPos[2] = 0;
        m3dMakePlanarShadowMatrix(mShadowMatrix, g_pPlane, fLightPos);
        break;

    case '4':
        isFireworkMode = true;
        fireworkTimer = 5.0f;
        camMode = MODE_FAR;
        farYaw = thirdPersonYaw;
        farPitch = farPitchANGLE;
        UpdateCameraView();
        break;

    case 'w': case 'W':
    case 'a': case 'A':
    case 's': case 'S':
    case 'd': case 'D':
        moving = true;
        swingReverse = !swingReverse;
        movefunction(key);
        break;

    case 'c':
        camMode = MODE_FAR;
        farYaw = thirdPersonYaw;
        farPitch = farPitchANGLE;
        UpdateCameraView();
        break;

    case 'v':
        camMode = MODE_TOP;
        topYaw = 0;
        topPitch = -M_PI / 2;
        UpdateCameraView();
        break;

    case 'q': case 'Q':
        if (camMode == MODE_FAR || camMode == MODE_TOP) {
            camMode = isFirstPerson ? MODE_FIRST : MODE_THIRD;
        }
        else {
            isFirstPerson = !isFirstPerson;
            camMode = isFirstPerson ? MODE_FIRST : MODE_THIRD;
        }
        if (camMode == MODE_FIRST) {
            frameCamera.SetOrigin(characterPos);
            frameCamera.SetForwardVector(firstForward);
            frameCamera.SetUpVector(firstUp);
            frameCamera.GetForwardVector(savedForward);
            frameCamera.GetUpVector(savedUp);
            firstPersonPitch = asin(savedForward[1]);
        }
        else {
            frameCamera.GetForwardVector(savedForward);
            thirdPersonPitch = asin(savedForward[1]);
            thirdPersonYaw = atan2(savedForward[0], savedForward[2]);
        }
        UpdateCameraView();
        break;

    case 'r':
    case 'R':
        ResetScene();
        break;

    default:
        return;
    }
    glutPostRedisplay();
}

void KeyboardUp(unsigned char key, int x, int y) {
    if (key == 'w' || key == 'a' || key == 's' || key == 'd') {
        moving = false;
    }
}

void TimerFunction(int value) {
    float dt = 0.033f;
    UpdateParticles(dt);

    if (isFireworkMode) {
        fireworkTimer -= dt;

        // MODIFIED: Launch a new firework every single frame to create a "sky-full" effect.
        // The old timer logic is removed.
        LaunchAutomaticFirework();

        if (fireworkTimer <= 0.0f) {
            ResetScene();
        }
    }

    // This animation block now runs ALL THE TIME, even during fireworks.
    if (!isPaused) {
        // MODIFIED: Restored faster rotation speed for orbiting objects.
        yRot += 2.0f;
    }

    if (animationEnabled) {
        animTime += dt;
        torsoRot = 5.0f * sinf(animTime * 0.5f);
    }

    // Only animate walking if the key is actually held down and not in firework mode
    if (moving && !isFireworkMode) {
        armSwingAngle += swingSpeed;
        float phase = armSwingAngle * 0.1f;
        if (swingReverse) phase += M_PI;
        rightArmRot = swingStep * sinf(phase);
        leftArmRot = -swingStep * sinf(phase);
        rightLegRot = -swingStep * sinf(phase);
        leftLegRot = swingStep * sinf(phase);
    }

    glutPostRedisplay();
    glutTimerFunc(33, TimerFunction, 1);
}

void ChangeSize(int w, int h) {
    winHeight = h;
    winWidth = w;
    GLfloat fAspect;
    if (h == 0) h = 1;
    glViewport(0, 0, w, h);
    fAspect = (GLfloat)w / (GLfloat)h;
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(35.0f, fAspect, 1.0f, 250.0f);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
}

int main(int argc, char* argv[]) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH | GLUT_STENCIL);
    glutInitWindowSize(winWidth, winHeight);
    glutCreateWindow("Final Project - Controls: Arrows/WASD, 1-3=Light, 4=Fireworks, Q/C/V=View, R=Reset");

    glutReshapeFunc(ChangeSize);
    glutDisplayFunc(RenderScene);
    glutSpecialFunc(SpecialKeys);
    glutKeyboardFunc(Keyboard);
    glutKeyboardUpFunc(KeyboardUp);
    glutMouseFunc(MouseClick);

    frameCamera.SetOrigin(characterPos);
    frameCamera.SetForwardVector(0.0f, 0.0f, -1.0f);
    frameCamera.SetUpVector(0.0f, 1.0f, 0.0f);

    SetupRC();
    glutTimerFunc(33, TimerFunction, 1);

    glutMainLoop();
    ShutdownRC();
    return 0;
}