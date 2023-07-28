#include "Game.h"
#include "Window.h"
Game::Game()
:
    window ("2D Game!", {SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 1400, 900}, 0),
    player(texturefolder + "player.png",{300,500,60,60},window)

{
    platforms.reserve(9);
    platforms.emplace_back(texturefolder+"grass.png",SDL_Rect{0,700,1700,200},window);
      
}

void Game::Draw()
{
    window.Clear();

    player.Draw();
    for(auto& platform : platforms)
    {
        platform.Draw();
    }


    window.Show();
}


void Game::InputAndPhysics()
{
        SDL_Event event;
        while (SDL_PollEvent(&event))
        {
            if (event.type == SDL_QUIT)
            { 
            }
            if (event.type == SDL_MOUSEBUTTONDOWN)
            {
                SDL_Point mousepos = window.GetMousePos(); 
                
                if(event.button.button == SDL_BUTTON_LEFT)
                {
                player.acceleration.x = -0.05;
               }
                if(event.button.button == SDL_BUTTON_RIGHT)
                {
                player.acceleration.x = 0.05;
               }
            }
            if (event.type == SDL_MOUSEBUTTONUP)
            {
                if(event.button.button == SDL_BUTTON_LEFT)
                {
                player.acceleration.x = 0;
               }
                if(event.button.button == SDL_BUTTON_RIGHT)
                {
                player.acceleration.x = 0;
               }

            }
            


            SDL_Scancode& key = event.key.keysym.scancode;
            if(event.type == SDL_KEYDOWN)
                {
                    if (key == SDL_SCANCODE_D&& event.key.repeat == 0)
                    {
                        player.velocity.x += 0.1;


                    }
                    if (key == SDL_SCANCODE_A && event.key.repeat == 0)
                    {
                        player.velocity.x -= 0.1;

                    }
                    if (key == SDL_SCANCODE_S && event.key.repeat == 0)
                    {

                    }

                    if(key == SDL_SCANCODE_Q)
                    {
                        quit = true;
                        break;
                    }
                    if ((key == SDL_SCANCODE_W || key == SDL_SCANCODE_SPACE) && event.key.repeat == 0 && (player.IsStanding()))
                    {

                   // int channel = Mix_PlayChannel( -1, jumpsound, 0 );
                    //Mix_Volume( channel, 5);
                    player.velocity.y = -1.0;
                    }

                }
            else if(event.type == SDL_KEYUP)
            {
                if (key == SDL_SCANCODE_D && event.key.repeat == 0)
                {
                    player.velocity.x-=0.1;
                }
                if (key == SDL_SCANCODE_A && event.key.repeat == 0)
                {
                    player.velocity.x+=0.1;
                }
            }
    }
 
}
