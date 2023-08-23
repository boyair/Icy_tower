#include "Game.h"
#include "Timer.h"
#include <cstdlib>
#include <optional>
#include <iostream>
int main()
{
    //initialize SDL2
    if (SDL_Init(SDL_INIT_EVERYTHING) != 0) {
        SDL_Log("Failed to initialize SDL2: %s", SDL_GetError());
        return -1;
    }

    if(Mix_OpenAudio( 48000, MIX_DEFAULT_FORMAT, 8, 1024 ) != 0)
    {        
        SDL_Log("Failed to initialize audio %s", SDL_GetError());
        return -1;
    }
    if(TTF_Init() != 0)
    {        
        SDL_Log("Failed to initialize fonts %s", SDL_GetError());
        return -1;
    }



    Game game;
    Timer timer;
    // start menu loop before game is running.
    while (!game.IsRunning() && !game.AppQuit())
    {
        timer.Start();
        game.StartMenu();
        timer.WaitUntilPassed(2000);
    }
    unsigned int last_iteration_time =2000;

    // full game loop including death screen.
    while(!game.AppQuit())
    {
        //game loop while alive
        while(game.IsRunning())
        {
            timer.Start();
            game.HandleInput();
            game.RunPhysics(last_iteration_time);
            game.Draw();
            timer.WaitUntilPassed(2000);
            last_iteration_time = timer.PassedTime().count();


        }
        //death screen loop
        while(!game.IsRunning()&& !game.AppQuit())
        {
            timer.Start();
            game.DeathScreen();
            timer.WaitUntilPassed(2000);

        }
    }
    //quiting game.
    SDL_Log("game ended.");
    SDL_Quit();
    SDL_Log("quit sdl!");
    

}



//place canons randomly on screen adges and make them point towards the center of the window.
//optional(seperate canon tube fron wheel to allow free rotation of tube)
//add option to initialize text with custom SDL_Color.
//make buttons create with costum font like text.
//make the font argument a string to file path instead of TTF_Font*.
//make a git commit.
