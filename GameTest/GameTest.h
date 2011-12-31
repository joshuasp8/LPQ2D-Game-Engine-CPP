/*GameTest.h
* Joshua Speight, 2011
* Liquid Pro Quo
*
* Header file for the test "game" made by this engine
*/
#pragma once

#include "Game2D.h"

#ifndef GAMETEST_H
#define GAMETEST_H

class GameTest : public Game2D
{
	public:
		GameTest();
		~GameTest() {}

	protected:

		int gameOverCtr;

		 void updateInGame();
		 void updateGameOver();
		 void buttonCheck();
		 void initPostScreen();

	private:

		GameManager* getGameManagerInstance();

};

class GTManager : public GameManager
{
	bool checkGameOver();
};

class GTObj : public GameObj
{
public:
	GTObj();
	~GTObj(){}

	void update(GameManager* gm);
};

class GTObjDos : public GameObj
{
public:
	GTObjDos();
	~GTObjDos(){}

	void update(GameManager* gm);
};
#endif