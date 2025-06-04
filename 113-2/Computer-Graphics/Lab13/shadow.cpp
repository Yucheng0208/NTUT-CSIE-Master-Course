// Shadow.cpp
// OpenGL SuperBible
// Demonstrates simple planar shadows
// Program by Richard S. Wright Jr.
// Modified as per assignment requirements.
// Uses FreeGLUT.

// Explicitly include OpenGL headers first
#include <GL/gl.h>    // For GLfloat, glColor3ub, glNormal3fv, etc.
#include <GL/freeglut.h> // Using FreeGLUT (should also include gl.h and glu.h)

#include <cmath>         // For math functions like sqrt, sin, cos, fabs
#include <cstring>       // For memcpy

// BEGIN Math3D Library Content ///////////////////////////////////////////////////

// Type Definitions (formerly in math3d.h)
typedef float M3DVector2f[2];
typedef float M3DVector3f[3];
typedef float M3DVector4f[4];
typedef float M3DMatrix33f[9];  // 3x3 matrix, column major
typedef float M3DMatrix44f[16]; // 4x4 matrix, column major

typedef double M3DVector2d[2];
typedef double M3DVector3d[3];
typedef double M3DVector4d[4];
typedef double M3DMatrix33d[9];  // 3x3 matrix, column major
typedef double M3DMatrix44d[16]; // 4x4 matrix, column major

// Forward declarations for Math3D functions

// Helper / "Inline" style functions (prototypes & definitions)
// These were typically inline in math3d.h
inline bool m3dCloseEnough(float fCandidate, float fCompare, float fEpsilon) {
    return (std::fabs(fCandidate - fCompare) < fEpsilon);
}

// Dot product
inline float m3dDotProduct(const M3DVector3f u, const M3DVector3f v) {
    return u[0]*v[0] + u[1]*v[1] + u[2]*v[2];
}

inline double m3dDotProduct(const M3DVector3d u, const M3DVector3d v) {
    return u[0]*v[0] + u[1]*v[1] + u[2]*v[2];
}

// Cross product
inline void m3dCrossProduct(M3DVector3f result, const M3DVector3f u, const M3DVector3f v) {
    result[0] = u[1]*v[2] - v[1]*u[2];
    result[1] = u[2]*v[0] - u[0]*v[2];
    result[2] = u[0]*v[1] - v[0]*u[1];
}

inline void m3dCrossProduct(M3DVector3d result, const M3DVector3d u, const M3DVector3d v) {
    result[0] = u[1]*v[2] - v[1]*u[2];
    result[1] = u[2]*v[0] - u[0]*v[2];
    result[2] = u[0]*v[1] - v[0]*u[1];
}

// Vector length squared
inline float m3dGetVectorLengthSquared(const M3DVector3f u) {
    return (u[0]*u[0]) + (u[1]*u[1]) + (u[2]*u[2]);
}

inline double m3dGetVectorLengthSquared(const M3DVector3d u) {
    return (u[0]*u[0]) + (u[1]*u[1]) + (u[2]*u[2]);
}

// For normalizing plane equations (vector part)
inline float m3dGetVectorLengthSquared4(const M3DVector4f u) {
    return (u[0]*u[0]) + (u[1]*u[1]) + (u[2]*u[2]);
}

inline double m3dGetVectorLengthSquared4(const M3DVector4d u) {
    return (u[0]*u[0]) + (u[1]*u[1]) + (u[2]*u[2]);
}


// Normalize vector
inline void m3dNormalizeVector(M3DVector3f u) {
    float fMag = std::sqrt(m3dGetVectorLengthSquared(u));
    if (fMag > 1e-6f) { // Prevent division by zero or very small numbers
        u[0] /= fMag;
        u[1] /= fMag;
        u[2] /= fMag;
    }
}

inline void m3dNormalizeVector(M3DVector3d u) {
    double fMag = std::sqrt(m3dGetVectorLengthSquared(u));
    if (fMag > 1e-9) { // Prevent division by zero
        u[0] /= fMag;
        u[1] /= fMag;
        u[2] /= fMag;
    }
}




// Transform a 4D vector by a 4x4 matrix
inline void m3dTransformVector4(M3DVector4f vOut, const M3DVector4f v, const M3DMatrix44f m) {
    vOut[0] = m[0] * v[0] + m[4] * v[1] + m[8] * v[2] + m[12] * v[3];
    vOut[1] = m[1] * v[0] + m[5] * v[1] + m[9] * v[2] + m[13] * v[3];
    vOut[2] = m[2] * v[0] + m[6] * v[1] + m[10] * v[2] + m[14] * v[3];
    vOut[3] = m[3] * v[0] + m[7] * v[1] + m[11] * v[2] + m[15] * v[3];
}

// Subtract Vectors
inline void m3dSubtractVectors3(M3DVector3f result, const M3DVector3f u, const M3DVector3f v) {
    result[0] = u[0] - v[0];
    result[1] = u[1] - v[1];
    result[2] = u[2] - v[2];
}

inline void m3dSubtractVectors3(M3DVector3d result, const M3DVector3d u, const M3DVector3d v) {
    result[0] = u[0] - v[0];
    result[1] = u[1] - v[1];
    result[2] = u[2] - v[2];
}

// Scale Vector
inline void m3dScaleVector3(M3DVector3f v, float scale) {
    v[0] *= scale;
    v[1] *= scale;
    v[2] *= scale;
}

inline void m3dScaleVector3(M3DVector3d v, double scale) {
    v[0] *= scale;
    v[1] *= scale;
    v[2] *= scale;
}


// "Non-inlined" functions from math3d.c (prototypes for clarity, definitions follow)
void m3dLoadIdentity33(M3DMatrix33f m);
void m3dLoadIdentity33(M3DMatrix33d m);
void m3dLoadIdentity44(M3DMatrix44f m);
void m3dLoadIdentity44(M3DMatrix44d m);
float m3dGetDistanceSquared(const M3DVector3f u, const M3DVector3f v);
double m3dGetDistanceSquared(const M3DVector3d u, const M3DVector3d v);
void m3dMatrixMultiply44(M3DMatrix44f product, const M3DMatrix44f a, const M3DMatrix44f b);
void m3dMatrixMultiply(M3DMatrix44d product, const M3DMatrix44d a, const M3DMatrix44d b); // Original name for double 4x4
void m3dMatrixMultiply33(M3DMatrix33f product, const M3DMatrix33f a, const M3DMatrix33f b);
void m3dMatrixMultiply33(M3DMatrix33d product, const M3DMatrix33d a, const M3DMatrix33d b);
void m3dRotationMatrix33(M3DMatrix33f m, float angle, float x, float y, float z);
void m3dRotationMatrix33(M3DMatrix33d m, double angle, double x, double y, double z);
void m3dRotationMatrix44(M3DMatrix44f m, float angle, float x, float y, float z);
void m3dRotationMatrix44(M3DMatrix44d m, double angle, double x, double y, double z);
bool m3dInvertMatrix44(M3DMatrix44f dst, const M3DMatrix44f src);
bool m3dInvertMatrix44(M3DMatrix44d dst, const M3DMatrix44d src);
void m3dProjectXY(const M3DMatrix44f mModelView, const M3DMatrix44f mProjection, const int iViewPort[4], const M3DVector3f vPointIn, M3DVector2f vPointOut);
void m3dProjectXYZ(const M3DMatrix44f mModelView, const M3DMatrix44f mProjection, const int iViewPort[4], const M3DVector3f vPointIn, M3DVector3f vPointOut);
void m3dFindNormal(M3DVector3f result, const M3DVector3f point1, const M3DVector3f point2, const M3DVector3f point3);
void m3dFindNormal(M3DVector3d result, const M3DVector3d point1, const M3DVector3d point2, const M3DVector3d point3);
void m3dGetPlaneEquation(M3DVector4f planeEq, const M3DVector3f p1, const M3DVector3f p2, const M3DVector3f p3);
void m3dGetPlaneEquation(M3DVector4d planeEq, const M3DVector3d p1, const M3DVector3d p2, const M3DVector3d p3);
void m3dCatmullRom3(M3DVector3f vOut, M3DVector3f vP0, M3DVector3f vP1, M3DVector3f vP2, M3DVector3f vP3, float t);
void m3dCatmullRom3(M3DVector3d vOut, M3DVector3d vP0, M3DVector3d vP1, M3DVector3d vP2, M3DVector3d vP3, double t);
double m3dRaySphereTest(const M3DVector3d point, const M3DVector3d ray, const M3DVector3d sphereCenter, double sphereRadius);
float m3dRaySphereTest(const M3DVector3f point, const M3DVector3f ray, const M3DVector3f sphereCenter, float sphereRadius);
void m3dCalculateTangentBasis(const M3DVector3f vTriangle[3], const M3DVector2f vTexCoords[3], const M3DVector3f N, M3DVector3f vTangent);
double m3dSmoothStep(double edge1, double edge2, double x);
float m3dSmoothStep(float edge1, float edge2, float x);
void m3dMakePlanarShadowMatrix(M3DMatrix44f proj, const M3DVector4f planeEq, const M3DVector3f vLightPos);
void m3dMakePlanarShadowMatrix(M3DMatrix44d proj, const M3DVector4d planeEq, const M3DVector3f vLightPos);
double m3dClosestPointOnRay(M3DVector3d vPointOnRay, const M3DVector3d vRayOrigin, const M3DVector3d vUnitRayDir, const M3DVector3d vPointInSpace);
float m3dClosestPointOnRay(M3DVector3f vPointOnRay, const M3DVector3f vRayOrigin, const M3DVector3f vUnitRayDir, const M3DVector3f vPointInSpace);

