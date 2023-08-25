#include "Game.h"
#include "Button.h"
#include "Utils.h"
#include "Window.h"
#include <SDL2/SDL_events.h>
#include <SDL2/SDL_mixer.h>
#include <SDL2/SDL_rect.h>
#include <SDL2/SDL_render.h>
#include <SDL2/SDL_ttf.h>

#include <cstdint>
#include <string>

const std::string texturefolder = "../textures/";
const std::string soundfolder = "../sounds/";
const std::string fontfolder = "../fonts/";

Game::Game()
:
    window ("2D Game!", {SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 1000, 1000}, 0),
    player({300,640,60,60},window),
    ScoreDisplay("0","../fonts/font.ttf", window, {20,20,100,150}),
    start_button("start",{400,400,200,100},8,window),
    restart_button("restart",{375,400,250,100},8,window),
    quit_button("Quit!",{400,550,200,100} , 8, window),
    bg(texturefolder + "sky.png",window,{0,0,1000,1000}),
    heart(texturefolder + "heart.png", window ,{400,0,100,100} ),
    death_screen_bg(texturefolder + "death_screen.png",window,{0,0,1000,1000}),
    canon({900,500,170,100},SDL_FLIP_NONE,window)
    

{
    std::cout<<0.002f + (-0.002f)<<std::endl;
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
    death_sound = Mix_LoadWAV((soundfolder + "death.wav").c_str());

    
    //setting functionalities for buttons.
    start_button.  OnClick = [this](){running = true;};
    restart_button.OnClick = [this](){running = true; this->Reset();};
    quit_button.OnClick = [this](){quit_app = true;running = true;};

    //set buttons and score with the correct colors and 
    start_button.   ChangeRectColor({190,0,0,255});
    restart_button. ChangeRectColor({190,0,0,255});
    quit_button.    ChangeRectColor({190,0,0,255});
    start_button.   ChangeColor({255,255,0,255});
    restart_button. ChangeColor({255,255,0,255});
    quit_button.    ChangeColor({255,255,0,255});
    ScoreDisplay.   ChangeColor({255,190,70,255});
    
    start_button  .ChangeFont("../fonts/button.ttf");  
    restart_button.ChangeFont("../fonts/button.ttf");  
    quit_button   .ChangeFont("../fonts/button.ttf"); 
    //recreates textures for all buttons and score to apply color and font changes.
   start_button  .RecreateTexture();
   restart_button.RecreateTexture();
   quit_button   .RecreateTexture();
   ScoreDisplay.RecreateTexture();
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
    ScoreDisplay.Draw();
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


void Game::Reset()
{
    player.Repos(800,640);
    player.Stop();
    ScoreDisplay.Resize({100,150});
    ScoreDisplay.Reposition({20,20});
    ScoreDisplay.ChangeColor({255,190,70,255});

    player.acceleration.x = 0;
    score = 0;
    ScoreDisplay.ChangeText("0");
    ScoreDisplay.RecreateTexture();
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
    canon.Reload(false);
}



void Game::Draw()
{
    window.Clear();
    bg.DrawOnWindow();
    heart.DrawOnWindow();
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

void Game::HandleLogic(uint32_t LastIterationTime)
{
    for(int i=0;i<platforms.size();i++)
    {
    if(platforms[i].position.y>window.CameraView.y+window.CameraView.h)
        {
            int TopPlatform = TopPlatformIndex();
            int newypos = Utils::RandInRange(platforms[TopPlatform].hitbox.y-300,platforms[TopPlatform].hitbox.y-100,i + (long)&platforms[i]);
            platforms[i].Repos(Utils::RandInRange(0,window.CameraView.w-600,i + (long)&platforms[i]),
                newypos);

            platforms[i].Resize(Utils::RandInRange(300, 600, i * (long)&i),50);

        }

    }


    // handle camera movement
   if(player.position.y - window.CameraView.y<300)
        cameraheight  = player.position.y - 300;
    cameraheight -= LastIterationTime/1000.0f *0.04*((int)(score/2500) + 1);
    
    //handle player death
    if (!SDL_HasIntersection(&player.hitbox, &window.CameraView))
    {
        ScoreDisplay.Resize({900,200});
        ScoreDisplay.Reposition({20,200});
        ScoreDisplay.ChangeText("Final score: "+std::to_string(score));
        ScoreDisplay.ChangeColor({255,0,0,255});
        ScoreDisplay.RecreateTexture();
        int channel = Mix_PlayChannel(-1,death_sound, 0);
        Mix_Volume(channel, 50);
        running = false;
        return;
    }
   if(canon.InTrajectory(player.hitbox))
        canon.Shot();
   if(canon.BallLeftScreen())
        canon.Reload(true);

    window.RepositionCamera(0,cameraheight);
    int savescore = score;
    score = std::max(score,abs(player.hitbox.y - 640));
    if(savescore != score)
        {
            std::string score_text =std::to_string(score); 
        ScoreDisplay.ChangeText(score_text);
        ScoreDisplay.Resize({(int)score_text.length() * 100,150});
        ScoreDisplay.RecreateTexture();
        }
iterations++;

}


void Game::RunPhysics(unsigned int LastIterationTime)
{
    player.Update(LastIterationTime);
    canon.Update(LastIterationTime);
    player.PhysicsCollision(canon.hitbox,0.2,0);

    if(canon.PhysicsCollision(player) != Side::none)
        lives -=1;
    if(canon.position.y>window.CameraView.y+window.CameraView.h)
        canon.Repos(900,canon.position.y - 2000);
  
   for(int i=0;i<platforms.size();i++)
    {
        if(player.CheckCollision(platforms[i].hitbox) == Side::top&&player.velocity.y>0&&player.hitbox.h+player.position.y<platforms[i].hitbox.y +5)
            player.PhysicsCollision(platforms[i].hitbox,0.2,0);
    }

    player.LimitXpos(0, window.CameraView.w - player.hitbox.w);
    //player.LimitXSpeed(0.7);

    player.SetLookDiraction();
 std::cerr<<lives<<std::endl;
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
        button.Resize({static_cast<int>(original_rect.w*1.2f),static_cast<int>(original_rect.h*1.2f)});
        button.Reposition({original_rect.x- static_cast<int>(original_rect.w*0.1f),original_rect.y- static_cast<int>(original_rect.h*0.1f)});
        button.ChangeColor({190,190,0,255});
        button.RecreateTexture();
    }
    if(!button.Hovered() &&button.GetRect().w != original_rect.w) 
    {
        button.Resize({original_rect.w,original_rect.h});
        button.Reposition({original_rect.x,original_rect.y});
        button.ChangeColor({255,255,0,255});
        button.RecreateTexture();
    }
        
    
} 
