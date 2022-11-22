#include "TetrisGame.h"

// Static Constants
const int TetrisGame::BLOCK_WIDTH = 32;
const int TetrisGame::BLOCK_HEIGHT = 32;
const double TetrisGame::MAX_SECONDS_PER_TICK = 0.75;
const double TetrisGame::MIN_SECONDS_PER_TICK = 0.20;

// constructor
//   initialize/assign private member vars names that match param names
//   reset() the game
//   load font from file: fonts/RedOctober.ttf
//   setup scoreText
// - params: already specified
TetrisGame::TetrisGame(sf::RenderWindow& window, sf::Sprite& blockSprite, const Point& gameboardOffset, const Point& nextShapeOffset)
	: window{ window }, blockSprite{ blockSprite }, gameboardOffset{ gameboardOffset }, nextShapeOffset{ nextShapeOffset }
{
	reset();

	// setup our font for drawing the score
	if (!scoreFont.loadFromFile("fonts/RedOctober.ttf"))
	{
		assert(false && "Missing font: RedOctober.ttf");
	};
	scoreText.setFont(scoreFont);
	scoreText.setCharacterSize(18);
	scoreText.setFillColor(sf::Color::White);
	scoreText.setPosition(425, 325);
}

// Draw anything to do with the game,
//   includes the board, currentShape, nextShape, score
//   called every game loop
// - params: none
// - return: nothing
void TetrisGame::draw() const
{
	drawTetromino(currentShape, gameboardOffset);
	drawTetromino(nextShape, nextShapeOffset);
	drawGameboard();
	window.draw(scoreText);
}

// Event and game loop processing
// handles keypress events (up, left, right, down, space)
// - param 1: sf::Event event
// - return: nothing
void TetrisGame::onKeyPressed(const sf::Event& event)
{
	switch (event.key.code)
	{
	case sf::Keyboard::Up:
		attemptRotate(currentShape);
		break;
	case sf::Keyboard::Left:
		attemptMove(currentShape, -1, 0);
		break;
	case sf::Keyboard::Right:
		attemptMove(currentShape, 1, 0);
		break;
	case sf::Keyboard::Down:
		if (!attemptMove(currentShape, 0, 1))
		{
			lock(currentShape);
		}
		break;
	case sf::Keyboard::Space:
		drop(currentShape);
		lock(currentShape);
		break;
	default:
		break;
	}
}

// called every game loop to handle ticks & tetromino placement (locking)
// - param 1: float secondsSinceLastLoop
// return: nothing
void TetrisGame::processGameLoop(float secondsSinceLastLoop)
{
	secondsSinceLastTick += secondsSinceLastLoop;
	if (secondsSinceLastTick >= secondsPerTick)
	{
		tick();
		secondsSinceLastTick -= secondsPerTick;
	}

	if (shapePlacedSinceLastGameLoop)
	{
		shapePlacedSinceLastGameLoop = false;
		if (spawnNextShape())
		{
			pickNextShape();
			switch (board.removeCompletedRows())
			{
			case 1:
				score += 40;
				updateScoreDisplay();
				break;
			case 2:
				score += 100;
				updateScoreDisplay();
				break;
			case 3:
				score += 300;
				updateScoreDisplay();
				break;
			case 4:
				score += 1200;
				updateScoreDisplay();
				break;
			default:
				break;
			}
			determineSecondsPerTick();
		}
		else
		{
			reset();
		}
	}

}
// A tick() forces the currentShape to move (if there were no tick,
// the currentShape would float in position forever). This should
// call attemptMove() on the currentShape.  If not successful, lock() 
// the currentShape (it can move no further).
// - params: none
// - return: nothing
void TetrisGame::tick()
{
	if (!attemptMove(currentShape, 0, 1))
	{
		lock(currentShape);
	}
}