// "Non-inlined" Function Implementations (from math3d.c)
////////////////////////////////////////////////////////////
// LoadIdentity
// For 3x3 and 4x4 float and double matricies.
// 3x3 float
void m3dLoadIdentity33(M3DMatrix33f m)
	{
	static M3DMatrix33f	identity = { 1.0f, 0.0f, 0.0f ,
									 0.0f, 1.0f, 0.0f,
									 0.0f, 0.0f, 1.0f };
	memcpy(m, identity, sizeof(M3DMatrix33f));
	}

// 3x3 double
void m3dLoadIdentity33(M3DMatrix33d m)
	{
	static M3DMatrix33d	identity = { 1.0, 0.0, 0.0 ,
									 0.0, 1.0, 0.0,
									 0.0, 0.0, 1.0 };
	memcpy(m, identity, sizeof(M3DMatrix33d));
	}

// 4x4 float
void m3dLoadIdentity44(M3DMatrix44f m)
	{
	static M3DMatrix44f	identity = { 1.0f, 0.0f, 0.0f, 0.0f,
									 0.0f, 1.0f, 0.0f, 0.0f,
									 0.0f, 0.0f, 1.0f, 0.0f,
									 0.0f, 0.0f, 0.0f, 1.0f };
	memcpy(m, identity, sizeof(M3DMatrix44f));
	}

// 4x4 double
void m3dLoadIdentity44(M3DMatrix44d m)
	{
	static M3DMatrix44d	identity = { 1.0, 0.0, 0.0, 0.0,
									 0.0, 1.0, 0.0, 0.0,
									 0.0, 0.0, 1.0, 0.0,
									 0.0, 0.0, 0.0, 1.0 };
	memcpy(m, identity, sizeof(M3DMatrix44d));
	}

float m3dGetDistanceSquared(const M3DVector3f u, const M3DVector3f v)
	{
	float x = u[0] - v[0]; x = x*x;
	float y = u[1] - v[1]; y = y*y;
	float z = u[2] - v[2]; z = z*z;
	return (x + y + z);
    }

double m3dGetDistanceSquared(const M3DVector3d u, const M3DVector3d v)
	{
	double x = u[0] - v[0]; x = x*x;
	double y = u[1] - v[1]; y = y*y;
	double z = u[2] - v[2]; z = z*z;
	return (x + y + z);
	}

#define A(row,col)  a[(col<<2)+row]
#define B(row,col)  b[(col<<2)+row]
#define P(row,col)  product[(col<<2)+row]

void m3dMatrixMultiply44(M3DMatrix44f product, const M3DMatrix44f a, const M3DMatrix44f b )
{
	for (int i = 0; i < 4; i++) {
		float ai0=A(i,0),  ai1=A(i,1),  ai2=A(i,2),  ai3=A(i,3);
		P(i,0) = ai0 * B(0,0) + ai1 * B(1,0) + ai2 * B(2,0) + ai3 * B(3,0);
		P(i,1) = ai0 * B(0,1) + ai1 * B(1,1) + ai2 * B(2,1) + ai3 * B(3,1);
		P(i,2) = ai0 * B(0,2) + ai1 * B(1,2) + ai2 * B(2,2) + ai3 * B(3,2);
		P(i,3) = ai0 * B(0,3) + ai1 * B(1,3) + ai2 * B(2,3) + ai3 * B(3,3);
	}
}

void m3dMatrixMultiply(M3DMatrix44d product, const M3DMatrix44d a, const M3DMatrix44d b ) // Original name from file
{
	for (int i = 0; i < 4; i++) {
		double ai0=A(i,0),  ai1=A(i,1),  ai2=A(i,2),  ai3=A(i,3);
		P(i,0) = ai0 * B(0,0) + ai1 * B(1,0) + ai2 * B(2,0) + ai3 * B(3,0);
		P(i,1) = ai0 * B(0,1) + ai1 * B(1,1) + ai2 * B(2,1) + ai3 * B(3,1);
		P(i,2) = ai0 * B(0,2) + ai1 * B(1,2) + ai2 * B(2,2) + ai3 * B(3,2);
		P(i,3) = ai0 * B(0,3) + ai1 * B(1,3) + ai2 * B(2,3) + ai3 * B(3,3);
	}
}
#undef A
#undef B
#undef P

#define A33(row,col)  a[(col*3)+row]
#define B33(row,col)  b[(col*3)+row]
#define P33(row,col)  product[(col*3)+row]

void m3dMatrixMultiply33(M3DMatrix33f product, const M3DMatrix33f a, const M3DMatrix33f b )
{
	for (int i = 0; i < 3; i++) {
		float ai0=A33(i,0), ai1=A33(i,1),  ai2=A33(i,2);
		P33(i,0) = ai0 * B33(0,0) + ai1 * B33(1,0) + ai2 * B33(2,0);
		P33(i,1) = ai0 * B33(0,1) + ai1 * B33(1,1) + ai2 * B33(2,1);
		P33(i,2) = ai0 * B33(0,2) + ai1 * B33(1,2) + ai2 * B33(2,2);
	}
}

void m3dMatrixMultiply33(M3DMatrix33d product, const M3DMatrix33d a, const M3DMatrix33d b )
{
	for (int i = 0; i < 3; i++) {
		double ai0=A33(i,0),  ai1=A33(i,1),  ai2=A33(i,2);
		P33(i,0) = ai0 * B33(0,0) + ai1 * B33(1,0) + ai2 * B33(2,0);
		P33(i,1) = ai0 * B33(0,1) + ai1 * B33(1,1) + ai2 * B33(2,1);
		P33(i,2) = ai0 * B33(0,2) + ai1 * B33(1,2) + ai2 * B33(2,2);
	}
}
#undef A33
#undef B33
#undef P33

#define M33_LOCAL(row,col)  m[col*3+row]
void m3dRotationMatrix33(M3DMatrix33f m, float angle, float x, float y, float z)
{
	float mag, s_val, c_val;
	float xx, yy, zz, xy, yz, zx, xs, ys, zs, one_c;

	s_val = static_cast<float>(std::sin(angle));
	c_val = static_cast<float>(std::cos(angle));
	mag = static_cast<float>(std::sqrt( x*x + y*y + z*z ));

	if (mag == 0.0f) { m3dLoadIdentity33(m); return; }

	x /= mag; y /= mag; z /= mag;
	xx = x * x; yy = y * y; zz = z * z;
	xy = x * y; yz = y * z; zx = z * x;
	xs = x * s_val; ys = y * s_val; zs = z * s_val;
	one_c = 1.0f - c_val;

	M33_LOCAL(0,0) = (one_c * xx) + c_val; M33_LOCAL(0,1) = (one_c * xy) - zs; M33_LOCAL(0,2) = (one_c * zx) + ys;
	M33_LOCAL(1,0) = (one_c * xy) + zs;  M33_LOCAL(1,1) = (one_c * yy) + c_val; M33_LOCAL(1,2) = (one_c * yz) - xs;
	M33_LOCAL(2,0) = (one_c * zx) - ys;  M33_LOCAL(2,1) = (one_c * yz) + xs;  M33_LOCAL(2,2) = (one_c * zz) + c_val;
}
#undef M33_LOCAL

