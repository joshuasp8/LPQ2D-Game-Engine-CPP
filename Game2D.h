/*Game2D.h
* Joshua Speight, 2011
* Liquid Pro Quo
*
* Core class of the engine. Has both Sprite and Game Manager member instances. Contains run method
* which houses game loop.
* 
* Flow of game is as follows:
* From the inside:
* start -> game2DInit -> initPreScreen -> initScreen -> initPostScreen -> run [checkUserInput, update(s),paint]
* From the outside (implementation wise)
* constructor -> getGameManagerInstance,getSpriteManagerInstance -> initPreScreen -> initPostScreen -> Loop [
* buttonCheck -> updateSplashScreen,updateStartScreen,updateInGame[],updateGamePaused,updateGameOver ->
* drawBackground -> drawGameObjs -> drawForeground]
*/

#pragma once

#include "GameManager.h"
#include "SpriteManager.h"
#include "FPSManager.h"

//-------------------- CONSTANTS ----------------------
#define DEFAULT_SCREEN_WIDTH 640
#define DEFAULT_SCREEN_HEIGHT 480
#define DEFAULT_FPS 60
#define BPP 32
#define DEADZONE -9999 //x,y point designated where no activity will happen
					// the purpose of this is to use this area as a storage zone for off-screenobjs, etc

// GAME STATES
#define SPLASHSCREEN -1
#define STARTSCREEN 0
#define INGAME 1
#define PAUSED 2
#define GAMEOVER 3
#define	MENU 4
#define CUTSCENE 5

// XBOX CONTROLLER - appropriate constants for an xbox 360 controller should 1 be supported
#define A_BTN 0
#define B_BTN 1
#define L_BTN 4
#define R_BTN 5
#define START_BTN 7
#define SELECT_BTN 6
	//----------------------------------------------

#ifndef GAME2D_H
#define GAME2D_H

// -------------------------------- CLASS: GAME 2D - CORE CLASS --------------------------------------
class Game2D
{
	public:

		Game2D() //default constructor should be called upon instantiation to set manager values to null
		{
			spriteMan = NULL;
			gameMan = NULL;
		}

		//GAME 2D DESTRUCTOR
		// will be called after the destructor of child class is invoked
		virtual ~Game2D()
		{
			SDL_JoystickClose(js);
			SDL_FreeSurface(screen);
			SDL_Quit();

			delete spriteMan;
			delete gameMan;
		}
		
		//called to start the engine
		void start(int sw, int sh, int fps,std::string title, bool fullScreen, int gs)
		{
			if(!game2DInit(sw,sh,fps,title,fullScreen, gs))
				return; // abort
			initPreScreen();
			if(!screenInit())
				return; // abort game

			run();
		}

		//starts engine using default values
		void start()
		{
			if(!game2DInit(DEFAULT_SCREEN_WIDTH,DEFAULT_SCREEN_HEIGHT,DEFAULT_FPS,"",false, STARTSCREEN))
				return; // abort
			initPreScreen();
			if(!screenInit())
				return; // abort game

			run();
		}

		// Static function used for drawing to a surface
		// This is a more generic version of the draw method, able to be used in situations where
		// the drawing destination may not necessarily be the screen itself
		// or to allow drawing outside of this class
		static void applySurface(int x, int y, SDL_Surface* src, SDL_Surface* dest, SDL_Rect* clip = NULL)
		{
			SDL_Rect offset;
			offset.x = x;
			offset.y = y;

			SDL_BlitSurface(src,clip,dest,&offset);
		}

		// Uses the rectangle clip to draw a clipped version the source image to the screen at position (x,y)
		// Clip value is NULL by default, so it may be ignored in practice, e.g: "draw(image, x, y);" would be legal
		// and used to draw the full image
		void draw(SDL_Surface* src, int x, int y, SDL_Rect* clip = NULL)
		{
			SDL_Rect offset;
			offset.x = x;
			offset.y = y;

			SDL_BlitSurface(src,clip,screen,&offset);
		}


	protected:
		GameManager* gameMan; // Manager of the game and its logic
		SpriteManager* spriteMan; // Manager of game images

		int screenWidth, screenHeight,  screenX, screenY;
		int gameState, framesPerSecond;
		int screenMode;
		std::string screenTitle;
		bool quit, joyStick; // quit = is game loop still running; joystick = is there a controller plugged into?
		SDL_Joystick* js; // if a joystick is connected, we will set this reference to it later

		SDL_Event event1; // our event listener
		SDL_Surface* screen; // default screen to which we draw

		virtual void initPreScreen()		{} // any initializations that must be done before the screen is created (rarely used)
		virtual void initPostScreen()		{} // any initializations that must be done after the screen is created
											// here you'll probably want to override and set up all custom game details here

		virtual void updateSplashScreen()	{} //game behavior when gameState == SPLASHSCREEN; ignore if no splash

		//game behavior when state == STARTSCREEN; This is the state the game starts in, by default this will just forward the state
		// of the game to INGAME, so if no start screen, then safe to ignore, otherwise override as appropriate 
		virtual void updateStartScreen()
		{
			gameState =   INGAME;
		} 
										
		virtual void updateGamePaused()		{} //game behavior when gameState == GAMEPAUSED; ignore if no pause function

