#include "Game.h"
#include "Timer.h"
#include "Utils.h"
#include <cstdint>
#include <cstdlib>
#include <functional>
#include <optional>
#include <iostream>
#include <thread>

void RunPhysics(Game& game);
int InitSDL2();

int main()
{

    //enclose game object inside a scope so that the destructors of game members are called before quitting SDL.
    {

        //initialize SDL2
        int SDLInitLog = InitSDL2();
        if(SDLInitLog)
            return SDLInitLog;
        Game game;
        Timer timer;


        // start menu loop before game is running.
        while (!game.IsRunning() && !game.AppQuit())
        {
            timer.Start();
            game.StartMenu();
            timer.WaitUntilPassed(2000);
        }
        uint32_t last_iteration_time =2000;



        // full game loop including death screen.
        while(!game.AppQuit())
        {
            std::thread physics_thread(RunPhysics,std::ref(game));
            //game loop while alive
            while(game.IsRunning())
            {
                timer.Start();
                game.HandleInput();
                game.HandleLogic(last_iteration_time);
                game.Draw();
                timer.WaitUntilPassed(2000);
                last_iteration_time = timer.PassedTime().count();


            }
            physics_thread.join();
            //death screen loop
            while(!game.IsRunning()&& !game.AppQuit())
            {
                timer.Start();
                game.DeathScreen();
                timer.WaitUntilPassed(2000);

            }
        }
    }
    //quiting game.
    SDL_Log("game ended.");
    SDL_Quit();
    SDL_Log("quit sdl!");


}

void RunPhysics(Game& game)
{
    
    Timer timer;
    uint32_t last_iteration_time =500;

    while(game.IsRunning())
    {
        timer.Start();
        game.RunPhysics(last_iteration_time);
        timer.WaitUntilPassed(500);
        last_iteration_time = timer.PassedTime().count();


    }
}
int InitSDL2()
{
    //if this is the linux build prefer wayland naitive windows.
#ifndef _WIN32
    SDL_SetHint(SDL_HINT_VIDEODRIVER, "wayland,x11");

#endif // !_WIN32
    if (SDL_Init(SDL_INIT_EVERYTHING) != 0) {
        SDL_Log("Failed to initialize SDL2: %s", SDL_GetError());
        return -1;
    }

    if(Mix_OpenAudio( 48000, MIX_DEFAULT_FORMAT, 4, 1024 ) != 0)
    {        
        SDL_Log("Failed to initialize audio %s", SDL_GetError());
        return -2;
    }
    if(TTF_Init() != 0)
    {        
        SDL_Log("Failed to initialize fonts %s", SDL_GetError());
        return -3;
    }
return 0;



}
//make cloud movement better.
//optional: add element of randomness in canon repositioning
//add seed generator when making platforms to the seed of position and size.
//every 50 platforms new world (test)
//world ideas:
//ice (low fricrtion).
//gum (high friction).
//windy (pushed by the air)
//breaking platforms (platform breaks after standing on it for too long).
//make a git commit.
