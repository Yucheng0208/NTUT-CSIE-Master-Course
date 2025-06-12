// SphereWorld_Robot_NoGLEW.cpp
// OpenGL SuperBible
// Demonstrates an immersive 3D environment with an animated robot.
// Original program by Richard S. Wright Jr.
// Modified to be a single file, include a robot, pause functionality,
// and operate without the GLEWE library.

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
    proj[1] = 0.0f - lightPos[1] * planeEq[0]; proj[5] = dot - lightPos[1] * planeEq[1]; proj[9] = 0.0f - lightPos[1] * planeEq[2]; proj[13] = 0.0f - lightPos[1] * planeEq[3]; // FIX: Changed planePos to planeEq
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
        *iComponents = GL_RGB8; // GL_RGB
        break;
    case 4:
        *eFormat = GL_BGRA_EXT;
        *iComponents = GL_RGBA8; // GL_RGBA
        break;
    case 1:
        *eFormat = GL_LUMINANCE;
        *iComponents = GL_LUMINANCE8; // GL_LUMINANCE
        break;
    };

    fclose(pFile);
    return pBits;
}

// gltDrawSphere and DrawCube are no longer directly used but kept for reference if needed.
void gltDrawSphere(GLfloat fRadius, GLint iSlices, GLint iStacks) {
    for (int i = 0; i < iStacks; ++i) {
        float lat0 = M_PI * (-0.5 + (float)i / iStacks);
        float z0 = sin(lat0);
        float zr0 = cos(lat0);

        float lat1 = M_PI * (-0.5 + (float)(i + 1) / iStacks);
        float z1 = sin(lat1);
        float zr1 = cos(lat1);

        glBegin(GL_QUAD_STRIP);
        for (int j = 0; j <= iSlices; ++j) {
            float lng = 2 * M_PI * (float)(j) / iSlices;
            float x = cos(lng);
            float y = sin(lng);

            float nx = x * zr0;
            float ny = y * zr0;
            float nz = z0;
            glNormal3f(nx, ny, nz);
            glTexCoord2f((float)j / iSlices, (float)i / iStacks);
            glVertex3f(fRadius * nx, fRadius * ny, fRadius * nz);

            nx = x * zr1;
            ny = y * zr1;
            nz = z1;
            glNormal3f(nx, ny, nz);
            glTexCoord2f((float)j / iSlices, (float)(i + 1) / iStacks);
            glVertex3f(fRadius * nx, fRadius * ny, fRadius * nz);
        }
        glEnd();
    }
}

