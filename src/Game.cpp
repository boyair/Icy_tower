#include "Game.h"
#include "Button.h"
#include "PEntity.h"
#include "Utils.h"
#include "Window.h"
#include "Sound.h"
#include <SDL2/SDL_events.h>
#include <SDL2/SDL_mixer.h>
#include <SDL2/SDL_rect.h>
#include <SDL2/SDL_render.h>
#include <SDL2/SDL_ttf.h>

#include <cstdint>
#include <exception>
#include <iostream>
#include <string>

const std::string texturefolder = "../textures/";
const std::string animationfolder = "../animations/";
const std::string soundfolder = "../sounds/";
const std::string fontfolder = "../fonts/";



Game::Game()
:
    window ("2D Game!", {SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 1600, 1000}, 0),
    player({300,640,60,100},window),
    platform_default(texturefolder+"grass.png",SDL_Rect{300,700,1000,300},window ),
    platform_ice(texturefolder+"ice_platform.png",SDL_Rect{300,1200,1000,300},window ),
    platform_gum(texturefolder+"gum_platform.png",SDL_Rect{300,1200,1000,300},window ),
    wind(4, window,{1200,0,400,400}, animationfolder+"wind2" ),
    score_display("0",{255,190,70,255},"../fonts/font.ttf", window, {20,20,100,150}),
    cloud(texturefolder+ "cloud_platform.png",{1400,600,200,100},window),
    bg(texturefolder + "sky.png",window,{0,0,window.width,window.height}),
    wall(texturefolder + "wall.png",window,{200,0,100,window.height}),
    heart(texturefolder + "heart.png", window ,{1050,30,100,100} ),
    death_screen_bg(texturefolder + "death_screen.png",window,{0,0,window.width,window.height}),
    canon({1400,500,170,100},SDL_FLIP_NONE,window),
    death_sound(soundfolder + "death.wav",70),
    wind_sound(soundfolder + "wind.wav",30),
    damage_sound (soundfolder + "damage.wav",80),
    player_animation(4, window, {300,640,120,120}, animationfolder+"player"),
    start_button("start",SDL_Color{255,255,0,255},fontfolder + "button.ttf",{700,400,200,100},8,window),
    restart_button("restart",SDL_Color{255,255,0,255},fontfolder + "button.ttf",{675,400,250,100},8,window),
    quit_button("Quit!",SDL_Color{255,255,0,255},fontfolder + "button.ttf",{700,550,200,100} , 8, window),
    button_hover_sound (soundfolder + "buttonhover.wav",50),
    death_score_display(SDL_Color{190,0,0,255}, window,{320,200,900,200})
{
    player_animation.SetBackAndForth(true);
    Canon::LoadSounds();


    //set properties for each platform type.
    platform_default.SetRoughness(0.2f);
    platform_ice.SetRoughness(0.0f);
    platform_gum.SetRoughness(0.31);
    platforms.reserve(10);
        
    //create and place all platfroms randomly.
    for(int i=0;i<10;i++)
    {
        platforms.emplace_back(platform_default);
        RepositionPlatformRandomly(platforms[i]);
    }
    platforms_created = 10;
    wind.SetBackAndForth(true);
    wind.SetTimePerImage(70000);

    cloud.acceleration.y = -0.0002;
    canon.acceleration.y = 0.04;
    
    seed_generator.Start();
    canon.Change_Power(2.0f);
    
    //setting functionalities for buttons.
    start_button.  OnClick = [this](){wind_sound.Play(-1);  running = true;};
    restart_button.OnClick = [this](){running = true; Reset();};
    quit_button.OnClick = [this](){quit_app = true;running = true;};

    //set buttons frame to dark red.
    start_button.   ChangeRectColor({190,0,0,255});
    restart_button. ChangeRectColor({190,0,0,255});
    quit_button.    ChangeRectColor({190,0,0,255});
      }

bool Game::IsRunning()
{
    return running;
}

