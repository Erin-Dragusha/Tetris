#include "GridTetromino.h"
#include "Point.h"

// constructor, initialize gridLoc to 0,0
GridTetromino::GridTetromino() : gridLoc{0, 0} 
{

}

// a setter for the tetronimo's location 
// - param 1: int x
// - param 2: int y
// return: nothing
void GridTetromino::setGridLoc(int x, int y)
{
	gridLoc.setXY(x, y);
}

// a setter for the tetromino's location
// param 1: a Point (the new gridLoc)
// return: nothing
void GridTetromino::setGridLoc(const Point& pt)
{
	gridLoc = pt;
}

// transpose the gridLoc of this shape
//	(1,0) represents a move to the right (x+1)
//	(-1,0) represents a move to the left (x-1)
//	(0,-1) represents a move down (y-1)
// - param 1: int xOffset, the x offset (distance) to move
// - param 2: int yOffset, the y offset (distance) to move
// - return: nothing
void GridTetromino::move(int xOffset, int yOffset) {
	gridLoc.setXY(gridLoc.getX() + xOffset, gridLoc.getY() + yOffset);
}

// Build and return a vector of Points to represent our inherited
// blockLocs vector mapped to the gridLoc of this object instance.
// You will need to provide this class access to blockLocs (from the Tetromino class).
// eg: if we have a Point [x,y] in our vector,
// and our gridLoc is [5,6] the mapped Point would be [5+x,6+y].
// params: none:
// return: a vector of Point objects.
std::vector<Point> GridTetromino::getBlockLocsMappedToGrid() const
{
	std::vector<Point> inheritedBlockLocs;

	for (const Point& p : blockLocs)
	{
		inheritedBlockLocs.push_back(Point(p.getX() + gridLoc.getX(), p.getY() + gridLoc.getY()));
	}

	return inheritedBlockLocs;

}