#define M44_ROT(row,col)  m[col*4+row]
void m3dRotationMatrix44(M3DMatrix44f m, float angle, float x, float y, float z)
{
	float mag, s_val, c_val;
	float xx, yy, zz, xy, yz, zx, xs, ys, zs, one_c;

	s_val = static_cast<float>(std::sin(angle));
	c_val = static_cast<float>(std::cos(angle));
	mag = static_cast<float>(std::sqrt( x*x + y*y + z*z ));

	if (mag == 0.0f) { m3dLoadIdentity44(m); return; }

	x /= mag; y /= mag; z /= mag;
	xx = x * x; yy = y * y; zz = z * z;
	xy = x * y; yz = y * z; zx = z * x;
	xs = x * s_val; ys = y * s_val; zs = z * s_val;
	one_c = 1.0f - c_val;

	M44_ROT(0,0) = (one_c * xx) + c_val; M44_ROT(0,1) = (one_c * xy) - zs; M44_ROT(0,2) = (one_c * zx) + ys; M44_ROT(0,3) = 0.0f;
	M44_ROT(1,0) = (one_c * xy) + zs;  M44_ROT(1,1) = (one_c * yy) + c_val; M44_ROT(1,2) = (one_c * yz) - xs; M44_ROT(1,3) = 0.0f;
	M44_ROT(2,0) = (one_c * zx) - ys;  M44_ROT(2,1) = (one_c * yz) + xs;  M44_ROT(2,2) = (one_c * zz) + c_val; M44_ROT(2,3) = 0.0f;
	M44_ROT(3,0) = 0.0f; M44_ROT(3,1) = 0.0f; M44_ROT(3,2) = 0.0f; M44_ROT(3,3) = 1.0f;
}
#undef M44_ROT

#define M33D_ROT(row,col) m[col*3+row]
void m3dRotationMatrix33(M3DMatrix33d m, double angle, double x, double y, double z)
{
	double mag, s_val, c_val;
	double xx, yy, zz, xy, yz, zx, xs, ys, zs, one_c;
	s_val = std::sin(angle); c_val = std::cos(angle);
	mag = std::sqrt( x*x + y*y + z*z );
	if (mag == 0.0) { m3dLoadIdentity33(m); return; }
	x /= mag; y /= mag; z /= mag;
	xx = x * x; yy = y * y; zz = z * z; xy = x * y; yz = y * z; zx = z * x;
	xs = x * s_val; ys = y * s_val; zs = z * s_val; one_c = 1.0 - c_val;
	M33D_ROT(0,0) = (one_c * xx) + c_val; M33D_ROT(0,1) = (one_c * xy) - zs; M33D_ROT(0,2) = (one_c * zx) + ys;
	M33D_ROT(1,0) = (one_c * xy) + zs;  M33D_ROT(1,1) = (one_c * yy) + c_val; M33D_ROT(1,2) = (one_c * yz) - xs;
	M33D_ROT(2,0) = (one_c * zx) - ys;  M33D_ROT(2,1) = (one_c * yz) + xs;  M33D_ROT(2,2) = (one_c * zz) + c_val;
}
#undef M33D_ROT

#define M44D_ROT(row,col) m[col*4+row]
void m3dRotationMatrix44(M3DMatrix44d m, double angle, double x, double y, double z)
{
	double mag, s_val, c_val;
	double xx, yy, zz, xy, yz, zx, xs, ys, zs, one_c;
	s_val = std::sin(angle); c_val = std::cos(angle);
	mag = std::sqrt( x*x + y*y + z*z );
	if (mag == 0.0) { m3dLoadIdentity44(m); return; }
	x /= mag; y /= mag; z /= mag;
	xx = x * x; yy = y * y; zz = z * z; xy = x * y; yz = y * z; zx = z * x;
	xs = x * s_val; ys = y * s_val; zs = z * s_val; one_c = 1.0 - c_val;
	M44D_ROT(0,0) = (one_c * xx) + c_val; M44D_ROT(0,1) = (one_c * xy) - zs; M44D_ROT(0,2) = (one_c * zx) + ys; M44D_ROT(0,3) = 0.0;
	M44D_ROT(1,0) = (one_c * xy) + zs;  M44D_ROT(1,1) = (one_c * yy) + c_val; M44D_ROT(1,2) = (one_c * yz) - xs; M44D_ROT(1,3) = 0.0;
	M44D_ROT(2,0) = (one_c * zx) - ys;  M44D_ROT(2,1) = (one_c * yz) + xs;  M44D_ROT(2,2) = (one_c * zz) + c_val; M44D_ROT(2,3) = 0.0;
	M44D_ROT(3,0) = 0.0; M44D_ROT(3,1) = 0.0; M44D_ROT(3,2) = 0.0; M44D_ROT(3,3) = 1.0;
}
#undef M44D_ROT

