#include <SDL2/SDL.h>
#include <SDL2/SDL_events.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_mixer.h>
#include <SDL2/SDL_render.h>
#include <iostream>
#include <string>
#include <vector>
#include <math.h>
#include "Entity.h"
#include "Window.h"
#include "Timer.h"
#include "PEntity.h"
const std::string texturefolder = "../textures/";
const std::string soundfolder = "../sounds/";

enum Direction
{
    up,down,left,right
};

int main(int argc, char* argv[])
{
    // Create a window
    Window window ("My image", {SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 1400, 900}, SDL_WINDOW_MAXIMIZED);
    //initiallize audio
    Mix_OpenAudio( 48000, MIX_DEFAULT_FORMAT, 2, 1024 );
    //Create Textures
    Texture flying ( texturefolder+"fly_air.png",window);
    Texture stand (texturefolder+"fly.png",window);
    Texture crouchright (texturefolder+"fly_crouch.png",window);
    crouchright.Rotate(0,SDL_FLIP_HORIZONTAL,false);
    Texture crouchleft = crouchright;
    crouchright.Rotate(0,SDL_FLIP_HORIZONTAL,false);
    //Texture grasstex (texturefolder+"grass.png")
    window.SetBGColor(120,120,255);
    Entity canon(texturefolder+"canon.png",{1000,600,200,100},window);
    Entity grass(texturefolder+"grass.png",{0,700,1400,200},window);
    PEntity platform(texturefolder + "platform.png",{100,0,220,70},window);
    PEntity player(texturefolder + "fly.png",{800,500,60,120},window);
    bool crouching = false;
    PEntity canon_ball(texturefolder+"canon_ball.png", {1000,600,25,25},window);
    PEntity player2(texturefolder+"bot.png",{200,200,180,230},window);
    platform.ChangeMass(50);
    bool dpressed= false,apressed = false;
    player.ChangeMass(10);
    player.velocity.x = -0.05;
    float ballangle = 0;
    //canon.texture.Rotate(50, SDL_FLIP_NONE,true);
    std::vector<PEntity> gems;
    bool shot = false;
    gems.reserve(3);
    
    //gems.emplace_back(texturefolder+"gem.png", SDL_Rect{1000, 580, 20, 20}, window);
    gems.emplace_back(texturefolder+"gem.png", SDL_Rect{450, 400, 20, 20}, window);
    //gems.emplace_back(texturefolder+"gem.png", SDL_Rect{300, 900, 20, 20}, window);

    Timer loopcontrol;
    std::vector <bool>takengems ={false,false,false};
    for(auto& gem: gems)
    {   gem.velocity.x = 0.1;
          gem.ChangeMass(100);
            gem.acceleration.y = 0.005;
        //  gem.acceleration.y = 0.0002;;
    }
    //player3.ReposX(800);
    Mix_Chunk* jumpsound = Mix_LoadWAV((soundfolder+"jump.wav").c_str());
    Mix_Chunk* canonsound = Mix_LoadWAV((soundfolder+"canon.wav").c_str());    
    Mix_Chunk* XPsound =   Mix_LoadWAV((soundfolder+"XP.wav").c_str());
    player.acceleration.y = 0.005;
    // Event loop
    bool quit = false;
    while (!quit)
    {
        //capture time
        loopcontrol.Start();
        //handle window events
        SDL_Event event;
        while (SDL_PollEvent(&event))
        {
            if (event.type == SDL_QUIT)
            { quit = true;
                break;
            }
            if (event.type == SDL_MOUSEBUTTONDOWN)
            {
            int mousex,mousey;
        SDL_GetMouseState(&mousex,&mousey);

        platform.Gravitate ({(float)mousex-platform.Rect.w/2.0f,(float)mousey},2);//1/ platform.GetDistance({(float)mousex-platform.Rect.w/2.0f,(float)mousey}));
        if (platform.GetDistance({(float)mousex-platform.Rect.w/2.0f,(float)mousey})<30)
        {
            platform.velocity = {0,0};
        }
        platform.LimitSpeed(0.4);
            }
            SDL_Scancode& key = event.key.keysym.scancode;
            if(event.type == SDL_KEYDOWN)
                {
                    if (key == SDL_SCANCODE_D&& event.key.repeat == 0)
                    {

                        player.velocity.x+=0.1;
                        dpressed = true;


                    }
                    if (key == SDL_SCANCODE_A && event.key.repeat == 0)
                    {
                        player.velocity.x-=0.1;
                        apressed = true;

                    }
                    if (key == SDL_SCANCODE_S && event.key.repeat == 0)
                    {
                        crouching = true;

                    }
                    
                    if(key == SDL_SCANCODE_Q)
                    {
                        quit = true;
                        break;
                    }
                    if ((key == SDL_SCANCODE_W || key == SDL_SCANCODE_SPACE) && event.key.repeat == 0 && (player.Rect.y>549 || player.standing))
                    {
                        if(!crouching)
                        {
                            int channel = Mix_PlayChannel( -1, jumpsound, 0 );
                            Mix_Volume( channel, 5);
                            player.velocity.y = -2.0;
                            if(!shot)
                            {canon_ball.velocity ={-0.2,-0.08};
                            shot = true;
                            channel = Mix_PlayChannel(-1, canonsound , 0);
                            }
                        }
                    }

                }
            else if(event.type == SDL_KEYUP)
            {
            if (key == SDL_SCANCODE_D && event.key.repeat == 0)
            {
                player.velocity.x-=0.1;
                dpressed = false;
            }
            if (key == SDL_SCANCODE_A && event.key.repeat == 0)
            {
                player.velocity.x+=0.1;
                apressed = false; 
            }
            if (key == SDL_SCANCODE_S && event.key.repeat == 0)

                crouching = false;


        }
        }
        //manage player movement and collision
        
        player.Update(2);
        player2.Update(2);
        platform.Update(2);
        canon_ball.Update(2);
        if(shot)  ballangle+=5;
        player.AvoidCollision(player2.Rect);
        //player.AvoidCollision(canon.Rect);
        player.AvoidCollision(grass.Rect);
        player2.AvoidCollision(grass.Rect);
        player.AvoidCollision(platform);
        
        for(int i=0;i<(int)gems.size();i++)
        {
            if(!takengems[i])
            {
                auto& gem = gems[i];
                gem.Update(2);
                if(player.Collides(gem.Rect))
                {
                    int channel = Mix_PlayChannel( -1, XPsound, 0 );
                    Mix_Volume( channel, 20);
                    gem.AvoidCollision(player);
                }
                //if(gem.GetDistance(player.position+ Vec2{player.Rect.w/2,player.Rect.h/2})<100)
                //    gem.Gravitate(player.position+ Vec2{player.Rect.w/2,player.Rect.h/2}, 0.002);
                //gem.LimitSpeed(0.2);
                    gem.AvoidCollision(grass.Rect);
                    gem.AvoidCollision(platform);
            }
        }

        if(player.standing)
        {
            player.velocity.y = 0;
            if(!(dpressed||apressed && abs(player.velocity.x)<0.1))player.velocity.x *=0.995;
        }
            if(gems[0].standing)
            gems[0].velocity.y = 0;

        //choose the size and texture of player
        if(player.standing&&crouching)
            {
                if(player.velocity.x > 0 )
                    player.texture = crouchright.s_tex();
                else
                    player.texture = crouchleft.s_tex();
                player.Resize(60,40);
            }
        else if(player.standing&&!crouching)
            {

            player.texture = stand.s_tex();

            player.Resize(60,120);
        }
        else 
        {
            player.texture = flying.s_tex();

            player.Resize(60,120);


        }
        //handle rendering
        window.Clear();
        canon.Draw();
        canon_ball.texture.DraxEX(canon_ball.Rect,ballangle,SDL_FLIP_NONE);
        player.Draw();
        player2.Draw();
        platform.Draw();
        grass.Draw();
        for(int i=0;i<(int)gems.size();i++)
        {
            if(!takengems[i])
                gems[i].Draw();
        }
        window.Show();

       // std::cout<<gems[0].velocity<<std::endl;
        loopcontrol.WaitUntilPassed(1950);
    }
    return 0;
}