		// by default, polls for an event which sets the listener in action
		// when an event is confirmed buttonCheck is called, ideally this will check for
		// event1.type == SDL_KEYUP or SDL_KEYDOWN, and respond appropriately depending on which key is pressed
		//hopefully this will be further abstracted later
		virtual void checkUserInput()
		{
			if(SDL_PollEvent(&event1))
				{
					if(event1.type == SDL_QUIT)
						quit = true;
					else
						buttonCheck();		
				}
		}

		// assuming a currBg has been set (probably in initPostScreen or at any other time during the game)
		// it should prob be drawn here (by default at 0,0), along with any other objs categorized as backgroundobjs
		// in many cases this will need to be overridden for custom cases
		virtual void drawBackground()
		{
			//we assume game to have a bg by default, so he draw it first here
			draw(gameMan->getCurrBg(),0,0);

			//next we draw any objs here
			for(int unsigned i = 0; i < gameMan->getBgObjs()->size(); i++)
			{
				GameObj* o = gameMan->getBgObjs()->at(i);
				Game2D::draw(spriteMan->getImage(o->getImageName()),o->getX(),o->getY());
			}
		}

		// same as draw bg, except by default we don't expect a foreground so there's a null check
		virtual void drawForeground()
		{
			//you may draw a foreground image here, followed by foreground objs or vice versa (or neither!)
			if(gameMan->getCurrFg() != NULL)
				draw(gameMan->getCurrFg(),0,0);
				
			//draw fg objs here
			for(int unsigned i = 0; i < gameMan->getFgObjs()->size(); i++)
			{
				GameObj* o = gameMan->getFgObjs()->at(i);
				Game2D::draw(spriteMan->getImage(o->getImageName()),o->getX(),o->getY());
			}
		}

		//if the game contains a player obj, by default it is drawn first, followed by all other game objects
		// will be overridden for custom cases
		virtual void drawGameObjects()
		{
			if(gameMan->getPlayer() != NULL)
			{
				GameObj* p = gameMan->getPlayer();
				Game2D::draw(spriteMan->getImage(p->getImageName()),p->getX(),p->getY());
			}

			for(unsigned int i = 0; i < gameMan->getObjs()->size();i++)
			{
				GameObj* temp = gameMan->getObjs()->at(i);
				Game2D::draw(spriteMan->getImage(temp->getImageName()),temp->getX(),temp->getY());
			}	
		}

		// behavior when gamestate == INGAME; usually what we want here is to update the game manager (gameMan->update()),
		// follwed by a check of the game manager's gameover bool. on true, set game values appropriately and change
		// gamestate to game over
		virtual void updateInGame() = 0; 
		virtual void updateGameOver() = 0; //here we handle the game over state, usually setting quit to true or allowing a restart
		virtual void buttonCheck()	= 0; // used to check event1 and handle button input using SDL's key variables
		

	private:
		//Contains the main Game Loop
		void run()
		{
			FPSManager* fpsMan = new FPSManager();

			while(!quit)
			{
				checkUserInput();

				fpsMan->start(); // deal with frame timing issues

				if(gameState == STARTSCREEN)
					updateStartScreen();
				if(gameState == INGAME)
					updateInGame();
				if(gameState == PAUSED)
					updateGamePaused();
				if(gameState == GAMEOVER)
					updateGameOver();
		
				if(!paint()) // draw screen
					return; // abort upon drawing error

				fpsMan->manageTime(framesPerSecond);
			}
		}

		//initialize screen details
		bool screenInit()
		{
			if(SDL_Init(SDL_INIT_EVERYTHING) == -1) // initialize all components
				return false;

			screen = SDL_SetVideoMode(screenWidth,screenHeight,BPP,screenMode);
			if(screen == NULL)
				return false;

			SDL_WM_SetCaption(screenTitle.c_str(),NULL);
	
			//check for joystick
			if(SDL_NumJoysticks() > 0)
			{
				joyStick = true; 
				//Open the joystick
				js = SDL_JoystickOpen(0);
			}

			spriteMan->loadImages();
			initPostScreen(); // do any initialization work needed now that screen is created

			return true;
		}

		//Initialize game details
		// first function called with start
		bool game2DInit(int sw, int sh, int fps, std::string title, bool fs, int gs)
		{

			gameMan = getGameManagerInstance();
			spriteMan = getSpriteManagerInstance();

			screenWidth = sw;
			screenHeight = sh;
			framesPerSecond = fps;
			screenTitle = title;
			
			if(fs)
				screenMode = SDL_FULLSCREEN;
			else
				screenMode = SDL_SWSURFACE;

			gameState = gs;

			//Assign default Values to variables
			js = NULL;
			screen = NULL;
			quit = false;
			joyStick = false;

			return true;
		}

		//Called every frame at the end of the run loop to draw the current screen
		bool paint()
		{
			drawBackground();
			drawGameObjects();
			drawForeground();

			if(SDL_Flip(screen) == -1)
					return false; // return false upon failure

			return true;
		}

		// a custom sprite Manager may be created and returned as needed, often this will be unecessary
		virtual SpriteManager* getSpriteManagerInstance()
		{
			return new SpriteManager();
		}

		// return a custom class extending from Game Manager, this class should ideally regulate all the necessary game logic 
		virtual GameManager* getGameManagerInstance() = 0;

};
	//END OF: GAME2D --------------------------------------
	//-----------------------------------------------------

#endif