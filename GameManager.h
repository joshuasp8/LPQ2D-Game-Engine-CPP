/*GameManager.h
* Joshua Speight, 2011
* Liquid Pro Quo
*
* Class responsible for regulating the logic of the game. Contains references to the player object,
* the background and foreground images, background and foreground objects as well as the main objects
* active in the game. The game manager should basically update all objects when appropriate and once a frame
* check for a game over condition and update its gameover bool to true upon finding one.
* 
*/

#pragma once 
#include <vector>

#include "GameObj.h"

#ifndef GAMEMANAGER_H
#define GAMEMANAGER_H
//----------------------------- CLASS: GAME MANAGER ------------------------
class GameManager
{
	public:
		bool isGameOver()						{return gameOver;}
		std::vector<GameObj*>* getObjs()		{return objs;}
		std::vector<GameObj*>* getBgObjs()		{return bgObjs;}
		std::vector<GameObj*>* getFgObjs()		{return fgObjs;}
		GameObj* getPlayer()					{return player;}
		SDL_Surface* getCurrBg()				{return currBg;}
		SDL_Surface* getCurrFg()				{return currFg;}

		void setCurrBg(SDL_Surface* bg)			{currBg = bg;}
		void setCurrFg(SDL_Surface* fg)			{currFg = fg;}
		

		GameManager()
		{
			gameOver = false;
			objs = new std::vector <GameObj*>();
			bgObjs = new std::vector <GameObj*>();
			fgObjs = new std::vector <GameObj*>();
			currBg = NULL;
			currFg = NULL;
			player = NULL;
		}

		virtual ~GameManager()
		{
			objs->erase(objs->begin(),objs->end());
			objs->clear();
			delete objs;
			objs->erase(objs->begin(),objs->end());
			objs->clear();
			delete bgObjs;
			objs->erase(objs->begin(),objs->end());
			objs->clear();
			delete fgObjs;
			clearBg();
			clearFg();
		}

		void setPlayer(GameObj* o)
		{
			if(player == NULL)
				player = o;
		}

		void clearPlayer()
		{
			delete player;
			player = NULL;
		}

		void clearBg()
		{
			SDL_FreeSurface(currBg);
			currBg = NULL;
		}

		void clearFg()
		{
			SDL_FreeSurface(currFg);
			currFg = NULL;
		}

		// add an object to main objects list
		void addObj(GameObj* o)
		{
			objs->push_back(o);
		}

		// add a background object
		void addBgObj(GameObj* o)
		{
			bgObjs->push_back(o);
		}

		// add a foreground object
		void addFgObj(GameObj* o)
		{
			fgObjs->push_back(o);
		}

		virtual bool checkGameOver() = 0; // checks the status of the game and sets isGameOver appropriately

		// update function by default updates all game objects, then the player, and finally checks the game to
		// see if there are any changes to the game over condition
		// may be suitable for some games, but likely will need to be overridden to allow for more complexity
		virtual void update()
		{
			//NOTE: by default, the background and foreground objects do NOT get calls to their update methods
			// since they are considered scenery. This will imply no movement/animation, etc

			for(unsigned int i = 0; i < objs->size();i++)
			{
				GameObj* temp = objs->at(i);
				temp->update(this);
			}
			
			if(player != NULL)
				player->update(this);

			gameOver = checkGameOver();
		}

		//static function that returns true if two game objs collide
		static bool collides(GameObj* a, GameObj* b)
		{
			int ax,ay,aw,ah;
			int bx, by, bw, bh;

			int numBoxesA = a->getHitBoxes()->size();
			int numBoxesB = b->getHitBoxes()->size();

			//incase an object has multiple hitBoxes, we will check all hitboxes against all hitboxes
			for(int i = 0; i < numBoxesA; i++)
			{
				// get appropriate box for object a
				ax = a->getX()+a->getHitBoxes()->at(i).x;
				ay = a->getY()+a->getHitBoxes()->at(i).y;
				aw = a->getHitBoxes()->at(i).w;
				ah = a->getHitBoxes()->at(i).h;

				for(int j = 0; j < numBoxesB; j++)
				{
					// get appropriate box for object b
					bx = b->getX()+b->getHitBoxes()->at(j).x;
					by = b->getY()+b->getHitBoxes()->at(j).y;
					bw = b->getHitBoxes()->at(j).w;
					bh = b->getHitBoxes()->at(j).h;

					if(((bx + bw) > ax) && (bx < (ax + aw)) && ((by + bh) > ay) && (by < (ay + ah))) //collision algo
						return true;
				}
			}

			return false;
		}

	protected:
		bool gameOver;
		std::vector<GameObj*>* objs; // list of main objects in the game
		std::vector<GameObj*>* bgObjs; // list of background objs; note these objs do not get to call their update methods
		std::vector<GameObj*>* fgObjs; // list of foreground objs; note these objs do not get to call their update methods
		SDL_Surface* currBg; // current background
		SDL_Surface* currFg; // current foreground
		GameObj* player; // seen as "key" object to a game 

};
	// END OF : GAME MANAGER-------------------------
#endif