#include "Vector2D.h"



Vector2D::Vector2D() {
	x = y = 0.0f;
}

Vector2D::Vector2D(float x, float y) {
	this->x = x;
	this->y = y;
}


Vector2D::~Vector2D(){}

Vector2D& Vector2D::add(const Vector2D& v) {
	this->x += v.x;
	this->x += v.y;
	
	return *this;
}

Vector2D& Vector2D::subtract(const Vector2D& v)
{
	this->x -= v.x;
	this->x -= v.y;

	return *this;
}

Vector2D& Vector2D::multiply(const Vector2D& v)
{
	this->x *= v.x;
	this->x *= v.y;

	return *this;
}

Vector2D& Vector2D::divide(const Vector2D& v)
{
	this->x /= v.x;
	this->x /= v.y;

	return *this;
}

/*
Vector2D& operator+(Vector2D& v1, const Vector2D& v2) {
	return v1.add(v2);
}


Vector2D& operator-(Vector2D&v1, const Vector2D& v2) {
	return v1.subtract(v2);
}

Vector2D& operator*(Vector2D& v1, const Vector2D& v2) {
	return v1.multiply(v2);
}

Vector2D& operator/(Vector2D& v1, const Vector2D& v2) {
	return v1.divide(v2);
}

Vector2D& Vector2D::operator+=(const Vector2D& v) {
	return this->add(v);
}
*/

Vector2D Vector2D::operator+ (const Vector2D& v) {
	Vector2D result;
	result.x = this->x + v.x;
	result.y = this->y + v.y;
	return result;
}

Vector2D Vector2D::operator- (const Vector2D& v) {
	Vector2D result;
	result.x = this->x - v.x;
	result.y = this->y - v.y;
	return result;
}

Vector2D Vector2D::operator* (const Vector2D& v) {
	Vector2D result;
	result.x = this->x * v.x;
	result.y = this->y * v.y;
	return result;
}

Vector2D Vector2D::operator/ (const Vector2D& v) {
	Vector2D result;
	result.x = this->x / v.x;
	result.y = this->y / v.y;
	return result;
}

Vector2D& Vector2D::operator-=(const Vector2D& v) {
	return this->subtract(v);
}

Vector2D& Vector2D::operator*=(const Vector2D& v) {
	return this->multiply(v);
}

Vector2D& Vector2D::operator/=(const Vector2D& v) {
	return this->divide(v);
}

ostream& operator<<(ostream& stream, const Vector2D& v) {
	stream << "(" << v.x << ", " << v.y << ");" << endl;
	return stream;
}