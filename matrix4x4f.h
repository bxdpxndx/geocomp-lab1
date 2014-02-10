//-----------------------------------------------------------------------------
//           Name: matrix4x4f.h
//         Author: Kevin Harris
//  Last Modified: 09/22/02
//    Description: OpenGL compatible utility class for a 4x4 matrix of floats.
//                 NOTE: This class has been left unoptimized for readability.
//-----------------------------------------------------------------------------

#ifndef _MATRIX4X4F_H_
#define _MATRIX4X4F_H_

#include "vector3f.h"
#include <memory>
#include <cmath>
#include <cstring>
#include <iostream>

#define DEGTORAD(degree) ((degree) * (3.141592654f / 180.0f))
#define RADTODEG(radian) ((radian) * (180.0f / 3.141592654f))

class matrix4x4f
{
public:

	float m[16];

    matrix4x4f()  { identity(); }

	matrix4x4f( float m0, float m4, float  m8, float m12,
                float m1, float m5, float  m9, float m13,
                float m2, float m6, float m10, float m14,
                float m3, float m7, float m11, float m15 );

	void identity(void);
	void translate(const vector3f &trans);
	void translate_x(const float &dist);
	void translate_y(const float &dist);
	void translate_z(const float &dist);
	void rotate(const float &angle, const vector3f &axis);
	void rotate_x(const float &angle);
	void rotate_y(const float &angle);
	void rotate_z(const float &angle);
	void scale(const vector3f &scale);
    void transformPoint( vector3f *vec );
    void transformVector( vector3f *vec );

	// Static utility methods
	static matrix4x4f invertMatrix( const matrix4x4f *matIn );
	
    // Operators...
	matrix4x4f operator + (const matrix4x4f &other);
	matrix4x4f operator - (const matrix4x4f &other);
	matrix4x4f operator * (const matrix4x4f &other);

	matrix4x4f operator * (const float scalar);
};

#endif // _MATRIX4X4F_H_