bool m3dInvertMatrix44(M3DMatrix44f dst, const M3DMatrix44f src )
{
    #define SWAP_ROWS_F(a, b) { float *_tmp = a; (a)=(b); (b)=_tmp; }
    #define MAT_F(m_param,r,c) (m_param)[(c)*4+(r)]
	float wtmp[4][8]; float m0, m1, m2, m3, s_val; float *r0, *r1, *r2, *r3;
	r0 = wtmp[0], r1 = wtmp[1], r2 = wtmp[2], r3 = wtmp[3];
	r0[0] = MAT_F(src,0,0), r0[1] = MAT_F(src,0,1), r0[2] = MAT_F(src,0,2), r0[3] = MAT_F(src,0,3), r0[4] = 1.0f, r0[5] = 0.0f, r0[6] = 0.0f, r0[7] = 0.0f; // Explicitly f
	r1[0] = MAT_F(src,1,0), r1[1] = MAT_F(src,1,1), r1[2] = MAT_F(src,1,2), r1[3] = MAT_F(src,1,3), r1[4] = 0.0f, r1[5] = 1.0f, r1[6] = 0.0f, r1[7] = 0.0f;
	r2[0] = MAT_F(src,2,0), r2[1] = MAT_F(src,2,1), r2[2] = MAT_F(src,2,2), r2[3] = MAT_F(src,2,3), r2[4] = 0.0f, r2[5] = 0.0f, r2[6] = 1.0f, r2[7] = 0.0f;
	r3[0] = MAT_F(src,3,0), r3[1] = MAT_F(src,3,1), r3[2] = MAT_F(src,3,2), r3[3] = MAT_F(src,3,3), r3[4] = 0.0f, r3[5] = 0.0f, r3[6] = 0.0f, r3[7] = 1.0f;

	if (std::fabs(r3[0])>std::fabs(r2[0])) SWAP_ROWS_F(r3, r2); if (std::fabs(r2[0])>std::fabs(r1[0])) SWAP_ROWS_F(r2, r1); if (std::fabs(r1[0])>std::fabs(r0[0])) SWAP_ROWS_F(r1, r0);
	if (m3dCloseEnough(r0[0], 0.0f, 1e-6f)) return false;
	m1 = r1[0]/r0[0]; m2 = r2[0]/r0[0]; m3 = r3[0]/r0[0];
	s_val = r0[1]; r1[1] -= m1 * s_val; r2[1] -= m2 * s_val; r3[1] -= m3 * s_val; s_val = r0[2]; r1[2] -= m1 * s_val; r2[2] -= m2 * s_val; r3[2] -= m3 * s_val;
	s_val = r0[3]; r1[3] -= m1 * s_val; r2[3] -= m2 * s_val; r3[3] -= m3 * s_val; s_val = r0[4]; if (!m3dCloseEnough(s_val, 0.0f, 1e-6f)) { r1[4] -= m1 * s_val; r2[4] -= m2 * s_val; r3[4] -= m3 * s_val; }
	s_val = r0[5]; if (!m3dCloseEnough(s_val, 0.0f, 1e-6f)) { r1[5] -= m1 * s_val; r2[5] -= m2 * s_val; r3[5] -= m3 * s_val; } s_val = r0[6]; if (!m3dCloseEnough(s_val, 0.0f, 1e-6f)) { r1[6] -= m1 * s_val; r2[6] -= m2 * s_val; r3[6] -= m3 * s_val; }
	s_val = r0[7]; if (!m3dCloseEnough(s_val, 0.0f, 1e-6f)) { r1[7] -= m1 * s_val; r2[7] -= m2 * s_val; r3[7] -= m3 * s_val; }
	if (std::fabs(r3[1])>std::fabs(r2[1])) SWAP_ROWS_F(r3, r2); if (std::fabs(r2[1])>std::fabs(r1[1])) SWAP_ROWS_F(r2, r1); if (m3dCloseEnough(r1[1], 0.0f, 1e-6f)) return false;
	m2 = r2[1]/r1[1]; m3 = r3[1]/r1[1]; r2[2] -= m2 * r1[2]; r3[2] -= m3 * r1[2]; r2[3] -= m2 * r1[3]; r3[3] -= m3 * r1[3];
	s_val = r1[4]; if (!m3dCloseEnough(s_val, 0.0f, 1e-6f)) { r2[4] -= m2 * s_val; r3[4] -= m3 * s_val; } s_val = r1[5]; if (!m3dCloseEnough(s_val, 0.0f, 1e-6f)) { r2[5] -= m2 * s_val; r3[5] -= m3 * s_val; }
	s_val = r1[6]; if (!m3dCloseEnough(s_val, 0.0f, 1e-6f)) { r2[6] -= m2 * s_val; r3[6] -= m3 * s_val; } s_val = r1[7]; if (!m3dCloseEnough(s_val, 0.0f, 1e-6f)) { r2[7] -= m2 * s_val; r3[7] -= m3 * s_val; }
	if (std::fabs(r3[2])>std::fabs(r2[2])) SWAP_ROWS_F(r3, r2); if (m3dCloseEnough(r2[2], 0.0f, 1e-6f)) return false;
	m3 = r3[2]/r2[2]; r3[3] -= m3 * r2[3]; r3[4] -= m3 * r2[4]; r3[5] -= m3 * r2[5]; r3[6] -= m3 * r2[6]; r3[7] -= m3 * r2[7];
	if (m3dCloseEnough(r3[3], 0.0f, 1e-6f)) return false;
	s_val = 1.0f/r3[3]; r3[4] *= s_val; r3[5] *= s_val; r3[6] *= s_val; r3[7] *= s_val;
	m2 = r2[3]; s_val = 1.0f/r2[2]; r2[4] = s_val * (r2[4] - r3[4] * m2); r2[5] = s_val * (r2[5] - r3[5] * m2); r2[6] = s_val * (r2[6] - r3[6] * m2); r2[7] = s_val * (r2[7] - r3[7] * m2);
	m1 = r1[3]; r1[4] -= r3[4] * m1; r1[5] -= r3[5] * m1; r1[6] -= r3[6] * m1; r1[7] -= r3[7] * m1;
	m0 = r0[3]; r0[4] -= r3[4] * m0; r0[5] -= r3[5] * m0; r0[6] -= r3[6] * m0; r0[7] -= r3[7] * m0;
	m1 = r1[2]; s_val = 1.0f/r1[1]; r1[4] = s_val * (r1[4] - r2[4] * m1); r1[5] = s_val * (r1[5] - r2[5] * m1); r1[6] = s_val * (r1[6] - r2[6] * m1); r1[7] = s_val * (r1[7] - r2[7] * m1);
	m0 = r0[2]; r0[4] -= r2[4] * m0; r0[5] -= r2[5] * m0; r0[6] -= r2[6] * m0; r0[7] -= r2[7] * m0;
	m0 = r0[1]; s_val = 1.0f/r0[0]; r0[4] = s_val * (r0[4] - r1[4] * m0); r0[5] = s_val * (r0[5] - r1[5] * m0); r0[6] = s_val * (r0[6] - r1[6] * m0); r0[7] = s_val * (r0[7] - r1[7] * m0);
	MAT_F(dst,0,0) = r0[4]; MAT_F(dst,0,1) = r0[5]; MAT_F(dst,0,2) = r0[6]; MAT_F(dst,0,3) = r0[7];
	MAT_F(dst,1,0) = r1[4]; MAT_F(dst,1,1) = r1[5]; MAT_F(dst,1,2) = r1[6]; MAT_F(dst,1,3) = r1[7];
	MAT_F(dst,2,0) = r2[4]; MAT_F(dst,2,1) = r2[5]; MAT_F(dst,2,2) = r2[6]; MAT_F(dst,2,3) = r2[7];
	MAT_F(dst,3,0) = r3[4]; MAT_F(dst,3,1) = r3[5]; MAT_F(dst,3,2) = r3[6]; MAT_F(dst,3,3) = r3[7];
	return true;
    #undef MAT_F
    #undef SWAP_ROWS_F
}

