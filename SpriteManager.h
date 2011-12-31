/*SpriteManager.h
* Joshua Speight, 2011
* Liquid Pro Quo
*
* Class responsible for loading in necessary game images from a text file, and returning an SDL_Surface* of the image when queried
* for it. By default this file is in files/images.txt
* 
*/

#pragma once

#include <map>
#include <vector>
#include <fstream>

#include <sstream>

#include "SDL.h"
#include "SDL_image.h"

using namespace std;

#ifndef SPRITEMANAGER_H
#define SPRITEMANAGER_H

//------------------------------- CLASS: SPRITE MANAGER ----------------------------------
class SpriteManager
{

public:
	map<string,SDL_Surface*>* getImages()		{return images;}

	SpriteManager()
	{
		images = new map<string,SDL_Surface*>();
	}

	virtual ~SpriteManager()
	{
		clearImages();
		delete images;
	}

	//when passed in a key (the name of the image), returns that image as an SDL_Surface*
	SDL_Surface* getImage(string key)
	{
		return images->at(key);
	}

	void clearImages()
	{
		images->erase(images->begin(),images->end());
		images->clear();
	}

	// loads a list of images into the manager's dictionary for later retrieval;
	// by default this file is "files/image.txt"
	// each line in the the .txt should be an image file name, and the end of the list should be
	// denoted by the word END
	virtual void loadImages(std::string fileName = "files/images.txt")
	{
		ifstream file(fileName.c_str());
		string line;

		while(true) // search list in its entirety, until we come upon an "END" tag
		{
			getline(file,line);
			if(line == "END")
				break;

			images->insert(pair<string,SDL_Surface*>(line,loadImage(line)));
		}

		file.close();
	}

	//static function used for image loading
	// returns an optimized image
		static SDL_Surface* loadImage(string file)
		{
			SDL_Surface* initImg = NULL;
			SDL_Surface* returnImg = NULL;

			initImg = IMG_Load(file.c_str());
	
			returnImg = SDL_DisplayFormat(initImg);

			SDL_FreeSurface(initImg);

			Uint32 colorkey = SDL_MapRGB(returnImg->format, 0xFF, 0, 0xFF); // Note: color that is being taken as transparent is a pink 
			SDL_SetColorKey( returnImg, SDL_SRCCOLORKEY, colorkey );											   //which is 255,0,255

			return returnImg;
		}

protected:
	map <string,SDL_Surface*>* images; // a map of the game's images, mapping the actual SDL_Surface* to a string name

};
	// END OF: SPRITE MANAGER -----------------------------------
#endif