/*GameTest.cpp
* Joshua Speight, 2011
* Liquid Pro Quo
*
* Hosts the implementation for the test game created for testing the basic engine
* Includes custom: game class, game manager class, and 2 game objects classes
* Shows ease and flow of engine
*/
#include "GameTest.h"

using namespace std;

//simply creates a new game test object, which will invoke the start method
int main(int argc, char* args[])
{
	new GameTest();

	return 0;
}

GameTest::GameTest()
{
	// simply call start, this will initialize game with all default values
	//equivalent of calling start(DEFAULT_SCREEN_WIDTH, DEFAULT_SCREEN_HEIGHT,DEFAULT_FPS,"",false,STARTSCREEN)
	start(); 
}

GameManager* GameTest::getGameManagerInstance()
{
	//GTManager will be our logic manager
	return new GTManager();
}

// Overriden to setup game details
void GameTest::initPostScreen()
{
	gameOverCtr = 60; // number of frames to show gameover screen; ~ 1 second

	gameMan->setPlayer(new GTObj()); // add a player

	gameMan->addObj(new GTObjDos()); // add a new object

	GTObjDos* temp = new GTObjDos();
	temp->setY(100);
	temp->setX(240);
	gameMan->addObj(temp); // add another

	gameMan->setCurrBg(SpriteManager::loadImage("images/sampleEngineBg.png")); // set a background
}

// update game manager, then check for game over and handle appropriately
void GameTest::updateInGame()
{
	gameMan->update();
	if(gameMan->isGameOver())
	{
		gameMan->clearBg();
		gameMan->setCurrBg(SpriteManager::loadImage("images/deadBg.png"));
		gameState = GAMEOVER;
	}
}

// earlier we set a counter to decide how long the GAMEOVER screen should be drawn,
// here we wait for the counter to expire, and set quit to true when it does to end the program
void GameTest::updateGameOver()
{
	if(gameOverCtr  < 1) // we'll wait about a second before closing the screen
		quit = true;
	else
		gameOverCtr--;
}

// here we know that there was an event, we check the event type (event1.type), and then figure out which key was pressed
// to handle appropriately
void GameTest::buttonCheck()
{
	if(gameState == INGAME)
	{
		if(gameMan->getPlayer() == NULL) // if there's no player, don't do anything
			return;

		if(event1.type == SDL_KEYUP) // has a key been released?
		{
			if(event1.key.keysym.sym == SDLK_LEFT) // a little verbose, but straight forward
			{
				gameMan->getPlayer()->incX(-48); //on a tap of the left arrow key, move player 48 pixels to the left
				return;
			}
			if(event1.key.keysym.sym == SDLK_RIGHT)
			{
				gameMan->getPlayer()->incX(48); // 48 to the right
				return;
			}
		}
	}
}
//-----------------------

//This is the first object we created, which will represent the player; on its update it simply falls each frame
// and adjusts its position to wrap horizontally around the screen
void GTObj::update(GameManager* gm)
{
	incY(8); // fall at a speed of 8

	//wrap around screen horizontally
	if(y >= 480)
		y = -32;
	if(x + 32 < 0)
		x = 608;
	if(x > 640)
		x = 0;
		
}

GTObj::GTObj()
{
	imageName = "images/smiley.png"; //which image should we use for this object

	//here we set the object's dimensions (specifically its default hit box) as a 32x 32 rectangle starting at the object's (0,0)
	getHitBoxes()->at(0).x = 0;
	getHitBoxes()->at(0).y = 0;
	getHitBoxes()->at(0).w = 32;
	getHitBoxes()->at(0).h = 32;

	//this will sort of be abstracted away in later versions
}

//-----------------------

// second obj type we made, simply moves to the left at a constant speed of 8, and wraps around the screen
void GTObjDos::update(GameManager* gm)
{
	if(x <= -32)
	{
		x = 662;	
	}
	incX(-8);
}


GTObjDos::GTObjDos()
{
	imageName = "images/mady.png";
	x = 400;
	y = 280;

	getHitBoxes()->at(0).x = 0;
	getHitBoxes()->at(0).y = 0;
	getHitBoxes()->at(0).w = 32;
	getHitBoxes()->at(0).h = 32;
}

//-----------------------------------------------------------------

// here we set the game over condition of the game; we make it very simple here
bool GTManager::checkGameOver()
{
	//let's set the game over condition for when the player and another obj on screen collide
	for(int unsigned i = 0; i < objs->size(); i ++)
	{
		if(collides(player,objs->at(i))) // does a collision check with the player and each of the other 2 faces
			return true;
	}

	return false;
}