void DrawCube(float fSize) {
    float size = fSize * 0.5f;
    glBegin(GL_QUADS);
    // Front Face
    glNormal3f(0.0f, 0.0f, 1.0f);
    glTexCoord2f(0.0f, 0.0f); glVertex3f(-size, -size, size);
    glTexCoord2f(1.0f, 0.0f); glVertex3f(size, -size, size);
    glTexCoord2f(1.0f, 1.0f); glVertex3f(size, size, size);
    glTexCoord2f(0.0f, 1.0f); glVertex3f(-size, size, size);
    // Back Face
    glNormal3f(0.0f, 0.0f, -1.0f);
    glTexCoord2f(1.0f, 0.0f); glVertex3f(-size, -size, -size);
    glTexCoord2f(1.0f, 1.0f); glVertex3f(-size, size, -size);
    glTexCoord2f(0.0f, 1.0f); glVertex3f(size, size, -size);
    glTexCoord2f(0.0f, 0.0f); glVertex3f(size, -size, -size);
    // Top Face
    glNormal3f(0.0f, 1.0f, 0.0f);
    glTexCoord2f(0.0f, 1.0f); glVertex3f(-size, size, -size);
    glTexCoord2f(0.0f, 0.0f); glVertex3f(-size, size, size);
    glTexCoord2f(1.0f, 0.0f); glVertex3f(size, size, size);
    glTexCoord2f(1.0f, 1.0f); glVertex3f(size, size, -size);
    // Bottom Face
    glNormal3f(0.0f, -1.0f, 0.0f);
    glTexCoord2f(1.0f, 1.0f); glVertex3f(-size, -size, -size);
    glTexCoord2f(0.0f, 1.0f); glVertex3f(size, -size, -size);
    glTexCoord2f(0.0f, 0.0f); glVertex3f(size, -size, size);
    glTexCoord2f(1.0f, 0.0f); glVertex3f(-size, -size, size);
    // Right face
    glNormal3f(1.0f, 0.0f, 0.0f);
    glTexCoord2f(1.0f, 0.0f); glVertex3f(size, -size, -size);
    glTexCoord2f(1.0f, 1.0f); glVertex3f(size, size, -size);
    glTexCoord2f(0.0f, 1.0f); glVertex3f(size, size, size);
    glTexCoord2f(0.0f, 0.0f); glVertex3f(size, -size, size);
    // Left Face
    glNormal3f(-1.0f, 0.0f, 0.0f);
    glTexCoord2f(0.0f, 0.0f); glVertex3f(-size, -size, -size);
    glTexCoord2f(1.0f, 0.0f); glVertex3f(-size, -size, size);
    glTexCoord2f(1.0f, 1.0f); glVertex3f(-size, size, size);
    glTexCoord2f(0.0f, 1.0f); glVertex3f(-size, size, -size);
    glEnd();
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
        // Ignore other types like mtllib, usemtl, o, g, s for this basic example
    }

    // After parsing all lines, build the final interleaved data and indices
    // This part is crucial for making the data compatible with glDrawElements
    // We create a unique vertex for each unique combination of (v, vt, vn)
    map<tuple<GLuint, GLuint, GLuint>, GLuint> uniqueVertexMap;
    GLuint current_index = 0;

    for (size_t i = 0; i < vertexIndices.size(); ++i) {
        GLuint vIndex = vertexIndices[i];
        GLuint uvIndex = uvIndices[i];
        GLuint normalIndex = normalIndices[i];

        // OBJ indices are 1-based, convert to 0-based for temp arrays
        vIndex--;
        if (uvIndex > 0) uvIndex--;
        if (normalIndex > 0) normalIndex--;

        tuple<GLuint, GLuint, GLuint> key = make_tuple(vIndex, uvIndex, normalIndex);

        if (uniqueVertexMap.find(key) == uniqueVertexMap.end()) {
            // New unique vertex combination
            uniqueVertexMap[key] = current_index;
            model.indices.push_back(current_index);

            // Add vertex position
            model.vertices.push_back(model.temp_vertices[vIndex * 3]);
            model.vertices.push_back(model.temp_vertices[vIndex * 3 + 1]);
            model.vertices.push_back(model.temp_vertices[vIndex * 3 + 2]);

            // Add texture coordinate if available
            if (uvIndex < model.temp_texCoords.size() / 2) {
                model.texCoords.push_back(model.temp_texCoords[uvIndex * 2]);
                model.texCoords.push_back(model.temp_texCoords[uvIndex * 2 + 1]);
            }
            else { // Placeholder if no texture coordinate
                model.texCoords.push_back(0.0f); model.texCoords.push_back(0.0f);
            }

            // Add normal if available
            if (normalIndex < model.temp_normals.size() / 3) {
                model.normals.push_back(model.temp_normals[normalIndex * 3]);
                model.normals.push_back(model.temp_normals[normalIndex * 3 + 1]);
                model.normals.push_back(model.temp_normals[normalIndex * 3 + 2]);
            }
            else { // Placeholder if no normal
                model.normals.push_back(0.0f); model.normals.push_back(1.0f); model.normals.push_back(0.0f);
            }
            current_index++;
        }
        else {
            // Already seen this combination, just add its index
            model.indices.push_back(uniqueVertexMap[key]);
        }
    }

    // Clear temporary data after building final arrays
    model.temp_vertices.clear();
    model.temp_normals.clear();
    model.temp_texCoords.clear();

    cout << "Loaded " << model.vertices.size() / 3 << " unique vertices and "
        << model.indices.size() / 3 << " faces from " << filename << endl;
    cout << "Normals count: " << model.normals.size() / 3 << ", TexCoords count: " << model.texCoords.size() / 2 << endl;
    return true;
}
// --- End OBJ Loader Definitions ---