bool m3dInvertMatrix44(M3DMatrix44d dst, const M3DMatrix44d src)
{
    #define SWAP_ROWS_D(a, b) { double *_tmp = a; (a)=(b); (b)=_tmp; }
    #define MAT_D(m_param,r,c) (m_param)[(c)*4+(r)]
	double wtmp[4][8]; double m0, m1, m2, m3, s_val; double *r0, *r1, *r2, *r3;
	r0 = wtmp[0], r1 = wtmp[1], r2 = wtmp[2], r3 = wtmp[3];
	r0[0] = MAT_D(src,0,0); r0[1] = MAT_D(src,0,1); r0[2] = MAT_D(src,0,2); r0[3] = MAT_D(src,0,3); r0[4] = 1.0; r0[5] = 0.0; r0[6] = 0.0; r0[7] = 0.0;
	r1[0] = MAT_D(src,1,0); r1[1] = MAT_D(src,1,1); r1[2] = MAT_D(src,1,2); r1[3] = MAT_D(src,1,3); r1[4] = 0.0; r1[5] = 1.0; r1[6] = 0.0; r1[7] = 0.0;
	r2[0] = MAT_D(src,2,0); r2[1] = MAT_D(src,2,1); r2[2] = MAT_D(src,2,2); r2[3] = MAT_D(src,2,3); r2[4] = 0.0; r2[5] = 0.0; r2[6] = 1.0; r2[7] = 0.0;
	r3[0] = MAT_D(src,3,0); r3[1] = MAT_D(src,3,1); r3[2] = MAT_D(src,3,2); r3[3] = MAT_D(src,3,3); r3[4] = 0.0; r3[5] = 0.0; r3[6] = 0.0; r3[7] = 1.0;

	if (std::fabs(r3[0])>std::fabs(r2[0])) SWAP_ROWS_D(r3, r2); if (std::fabs(r2[0])>std::fabs(r1[0])) SWAP_ROWS_D(r2, r1); if (std::fabs(r1[0])>std::fabs(r0[0])) SWAP_ROWS_D(r1, r0);
	if (std::fabs(r0[0]) < 1e-9) return false; // Close enough to zero for double
	m1 = r1[0]/r0[0]; m2 = r2[0]/r0[0]; m3 = r3[0]/r0[0];
	s_val = r0[1]; r1[1] -= m1 * s_val; r2[1] -= m2 * s_val; r3[1] -= m3 * s_val; s_val = r0[2]; r1[2] -= m1 * s_val; r2[2] -= m2 * s_val; r3[2] -= m3 * s_val;
	s_val = r0[3]; r1[3] -= m1 * s_val; r2[3] -= m2 * s_val; r3[3] -= m3 * s_val; s_val = r0[4]; if (std::fabs(s_val) > 1e-9) { r1[4] -= m1 * s_val; r2[4] -= m2 * s_val; r3[4] -= m3 * s_val; }
	s_val = r0[5]; if (std::fabs(s_val) > 1e-9) { r1[5] -= m1 * s_val; r2[5] -= m2 * s_val; r3[5] -= m3 * s_val; } s_val = r0[6]; if (std::fabs(s_val) > 1e-9) { r1[6] -= m1 * s_val; r2[6] -= m2 * s_val; r3[6] -= m3 * s_val; }
	s_val = r0[7]; if (std::fabs(s_val) > 1e-9) { r1[7] -= m1 * s_val; r2[7] -= m2 * s_val; r3[7] -= m3 * s_val; }
	if (std::fabs(r3[1])>std::fabs(r2[1])) SWAP_ROWS_D(r3, r2); if (std::fabs(r2[1])>std::fabs(r1[1])) SWAP_ROWS_D(r2, r1); if (std::fabs(r1[1]) < 1e-9) return false;
	m2 = r2[1]/r1[1]; m3 = r3[1]/r1[1]; r2[2] -= m2 * r1[2]; r3[2] -= m3 * r1[2]; r2[3] -= m2 * r1[3]; r3[3] -= m3 * r1[3];
	s_val = r1[4]; if (std::fabs(s_val) > 1e-9) { r2[4] -= m2 * s_val; r3[4] -= m3 * s_val; } s_val = r1[5]; if (std::fabs(s_val) > 1e-9) { r2[5] -= m2 * s_val; r3[5] -= m3 * s_val; }
	s_val = r1[6]; if (std::fabs(s_val) > 1e-9) { r2[6] -= m2 * s_val; r3[6] -= m3 * s_val; } s_val = r1[7]; if (std::fabs(s_val) > 1e-9) { r2[7] -= m2 * s_val; r3[7] -= m3 * s_val; }
	if (std::fabs(r3[2])>std::fabs(r2[2])) SWAP_ROWS_D(r3, r2); if (std::fabs(r2[2]) < 1e-9) return false;
	m3 = r3[2]/r2[2]; r3[3] -= m3 * r2[3]; r3[4] -= m3 * r2[4]; r3[5] -= m3 * r2[5]; r3[6] -= m3 * r2[6]; r3[7] -= m3 * r2[7];
	if (std::fabs(r3[3]) < 1e-9) return false;
	s_val = 1.0/r3[3]; r3[4] *= s_val; r3[5] *= s_val; r3[6] *= s_val; r3[7] *= s_val;
	m2 = r2[3]; s_val = 1.0/r2[2]; r2[4] = s_val * (r2[4] - r3[4] * m2); r2[5] = s_val * (r2[5] - r3[5] * m2); r2[6] = s_val * (r2[6] - r3[6] * m2); r2[7] = s_val * (r2[7] - r3[7] * m2);
	m1 = r1[3]; r1[4] -= r3[4] * m1; r1[5] -= r3[5] * m1; r1[6] -= r3[6] * m1; r1[7] -= r3[7] * m1;
	m0 = r0[3]; r0[4] -= r3[4] * m0; r0[5] -= r3[5] * m0; r0[6] -= r3[6] * m0; r0[7] -= r3[7] * m0;
	m1 = r1[2]; s_val = 1.0/r1[1]; r1[4] = s_val * (r1[4] - r2[4] * m1); r1[5] = s_val * (r1[5] - r2[5] * m1); r1[6] = s_val * (r1[6] - r2[6] * m1); r1[7] = s_val * (r1[7] - r2[7] * m1);
	m0 = r0[2]; r0[4] -= r2[4] * m0; r0[5] -= r2[5] * m0; r0[6] -= r2[6] * m0; r0[7] -= r2[7] * m0;
	m0 = r0[1]; s_val = 1.0/r0[0]; r0[4] = s_val * (r0[4] - r1[4] * m0); r0[5] = s_val * (r0[5] - r1[5] * m0); r0[6] = s_val * (r0[6] - r1[6] * m0); r0[7] = s_val * (r0[7] - r1[7] * m0);
	MAT_D(dst,0,0) = r0[4]; MAT_D(dst,0,1) = r0[5]; MAT_D(dst,0,2) = r0[6]; MAT_D(dst,0,3) = r0[7];
	MAT_D(dst,1,0) = r1[4]; MAT_D(dst,1,1) = r1[5]; MAT_D(dst,1,2) = r1[6]; MAT_D(dst,1,3) = r1[7];
	MAT_D(dst,2,0) = r2[4]; MAT_D(dst,2,1) = r2[5]; MAT_D(dst,2,2) = r2[6]; MAT_D(dst,2,3) = r2[7];
	MAT_D(dst,3,0) = r3[4]; MAT_D(dst,3,1) = r3[5]; MAT_D(dst,3,2) = r3[6]; MAT_D(dst,3,3) = r3[7];
	return true;
    #undef MAT_D
    #undef SWAP_ROWS_D
}

void m3dProjectXY(const M3DMatrix44f mModelView, const M3DMatrix44f mProjection, const int iViewPort[4], const M3DVector3f vPointIn, M3DVector2f vPointOut)
{
    M3DVector4f vBack, vForth;
	memcpy(vBack, vPointIn, sizeof(float)*3); vBack[3] = 1.0f;
    m3dTransformVector4(vForth, vBack, mModelView);
    m3dTransformVector4(vBack, vForth, mProjection);
    if(!m3dCloseEnough(vBack[3], 0.0f, 1e-6f)) { // Epsilon for float comparisons
        float div = 1.0f / vBack[3];
        vBack[0] *= div; vBack[1] *= div;
    }
    vPointOut[0] = vBack[0] * 0.5f + 0.5f; vPointOut[1] = vBack[1] * 0.5f + 0.5f;
    vPointOut[0] = (vPointOut[0] * iViewPort[2]) + iViewPort[0];
    vPointOut[1] = (vPointOut[1] * iViewPort[3]) + iViewPort[1];
}

void m3dProjectXYZ(const M3DMatrix44f mModelView, const M3DMatrix44f mProjection, const int iViewPort[4], const M3DVector3f vPointIn, M3DVector3f vPointOut)
{
    M3DVector4f vBack, vForth;
	memcpy(vBack, vPointIn, sizeof(float)*3); vBack[3] = 1.0f;
    m3dTransformVector4(vForth, vBack, mModelView);
    m3dTransformVector4(vBack, vForth, mProjection);
    if(!m3dCloseEnough(vBack[3], 0.0f, 1e-6f)) {
        float div = 1.0f / vBack[3];
        vBack[0] *= div; vBack[1] *= div; vBack[2] *= div;
    }
    vPointOut[0] = vBack[0] * 0.5f + 0.5f; vPointOut[1] = vBack[1] * 0.5f + 0.5f; vPointOut[2] = vBack[2] * 0.5f + 0.5f;
    vPointOut[0] = (vPointOut[0] * iViewPort[2]) + iViewPort[0];
    vPointOut[1] = (vPointOut[1] * iViewPort[3]) + iViewPort[1];
}

void m3dFindNormal(M3DVector3f result, const M3DVector3f point1, const M3DVector3f point2, const M3DVector3f point3)
{
	M3DVector3f v1,v2;
	v1[0] = point1[0] - point2[0]; v1[1] = point1[1] - point2[1]; v1[2] = point1[2] - point2[2];
	v2[0] = point2[0] - point3[0]; v2[1] = point2[1] - point3[1]; v2[2] = point2[2] - point3[2];
	m3dCrossProduct(result, v1, v2);
}

void m3dFindNormal(M3DVector3d result, const M3DVector3d point1, const M3DVector3d point2, const M3DVector3d point3)
{
	M3DVector3d v1,v2;
	v1[0] = point1[0] - point2[0]; v1[1] = point1[1] - point2[1]; v1[2] = point1[2] - point2[2];
	v2[0] = point2[0] - point3[0]; v2[1] = point2[1] - point3[1]; v2[2] = point2[2] - point3[2];
	m3dCrossProduct(result, v1, v2);
}

