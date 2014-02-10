#include "vector3f.h"
#include <iostream>

vector3f::vector3f( float x_, float y_, float z_ )
{
    x = x_;
    y = y_;
    z = z_;
}

void vector3f::set( float x_, float y_, float z_ )
{
    x = x_;
    y = y_;
    z = z_;
}

float vector3f::length( void )
{
    return( (float)sqrt( x * x + y * y + z * z ) );
}

float vector3f::normalize( void )
{
	float fLength = length();

	x = x / fLength;
	y = y / fLength;
	z = z / fLength;

	return fLength;
}

float distance( const vector3f &v1,  const vector3f &v2  )
{
	float dx = v1.x - v2.x;
	float dy = v1.y - v2.y;
	float dz = v1.z - v2.z;

	return (float)sqrt( dx * dx + dy * dy + dz * dz );
}

float dotProduct( const vector3f &v1,  const vector3f &v2 )
{
	return( v1.x * v2.x + v1.y * v2.y + v1.z * v2.z  );
}

vector3f crossProduct( const vector3f &v1,  const vector3f &v2 )
{
	vector3f vCrossProduct;

	vCrossProduct.x =  v1.y * v2.z - v1.z * v2.y;
	vCrossProduct.y = -v1.x * v2.z + v1.z * v2.x;
	vCrossProduct.z =  v1.x * v2.y - v1.y * v2.x;

	return vCrossProduct;
}

// Operators...

vector3f vector3f::operator + (const vector3f &other)const
{
    vector3f vResult(0.0f, 0.0f, 0.0f);

    vResult.x = x + other.x;
    vResult.y = y + other.y;
    vResult.z = z + other.z;

    return vResult;
}

vector3f vector3f::operator + ( void ) const
{
    return *this;
}

vector3f vector3f::operator - (const vector3f &other)const
{
    vector3f vResult(0.0f, 0.0f, 0.0f);

    vResult.x = x - other.x;
    vResult.y = y - other.y;
    vResult.z = z - other.z;

    return vResult;
}

vector3f vector3f::operator - ( void ) const
{
    vector3f vResult(-x, -y, -z);

    return vResult;
}

vector3f vector3f::operator * (const vector3f &other)const
{
    vector3f vResult(0.0f, 0.0f, 0.0f);

    vResult.x = x * other.x;
    vResult.y = y * other.y;
    vResult.z = z * other.z;

    return vResult;
}

vector3f vector3f::operator * (const float scalar)const
{
    vector3f vResult(0.0f, 0.0f, 0.0f);

    vResult.x = x * scalar;
    vResult.y = y * scalar;
    vResult.z = z * scalar;

    return vResult;
}

vector3f operator * ( const float scalar, const vector3f &other )
{
    vector3f vResult(0.0f, 0.0f, 0.0f);

    vResult.x = other.x * scalar;
    vResult.y = other.y * scalar;
    vResult.z = other.z * scalar;

    return vResult;
}

vector3f vector3f::operator / (const vector3f &other)const
{
    vector3f vResult(0.0f, 0.0f, 0.0f);

    vResult.x = x / other.x;
    vResult.y = y / other.y;
    vResult.z = z / other.z;

    return vResult;
}

vector3f& vector3f::operator = ( const vector3f &other )
{
	x = other.x;
	y = other.y;
	z = other.z;

    return *this;
}

vector3f& vector3f::operator += ( const vector3f &other )
{
    x += other.x;
    y += other.y;
    z += other.z;

    return *this;
}

vector3f& vector3f::operator -= ( const vector3f &other )
{
    x -= other.x;
    y -= other.y;
    z -= other.z;

    return *this;
}

std::ostream & operator<<(std::ostream& os, const vector3f &v) {
    os << "(" << v.x << ", " << v.y << ", " << v.z <<")";
    return os;
}
