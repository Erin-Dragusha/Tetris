#include "Tetromino.h"

Tetromino::Tetromino()
{
	setShape(TetShape::I);
}

void Tetromino::setShape(TetShape shape)
{
	this->shape = shape;
	color = static_cast<TetColor>(shape);

	switch (shape) 
	{
		case TetShape::O :
		{
			blockLocs = { Point{0, 1}, Point{1, 1}, Point{0, 0}, Point{1, 0} };
			break;
		}
		case TetShape::I :
		{
			blockLocs = { Point{0, -1}, Point{0, 0}, Point{0, 1}, Point{0, 2} };
			break;
		}
		case TetShape::S :
		{
			blockLocs = { Point{-1, 0}, Point{0, 0}, Point{0, 1}, Point{1, 1} };
			break;
		}
		case TetShape::Z :
		{
			blockLocs = { Point{-1, 1}, Point{0, 1}, Point{0, 0}, Point{1, 0} };
			break;
		}
		case TetShape::L :
		{
			blockLocs = { Point{0, 1}, Point{0, 0}, Point{0, -1}, Point{1, -1} };
			break;
		}
		case TetShape::J :
		{
			blockLocs = { Point{-1, -1}, Point{0, -1}, Point{0, 0}, Point{0, 1} };
			break;
		}
		case TetShape::T :
		{
			blockLocs = { Point{-1, 0}, Point{0, 0}, Point{1, 0}, Point{0, -1} };
			break;
		}
	}
	
}

void Tetromino::rotateClockwise() 
{
	for (Point& point : blockLocs) {
		point.multiplyX(-1);
		point.swapXY();
	}
}

void Tetromino::printToConsole() const
{
	for (int y = 3; y >= -3; y--) {
		std::cout << "\n";
		for (int x = -3; x <= 3; x++) {
			bool match = false;
			for (const Point& point : blockLocs) {
				if (point.getX() == x && point.getY() == y) match = true;
			}
			if (match)
			{
				std::cout << "X";
			}
			else std::cout << ".";
		}
	}
	std::cout << "\n";
}

TetShape Tetromino::getRandomShape()
{
	int randomShape = rand() % static_cast<int>(TetShape::COUNT);
	return static_cast<TetShape>(randomShape);
}