#include <string>
#include "Vector2D.h"
#include <math.h>

int Vector2D::getX() const{ return this->x;}

int Vector2D::getY() const{ return this->y; }

void Vector2D::normaliza() {
	double mag = sqrt(pow(x, 2) + pow(y, 2));
	if (mag > 0.0) {
		x = x / mag;
		y = y / mag;
	}
}

Vector2D Vector2D::operator+(const Vector2D& v) const {
	Vector2D r;
	r.x = this->x + v.x;
	r.y = this->y + v.y;
	return r;
}

Vector2D Vector2D::operator-(const Vector2D& v) const {
	Vector2D r;
	r.x = this->x - v.x;
	r.y = this->y - v.y;
	return r;
}

Vector2D Vector2D::operator*(double d) const {
	Vector2D r;
	r.x = x * d;
	r.y = y * d;
	return r;
}

double Vector2D::operator*(const Vector2D& d) const {
	return d.x * x + d.y * y;
}