void m3dGetPlaneEquation(M3DVector4f planeEq, const M3DVector3f p1, const M3DVector3f p2, const M3DVector3f p3)
{
    M3DVector3f v1, v2;
    v1[0] = p3[0] - p1[0]; v1[1] = p3[1] - p1[1]; v1[2] = p3[2] - p1[2];
    v2[0] = p2[0] - p1[0]; v2[1] = p2[1] - p1[1]; v2[2] = p2[2] - p1[2];
    m3dCrossProduct(planeEq, v1, v2);
    m3dNormalizeVector(planeEq);
    planeEq[3] = -(planeEq[0] * p3[0] + planeEq[1] * p3[1] + planeEq[2] * p3[2]);
}

void m3dGetPlaneEquation(M3DVector4d planeEq, const M3DVector3d p1, const M3DVector3d p2, const M3DVector3d p3)
{
    M3DVector3d v1, v2;
    v1[0] = p3[0] - p1[0]; v1[1] = p3[1] - p1[1]; v1[2] = p3[2] - p1[2];
    v2[0] = p2[0] - p1[0]; v2[1] = p2[1] - p1[1]; v2[2] = p2[2] - p1[2];
    m3dCrossProduct(planeEq, v1, v2);
    m3dNormalizeVector(planeEq);
    planeEq[3] = -(planeEq[0] * p3[0] + planeEq[1] * p3[1] + planeEq[2] * p3[2]);
}

void m3dCatmullRom3(M3DVector3f vOut, M3DVector3f vP0, M3DVector3f vP1, M3DVector3f vP2, M3DVector3f vP3, float t)
{
    float t2 = t * t; float t3 = t2 * t;
    vOut[0] = 0.5f * ((2.0f * vP1[0]) + (-vP0[0] + vP2[0]) * t + (2.0f*vP0[0] - 5.0f*vP1[0] + 4.0f*vP2[0] - vP3[0]) * t2 + (-vP0[0] + 3.0f*vP1[0] - 3.0f*vP2[0] + vP3[0]) * t3);
    vOut[1] = 0.5f * ((2.0f * vP1[1]) + (-vP0[1] + vP2[1]) * t + (2.0f*vP0[1] - 5.0f*vP1[1] + 4.0f*vP2[1] - vP3[1]) * t2 + (-vP0[1] + 3.0f*vP1[1] - 3.0f*vP2[1] + vP3[1]) * t3);
    vOut[2] = 0.5f * ((2.0f * vP1[2]) + (-vP0[2] + vP2[2]) * t + (2.0f*vP0[2] - 5.0f*vP1[2] + 4.0f*vP2[2] - vP3[2]) * t2 + (-vP0[2] + 3.0f*vP1[2] - 3.0f*vP2[2] + vP3[2]) * t3);
}

void m3dCatmullRom3(M3DVector3d vOut, M3DVector3d vP0, M3DVector3d vP1, M3DVector3d vP2, M3DVector3d vP3, double t)
{
    double t2 = t * t; double t3 = t2 * t;
    vOut[0] = 0.5 * ((2.0 * vP1[0]) + (-vP0[0] + vP2[0]) * t + (2.0*vP0[0] - 5.0*vP1[0] + 4.0*vP2[0] - vP3[0]) * t2 + (-vP0[0] + 3.0*vP1[0] - 3.0*vP2[0] + vP3[0]) * t3);
    vOut[1] = 0.5 * ((2.0 * vP1[1]) + (-vP0[1] + vP2[1]) * t + (2.0*vP0[1] - 5.0*vP1[1] + 4.0*vP2[1] - vP3[1]) * t2 + (-vP0[1] + 3.0*vP1[1] - 3.0*vP2[1] + vP3[1]) * t3);
    vOut[2] = 0.5 * ((2.0 * vP1[2]) + (-vP0[2] + vP2[2]) * t + (2.0*vP0[2] - 5.0*vP1[2] + 4.0*vP2[2] - vP3[2]) * t2 + (-vP0[2] + 3.0*vP1[2] - 3.0*vP2[2] + vP3[2]) * t3);
}

double m3dRaySphereTest(const M3DVector3d point, const M3DVector3d ray, const M3DVector3d sphereCenter, double sphereRadius)
{
	M3DVector3d rayToCenter;
	rayToCenter[0] = sphereCenter[0] - point[0]; rayToCenter[1] = sphereCenter[1] - point[1]; rayToCenter[2] = sphereCenter[2] - point[2];
	double a = m3dDotProduct(rayToCenter, ray);
	double distance2 = m3dDotProduct(rayToCenter, rayToCenter);
	double dRet = (sphereRadius * sphereRadius) - distance2 + (a*a);
	if(dRet > 1e-9) dRet = a - std::sqrt(dRet); // Check against epsilon for positive
    else if (dRet < -1e-9) return dRet; // No intersection
    else dRet = a; // Tangent or very close
	return dRet;
}

float m3dRaySphereTest(const M3DVector3f point, const M3DVector3f ray, const M3DVector3f sphereCenter, float sphereRadius)
{
	M3DVector3f rayToCenter;
	rayToCenter[0] = sphereCenter[0] - point[0]; rayToCenter[1] = sphereCenter[1] - point[1]; rayToCenter[2] = sphereCenter[2] - point[2];
	float a = m3dDotProduct(rayToCenter, ray);
	float distance2 = m3dDotProduct(rayToCenter, rayToCenter);
	float dRet = (sphereRadius * sphereRadius) - distance2 + (a*a);
	if(dRet > 1e-6f) dRet = a - std::sqrt(dRet);
    else if (dRet < -1e-6f) return dRet;
    else dRet = a;
	return dRet;
}

void m3dCalculateTangentBasis(const M3DVector3f vTriangle[3], const M3DVector2f vTexCoords[3], const M3DVector3f N, M3DVector3f vTangent)
{
    M3DVector3f dv2v1, dv3v1; float dc2c1t, dc2c1b, dc3c1t, dc3c1b; float M_val;
    m3dSubtractVectors3(dv2v1, vTriangle[1], vTriangle[0]); m3dSubtractVectors3(dv3v1, vTriangle[2], vTriangle[0]);
    dc2c1t = vTexCoords[1][0] - vTexCoords[0][0]; dc2c1b = vTexCoords[1][1] - vTexCoords[0][1];
    dc3c1t = vTexCoords[2][0] - vTexCoords[0][0]; dc3c1b = vTexCoords[2][1] - vTexCoords[0][1];
    M_val = (dc2c1t * dc3c1b) - (dc3c1t * dc2c1b);
    if (std::fabs(M_val) < 1e-6f) { vTangent[0]=1.0f; vTangent[1]=0.0f; vTangent[2]=0.0f; return; }
    M_val = 1.0f / M_val;
    m3dScaleVector3(dv2v1, dc3c1b); m3dScaleVector3(dv3v1, dc2c1b);
    m3dSubtractVectors3(vTangent, dv2v1, dv3v1); m3dScaleVector3(vTangent, M_val); m3dNormalizeVector(vTangent);
    M3DVector3f B_vec;
    m3dCrossProduct(B_vec, N, vTangent); m3dCrossProduct(vTangent, B_vec, N); m3dNormalizeVector(vTangent);
}

double m3dSmoothStep(double edge1, double edge2, double x)
{
    double t_ratio = (edge2 - edge1);
    if (std::fabs(t_ratio) < 1e-9) t_ratio = (x >= edge1) ? 1e9 : -1e9; // Avoid div by zero if edges are same
    double t = (x - edge1) / t_ratio;
    if(t > 1.0) t = 1.0; if(t < 0.0) t = 0.0;
    return t * t * (3.0 - 2.0 * t);
}

float m3dSmoothStep(float edge1, float edge2, float x)
{
    float t_ratio = (edge2 - edge1);
    if (std::fabs(t_ratio) < 1e-6f) t_ratio = (x >= edge1) ? 1e6f : -1e6f;
    float t = (x - edge1) / t_ratio;
    if(t > 1.0f) t = 1.0f; if(t < 0.0f) t = 0.0f;
    return t * t * (3.0f - 2.0f * t);
}