// reset everything for a new game (use existing functions) 
//  - set the score to 0 and call updateScoreDisplay()
//  - call determineSecondsPerTick() to determine the tick rate.
//  - clear the gameboard,
//  - pick & spawn next shape
//  - pick next shape again (for the "on-deck" shape)
// - params: none
// - return: nothing
void TetrisGame::reset()
{
	score = 0;
	updateScoreDisplay();
	determineSecondsPerTick();
	board.empty();
	pickNextShape();
	spawnNextShape();
	pickNextShape();
}

// assign nextShape.setShape a new random shape  
// - params: none
// - return: nothing
void TetrisGame::pickNextShape()
{
	nextShape.setShape(Tetromino::getRandomShape());
}

// copy the nextShape into the currentShape (through assignment)
//   position the currentShape to its spawn location.
// - params: none
// - return: bool, true/false based on isPositionLegal()
bool TetrisGame::spawnNextShape()
{
	currentShape = nextShape;
	currentShape.setGridLoc(board.getSpawnLoc());
	if (isPositionLegal(currentShape))
	{
		return true;
	}

	return false;	
}

// Test if a rotation is legal on the tetromino and if so, rotate it. 
//  To accomplish this:
//	 1) create a (local) temporary copy of the tetromino
//	 2) rotate it (temp.rotateClockwise())
//	 3) test if temp rotation was legal (isPositionLegal()), 
//      if so - rotate the original tetromino.
// - param 1: GridTetromino shape
// - return: bool, true/false to indicate successful movement
bool TetrisGame::attemptRotate(GridTetromino& shape)
{
	GridTetromino temp = shape;
	temp.rotateClockwise();
	
	if (isPositionLegal(temp))
	{
		shape.rotateClockwise();
		return true;
	}

	return false;
}

// test if a move is legal on the tetromino, if so, move it.
//  To do this:
//	 1) create a (local) temporary copy of the tetromino
//	 2) move it (temp.move())
//	 3) test if temp move was legal (isPositionLegal(),
//      if so - move the original.	
// - param 1: GridTetromino shape
// - param 2: int x;
// - param 3: int y;
// - return: true/false to indicate successful movement
bool TetrisGame::attemptMove(GridTetromino& shape, int x, int y)
{
	GridTetromino temp = shape;
	temp.move(x, y);
	if (isPositionLegal(temp)) 
	{
		shape.move(x, y);
		return true;
	}
	return false;
}

// drops the tetromino vertically as far as it can 
//   legally go.  Use attemptMove(). This can be done in 1 line.
// - param 1: GridTetromino shape
// - return: nothing;
void TetrisGame::drop(GridTetromino& shape)
{
	while (attemptMove(shape, 0, 1)){};	
}

// copy the contents (color) of the tetromino's mapped block locs to the grid.
	//	 1) get the tetromino's mapped locs via tetromino.getBlockLocsMappedToGrid()
	//   2) use the board's setContent() method to set the content at the mapped locations.
	//   3) record the fact that we placed a shape by setting shapePlacedSinceLastGameLoop
	//      to true
	// - param 1: GridTetromino shape
	// - return: nothing
void TetrisGame::lock(const GridTetromino& shape)
{
	std::vector<Point> temp = shape.getBlockLocsMappedToGrid();
	board.setContent(temp, static_cast<int>(shape.getColor()));
	shapePlacedSinceLastGameLoop = true;
}

// Graphics methods ==============================================

