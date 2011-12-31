LPQ2D (C++) v0.4 ReadMe
(c) 2011 Joshua Speight [Liquid Pro Quo]
----------------------------------------

This is a game engine I extracted from a 2D C++ game I had written around a year ago. While it's definitely not yet complete, as there are
plenty of other useful tools I'd like to add to this, it's more than enough to pump out a good quality game with a reasonable level of
abstraction so that the game logic can be focused on instead of some of the tedious boiler plate that come along with programming it.
I guess I'm releasing this under the GNU General Public License, meaning feel free to do whatever you like with it. 

The engine uses SDL 1.2 for now since that's what the game was written in. I'll do an update to 1.3 when I get some spare time to
learn what's new. In case you don't have this version (or SDL at all) I've included a zip folder with the neccessary files.

Set Up:

- This set up assumes you're using Microsoft Visual Studios
- If you don't already have SDL 1.2, d/l and install it, or unzip the zip file and extract the folder inside to your C: drive 
- Start a new Visual Studios C++ Project.
- Copy the engine (all the .h files and dlls) to the project's root directory.
- In Visual Studio's project properties: under Config Properties->VC++->include: add the include directory to SDL (C:\SDL-1.2.14\include)
- Do the same for the library directory, adding C:\SDL-1.2.14\lib
- Under Config Properties->C/C++->Code Generation->RunTime Library to Multi Threaded DLL (the open project may need a .cpp file with a main function
  written before this open appears)
- Finally, Under Config Properties->Linker->Input->Additional Dependencies, add the following: SDL.lib; SDLmain.lib; SDL_image.lib
- Now just add the .h engine files to your current project (via drag and drop or through the add function) 
- You should be all set. If something went wrong, check out Lazy Foo's tutorial on setting up SDL (and also on how to use SDL in general)
  http://lazyfoo.net/SDL_tutorials/
  

How To Use:

There's a bit of commentary in the source and also a bare minimum example in the GameTest folder to help explain anything confusing. You should
note that as with any engine, it's unecessary to change any of the provided code to make anything work. The only thing that needs to be done is
creating custom class which extend Game2D, GameManager, and GameObj (also optionally SpriteManager), and implementing/overriding their abstract
and desired virtual functions. The basic set up of a game using the engine should look something like and contain the following:

A main game class which will extend Game2D.

int main(int argc, char* args[])
{
    new classThatExtendedGame2D();
	classThatExtendedGame2D->start(screen width, screen height, framerate (in fps),screen title,fullscreen?,state to start game in)
	// classThatExtendedGame2D->start() can also be used and  will use values 640,480,60,"",false,STARTSCREEN by default
}

Override any of the update methods to control what happens when the game is in that state. What MUST be overriden is:
updateInGame, updateGameOver, buttonCheck and getGameManagerInstance

Following would be a class extending from GameManager, which may want to override the update method depending on complexity.
What must be overridden is: checkGameOver which creates an end game condition to check for and returns true upon reaching that condition
(usually in the fashion gameOver = checkGameOver()). A new instance of this class is what should be returned in the getGameManagerInstance
function.

Next 1 or more classes extending from the GameObj class should be created. These classes need to override the update method which will be
called by the manager every frame when appropriate. In addition the extended class should set imageName to the path of the image this object
uses (Side Note: 255,0,255) is the transparent color key SpriteManager uses. Also if the object is involved in any collisions, its hitbox should
be set to the appropriate size of the image, e.g: hitbox.w = 32; hitbox.y = 32; for a 32x32 image starting at the top left(0,0 [by default])
corner of the image.

Finally, by default [and at the start of the engine], the sprite manager looks for a file "files/images.txt" and reads off each line as an image
file to load into memory, until it reaches an END tag. So unless creating a custom SpriteManager class and overriding this method, this file should
be present. 


Contact me: LiquidProQuoDev@gmail.com
Git Source Repository: github.com/LiquidProQuo/LPQ2D-Game-Engine-CPP

Thanks!
