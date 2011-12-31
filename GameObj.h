/*GameObj.h
* Joshua Speight, 2011
* Liquid Pro Quo
*
* Contains a hitbox struct used for collision detection or anything else where
* width/height calculations are used. [width and height would be stored in hitbox.w and hitbox.h]
*
* Game Objects are the basic pieces that operate under the Game Manager's supervision. They are
* regulated mainly by their update method which should determine almost all of their functionality.
* This method will do drastically different things depending on the game and what piece this object
* plays in the game.
* 
*/

#pragma once

#include <string>
#include <vector>

#include "SDL.h"

#ifndef GAMEOBJ_H
#define GAMEOBJ_H

class GameManager; //forward declaration

//--------------------- STRUCT : HITBOX
// Basically a rectangle used for collision detection or anything else needed
// includes name field incase of multiple hitboxes
struct HitBox
{
	int x; // x location relative to object
	int y; // y location ^
	int w; // width of box
	int h; // height of box

	std::string boxName;
};
	//END OF: HITBOX---------------------


//-------------------------------- CLASS: GAME OBJECT ---------------------------

class GameObj
{
	public:
		int getX()							{return x;}
		int getY()							{return y;}
		int getState()						{return state;}
		std::string getImageName()			{return imageName;}
		SDL_Rect getClip()					{return clip;}
		std::vector<HitBox>* getHitBoxes()	{return hitboxes;}

		void setX(int i)					{x = i;}
		void setY(int i)					{y = i;}
		void setImageName(std::string i)	{imageName = i;}
		void setState(int i)				{state = i;}
	
		void incX(int i)					{x += i;}
		void incY(int i)					{y += i;}

		GameObj()
		{
			defaultValues();
		}

		//constructor with all values set, if not provided, state is 0 by default
		GameObj(int posX, int posY, std::string img, int s = 0)
		{
			x = posX;
			y = posY;
			imageName = img;
			state = s;
		}

		virtual ~GameObj()
		{

		}

		//adds a new hit box to this object, will be used sparingly, but useful for objects
		// more with complicated shapes
		void addHitBox(int x1, int y1, int w1, int h1, std::string name)
		{
			HitBox h;
			h.x = x1;
			h.y = y1;
			h.w = w1;
			h.h = h1;
			h.boxName = name;

			hitboxes->push_back(h);
		}

		// by default this will return the default hitbox created with the object
		// error if the obj does not have a hitbox
		virtual HitBox getHitBox()
		{
			return hitboxes->at(0);
		}

		//this method should regulate the behavior of the object, whatever that may entail
		virtual void update(GameManager* gm) = 0;

	protected:
		int x, y, state;
		std::string imageName; // name of image file
		SDL_Rect clip; // used for animation or simply clipping an image to show the desired piece
		std::vector<HitBox>* hitboxes; // a list of an object's hitboxes, most should only have 1
									// each hitbox has a x,y location relative to the obj, and a width,
									//height, and name(for identification purposes)

	private:
		void defaultValues()
		{
			x = y = state = 0;
			imageName = "";
			hitboxes = new std::vector<HitBox>();

			clip.x = 0;
			clip.y = 0;
			clip.w = 0;
			clip.h = 0;

			addHitBox(0,0,0,0,"default");
		}
};
	// END OF: GAMEOBJ-----------------------------
	//---------------------------------------------
#endif