// Draw a tetris block sprite on the canvas		
// The block position is specified in terms of 2 offsets: 
//    1) the top left (of the gameboard in pixels)
//    2) an x & y offset into the gameboard - in blocks (not pixels)
//       meaning they need to be multiplied by BLOCK_WIDTH and BLOCK_HEIGHT
//       to get the pixel offset.
//	 1) set the block color using blockSprite.setTextureRect()
//   2) set the block location using blockSprite.setPosition()   
//	 3) draw the block using window.draw()
//   For details/instructions on these 3 operations see:
//       www.sfml-dev.org/tutorials/2.5/graphics-sprite.php
//       use member variables: window and blockSprite (assigned in constructor)
// param 1: Point topLeft
// param 2: int xOffset
// param 3: int yOffset
// param 4: TetColor color
// return: nothing
void TetrisGame::drawBlock(const Point& topLeft, int xOffset, int yOffset, TetColor color) const
{
	blockSprite.setTextureRect(sf::IntRect(static_cast<int>(color) * BLOCK_WIDTH, 0, BLOCK_WIDTH, BLOCK_HEIGHT));
	blockSprite.setPosition(topLeft.getX() + BLOCK_HEIGHT * xOffset,topLeft.getY() + BLOCK_WIDTH * yOffset);
	window.draw(blockSprite);
}

// Draw the gameboard blocks on the window
//   Iterate through each row & col, use drawBlock() to 
//   draw a block if it isn't empty.
// params: none
// return: nothing
void TetrisGame::drawGameboard() const
{
	for (int y{ 0 }; y < board.MAX_Y; y++)
	{
		for (int x{ 0 }; x < board.MAX_X; x++)
		{
			if (board.getContent(x, y) != Gameboard::EMPTY_BLOCK)
			{
				drawBlock(gameboardOffset, x, y, static_cast<TetColor>(board.getContent(x, y)));
			}	
		}
	}
}

// Draw a tetromino on the window
//	 Iterate through each mapped loc & drawBlock() for each.
//   The topLeft determines a 'base point' from which to calculate block offsets
//      If the Tetromino is on the gameboard: use gameboardOffset
// param 1: GridTetromino tetromino
// param 2: Point topLeft
// return: nothing
void TetrisGame::drawTetromino(const GridTetromino& tetromino, const Point& topLeft) const 
{
	for (Point& p : tetromino.getBlockLocsMappedToGrid())
	{
		drawBlock(topLeft, p.getX(), p.getY(), tetromino.getColor());
	}
}

// update the score display
// form a string "score: ##" to display the current score
// user scoreText.setString() to display it.
// params: none:
// return: nothing
void TetrisGame::updateScoreDisplay()
{
	std::stringstream message;
	std::string messageToOutput;
	message << "score: " << score;
	messageToOutput = message.str();

	scoreText.setString(messageToOutput);
}

// State & gameplay/logic methods ================================

// Determine if a Tetromino can legally be placed at its current position
// on the gameboard.
//   Tip: Make use of Gameboard's areLocsEmpty() and pass it the shape's mapped locs.
// - param 1: GridTetromino shape
// - return: bool, true if shape is within borders (isWithinBorders()) and 
//           the shape's mapped board locs are empty (false otherwise).
bool TetrisGame::isPositionLegal(const GridTetromino& shape) const
{
	return isWithinBorders(shape) && board.areAllLocsEmpty(shape.getBlockLocsMappedToGrid());
}


// Determine if the shape is within the left, right, & bottom gameboard borders
//   * Ignore the upper border because we want shapes to be able to drop
//     in from the top of the gameboard.
//   All of a shape's blocks must be inside these 3 borders to return true
// - param 1: GridTetromino shape
// - return: bool, true if the shape is within the left, right, and lower border
//	         of the grid, but *NOT* the top border (false otherwise)
bool TetrisGame::isWithinBorders(const GridTetromino& shape) const
{
	for (const Point& p : shape.getBlockLocsMappedToGrid())
	{
		if (!(p.getX() < Gameboard::MAX_X && p.getY() < Gameboard::MAX_Y && p.getX() >= 0))
		{
			return false;
		}
	}
	return true;
}


// set secsPerTick 
//   - basic: use MAX_SECS_PER_TICK
//   - advanced: base it on score (higher score results in lower secsPerTick)
// params: none
// return: nothing
void TetrisGame::determineSecondsPerTick() 
{
	secondsPerTick = MAX_SECONDS_PER_TICK;
}