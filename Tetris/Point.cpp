#include "Point.h"

void Point::setX(int x) 
{
	this->x = x;
}

void Point::setY(int y)
{
	this->y = y;
}

void Point::setXY(int x, int y)
{
	this->x = x;
	this->y = y;
}

void Point::swapXY() 
{
	int temp = x;
	x = y;
	y = temp;
}

void Point::multiplyX(int factor)
{
	x *= factor;
}

void Point::multiplyY(int factor)
{
	y *= factor;
}

std::string Point::toString() const
{
	std::stringstream ss;
	ss << "[" << x << "," << y << "]";
	return ss.str();
}

Point operator+(const Point& p1, const Point& p2)
{
	return Point(p1.getX() + p2.getX(), p1.getY() + p2.getY());
}