void Game::DeathScreen()
{

    ResizeButtonCorrectly(restart_button, {675,400,250,100});
    ResizeButtonCorrectly(quit_button, {700,550,200,100});
    
    window.Clear();
    death_screen_bg.DrawOnWindow();
    restart_button.Draw();
    quit_button.Draw();
    death_score_display.Draw();
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
    ResizeButtonCorrectly(start_button, {700,400,200,100});
    ResizeButtonCorrectly(quit_button, {700,550,200,100});

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
    lives = 3;
    player.acceleration.x = 0;
    score = 0;
    player_level = 1;
    wind_sound.Play(-1);
    platform_level = 1;
    player.highest_platform_passed = 700;
    score_display.Resize({100,150});
    score_display.Reposition({20,20});
    score_display.ChangeColor({255,190,70,255});
    score_display.ChangeText("0");
    score_display.RecreateTexture();
    //set all platforms outside the screen so that the next update will reposition them randomly.
    for(auto& platform : platforms)
    {   
        platform = platform_default;
        platform.Repos(0,10000);
    }

    platforms_created = 9;
    cameraheight = 0;
    cameraspeed = 0.0001;
    window.RepositionCamera(0, 0);
    
    for(auto& platform : platforms)
        {
            RepositionPlatformRandomly(platform);

       }
    canon.Repos(1400,500);
    canon.SetFlip(SDL_FLIP_NONE);
    cloud.Repos(canon.position.x,canon.position.y + canon.hitbox.h);
    canon.Reload(false);
}



void Game::Draw()
{
    window.Clear();
    bg.DrawOnWindow();
    if(player_level == 1)
    {
        int original_y = wind.rect.y;
        for (int j = 0; j<4; j++) {


            int original_x = wind.rect.x;
            for(int i=0;i<5;i++)
            {
                wind.DrawOnWindow();
                wind.rect.x -= wind.rect.w;
            }
            wind.rect.x = original_x;
            wind.rect.y +=wind.rect.h;
        }
        wind.rect.y = original_y;
    }
    cloud.Draw();
    wall.rect.y = int(player.position.y/1000) * 1000;
    wall.Draw();
    for (int i=0;i<2 ; i++) {
        wall.rect.y -= 1000;
        wall.Draw();
    }
    wall.rect.x = 1300;
    wall.rect.y = int(player.position.y/1000) * 1000;
    wall.Draw();
    for (int i=0;i<2 ; i++) {
        wall.rect.y -= 1000;
        wall.Draw();
    }
    wall.rect.x = 200; 
    for(auto& platform : platforms)
    {
        platform.Draw();
    }
    platform_default.Draw();
    platform_ice.Draw();
    platform_gum.Draw();
    score_display.Draw();
    canon.Draw();
    //draws the heart texture as many times as there are lives left and reset the texture position for the next draw.
    for (int i=0; i<lives; i++) 
    {
        heart.DrawOnWindow();
        heart.rect.x -= 120;

    }
    heart.rect.x = 1450;
    
    SDL_RendererFlip direction = SDL_FLIP_HORIZONTAL;

    if(player.velocity.x>0)

         direction = SDL_FLIP_NONE;

    else if(player.velocity.x<0) 

        direction = SDL_FLIP_HORIZONTAL;



    player_animation.rect = player.hitbox;
    player_animation.DrawEX(0,direction);
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
    player_animation.SetTimePerImage(std::max((int)abs(1/player.velocity.x*10000),10000)); // fits animation speed to player speed.
    
    // pause running animation when player in air.
    if (player.Standing()) 
        player_animation.Resume(-1);
    
    else 
        player_animation.Pause(-1);
    


    int savescore = score; //saves score value to see if changed and redrawing is needed. 
    for(int i=0;i<platforms.size();i++)
    {
    if(player.position.y + player.hitbox.h < platforms[i].position.y && player.highest_platform_passed > platforms[i].position.y)
    {
        player.highest_platform_passed = platforms[i].position.y;
        score++;
        if(score % 25 == 0)
        {
            player_level++;
            cameraspeed += 0.00005;
        if(player_level == 1 || player_level == 4)
            wind_sound.Play(-1);
        if(player_level == 2)
            wind_sound.Cut();
        }
        if(player_level == 4)
        {
            player_level = 1;
        }
    }
    //handle platform recreation as the game goes.
    if(platforms[i].position.y>window.CameraView.y+window.CameraView.h)
        {
            platforms_created ++;
            if(platforms_created == 25)
            {
                platform_level++;
                if(platform_level == 4)
                    platform_level = 1;
                platforms_created = 0;

            }
            FitPlatformToLevel(platforms[i]);
            RepositionPlatformRandomly(platforms[i]);
        }

    }

    // handle camera movement
   if(player.position.y - window.CameraView.y<300)
        cameraheight  = player.position.y - 300;
    cameraheight -= LastIterationTime * cameraspeed;
    
    //handle player death
    if (!SDL_HasIntersection(&player.hitbox, &window.CameraView) || lives < 1)
    {
        Sound::CutAll();
        death_sound.Play(0);
        if(lives < 1)
        {
            canon.ball.Repos(player.position.x+20,player.position.y+20);
            Draw();
            SDL_Delay(2000);
 
        }
        
        death_score_display.ChangeText("Final score: "+std::to_string(score));
        death_score_display.RecreateTexture();
        running = false;
        return;
    }
   if(canon.InTrajectory(player.hitbox))
        canon.Shot();
   if(canon.BallLeftScreen())
        canon.Reload(true);

    window.RepositionCamera(0,cameraheight);
    wind.rect.x +=LastIterationTime/1500;
    if(wind.rect.x >=1600)
        wind.rect.x -=400;
    //recreate score texture if needed.
    if(savescore != score)
        {
            std::string score_text =std::to_string(score); 
        score_display.ChangeText(score_text);
        score_display.Resize({(int)score_text.length() * 100,150});
        score_display.RecreateTexture();
        }
    player.SetLookDiraction();
iterations++;

}


