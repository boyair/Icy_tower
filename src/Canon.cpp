#include "Canon.h"
#include "Entity.h"
#include "Window.h"
#include <SDL2/SDL_rect.h>
#include <SDL2/SDL_render.h>
#include <cmath>
#include <iostream>
const std::string texturefolder = "../textures/";
const std::string soundfolder = "../sounds/";
float Canon::gravity = 0.004;
Mix_Chunk* Canon::shotsound   = 0;
Mix_Chunk* Canon::reloadsound = 0;
Canon::Canon(SDL_Rect rect,SDL_RendererFlip direction,Window& window)
:
    PEntity(texturefolder + "canon.png", rect,window ),
    ball(texturefolder+"canon_ball.png", {rect.x,rect.y,rect.w/6,rect.w/6},window ),
    direction(direction) 

{
if(!shotsound)
    shotsound = Mix_LoadWAV((soundfolder+"canonshot.wav").c_str());
if(!reloadsound)
    reloadsound = Mix_LoadWAV((soundfolder+"canonreload.wav").c_str());
    
FitBall();
ball.SetRoughness(100000);
ball.ChangeMass(ball.hitbox.w*ball.hitbox.h );
mass = 700;
}

void Canon::Change_Power(float power)
{
    this->power = power;
}



void Canon::Draw()
{
    ball.Draw();
    Entity::texture.DrawEX(hitbox, 0, direction);
}
void Canon::Reload(bool play_sound)
{
if(play_sound)
{
    reload_timer.Start();
    int channel = Mix_PlayChannel( -1, reloadsound, 0 );
    Mix_Volume( channel, 90);
    on_reload_process = true;
}
else
{
    FitBall();
}
    ball.velocity = {0,0};

   loaded = true;

}
void Canon::Update(unsigned int microseconds)
{
    if(on_reload_process )
    {
        if(reload_timer.PassedTime().count()>1000000)
            on_reload_process = false;
        else
            return;
    }
    if(loaded)
    {
        FitBall();
    }
    else 
    {
        ball.Update(microseconds); 
    }


}
bool Canon::BallLeftScreen()
{
    return !ball.texture.OnScreen() && !loaded;
}
void Canon::Shot()
{
    if(loaded)
    {
        if (direction == SDL_FLIP_HORIZONTAL)
        {
            ball.velocity = {power, power * -0.5f};

        }
        else
        {
            ball.velocity = {power * -1,power * -0.5f};
        }
        int channel = Mix_PlayChannel( -1, shotsound, 0 );
        Mix_Volume( channel, power * 40);


        ball.acceleration.y = gravity;
        loaded = false;
    }
}

bool Canon::InTrajectory(SDL_Rect rect)
{
    //units:
    //velocity: pixels per ms
    //distance: pixels
    //acceleration pixels per ms^2
    //*using milliseconds instead of micro because on update i use milliseconds.*
    
    //ball velocity at moment of shooting;
    Vec2 ballvel = direction == SDL_FLIP_HORIZONTAL ? Vec2{power, power * -0.5f} : Vec2{power * -1,power * -0.5f};
    int distance_between_centers =std::max(((rect.x  ) - (ball.hitbox.x + ball.hitbox.w)),((rect.x + rect.w ) - (ball.hitbox.x )));
    float time_to_reach_rect_center = distance_between_centers/ballvel.x;
    if (time_to_reach_rect_center < 0)
        return false;
    //y = y0 + time * vel.y + time * acc^2 / 2
    //                          //center of aball at start
    float YCenterOnXcollision = (ball.position.y  + ball.hitbox.h/2.0f) +
         time_to_reach_rect_center * ballvel.y + (std::pow(time_to_reach_rect_center,2) *gravity/2.0f);
    //std::cout<<YCenterOnXcollision<<std::endl;
    return rect.y< YCenterOnXcollision && rect.y+rect.h > YCenterOnXcollision; 
}

Side Canon::PhysicsCollision(PEntity& other)
{
    return ball.PhysicsCollision(other);

}


Side Canon::PhysicsCollision(const SDL_Rect& other,float friction_cof,float elasticity)
{
    return ball.PhysicsCollision(other,friction_cof,elasticity);
}



void Canon::FitBall()
{
    if(loaded)
    {
        if(direction == SDL_FLIP_HORIZONTAL)
        {
            ball.Repos(hitbox.x+hitbox.w - ball.hitbox.w,hitbox.y);
        }
        else 
        {
            ball.Repos(hitbox.x,hitbox.y);
        }

    }

}
