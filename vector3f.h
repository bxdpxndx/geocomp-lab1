//-----------------------------------------------------------------------------
//           Name: vector3f.h
//         Author: Kevin Harris
//  Last Modified: 09/25/02
//    Description: OpenGL compatible utility class for a 3D vector of floats
//                 NOTE: This class has been left unoptimized for readability.
//-----------------------------------------------------------------------------

#ifndef _VECTOR3F_H_
#define _VECTOR3F_H_

#include <cmath>
#include <iostream>

class vector3f
{
public:

    union {
        struct { float x, y, z; };
        struct { float t[3]; };
    };

    vector3f(void)
    {
        x = 0.0f;
        y = 0.0f;
        z = 0.0f;
    }

    vector3f(float x_, float y_, float z_);
    void set(float x_, float y_, float z_);
    float length(void);
    float normalize(void);

    // Operators...
	vector3f operator + (const vector3f &other)const;
    vector3f operator - (const vector3f &other)const;
	vector3f operator * (const vector3f &other)const;
	vector3f operator / (const vector3f &other)const;

	vector3f operator * (const float scalar)const;
	friend vector3f operator * (const float scalar, const vector3f &other);
    
	vector3f& operator = (const vector3f &other);
    vector3f& operator += (const vector3f &other);
    vector3f& operator -= (const vector3f &other);

	vector3f operator + (void) const;
	vector3f operator - (void) const;

	operator const float*() const {return &x;};
    friend std::ostream & operator<<(std::ostream& os, const vector3f &v);
};

float distance(const vector3f &v1, const vector3f &v2);
float dotProduct(const vector3f &v1,  const vector3f &v2 );
vector3f crossProduct(const vector3f &v1, const vector3f &v2);


#endif // _VECTOR3F_H_