void Game::RunPhysics(unsigned int LastIterationTime)
{
    player.Update(LastIterationTime);
    canon.Update(LastIterationTime);
    
    player.GhostPhysicsCollision(platform_default);
    player.GhostPhysicsCollision(platform_ice);
    if(player_level==1)
        player.ApplyForce({0.000001f * LastIterationTime,0});
    canon.PhysicsCollision(cloud.hitbox,0.5,0);
    if(!canon.IsLoaded() && std::abs(static_cast<int>(canon.ball.PhysicsCollision(player))) == static_cast<int>(Side::right)&& canon.CanDoDamage())
    {
        lives -=1;
        canon.DisableDamage();
        damage_sound.Play(0);
    }
    if(canon.position.y>window.CameraView.y+window.CameraView.h)
    {

        if(canon.position.x == 1400)
        {
            canon.Repos(0,canon.position.y - 2000);
            canon.SetFlip(SDL_FLIP_HORIZONTAL);
        }
        else
        {
            canon.Repos(1400,canon.position.y - 2000);
            canon.SetFlip(SDL_FLIP_NONE);
        }
        cloud.Repos(canon.position.x,canon.position.y + canon.hitbox.h);
    }
   for(auto& platform : platforms)
    {
        if(player.CheckCollision(platform.hitbox) == Side::top&&player.velocity.y>0&&player.hitbox.h+player.position.y<platform.hitbox.y +5)
            player.GhostPhysicsCollision(platform);
    }
    player.LimitXpos(300, window.CameraView.w - 300 - player.hitbox.w);
    player.LimitXSpeed(1.2f);
    
    cloud.Update(LastIterationTime);
    if(cloud.velocity.y > 0.15f)
        cloud.acceleration.y = -0.0002;    
    if(cloud.velocity.y< -0.15f)
        cloud.acceleration.y = 0.0002;  


 }

int Game::TopPlatformPosition()
{
    int topposition = 1000;
    for (auto& platform : platforms)
    {
        if(topposition > platform.hitbox.y)
        {
            topposition  = platform.hitbox.y;
        }
    }
    if(topposition > platform_default.hitbox.y)
        topposition = platform_default.hitbox.y;
        
    return topposition;
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
        button_hover_sound.Play(0);
    }
    if(!button.Hovered() &&button.GetRect().w != original_rect.w) 
    {
        button.Resize({original_rect.w,original_rect.h});
        button.Reposition({original_rect.x,original_rect.y});
        button.ChangeColor({255,255,0,255});
        button.RecreateTexture();
    }
        
    
} 



void Game::RepositionPlatformRandomly(PEntity& platform)
{
             int TopPlatform = TopPlatformPosition();
            platform.Repos(Utils::RandInRange(300,window.CameraView.w-900,TopPlatform + GetGoodSeed()),
                Utils::RandInRange(TopPlatform-300,TopPlatform-100,TopPlatform + GetGoodSeed()));

            platform.Resize(Utils::RandInRange(300, 600, TopPlatform * GetGoodSeed()),50);

   
}

void Game::FitPlatformToLevel(PEntity& platform)
{
    switch (platform_level)
    {
        case 1:
            if(!platform.EqualProperties(platform_default))
                platform = platform_default;
        break;
        case 2:
            if(!platform.EqualProperties(platform_ice))
                platform = platform_ice;
        break;
       case 3:
            if(!platform.EqualProperties(platform_gum))
                platform = platform_gum;
        break;



    }
}