// Main Application Globals
GLFrame frameCamera;

GLfloat fLightPos[4] = { -100.0f, 100.0f, 50.0f, 1.0f };
GLfloat fNoLight[] = { 0.0f, 0.0f, 0.0f, 0.0f };
GLfloat fLowLight[] = { 0.25f, 0.25f, 0.25f, 1.0f };
GLfloat fBrightLight[] = { 1.0f, 1.0f, 1.0f, 1.0f };

M3DMatrix44f mShadowMatrix;

#define GROUND_TEXTURE       0
#define DINING_TABLE_TEXTURE 1
#define BUSH_OBJ_TEXTURE    2 // This will now point to bush.tga
#define POTTED_PLANT_TEXTURE 3 // This will now point to pottedPlant.tga
#define NUM_TEXTURES         4
GLuint  textureObjects[NUM_TEXTURES];

const char* szTextureFiles[] = { "TGA/blackandwhiteTiles.tga", "TGA/wood.tga", "TGA/bush.tga", "TGA/pottedPlant.tga" };
// IMPORTANT: Ensure you have 'black&whiteTiles.tga', 'wood.tga', 'bush.tga', and 'pottedPlant.tga'
// in your 'TGA' folder.

bool isPaused = false;

// Global OBJ models
OBJModel diningTable_obj;
OBJModel grass_obj_model;
OBJModel pottedPlant_obj;

// Forward Declarations
void DrawOBJModel(const OBJModel& model, GLuint textureID, GLint nShadow);
void DrawInhabitants(GLint nShadow);
void Keyboard(unsigned char key, int x, int y);


void SetupRC() {
    // These points define the plane for the shadow matrix.
    // They are on the Y=-0.4f plane (our ground).
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

    M3DVector4f pPlane;
    m3dGetPlaneEquation(pPlane, vPoints[0], vPoints[1], vPoints[2]);
    m3dMakePlanarShadowMatrix(mShadowMatrix, pPlane, fLightPos);

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
            // You could consider binding a fallback texture here, e.g., a simple white pixel
            // GLubyte fallbackTex[] = {255, 255, 255, 255};
            // glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, 1, 1, 0, GL_RGBA, GL_UNSIGNED_BYTE, fallbackTex);
            continue;
        }
        gluBuild2DMipmaps(GL_TEXTURE_2D, iComponents, iWidth, iHeight, eFormat, GL_UNSIGNED_BYTE, pBytes);
        free(pBytes);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE); // Default for most OBJ textures
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE); // Default for most OBJ textures
    }
    // Ensure ground texture uses GL_REPEAT for tiling
    glBindTexture(GL_TEXTURE_2D, textureObjects[GROUND_TEXTURE]);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

    // --- Load OBJ Models ---
    if (!LoadOBJ("OBJ/diningTable.obj", diningTable_obj)) {
        cerr << "Failed to load OBJ/diningTable.obj" << endl;
    }
    if (!LoadOBJ("OBJ/bush.obj", grass_obj_model)) {
        cerr << "Failed to load OBJ/bush.obj" << endl;
    }
    if (!LoadOBJ("OBJ/pottedPlant.obj", pottedPlant_obj)) {
        cerr << "Failed to load OBJ/pottedPlant.obj" << endl;
    }
    // --- END OBJ Loading ---
}

void ShutdownRC(void) {
    glDeleteTextures(NUM_TEXTURES, textureObjects);
    // OBJModel uses vectors, which manage their own memory.
}

