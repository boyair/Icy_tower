#include "Game.h"
#include "Button.h"
#include "Utils.h"
#include "Window.h"
#include <SDL2/SDL_events.h>
#include <SDL2/SDL_rect.h>
#include <SDL2/SDL_render.h>
#include <SDL2/SDL_ttf.h>
#include <string>

const std::string texturefolder = "../textures/";
const std::string soundfolder = "../sounds/";


Game::Game()
:
    window ("2D Game!", {SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 1000, 1000}, 0),
    player({300,640,60,60},window),
    start_button("start",{400,400,200,100},5,window),
    restart_button("restart",{375,400,250,100},5,window),
    quit_button("Quit!",{400,550,200,100} , 5, window),
    bg(texturefolder + "sky.png",window,{0,0,1000,1000}),
    death_screen_bg(texturefolder + "death_screen.png",window,{0,0,1000,1000}),
    ScoreDisplay("score: 0", TTF_OpenFont("../fonts/font.ttf", 100), window, {50,50,500,200}),
    canon({900,500,170,100},SDL_FLIP_NONE,window)
    

{
    //initiallize audio
   platforms.reserve(9);
    platforms.emplace_back(texturefolder+"grass.png",SDL_Rect{0,700,1000,300},window);
    for(int i=1;i<9;i++)
        {
        platforms.emplace_back(texturefolder+"grass.png",
        SDL_Rect{
        Utils::RandInRange(0,window.CameraView.w-600,i + (long)&platforms[i]),Utils::RandInRange(platforms[i-1].hitbox.y-300,platforms[i-1].hitbox.y-100,i + (long)&platforms[i]),
        Utils::RandInRange(250, 500, i * (long)&i),50
        },
        window);
    }
    

    canon.Change_Power(2.0f);




    start_button.  OnClick = [this](){running = true;};
    restart_button.OnClick = [this](){running = true; this->reset();};
    quit_button.OnClick = [this](){quit_app = true;running = true;};
    start_button.ChangeColor({200,200,0});
    start_button.ChangeRectColor({255,105,180,255});
    start_button  .ChangeFont("../fonts/button.ttf");  
    restart_button.ChangeFont("../fonts/button.ttf");  
    quit_button   .ChangeFont("../fonts/button.ttf"); 
    }
bool Game::IsRunning()
{
    return running;
}

void Game::DeathScreen()
{

    ResizeButtonCorrectly(restart_button, {375,400,250,100});
    ResizeButtonCorrectly(quit_button, {400,550,200,100});
    
    window.Clear();
    death_screen_bg.DrawOnWindow();
    restart_button.Draw();
    quit_button.Draw();
    //ScoreDisplay.Draw();
    window.Show();
    SDL_Event event;
        while(SDL_PollEvent(&event))
    {
            if (event.type == SDL_QUIT)
            { 
                running = false;
                quit_app = true;
            }
 
            restart_button.HandleEvent(event);
            quit_button.HandleEvent(event);
    } 
}


void Game::StartMenu()
{
    ResizeButtonCorrectly(start_button, {400,400,200,100});
    ResizeButtonCorrectly(quit_button, {400,550,200,100});

    window.Clear();
    bg.DrawOnWindow();
    start_button.Draw();
    quit_button.Draw();
    window.Show();
    SDL_Event event;
        while(SDL_PollEvent(&event))
    {
            if (event.type == SDL_QUIT)
            { 
                running = false;
                quit_app = true;
            }
 
            start_button.HandleEvent(event);
            quit_button.HandleEvent(event);

    }   
}


void Game::reset()
{
    player.Repos(800,640);
    player.Stop();
    player.acceleration.x = 0;
    score = 0;
    ScoreDisplay.ChangeText("score: 0");
    for(auto& platform : platforms)
    {
        platform.Repos(0,10000);
    }
    platforms[0].Resize(1000,50);
    platforms[0].Repos(0,700);
    cameraheight = 0;
    window.RepositionCamera(0, 0);
    for(int i=1;i<platforms.size();i++)
        {
            int TopPlatform = TopPlatformIndex();
            platforms[i].Repos(Utils::RandInRange(0,window.CameraView.w-600,i + (long)&platforms[i]),Utils::RandInRange(platforms[TopPlatform].hitbox.y-300,platforms[TopPlatform].hitbox.y-100,i + (long)&platforms[i]));
            platforms[i].Resize(Utils::RandInRange(300, 600, i * (long)&i),50);
        }
    canon.Repos(900,500);
}



void Game::Draw()
{
    window.Clear();
    bg.DrawOnWindow();
    player.Draw();
    canon.Draw();
    for(auto& platform : platforms)
    {
        platform.Draw();
    }

    ScoreDisplay.Draw();

    window.Show();
}


