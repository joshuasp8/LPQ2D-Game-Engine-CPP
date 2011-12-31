/*FPSManager.h
* Joshua Speight, 2011
* Liquid Pro Quo
*
* Simple class whose main function is to regulate and maintain a constistent fps rate
*/
#pragma once

#include "SDL.h"

#ifndef FPSMANAGER_H
#define FPSMANAGER_H

class FPSManager
{

public:
	FPSManager()
	{
		startTime = 0;
	}

	// mark the current time
	void start()
	{
		startTime  = SDL_GetTicks();
	}

	// this function is used in timing loops. when given a desired fps, this function
	// will have the program delay to keep constant fps
	void manageTime(int fps)
	{
		if(getTime() < (1000/fps)) // resolve fps
		{
			SDL_Delay((1000/fps) - getTime()); 
		}
	}

protected:
	int startTime;
	
	//Get the elapsed time
	int getTime()
	{
		return SDL_GetTicks() - startTime;
	}

};
#endif