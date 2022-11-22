#pragma once
#include <string>
#include <ostream>
#include <sstream>

class Point
{
	friend class TestSuite;
private:
	int x;
	int y;
public:
	Point() : x{ 0 }, y{0}
	{
	}
	Point(int x, int y) 
	{
		this->x = x;
		this->y = y;
	}
	int getX() const{ return x; }
	int getY() const{ return y; }
	void setX(int x);
	void setY(int y); 
	void setXY(int x, int y);

	// swap x and y
	void swapXY();

	// multiply x by some factor
	void multiplyX(int factor);

	// multiply y by some factor
	void multiplyY(int factor);

	// return a string in the form "[x,y]" to represent the state of the Point instance
	// (for debugging) You could use stringstream if you want to try a forming a string in
	// the same way you use cout http://stev.org/post/cstringstreamexample for an example
	std::string toString() const;

};

Point operator+(const Point& p1, const Point& p2);


