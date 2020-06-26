#include <iostream>
#include <string>

using namespace std;

class Vector2D
{
private:
	double x = 0;
	double y = 0;

public:
	Vector2D() {}
	Vector2D(double x, double y) :
		x(x), y(y) {}
	int16_t getX() const;
	int16_t getY() const;
	void normaliza();
	float magnitude();
	Vector2D operator+(const Vector2D& v) const;
	Vector2D operator-(const Vector2D& v) const;
	Vector2D operator*(double d) const;
	double operator*(const Vector2D& v) const;

};