void m3dMakePlanarShadowMatrix(M3DMatrix44f proj, const M3DVector4f planeEq, const M3DVector3f vLightPos)
{
	float a = planeEq[0], b = planeEq[1], c = planeEq[2], d = planeEq[3];
	float dx = -vLightPos[0], dy = -vLightPos[1], dz = -vLightPos[2];
	proj[0] = b*dy + c*dz; proj[1] = -a*dy; proj[2] = -a*dz; proj[3] = 0.0f;
	proj[4] = -b*dx; proj[5] = a*dx + c*dz; proj[6] = -b*dz; proj[7] = 0.0f;
	proj[8] = -c*dx; proj[9] = -c*dy; proj[10] = a*dx + b*dy; proj[11] = 0.0f;
	proj[12] = -d*dx; proj[13] = -d*dy; proj[14] = -d*dz; proj[15] = a*dx + b*dy + c*dz;
}

void m3dMakePlanarShadowMatrix(M3DMatrix44d proj, const M3DVector4d planeEq, const M3DVector3f vLightPos_f)
{
	double a = planeEq[0], b = planeEq[1], c = planeEq[2], d = planeEq[3];
	double dx = -static_cast<double>(vLightPos_f[0]);
	double dy = -static_cast<double>(vLightPos_f[1]);
	double dz = -static_cast<double>(vLightPos_f[2]);
	proj[0] = b*dy + c*dz; proj[1] = -a*dy; proj[2] = -a*dz; proj[3] = 0.0;
	proj[4] = -b*dx; proj[5] = a*dx + c*dz; proj[6] = -b*dz; proj[7] = 0.0;
	proj[8] = -c*dx; proj[9] = -c*dy; proj[10] = a*dx + b*dy; proj[11] = 0.0;
	proj[12] = -d*dx; proj[13] = -d*dy; proj[14] = -d*dz; proj[15] = a*dx + b*dy + c*dz;
}

double m3dClosestPointOnRay(M3DVector3d vPointOnRay, const M3DVector3d vRayOrigin, const M3DVector3d vUnitRayDir, const M3DVector3d vPointInSpace)
{
	M3DVector3d v_vec; m3dSubtractVectors3(v_vec, vPointInSpace, vRayOrigin);
	double t = m3dDotProduct(vUnitRayDir, v_vec);
	vPointOnRay[0] = vRayOrigin[0] + (t * vUnitRayDir[0]); vPointOnRay[1] = vRayOrigin[1] + (t * vUnitRayDir[1]); vPointOnRay[2] = vRayOrigin[2] + (t * vUnitRayDir[2]);
	return m3dGetDistanceSquared(vPointOnRay, vPointInSpace);
}

float m3dClosestPointOnRay(M3DVector3f vPointOnRay, const M3DVector3f vRayOrigin, const M3DVector3f vUnitRayDir, const M3DVector3f vPointInSpace)
{
	M3DVector3f v_vec; m3dSubtractVectors3(v_vec, vPointInSpace, vRayOrigin);
	float t = m3dDotProduct(vUnitRayDir, v_vec);
	vPointOnRay[0] = vRayOrigin[0] + (t * vUnitRayDir[0]); vPointOnRay[1] = vRayOrigin[1] + (t * vUnitRayDir[1]); vPointOnRay[2] = vRayOrigin[2] + (t * vUnitRayDir[2]);
	return m3dGetDistanceSquared(vPointOnRay, vPointInSpace);
}
// END Math3D Library Content /////////////////////////////////////////////////////


// Rotation amounts
static GLfloat xRot = 0.0f;
static GLfloat yRot = 0.0f;

// Light values (material properties are handled by glColorMaterial)
GLfloat  ambientLight[] = { 0.3f, 0.3f, 0.3f, 1.0f };
GLfloat  diffuseLight[] = { 0.7f, 0.7f, 0.7f, 1.0f };
GLfloat  specular[] = { 1.0f, 1.0f, 1.0f, 1.0f}; // For light source
GLfloat  specref[] =  { 1.0f, 1.0f, 1.0f, 1.0f }; // For material specularity

// Define 4 light positions
GLfloat lightPos0[] = { -75.0f, 150.0f, -50.0f, 0.0f };
GLfloat lightPos1[] = { 75.0f, 100.0f, 50.0f, 1.0f };
GLfloat lightPos2[] = { 0.0f, 150.0f, 75.0f, 0.0f };
GLfloat lightPos3[] = { 50.0f, 100.0f, -75.0f, 1.0f };

GLfloat* allLightPos[4] = { lightPos0, lightPos1, lightPos2, lightPos3 };
int currentLightIndex = 0;
GLfloat currentLightPos[4];

M3DMatrix44f shadowMat;
M3DVector4f vPlaneEquation;


void DrawJet(int nShadow)
	{
	M3DVector3f vNormal;

	if(nShadow == 0) glColor3ub(255, 0, 0);
	else glColor3ub(128, 0, 0);

	glBegin(GL_TRIANGLES);
        glNormal3f(0.0f, -1.0f, 0.0f);
		glVertex3f(0.0f, 0.0f, 60.0f); glVertex3f(-15.0f, 0.0f, 30.0f); glVertex3f(15.0f,0.0f,30.0f);
        { M3DVector3f vP[] = {{15.0f,0.0f,30.0f},{0.0f,15.0f,30.0f},{0.0f,0.0f,60.0f}}; m3dFindNormal(vNormal,vP[0],vP[1],vP[2]); glNormal3fv(vNormal); glVertex3fv(vP[0]); glVertex3fv(vP[1]); glVertex3fv(vP[2]); }
        { M3DVector3f vP[] = {{0.0f,0.0f,60.0f},{0.0f,15.0f,30.0f},{-15.0f,0.0f,30.0f}}; m3dFindNormal(vNormal,vP[0],vP[1],vP[2]); glNormal3fv(vNormal); glVertex3fv(vP[0]); glVertex3fv(vP[1]); glVertex3fv(vP[2]); }
        { M3DVector3f vP[] = {{-15.0f,0.0f,30.0f},{0.0f,15.0f,30.0f},{0.0f,0.0f,-56.0f}}; m3dFindNormal(vNormal,vP[0],vP[1],vP[2]); glNormal3fv(vNormal); glVertex3fv(vP[0]); glVertex3fv(vP[1]); glVertex3fv(vP[2]); }
        { M3DVector3f vP[] = {{0.0f,0.0f,-56.0f},{0.0f,15.0f,30.0f},{15.0f,0.0f,30.0f}}; m3dFindNormal(vNormal,vP[0],vP[1],vP[2]); glNormal3fv(vNormal); glVertex3fv(vP[0]); glVertex3fv(vP[1]); glVertex3fv(vP[2]); }
		glNormal3f(0.0f, -1.0f, 0.0f); glVertex3f(15.0f,0.0f,30.0f); glVertex3f(-15.0f,0.0f,30.0f); glVertex3f(0.0f,0.0f,-56.0f);
        { M3DVector3f vP[] = {{0.0f,2.0f,27.0f},{-60.0f,2.0f,-8.0f},{60.0f,2.0f,-8.0f}}; m3dFindNormal(vNormal,vP[0],vP[1],vP[2]); glNormal3fv(vNormal); glVertex3fv(vP[0]); glVertex3fv(vP[1]); glVertex3fv(vP[2]); }
        { M3DVector3f vP[] = {{0.0f,2.0f,27.0f},{60.0f,2.0f,-8.0f},{0.0f,7.0f,-8.0f}}; m3dFindNormal(vNormal,vP[0],vP[1],vP[2]); glNormal3fv(vNormal); glVertex3fv(vP[0]); glVertex3fv(vP[1]); glVertex3fv(vP[2]); }
        { M3DVector3f vP[] = {{60.0f,2.0f,-8.0f},{-60.0f,2.0f,-8.0f},{0.0f,7.0f,-8.0f}}; m3dFindNormal(vNormal,vP[0],vP[1],vP[2]); glNormal3fv(vNormal); glVertex3fv(vP[0]); glVertex3fv(vP[1]); glVertex3fv(vP[2]); }
        { M3DVector3f vP[] = {{0.0f,2.0f,27.0f},{0.0f,7.0f,-8.0f},{-60.0f,2.0f,-8.0f}}; m3dFindNormal(vNormal,vP[0],vP[1],vP[2]); glNormal3fv(vNormal); glVertex3fv(vP[0]); glVertex3fv(vP[1]); glVertex3fv(vP[2]); }
		glNormal3f(0.0f,-1.0f,0.0f); glVertex3f(-30.0f,-0.50f,-57.0f); glVertex3f(30.0f,-0.50f,-57.0f); glVertex3f(0.0f,-0.50f,-40.0f);
        { M3DVector3f vP[] = {{0.0f,-0.5f,-40.0f},{30.0f,-0.5f,-57.0f},{0.0f,4.0f,-57.0f}}; m3dFindNormal(vNormal,vP[0],vP[1],vP[2]); glNormal3fv(vNormal); glVertex3fv(vP[0]); glVertex3fv(vP[1]); glVertex3fv(vP[2]); }
        { M3DVector3f vP[] = {{0.0f,4.0f,-57.0f},{-30.0f,-0.5f,-57.0f},{0.0f,-0.5f,-40.0f}}; m3dFindNormal(vNormal,vP[0],vP[1],vP[2]); glNormal3fv(vNormal); glVertex3fv(vP[0]); glVertex3fv(vP[1]); glVertex3fv(vP[2]); }
        { M3DVector3f vP[] = {{30.0f,-0.5f,-57.0f},{-30.0f,-0.5f,-57.0f},{0.0f,4.0f,-57.0f}}; m3dFindNormal(vNormal,vP[0],vP[1],vP[2]); glNormal3fv(vNormal); glVertex3fv(vP[0]); glVertex3fv(vP[1]); glVertex3fv(vP[2]); }
        { M3DVector3f vP[] = {{0.0f,0.5f,-40.0f},{3.0f,0.5f,-57.0f},{0.0f,25.0f,-65.0f}}; m3dFindNormal(vNormal,vP[0],vP[1],vP[2]); glNormal3fv(vNormal); glVertex3fv(vP[0]); glVertex3fv(vP[1]); glVertex3fv(vP[2]); }
        { M3DVector3f vP[] = {{0.0f,25.0f,-65.0f},{-3.0f,0.5f,-57.0f},{0.0f,0.5f,-40.0f}}; m3dFindNormal(vNormal,vP[0],vP[1],vP[2]); glNormal3fv(vNormal); glVertex3fv(vP[0]); glVertex3fv(vP[1]); glVertex3fv(vP[2]); }
        { M3DVector3f vP[] = {{3.0f,0.5f,-57.0f},{-3.0f,0.5f,-57.0f},{0.0f,25.0f,-65.0f}}; m3dFindNormal(vNormal,vP[0],vP[1],vP[2]); glNormal3fv(vNormal); glVertex3fv(vP[0]); glVertex3fv(vP[1]); glVertex3fv(vP[2]); }
    glEnd();
	}