void Game::HandleInput()
{
        SDL_Event event;
        while (SDL_PollEvent(&event))
        {
            if (event.type == SDL_QUIT)
            { 
                running = false;
                quit_app = true;
            }
            if (event.type == SDL_MOUSEBUTTONDOWN)
            {
                SDL_Point mousepos = window.GetMousePos(); 
                
                if(event.button.button == SDL_BUTTON_LEFT)
                {
               }
                if(event.button.button == SDL_BUTTON_RIGHT)
                {
               }
            }
            if (event.type == SDL_MOUSEBUTTONUP)
            {
                if(event.button.button == SDL_BUTTON_LEFT)
                {
               }
                if(event.button.button == SDL_BUTTON_RIGHT)
                {
               }

            }
            
    player.HandleInput(event);

   }
 
}


void Game::RunPhysics(unsigned int LastIterationTime)
{
    player.Update(LastIterationTime);
    canon.Update(LastIterationTime);
    player.PhysicsCollision(canon.hitbox,0.2,0);
    canon.PhysicsCollision(player);
    if(canon.position.y>window.CameraView.y+window.CameraView.h)
        canon.Repos(900,canon.position.y - 2000);
    if(canon.InTrajectory(player.hitbox))
    {
        canon.Shot();
      //  int channel = Mix_PlayChannel( -1, canonsound, 0 );
      //  Mix_Volume( channel, 5);                              
    }
    if(canon.InTrajectory(player.hitbox))
    {
    }
    if(canon.BallLeftScreen())
    {
        canon.Reload();
    }
    for(int i=0;i<platforms.size();i++)
    {
        if(player.CheckCollision(platforms[i].hitbox) == Side::top&&player.velocity.y>0&&player.hitbox.h+player.position.y<platforms[i].hitbox.y +20)
            player.PhysicsCollision(platforms[i].hitbox,0.2,0);
      // canon.PhysicsCollision(platforms[i].hitbox,0,0); 
    if(platforms[i].position.y>window.CameraView.y+window.CameraView.h)
        {
            
            int TopPlatform = TopPlatformIndex();
            platforms[i].Repos(Utils::RandInRange(0,window.CameraView.w-600,i + (long)&platforms[i]),Utils::RandInRange(platforms[TopPlatform].hitbox.y-300,platforms[TopPlatform].hitbox.y-100,i + (long)&platforms[i]));
            platforms[i].Resize(Utils::RandInRange(300, 600, i * (long)&i),50);
        }

    }
    if(player.position.x<0)
    {
        player.position.x =0;
        player.velocity.x *= -0.7;
    }
    if(player.position.x>window.CameraView.w - player.hitbox.w)
    {
        player.position.x =window.CameraView.w - player.hitbox.w;
        player.velocity.x *= -0.7;
    }

    player.SetLookDiraction();
    player.LimitXSpeed(0.7);

   if(player.position.y - window.CameraView.y<300)
        cameraheight  = player.position.y - 300;
    cameraheight -= LastIterationTime/1000.0f *0.04*((int)(score/2500) + 1);
    if (!SDL_HasIntersection(&player.hitbox, &window.CameraView))
        running = false;

    window.RepositionCamera(0,cameraheight);
    int savescore = score;
    score = std::max(score,abs(player.hitbox.y - 640));
    if(savescore != score)
        {
        ScoreDisplay.ChangeText("Score:" + std::to_string(score));
        }
iterations++;
}

int Game::TopPlatformIndex()
{
    int topindex = 0;
    for (int i=1;i<platforms.size();i++)
    {
        if(platforms[topindex].hitbox.y > platforms[i].hitbox.y)
        {
            topindex = i;
        }
    }
        return topindex;
}



bool Game::AppQuit()
{
    return quit_app;
}


Game::~Game()
{
 
}










void Game::ResizeButtonCorrectly(Button& button,SDL_Rect original_rect)
{

    if(button.Hovered() && button.GetRect().w == original_rect.w)
    {
        button.Resize({static_cast<int>(original_rect.w*1.25f),static_cast<int>(original_rect.h*1.25f)});
        button.Reposition({original_rect.x- static_cast<int>(original_rect.w*0.125f),original_rect.y- static_cast<int>(original_rect.h*0.125f)});
        button.ChangeColor({255,0,0,255});

    }
    if(!button.Hovered() &&button.GetRect().w != original_rect.w) 
    {
        button.Resize({original_rect.w,original_rect.h});
        button.Reposition({original_rect.x,original_rect.y});
        button.ChangeColor({255,255,0,255});
    }
        
    
} 
