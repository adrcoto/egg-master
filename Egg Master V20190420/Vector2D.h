#pragma once
#include <iostream>
using namespace std;

class Vector2D {
public:
	float x, y;

	Vector2D();
	Vector2D(float, float);
	~Vector2D();

	Vector2D& add(const Vector2D&);
	Vector2D& subtract(const Vector2D&);
	Vector2D& multiply(const Vector2D&);
	Vector2D& divide(const Vector2D&);

	/*
	friend Vector2D& operator+(Vector2D&, const Vector2D&);
	friend Vector2D& operator-(Vector2D&, const Vector2D&);
	friend Vector2D& operator*(Vector2D&, const Vector2D&);
	friend Vector2D& operator/(Vector2D&, const Vector2D&);
	*/

	Vector2D operator+(const Vector2D&);
	Vector2D operator-(const Vector2D&);
	Vector2D operator*(const Vector2D&);
	Vector2D operator/(const Vector2D&);

	Vector2D& operator+=(const Vector2D&);
	Vector2D& operator-=(const Vector2D&);
	Vector2D& operator*=(const Vector2D&);
	Vector2D& operator/=(const Vector2D&);

	friend ostream& operator<<(ostream&, const Vector2D&);

	Vector2D& operator+(const int&);
	Vector2D& operator*(const int&);
	
	Vector2D& zero();
};