void RenderScene(void)
    {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glBegin(GL_QUADS);
        glColor3ub(0,32,0); glVertex3f(400.0f,-150.0f,-200.0f); glVertex3f(-400.0f,-150.0f,-200.0f);
        glColor3ub(0,255,0); glVertex3f(-400.0f,-150.0f,200.0f); glVertex3f(400.0f,-150.0f,200.0f);
    glEnd();

    glPushMatrix();
        glEnable(GL_LIGHTING); glLightfv(GL_LIGHT0,GL_POSITION,currentLightPos);
        glRotatef(xRot,1.0f,0.0f,0.0f); glRotatef(yRot,0.0f,1.0f,0.0f);
        DrawJet(0);
    glPopMatrix();

    glDisable(GL_DEPTH_TEST); glDisable(GL_LIGHTING);
    glPushMatrix();
        glMultMatrixf((GLfloat*)shadowMat);
        glRotatef(xRot,1.0f,0.0f,0.0f); glRotatef(yRot,0.0f,1.0f,0.0f);
        DrawJet(1);
    glPopMatrix();
    glEnable(GL_LIGHTING); glEnable(GL_DEPTH_TEST);

    if (currentLightPos[3] == 1.0f) {
        glPushMatrix();
            glTranslatef(currentLightPos[0],currentLightPos[1],currentLightPos[2]);
            glColor3ub(255,255,0);
            glDisable(GL_LIGHTING); glutSolidSphere(5.0f,10,10); glEnable(GL_LIGHTING);
        glPopMatrix();
    }
    glutSwapBuffers();
    }

void SetupRC()
    {
    M3DVector3f points[3] = {{-30.0f,-150.0f,-20.0f},{-30.0f,-150.0f,20.0f},{40.0f,-150.0f,20.0f}};

    glEnable(GL_DEPTH_TEST); glEnable(GL_CULL_FACE); glFrontFace(GL_CCW);
    glLightfv(GL_LIGHT0,GL_AMBIENT,ambientLight); glLightfv(GL_LIGHT0,GL_DIFFUSE,diffuseLight);
    glLightfv(GL_LIGHT0,GL_SPECULAR,specular); glEnable(GL_LIGHT0);
    glEnable(GL_COLOR_MATERIAL); glColorMaterial(GL_FRONT,GL_AMBIENT_AND_DIFFUSE);
    glMaterialfv(GL_FRONT,GL_SPECULAR,specref); glMateriali(GL_FRONT,GL_SHININESS,128);
    glClearColor(0.0f,0.0f,1.0f,1.0f);
    m3dGetPlaneEquation(vPlaneEquation,points[0],points[1],points[2]);
    memcpy(currentLightPos,allLightPos[currentLightIndex],sizeof(GLfloat)*4);
    m3dMakePlanarShadowMatrix(shadowMat,vPlaneEquation,currentLightPos);
    glEnable(GL_NORMALIZE);
    }

void Keyboard(unsigned char key, int x, int y) {
    bool lightChanged = false;
    switch (key) {
        case '1': currentLightIndex = 0; lightChanged = true; break;
        case '2': currentLightIndex = 1; lightChanged = true; break;
        case '3': currentLightIndex = 2; lightChanged = true; break;
        case '4': currentLightIndex = 3; lightChanged = true; break;
        case 'r': case 'R': xRot = 0.0f; yRot = 0.0f; break;
        default: return;
    }
    if (lightChanged) {
        memcpy(currentLightPos,allLightPos[currentLightIndex],sizeof(GLfloat)*4);
        m3dMakePlanarShadowMatrix(shadowMat,vPlaneEquation,currentLightPos);
    }
    glutPostRedisplay();
}

void SpecialKeys(int key, int x, int y) {
    if(key == GLUT_KEY_UP) xRot-=5.0f; if(key == GLUT_KEY_DOWN) xRot+=5.0f;
    if(key == GLUT_KEY_LEFT) yRot-=5.0f; if(key == GLUT_KEY_RIGHT) yRot+=5.0f;
    xRot = fmod(xRot,360.0f); if(xRot<0.0f) xRot+=360.0f;
    yRot = fmod(yRot,360.0f); if(yRot<0.0f) yRot+=360.0f;
    glutPostRedisplay();
}

void ChangeSize(int w, int h) {
    if(h==0) h=1; glViewport(0,0,w,h);
    glMatrixMode(GL_PROJECTION); glLoadIdentity();
    gluPerspective(60.0f,(GLfloat)w/(GLfloat)h,1.0,1000.0);
    glMatrixMode(GL_MODELVIEW); glLoadIdentity();
    glTranslatef(0.0f,0.0f,-400.0f);
}

int main(int argc, char* argv[]) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
    glutInitWindowSize(800, 600);
    glutCreateWindow("Shadow - Modified (FreeGLUT)");
    glutReshapeFunc(ChangeSize); glutDisplayFunc(RenderScene);
    glutSpecialFunc(SpecialKeys); glutKeyboardFunc(Keyboard);
    SetupRC();
    glutMainLoop();
    return 0;
}