void DrawGround(void) {
    GLfloat fExtent = 20.0f;
    GLfloat fStep = 1.0f;
    GLfloat y = -0.4f; // Ground plane is at Y = -0.4f
    GLfloat s = 0.0f;
    GLfloat t = 0.0f;
    GLfloat texStep = 1.0f / (fExtent * .075f); // Controls how often texture repeats

    glBindTexture(GL_TEXTURE_2D, textureObjects[GROUND_TEXTURE]);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

    for (GLfloat iStrip = -fExtent; iStrip <= fExtent; iStrip += fStep) {
        t = 0.0f;
        glBegin(GL_TRIANGLE_STRIP);
        for (GLfloat iRun = fExtent; iRun >= -fExtent; iRun -= fStep) {
            glTexCoord2f(s, t);
            glNormal3f(0.0f, 1.0f, 0.0f); // Normal points straight up (Y-axis)
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

// Function to draw an OBJ model
void DrawOBJModel(const OBJModel& model, GLuint textureID, GLint nShadow) {
    if (nShadow == 0) {
        glEnable(GL_TEXTURE_2D);
        glBindTexture(GL_TEXTURE_2D, textureID);
        glColor4f(1.0f, 1.0f, 1.0f, 1.0f); // Reset color to white to allow texture colors to show
        glMaterialfv(GL_FRONT, GL_SPECULAR, fBrightLight); // Ensure specular is on for real objects
    }
    else {
        glDisable(GL_TEXTURE_2D);
        glColor4f(0.0f, 0.0f, 0.0f, 0.6f); // Shadow color (black with transparency)
        glMaterialfv(GL_FRONT, GL_SPECULAR, fNoLight); // Shadows don't need specular
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
        // Fallback: If no normals are loaded, use a default normal for all vertices.
        // This will result in flat shading unless calculated per-face.
        // For OBJ models, having normals is crucial for correct lighting.
        glNormal3f(0.0f, 1.0f, 0.0f); // Example: point up
    }

    if (nShadow == 0 && !model.texCoords.empty()) {
        glEnableClientState(GL_TEXTURE_COORD_ARRAY);
        glTexCoordPointer(2, GL_FLOAT, 0, model.texCoords.data());
    }

    if (!model.indices.empty()) {
        glDrawElements(GL_TRIANGLES, model.indices.size(), GL_UNSIGNED_INT, model.indices.data());
    }
    else {
        // If no indices, draw using glDrawArrays (less efficient, assumes linear data)
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


void DrawInhabitants(GLint nShadow) {
    static GLfloat yRot = 0.0f;
    static float orbitSpeed = 1.0f;

    if (!isPaused) {
        yRot += 0.5f;
    }

    // Draw diningTable.obj
    glPushMatrix();
    glTranslatef(0.0f, -0.4f, -2.5f);
    glRotatef(270, 1.0f, 0.0f, 0.0f);
    glScalef(0.3f, 0.3f, 0.3f); // Dining table scaled to 0.2x
    DrawOBJModel(diningTable_obj, textureObjects[DINING_TABLE_TEXTURE], nShadow);
    glPopMatrix();

    // Draw 5 Draw bush.obj (using bush.tga)
    for (int i = 0; i < 5; ++i) {
        glPushMatrix();
        float initialZ[5] = { 0.0f, 1.0f, 2.0f, 3.0f, 4.0f }; // 稍微錯開 Z 軸

        // 第一個平移，將模型帶到一個基準點
        glTranslatef(0.0f, -0.4f, -2.5f); // 餐桌的基礎位置

        // 旋轉，這是你提供的旋轉邏輯
        // 我們讓每個 bush 的旋轉角度稍微錯開，這樣它們看起來不會完全同步
        glRotatef(yRot * orbitSpeed * 2.0f, 0.0f, 1.0f, 0.0f);

        // 第二個平移，這是在旋轉後的局部坐標系中進行的
        // 調整這個平移，讓它們繞著中心點形成一個圓圈
        glTranslatef(10.0f, -0.4f, initialZ[i]); // 10.0f 變成了 initialX，且 Y 仍為 0.0f 以保持在地面

        glScalef(0.3f, 0.3f, 0.3f); // 統一使用 0.3f 的縮放比例

        DrawOBJModel(grass_obj_model, textureObjects[BUSH_OBJ_TEXTURE], nShadow);
        glPopMatrix();
    }

    // Draw pottedPlant.obj (using pottedPlant.tga)
    glPushMatrix();
    glTranslatef(0.0f, -0.2f, -2.5f);
    glRotatef(yRot, 0.0f, 1.0f, 0.0f);
    glTranslatef(0.0f, 0.5f, 0.0f); // Adjust Y to place it on top of the table. You'll likely need to fine-tune this.
    glScalef(0.02f, 0.02f, 0.02f);
    DrawOBJModel(pottedPlant_obj, textureObjects[POTTED_PLANT_TEXTURE], nShadow); // POTTED_PLANT_TEXTURE maps to pottedPlant.tga
    glPopMatrix();
}

void RenderScene(void) {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

    glPushMatrix();
    frameCamera.ApplyCameraTransform();
    glLightfv(GL_LIGHT0, GL_POSITION, fLightPos);

    glColor3f(1.0f, 1.0f, 1.0f); // Reset color to white to ensure texture colors show properly
    DrawGround();

    // Draw shadows
    glDisable(GL_DEPTH_TEST);
    glDisable(GL_LIGHTING);
    glDisable(GL_TEXTURE_2D); // No textures for shadows
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glEnable(GL_STENCIL_TEST);
    glPushMatrix();
    glMultMatrixf(mShadowMatrix);
    DrawInhabitants(1); // Draw shadows (nShadow = 1)
    glPopMatrix();
    glDisable(GL_STENCIL_TEST);
    glDisable(GL_BLEND);

    // Draw real objects
    glEnable(GL_LIGHTING);
    glEnable(GL_TEXTURE_2D); // Enable textures for real objects
    glEnable(GL_DEPTH_TEST);

    DrawInhabitants(0); // Draw real objects (nShadow = 0)

    glPopMatrix();
    glutSwapBuffers();
}

void SpecialKeys(int key, int x, int y) {
    if (key == GLUT_KEY_UP) frameCamera.MoveForward(0.1f);
    if (key == GLUT_KEY_DOWN) frameCamera.MoveForward(-0.1f);
    if (key == GLUT_KEY_LEFT) frameCamera.RotateLocalY(0.1f);
    if (key == GLUT_KEY_RIGHT) frameCamera.RotateLocalY(-0.1f);
    glutPostRedisplay();
}

void Keyboard(unsigned char key, int x, int y) {
    if (key == ' ') {
        isPaused = !isPaused;
    }
}

void TimerFunction(int value) {
    glutPostRedisplay();
    glutTimerFunc(16, TimerFunction, 1);
}

void ChangeSize(int w, int h) {
    GLfloat fAspect;
    if (h == 0) h = 1;
    glViewport(0, 0, w, h);
    fAspect = (GLfloat)w / (GLfloat)h;
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(35.0f, fAspect, 1.0f, 50.0f);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
}

int main(int argc, char* argv[]) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH | GLUT_STENCIL);
    glutInitWindowSize(800, 600);
    glutCreateWindow("Final Project - Group 08");

    glutReshapeFunc(ChangeSize);
    glutDisplayFunc(RenderScene);
    glutSpecialFunc(SpecialKeys);
    glutKeyboardFunc(Keyboard);

    // Set initial camera position to look down at the XZ plane.
    // Y=5.0f (height), Z=10.0f (distance back from origin)
    frameCamera.SetOrigin(0.0f, 0.0f, 5.0f);
    // The camera's default vForward (0,0,-1) means it looks towards negative Z.
    // From (0, 5, 10), looking at -Z will naturally point towards the XZ plane near the origin.

    SetupRC();
    glutTimerFunc(33, TimerFunction, 1);

    glutMainLoop();
    ShutdownRC();
    return 0;
}