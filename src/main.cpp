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
    unsigned int last_iteration_time =2000;
    while (!game.IsRunning() && !game.AppQuit())
    {
        game.StartMenu();
    }
    while(!game.AppQuit())
    {
        while(game.IsRunning())
        {
            timer.Start();
            game.HandleInput();
            game.RunPhysics(last_iteration_time);
            game.Draw();
            timer.WaitUntilPassed(2000);
            last_iteration_time = timer.PassedTime().count();


        }
        while(!game.IsRunning()&& !game.AppQuit())
        {
            game.DeathScreen();
        }
    }
    std::cout<<"game Over!"<<std::endl;
    SDL_Quit();
    std::cout<<"quit sdl!"<<std::endl;
    

}



//place canons randomly on platforms.
//optional(seperate canon tube fron wheel to allow free rotation of tube)
//make a git commit.
//make score and buttons with better colors.
//change background
//make player class
