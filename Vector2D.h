#pragma once
#include "SDK.h"

struct Vector2D
{
public:
	float x, y;

	Vector2D() {}
	Vector2D(float x_, float y_) { x = x_; y = y_; }

	__forceinline Vector2D&	operator+=(const Vector2D &v);

	Vector2D& operator=(const Vector2D &vOther);
	Vector2D operator+(const Vector2D &v) const;
	Vector2D operator-(const Vector2D &v) const;
	Vector2D operator/(const Vector2D &v) const;
	Vector2D operator*(const Vector2D &v) const;
	Vector2D operator+(float fl) const;
	Vector2D operator-(float fl) const;
	Vector2D operator/(float fl) const;
	Vector2D operator*(float fl) const;

	float toAngle();
};

__forceinline  Vector2D& Vector2D::operator+=(const Vector2D& v)
{
	CHECK_VALID(*this);
	CHECK_VALID(v);
	x += v.x; y += v.y;
	return *this;
}
inline Vector2D& Vector2D::operator=(const Vector2D &vOther)
{
	CHECK_VALID(vOther);
	x = vOther.x; y = vOther.y;
	return *this;
}
inline Vector2D Vector2D::operator+(const Vector2D& v) const
{
	Vector2D res;
	res.x = x + v.x;
	res.y = y + v.y;
	return res;
}
inline Vector2D Vector2D::operator-(const Vector2D& v) const
{
	Vector2D res;
	res.x = x - v.x;
	res.y = y - v.y;
	return res;
}
inline Vector2D Vector2D::operator/(const Vector2D& v) const
{
	Vector2D res;
	res.x = x / v.x;
	res.y = y / v.y;
	return res;
}
inline Vector2D Vector2D::operator*(const Vector2D& v) const
{
	Vector2D res;
	res.x = x * v.x;
	res.y = y * v.y;
	return res;
}
inline Vector2D Vector2D::operator+(float fl) const
{
	Vector2D res;
	res.x = x + fl;
	res.y = y + fl;
	return res;
}
inline Vector2D Vector2D::operator-(float fl) const
{
	Vector2D res;
	res.x = x - fl;
	res.y = y - fl;
	return res;
}
inline Vector2D Vector2D::operator/(float fl) const
{
	Vector2D res;
	res.x = x / fl;
	res.y = y / fl;
	return res;
}
inline Vector2D Vector2D::operator*(float fl) const
{
	Vector2D res;
	res.x = x * fl;
	res.y = y * fl;
	return res;
}
inline float Vector2D::toAngle()
{
	return atan2(this->x, this->y);
}

inline Vector2D rotate_vector(Vector2D origin, float radians, Vector2D p)
{
	float s = sin(radians);
	float c = cos(radians);

	// translate point back to origin:
	p.x -= origin.x;
	p.y -= origin.y;

	// rotate point
	float xnew = p.x * c - p.y * s;
	float ynew = p.x * s + p.y * c;

	// translate point back:
	p.x = xnew + origin.x;
	p.y = ynew + origin.y;
	return p;
}
inline Vector2D vectorAngle(float radians, float length)
{
	return Vector2D(sin(radians) * length, cos(radians) * length);
}